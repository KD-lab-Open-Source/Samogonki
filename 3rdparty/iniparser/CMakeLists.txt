project(iniparser C)

add_library(iniparser
    src/dictionary.c
    src/iniparser.c
)

target_include_directories(iniparser PUBLIC include)
