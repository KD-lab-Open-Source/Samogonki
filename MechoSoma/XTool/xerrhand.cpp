#include "xerrhand.h"

#include <cstdarg>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <dbghelp.h>
#else
#include <signal.h>
#include <unistd.h>
#if defined(__linux__) || defined(__APPLE__)
#include <execinfo.h>
#endif
#endif

#ifdef XERRH_USE_SDL
#include <SDL2/SDL.h>
#endif

#ifndef GIT_BRANCH
#define GIT_BRANCH "unknown"
#endif
#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "unknown"
#endif

namespace {

// The log is written next to the executable, which is where the PDB lives too.
std::string& logPathStorage() {
  static std::string path;
  return path;
}

#ifdef _WIN32
std::string executableDirectory() {
  char buffer[MAX_PATH * 4];
  DWORD length = GetModuleFileNameA(nullptr, buffer, sizeof(buffer));
  if (!length || length >= sizeof(buffer)) return {};

  std::string path(buffer, length);
  const size_t slash = path.find_last_of("\\/");
  if (slash == std::string::npos) return {};
  path.resize(slash + 1);
  return path;
}
#else
std::string executableDirectory() {
#ifdef XERRH_USE_SDL
  // SDL knows how to locate the executable on every supported desktop OS.
  char* base = SDL_GetBasePath();
  if (base) {
    std::string path(base);
    SDL_free(base);
    return path;
  }
#endif
  return {};
}
#endif

std::string tempLogPath() {
#ifdef _WIN32
  char buffer[MAX_PATH * 4];
  DWORD length = GetTempPathA(sizeof(buffer), buffer);
  if (length) return std::string(buffer, length) + "XErrH.log";
#else
  const char* tmp = getenv("TMPDIR");
  if (tmp && *tmp) return std::string(tmp) + "/XErrH.log";
#endif
  return "XErrH.log";
}

std::string& logPath() {
  std::string& path = logPathStorage();
  if (path.empty()) {
    path = executableDirectory() + "XErrH.log";
    FILE* probe = fopen(path.c_str(), "ab");
    if (probe) {
      fclose(probe);
    } else {
#ifdef XERRH_USE_SDL
      char* pref = SDL_GetPrefPath("KD-lab", "Moonshine Runners");
      if (pref) {
        path = std::string(pref) + "XErrH.log";
        SDL_free(pref);
      } else {
        path = tempLogPath();
      }
#else
      path = tempLogPath();
#endif
    }
  }
  return path;
}

std::string currentTimestamp(bool compact) {
  std::time_t now = std::time(nullptr);
  std::tm local{};
#ifdef _WIN32
  localtime_s(&local, &now);
#else
  localtime_r(&now, &local);
#endif
  char buffer[64];
  strftime(buffer, sizeof(buffer), compact ? "%Y%m%d-%H%M%S" : "%Y-%m-%d %H:%M:%S", &local);
  return buffer;
}

void appendBuildInfo(std::string& out) {
  out += "Build:      ";
  out += GIT_BRANCH;
  out += " ";
  out += GIT_COMMIT_HASH;
  out += "\n";
}

void appendInstructions(std::string& out, const std::string& dumpPath) {
  out += "\nPlease send this report and the files below to:\n";
  out += "  https://github.com/KD-lab-Open-Source/Samogonki/issues\n\n";
  out += "  - this message\n";
  out += "  - the log file: ";
  out += logPath();
  out += "\n";
  if (!dumpPath.empty()) {
    out += "  - the crash dump: ";
    out += dumpPath;
    out += "\n";
  }
  out += "  - your savegame (save*.dat) if the crash happened during a race\n";
#ifndef _WIN32
  out += "\nTo resolve raw addresses in this report (build with debug info):\n";
#ifdef __APPLE__
  out += "  atos -o <path-to-moonshine-runners> <address>\n";
#else
  out += "  addr2line -e <path-to-moonshine-runners> -f -C <address>\n";
#endif
#endif
}

void writeLog(const std::string& text) {
  FILE* file = fopen(logPath().c_str(), "ab");
  if (!file) return;
  fwrite(text.data(), 1, text.size(), file);
  fflush(file);
  fclose(file);
}

bool messageBoxEnabled() {
  const char* quiet = getenv("MOONSHINE_CRASH_NO_DIALOG");
  return !(quiet && *quiet && strcmp(quiet, "0") != 0);
}

void showReport(const std::string& report) {
  if (!messageBoxEnabled()) return;

  // The native/SDL dialog has a hard limit around 64 KiB; be conservative.
  const size_t maxLength = 32000;
  const std::string text = report.size() > maxLength ? report.substr(0, maxLength) + "\n...[truncated]" : report;

#ifdef XERRH_USE_SDL
  if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Moonshine Runners - crash report", text.c_str(), nullptr) == 0) {
    return;
  }
#endif

#ifdef _WIN32
  // Works even when SDL is not initialized or is in a bad state.
  MessageBoxA(nullptr, text.c_str(), "Moonshine Runners - crash report",
              MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
#else
  (void)text;
#endif
}

void reportFatal(const std::string& title, const std::string& details, int exitCode, const std::string& dumpPath) {
  std::string report = "=== Moonshine Runners crash report ===\n";
  report += "Time:       " + currentTimestamp(false) + "\n";
  appendBuildInfo(report);
  report += "\n" + title + "\n";
  report += details;
  appendInstructions(report, dumpPath);

  writeLog(report);
  std::cout << report << std::endl;
  showReport(report);

#ifdef _WIN32
  TerminateProcess(GetCurrentProcess(), static_cast<UINT>(exitCode ? exitCode : 1));
#else
  std::_Exit(exitCode ? exitCode : 1);
#endif
}

#ifdef _WIN32
volatile LONG g_crashInProgress = 0;

const char* exceptionName(DWORD code) {
  switch (code) {
    // C++ exceptions are raised as SEH exceptions by the runtime.
    case 0x20474343: return "GCC C++ exception";
    case 0xE06D7363: return "MSVC C++ exception";
    case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
    case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
    case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
    case EXCEPTION_FLT_DENORMAL_OPERAND: return "EXCEPTION_FLT_DENORMAL_OPERAND";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
    case EXCEPTION_FLT_INEXACT_RESULT: return "EXCEPTION_FLT_INEXACT_RESULT";
    case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION";
    case EXCEPTION_FLT_OVERFLOW: return "EXCEPTION_FLT_OVERFLOW";
    case EXCEPTION_FLT_STACK_CHECK: return "EXCEPTION_FLT_STACK_CHECK";
    case EXCEPTION_FLT_UNDERFLOW: return "EXCEPTION_FLT_UNDERFLOW";
    case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
    case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
    case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
    case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
    case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
    case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
    case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
    case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
    case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
    default: return "Unrecognized exception";
  }
}

void appendExceptionInfo(std::string& out, EXCEPTION_POINTERS* info) {
  EXCEPTION_RECORD* record = info->ExceptionRecord;
  char buffer[512];

  snprintf(buffer, sizeof(buffer), "Exception:  %s (0x%08lX) at 0x%p\n", exceptionName(record->ExceptionCode),
           static_cast<unsigned long>(record->ExceptionCode), record->ExceptionAddress);
  out += buffer;

  if (record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && record->NumberParameters >= 2) {
    const ULONG_PTR operation = record->ExceptionInformation[0];
    const ULONG_PTR address = record->ExceptionInformation[1];
    snprintf(buffer, sizeof(buffer), "Access:     %s of address 0x%llX\n",
             operation == 0 ? "read" : (operation == 1 ? "write" : "execute"),
             static_cast<unsigned long long>(address));
    out += buffer;
  }

  const CONTEXT* context = info->ContextRecord;
  if (!context) return;

#if defined(_M_X64) || defined(__x86_64__)
  snprintf(buffer, sizeof(buffer),
           "Registers:  RAX=%016llX RBX=%016llX RCX=%016llX RDX=%016llX\n"
           "            RSI=%016llX RDI=%016llX RBP=%016llX RSP=%016llX\n"
           "            R8=%016llX R9=%016llX R10=%016llX R11=%016llX\n"
           "            R12=%016llX R13=%016llX R14=%016llX R15=%016llX\n"
           "            RIP=%016llX EFLAGS=%08lX\n",
           static_cast<unsigned long long>(context->Rax), static_cast<unsigned long long>(context->Rbx),
           static_cast<unsigned long long>(context->Rcx), static_cast<unsigned long long>(context->Rdx),
           static_cast<unsigned long long>(context->Rsi), static_cast<unsigned long long>(context->Rdi),
           static_cast<unsigned long long>(context->Rbp), static_cast<unsigned long long>(context->Rsp),
           static_cast<unsigned long long>(context->R8), static_cast<unsigned long long>(context->R9),
           static_cast<unsigned long long>(context->R10), static_cast<unsigned long long>(context->R11),
           static_cast<unsigned long long>(context->R12), static_cast<unsigned long long>(context->R13),
           static_cast<unsigned long long>(context->R14), static_cast<unsigned long long>(context->R15),
           static_cast<unsigned long long>(context->Rip), static_cast<unsigned long>(context->EFlags));
#else
  snprintf(buffer, sizeof(buffer),
           "Registers:  EAX=%08lX EBX=%08lX ECX=%08lX EDX=%08lX\n"
           "            ESI=%08lX EDI=%08lX EBP=%08lX ESP=%08lX\n"
           "            EIP=%08lX EFLAGS=%08lX\n",
           static_cast<unsigned long>(context->Eax), static_cast<unsigned long>(context->Ebx),
           static_cast<unsigned long>(context->Ecx), static_cast<unsigned long>(context->Edx),
           static_cast<unsigned long>(context->Esi), static_cast<unsigned long>(context->Edi),
           static_cast<unsigned long>(context->Ebp), static_cast<unsigned long>(context->Esp),
           static_cast<unsigned long>(context->Eip), static_cast<unsigned long>(context->EFlags));
#endif
  out += buffer;
}

void appendStackTrace(std::string& out, EXCEPTION_POINTERS* info) {
  HANDLE process = GetCurrentProcess();
  HANDLE thread = GetCurrentThread();

  const std::string dir = executableDirectory();
  SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
  if (!SymInitialize(process, dir.empty() ? nullptr : dir.c_str(), TRUE)) {
    out += "Call stack: <symbols unavailable: SymInitialize failed>\n";
    return;
  }

  CONTEXT* context = info->ContextRecord;
  STACKFRAME64 frame{};
#if defined(_M_X64) || defined(__x86_64__)
  const DWORD machine = IMAGE_FILE_MACHINE_AMD64;
  frame.AddrPC.Offset = context->Rip;
  frame.AddrPC.Mode = AddrModeFlat;
  frame.AddrStack.Offset = context->Rsp;
  frame.AddrStack.Mode = AddrModeFlat;
  frame.AddrFrame.Offset = context->Rbp;
  frame.AddrFrame.Mode = AddrModeFlat;
#else
  const DWORD machine = IMAGE_FILE_MACHINE_I386;
  frame.AddrPC.Offset = context->Eip;
  frame.AddrPC.Mode = AddrModeFlat;
  frame.AddrStack.Offset = context->Esp;
  frame.AddrStack.Mode = AddrModeFlat;
  frame.AddrFrame.Offset = context->Ebp;
  frame.AddrFrame.Mode = AddrModeFlat;
#endif

  out += "Call stack:\n";

  for (int index = 0; index < 64; ++index) {
    if (!StackWalk64(machine, process, thread, &frame, context, nullptr, SymFunctionTableAccess64,
                     SymGetModuleBase64, nullptr)) {
      break;
    }
    if (!frame.AddrPC.Offset) break;

    char buffer[4096];
    IMAGEHLP_MODULE64 module{};
    module.SizeOfStruct = sizeof(module);
    const bool haveModule = SymGetModuleInfo64(process, frame.AddrPC.Offset, &module) != FALSE;
    const char* moduleName = haveModule ? module.ModuleName : "unknown";

    alignas(SYMBOL_INFO) char symbolStorage[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)]{};
    PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(symbolStorage);
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;

    DWORD64 displacement = 0;
    IMAGEHLP_LINE64 line{};
    line.SizeOfStruct = sizeof(line);
    DWORD lineDisplacement = 0;

    const bool haveLine = SymGetLineFromAddr64(process, frame.AddrPC.Offset, &lineDisplacement, &line) != FALSE;
    // Nearest public symbols can be far away (e.g. libstdc++ frames). Without
    // line info, only trust them when the offset is small.
    if (SymFromAddr(process, frame.AddrPC.Offset, &displacement, symbol) &&
        (haveLine || displacement <= 0x1000)) {
      if (haveLine) {
        snprintf(buffer, sizeof(buffer), "  #%02d 0x%016llX %s!%s+0x%llX  %s(%lu)\n", index,
                 static_cast<unsigned long long>(frame.AddrPC.Offset), moduleName, symbol->Name,
                 static_cast<unsigned long long>(displacement), line.FileName, line.LineNumber);
      } else {
        snprintf(buffer, sizeof(buffer), "  #%02d 0x%016llX %s!%s+0x%llX\n", index,
                 static_cast<unsigned long long>(frame.AddrPC.Offset), moduleName, symbol->Name,
                 static_cast<unsigned long long>(displacement));
      }
    } else {
      const unsigned long long moduleOffset =
          haveModule ? static_cast<unsigned long long>(frame.AddrPC.Offset - module.BaseOfImage) : 0ULL;
      snprintf(buffer, sizeof(buffer), "  #%02d 0x%016llX %s+0x%llX\n", index,
               static_cast<unsigned long long>(frame.AddrPC.Offset), moduleName, moduleOffset);
    }
    out += buffer;
  }

  SymCleanup(process);
}

std::string writeMiniDump(EXCEPTION_POINTERS* info) {
  const std::string path = executableDirectory() + "crash-" + currentTimestamp(true) + ".dmp";

  HANDLE file = CreateFileA(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) return {};

  MINIDUMP_EXCEPTION_INFORMATION exceptionInfo{};
  exceptionInfo.ThreadId = GetCurrentThreadId();
  exceptionInfo.ExceptionPointers = info;
  exceptionInfo.ClientPointers = FALSE;

  const MINIDUMP_TYPE type = static_cast<MINIDUMP_TYPE>(MiniDumpWithThreadInfo | MiniDumpWithDataSegs);
  const BOOL written = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, type, &exceptionInfo, nullptr, nullptr);
  CloseHandle(file);

  return written ? path : std::string();
}

LONG WINAPI topLevelExceptionFilter(EXCEPTION_POINTERS* info) {
  if (InterlockedCompareExchange(&g_crashInProgress, 1, 0) != 0) {
    TerminateProcess(GetCurrentProcess(), 1);
    return EXCEPTION_EXECUTE_HANDLER;
  }

  std::string details;
  appendExceptionInfo(details, info);

  const std::string dumpPath = writeMiniDump(info);
  // Walking the stack after a stack overflow usually faults again.
  if (info->ExceptionRecord->ExceptionCode != EXCEPTION_STACK_OVERFLOW) {
    appendStackTrace(details, info);
  } else {
    details += "Call stack: <unavailable after EXCEPTION_STACK_OVERFLOW>\n";
  }

  reportFatal("Unhandled exception", details, 1, dumpPath);
  return EXCEPTION_EXECUTE_HANDLER;
}

void terminateHandler() {
  std::string details;
  try {
    std::exception_ptr current = std::current_exception();
    if (current) {
      try {
        std::rethrow_exception(current);
      } catch (const std::exception& e) {
        details += "std::terminate: ";
        details += e.what();
        details += "\n";
      } catch (...) {
        details += "std::terminate: unknown exception\n";
      }
    } else {
      details += "std::terminate: no active exception\n";
    }
  } catch (...) {
    details += "std::terminate: failed to describe the exception\n";
  }
  reportFatal("Unhandled C++ exception", details, 1, {});
}

void signalHandler(int sig) {
  const char* name = "unknown signal";
  switch (sig) {
    case SIGABRT: name = "SIGABRT (abort)"; break;
    case SIGFPE: name = "SIGFPE (arithmetic error)"; break;
    case SIGILL: name = "SIGILL (illegal instruction)"; break;
    case SIGSEGV: name = "SIGSEGV (invalid memory access)"; break;
    case SIGINT: name = "SIGINT"; break;
    case SIGTERM: name = "SIGTERM"; break;
    default: break;
  }

  std::string details = name;
  details += "\n";
  reportFatal("Fatal signal", details, 128 + sig, {});
}
#else
void signalHandler(int sig) {
  const char* name = "unknown signal";
  switch (sig) {
    case SIGABRT: name = "SIGABRT (abort)"; break;
    case SIGFPE: name = "SIGFPE (arithmetic error)"; break;
    case SIGILL: name = "SIGILL (illegal instruction)"; break;
    case SIGSEGV: name = "SIGSEGV (invalid memory access)"; break;
#ifdef SIGBUS
    case SIGBUS: name = "SIGBUS (bus error)"; break;
#endif
    case SIGINT: name = "SIGINT"; break;
    case SIGTERM: name = "SIGTERM"; break;
    default: break;
  }

  std::string details = name;
  details += "\n";
#if defined(__linux__) || defined(__APPLE__)
  void* frames[64];
  const int count = backtrace(frames, sizeof(frames) / sizeof(frames[0]));
  char** symbols = backtrace_symbols(frames, count);
  if (symbols) {
    details += "Call stack:\n";
    for (int i = 0; i < count; ++i) {
      details += "  ";
      details += symbols[i];
      details += "\n";
    }
    free(symbols);
  }
#endif
  reportFatal("Fatal signal", details, 128 + sig, {});
}

void terminateHandler() {
  reportFatal("Unhandled C++ exception", "std::terminate\n", 1, {});
}
#endif

void overflowStack(int depth) {
  volatile char buffer[4096];
  buffer[0] = static_cast<char>(depth);
  overflowStack(depth + 1);
  // Keep the recursive call alive so this does not become a tail call.
  if (buffer[0] == 127) std::cout << depth << std::endl;
}

}  // namespace

XErrorHandler ErrH;

XErrorHandler::XErrorHandler() : flags(0), state(0), prefix(nullptr), postfix(nullptr), restore_func(nullptr) {}

XErrorHandler::~XErrorHandler() {}

void XErrorHandler::SetFlags(unsigned f) { flags |= f; }

void XErrorHandler::ClearFlags(unsigned f) { flags &= ~f; }

unsigned XErrorHandler::InfoFlags(unsigned f) { return flags & f; }

void XErrorHandler::SetPrefix(const char* s) { prefix = s; }

void XErrorHandler::SetPostfix(const char* s) { postfix = s; }

void XErrorHandler::SetRestore(void (*rf)()) { restore_func = rf; }

void XErrorHandler::Abort(const char* message, int code, int addval, const char* subj) {
  std::string details;
  details += "Abort:      ";
  details += message ? message : "(null)";
  details += " code:";
  details += std::to_string(code);
  details += " val:";
  details += std::to_string(addval);
  details += "\n";
  if (subj) {
    details += "Subject:    ";
    details += subj;
    details += "\n";
  }

  std::cout << "XErrorHandler: " << (message ? message : "(null)") << ", code " << code << ", addval " << addval;
  if (subj) std::cout << ", subj: " << subj;
  std::cout << std::endl;

  reportFatal("Application abort", details, code ? code : 1, {});
}

void XErrorHandler::Exit() { exit(0); }

void XErrorHandler::RTC(const char* file, unsigned int line, const char* expr) {
  std::stringstream message;
  message << file << ", LINE: " << line << std::endl << expr;
  Abort("RUN-TIME ERROR", XERR_USER, -1, message.str().c_str());
}

void XErrorHandler::WriteLog(char* error, char* msg) {
  std::string text;
  if (error) text += error;
  if (msg) text += msg;
  text += "\n";
  writeLog(text);
}

void XErrorHandler::Log(const char* message) {
  if (!message) return;
  std::string line = "[";
  line += currentTimestamp(false);
  line += "] ";
  line += message;
  line += "\n";
  writeLog(line);
}

void set_signal_handler() {
  // Resolve the log path now, while std::string and the CRT are healthy.
  (void)logPath();

#ifdef _WIN32
  SetUnhandledExceptionFilter(topLevelExceptionFilter);
  std::set_terminate(terminateHandler);

  // Access violations, arithmetic faults and illegal instructions are SEH
  // exceptions on Windows; let SetUnhandledExceptionFilter handle them so the
  // report contains registers, a stack trace and a minidump.
  signal(SIGABRT, signalHandler);
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);
#else
  std::set_terminate(terminateHandler);

  signal(SIGABRT, signalHandler);
  signal(SIGFPE, signalHandler);
  signal(SIGILL, signalHandler);
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);

#if defined(__linux__) || defined(__APPLE__)
  // Run the fatal-fault handlers on a separate stack so a stack overflow can
  // still produce a report.
  static char alternateStack[256 * 1024];
  stack_t stack{};
  stack.ss_sp = alternateStack;
  stack.ss_size = sizeof(alternateStack);
  stack.ss_flags = 0;
  sigaltstack(&stack, nullptr);

  struct sigaction action {};
  sigemptyset(&action.sa_mask);
  action.sa_handler = signalHandler;
  action.sa_flags = SA_ONSTACK;

  sigaction(SIGSEGV, &action, nullptr);
#ifdef SIGBUS
  sigaction(SIGBUS, &action, nullptr);
#endif
#else
  signal(SIGSEGV, signalHandler);
#endif
#endif
}

void run_crash_test_from_env() {
  const char* mode = getenv("MOONSHINE_CRASH_TEST");
  if (!mode || !*mode) return;

  if (strcmp(mode, "av") == 0) {
    volatile int* invalid = reinterpret_cast<volatile int*>(static_cast<uintptr_t>(0x10));
    *invalid = 42;
  } else if (strcmp(mode, "abort") == 0) {
    ErrH.Abort("Crash test requested via MOONSHINE_CRASH_TEST=abort", XERR_USER);
  } else if (strcmp(mode, "exception") == 0) {
    throw std::runtime_error("Crash test requested via MOONSHINE_CRASH_TEST=exception");
  } else if (strcmp(mode, "stack") == 0) {
    overflowStack(0);
  }
}
