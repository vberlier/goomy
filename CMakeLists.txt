cmake_minimum_required(VERSION 3.14)
project(goomy)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        include/goomy/Engine.h
        include/goomy/EntityRegistry.h
        include/goomy/SignalDispatcher.h
        include/goomy.h
        src/EngineBase.cpp include/goomy/EngineBase.h)

target_include_directories(${PROJECT_NAME} PUBLIC include)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(examples/basic)
add_subdirectory(examples/sfml)
add_subdirectory(examples/singlefile)
