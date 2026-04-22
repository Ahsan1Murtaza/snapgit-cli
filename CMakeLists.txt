cmake_minimum_required(VERSION 3.16)

project(SnapGit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE SNAPGIT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/CommandHandlers/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Helper/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Objects/*.cpp"
)

add_executable(mygit
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    ${SNAPGIT_SOURCES}
)

target_include_directories(mygit PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/CommandHandlers"
    "${CMAKE_CURRENT_SOURCE_DIR}/Helper"
    "${CMAKE_CURRENT_SOURCE_DIR}/Objects"
)

target_link_libraries(mygit PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)
