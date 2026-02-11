add_library(common STATIC)

target_include_directories(common PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/Inc
)

target_sources(common PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Scr/logger.cpp
)