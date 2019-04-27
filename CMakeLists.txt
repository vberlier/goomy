cmake_minimum_required(VERSION 3.14)
project(goomy)

set(CMAKE_CXX_STANDARD 17)
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")

add_library(${PROJECT_NAME}
        include/goomy/Component.hpp
        include/goomy/Engine.hpp
        include/goomy/EngineBase.hpp
        src/EngineBase.cpp
        include/goomy/Entity.hpp
        include/goomy/Registry.hpp
        include/goomy/System.hpp
        include/goomy/Signal.hpp
        include/goomy/Wrapper.hpp
        include/goomy.hpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(examples/basic)
add_subdirectory(examples/sfml)
