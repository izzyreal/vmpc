cmake_minimum_required(VERSION 2.8.12)

project(vmpc)
set(CMAKE_CXX_STANDARD 17)

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
else()
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
endif()

conan_basic_setup(NO_OUTPUT_DIRS)

## Configure main lib ##

set(_src_root_path_main "${CMAKE_CURRENT_SOURCE_DIR}/main/cpp")

include_directories(${_src_root_path_main})

file(
    GLOB_RECURSE _source_list_main 
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${_src_root_path_main}/.c*"
    "${_src_root_path_main}/*.cpp*"
    "${_src_root_path_main}/*.hpp*"
	"${_src_root_path_main}/*.hxx*"
)

foreach(_source IN ITEMS ${_source_list_main})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

add_executable(vmpc ${_source_list_main})
conan_target_link_libraries(vmpc)


if(MSVC)
    SET(CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO)
	add_custom_command(TARGET
                     vmpc
                   POST_BUILD
                   COMMAND
                     "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}/main/resources/dpiaware.manifest" -outputresource:\"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/vmpc.exe"\;\#1
                   COMMENT
                     "Adding custom manifest for dpi awareness..." 
                  )
endif(MSVC)

## Configure test suite application ##

set(_src_root_path_test "${CMAKE_CURRENT_SOURCE_DIR}/test")

include_directories(${_src_root_path_test})

file(
    GLOB_RECURSE _source_list_test 
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${_src_root_path_test}/*.c*"
    "${_src_root_path_test}/*.cpp*"
    "${_src_root_path_test}/*.hpp*"
)

foreach(_source IN ITEMS ${_source_list_test})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

add_executable(vmpc-tests ${_source_list_test})
conan_target_link_libraries(vmpc-tests)
