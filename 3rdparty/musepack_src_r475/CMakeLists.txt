CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project(libmpcdec C)

include(TestBigEndian)
TEST_BIG_ENDIAN(MPC_ENDIANNESS)

add_definitions(-DFAST_MATH -DCVD_FASTLOG)

set(SOURCES
  common/crc32.c
  libmpcdec/huffman.c
  libmpcdec/mpc_bits_reader.c
  libmpcdec/mpc_decoder.c
  libmpcdec/mpc_demux.c
  libmpcdec/mpc_reader.c
  libmpcdec/requant.c
  libmpcdec/streaminfo.c
  libmpcdec/synth_filter.c
)

add_library(mpcdec STATIC ${SOURCES})
set_property(TARGET mpcdec PROPERTY C_STANDARD 99)
target_include_directories(mpcdec PUBLIC include)
