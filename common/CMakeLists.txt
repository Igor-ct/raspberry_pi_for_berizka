add_library(common STATIC)

target_include_directories(common PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(common PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/logger.cpp
)