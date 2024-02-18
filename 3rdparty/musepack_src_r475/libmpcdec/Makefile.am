EXTRA_DIST = CMakeLists.txt

AM_CPPFLAGS = -I$(top_srcdir)/include

if HAVE_VISIBILITY
  AM_CFLAGS = -fvisibility=hidden
endif

common_sources = ../common/crc32.c \
		../common/huffman-bcl.c

METASOURCES = AUTO
lib_LTLIBRARIES = libmpcdec.la
libmpcdec_la_SOURCES = huffman.c mpc_decoder.c mpc_reader.c \
	requant.c	streaminfo.c synth_filter.c mpc_bits_reader.c mpc_demux.c \
	mpc_bits_reader.h huffman.h decoder.h internal.h requant.h mpcdec_math.h \
	$(common_sources)

libmpcdec_la_LDFLAGS = -no-undefined -version-info 7:0:1
