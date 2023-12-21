/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include <cctype>
#include <locale.h>

#include "LocalVersion.h"

#include "mch_common.h" // For far target

#ifdef _WIN32
#include <Windows.h>
#endif

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern const char* mchLocalINI;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void win32_InitLocale(void);
void win32_ChangeKeybLayout(void);
int win32_GetKeybLayoutID(void);

const char* getIniKey(const char* fname,const char* section,const char* key);

/* --------------------------- DEFINITION SECTION --------------------------- */

#ifdef _WIN32
static int lv_primaryLangID[] = 
{
	LANG_AFRIKAANS,		// 0
	LANG_ALBANIAN,		// 1
	LANG_ARABIC,		// 2
	LANG_ARMENIAN,		// 3
	LANG_ASSAMESE,		// 4 
	LANG_AZERI,		// 5
	LANG_BASQUE,		// 6
	LANG_BELARUSIAN,	// 7
	LANG_BENGALI,		// 8
	LANG_BULGARIAN,		// 9
	LANG_CATALAN,		// 10
	LANG_CHINESE,		// 11
	LANG_CROATIAN,		// 12
	LANG_CZECH,		// 13
	LANG_DANISH,		// 14
	LANG_DUTCH,		// 15
	LANG_ENGLISH,		// 16
	LANG_ESTONIAN,		// 17
	LANG_FAEROESE,		// 18
	LANG_FARSI,		// 19
	LANG_FINNISH,		// 20
	LANG_FRENCH,		// 21
	LANG_GEORGIAN,		// 22
	LANG_GERMAN,		// 23
	LANG_GREEK,		// 24
	LANG_GUJARATI,		// 25
	LANG_HEBREW,		// 26
	LANG_HINDI,		// 27
	LANG_HUNGARIAN,		// 28
	LANG_ICELANDIC,		// 29
	LANG_INDONESIAN,	// 30
	LANG_ITALIAN,		// 31
	LANG_JAPANESE,		// 32
	LANG_KANNADA,		// 33
	LANG_KASHMIRI,		// 34
	LANG_KAZAK,		// 35
	LANG_KONKANI,		// 36
	LANG_KOREAN,		// 37
	LANG_LATVIAN,		// 38
	LANG_LITHUANIAN,	// 39
	LANG_MACEDONIAN,	// 40
	LANG_MALAY,		// 41
	LANG_MALAYALAM,		// 42
	LANG_MANIPURI,		// 43
	LANG_MARATHI,		// 44
	LANG_NEPALI,		// 45
	LANG_NEUTRAL,		// 46
	LANG_NORWEGIAN,		// 47
	LANG_ORIYA,		// 48
	LANG_POLISH,		// 49
	LANG_PORTUGUESE,	// 50
	LANG_PUNJABI,		// 51
	LANG_ROMANIAN,		// 52
	LANG_RUSSIAN,		// 53
	LANG_SANSKRIT,		// 54
	LANG_SERBIAN,		// 55
	LANG_SINDHI,		// 56
	LANG_SLOVAK,		// 57
	LANG_SLOVENIAN,		// 58
	LANG_SPANISH,		// 59
	LANG_SWAHILI,		// 60
	LANG_SWEDISH,		// 61
	LANG_TAMIL,		// 62
	LANG_TATAR,		// 63
	LANG_TELUGU,		// 64
	LANG_THAI,		// 65
	LANG_TURKISH,		// 66
	LANG_UKRAINIAN,		// 67
	LANG_URDU,		// 68
	LANG_UZBEK,		// 69
	LANG_VIETNAMESE		// 70
};
#endif

int lv_curPrimaryLangID = 16;

void win32_InitLocale(void)
{
#ifdef _WIN32
	int lang_id = atoi(getIniKey(mchLocalINI,"language","primary_lang_id"));
	const char* locale_str = getIniKey(mchLocalINI,"language","locale");

	setlocale(LC_CTYPE,locale_str);

	if(lang_id) 
		lv_curPrimaryLangID = lang_id;
#endif
}

void win32_ChangeKeybLayout(void)
{
#ifdef _WIN32
	ActivateKeyboardLayout((HKL)HKL_NEXT,0);
#endif
}

int win32_GetKeybLayoutID(void)
{
#ifdef _WIN32
	int lang_id,primary_lang_id;
	static char name_str[KL_NAMELENGTH];

	GetKeyboardLayoutName(name_str);
	lang_id = strtol(name_str,NULL,16);

	primary_lang_id = PRIMARYLANGID(lang_id);

	if(primary_lang_id == LANG_ENGLISH)
		return 0;
	if(primary_lang_id == lv_primaryLangID[lv_curPrimaryLangID])
		return 1;

	return 2;
#endif
	return 0;
}

void lvInit(void)
{
#ifdef _LOCAL_VERSION_
	win32_InitLocale();
#endif
}

unsigned mch_toupper(unsigned ch)
{
	// CP1251 character table
	if (ch >= 0xE0 && ch <= 0xFF)
	{
		return 0xC0 + (ch - 0xE0); 
	}

	return std::toupper(ch);
}
