cmake_minimum_required(VERSION 3.10)

# set the project name
project(mrgls)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(mrgls main.cpp)

#add compile options
# add_compile_definitions(NDEBUG)
# add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

# Add sources
target_sources( mrgls PUBLIC
                )

target_sources( mrgls PRIVATE
                utils/utils.h       
                )
