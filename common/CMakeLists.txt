add_library(common STATIC)

target_include_directories(common PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(common PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/UartInterface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ConfigManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MessageParser.cpp
)

target_link_libraries(common PRIVATE jsoncpp_lib)