cmake_minimum_required(VERSION 2.8)
project(wcrt)
set(WCRT_VERSION 1.12)

include_directories(include)
# Don't forget to install 'include'
add_subdirectory(src)
add_subdirectory(test)
