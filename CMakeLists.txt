cmake_minimum_required(VERSION 3.16)

project(SnapGit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE SNAPGIT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/commands/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/objects/*.cpp"
)

add_executable(mygit
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/main.cpp"
    ${SNAPGIT_SOURCES}
)

target_include_directories(mygit PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cli"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/commands"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/utils"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/objects"
)

target_link_libraries(mygit PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)
