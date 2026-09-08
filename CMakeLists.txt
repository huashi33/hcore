cmake_minimum_required(VERSION 3.10)
project(hcore C)


set(CMAKE_INSTALL_PREFIX "/home/huashi/code/")
set(CMAKE_BUILD_TYPE Debug)



include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)
link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB HC_SRCS "src/*.c")

# to object library
add_library(hcore_obj OBJECT ${HC_SRCS})
set_target_properties(hcore_obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

# to shared and static library
add_library(hcore_shared SHARED $<TARGET_OBJECTS:hcore_obj>)
add_library(hcore_static STATIC $<TARGET_OBJECTS:hcore_obj>)
set_target_properties(hcore_shared hcore_static PROPERTIES OUTPUT_NAME hcore)
target_link_libraries(hcore_shared PRIVATE zlog iniparser)
target_link_libraries(hcore_static PRIVATE zlog iniparser)

# install
# file(GLOB HC_INCS "*.h")
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include/hcore
)
# install(
#     DIRECTORY ${CMAKE_SOURCE_DIR}/lib
#     DESTINATION .
# )
install(TARGETS
    hcore_static
    hcore_shared
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)



add_subdirectory(test)
