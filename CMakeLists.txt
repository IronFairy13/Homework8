cmake_minimum_required(VERSION 3.12)

project(homework_8 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BAYAN_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(BAYAN_ENABLE_LTO "Enable link-time optimization if available" ON)

if (MSVC)
    add_compile_options(/permissive- /W4)
    if (BAYAN_WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if (BAYAN_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror)
    endif()
endif()

find_package(Boost REQUIRED COMPONENTS filesystem program_options regex)

set(BAYAN_SOURCES
    src/main.cpp
    src/app.cpp
    src/find_duplicates.cpp
    src/file_scan.cpp
    src/io_stats.cpp
    src/lazy_hasher.cpp
    src/lex_compare.cpp
    src/masks.cpp
    src/path_utils.cpp
)

add_executable(bayan ${BAYAN_SOURCES})

target_include_directories(bayan PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(bayan PRIVATE
    Boost::filesystem
    Boost::program_options
    Boost::regex
)
install(TARGETS bayan RUNTIME DESTINATION bin)

target_compile_features(bayan PRIVATE cxx_std_20)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "homework_8")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Savchenko")  

include(CPack)




