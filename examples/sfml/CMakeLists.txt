project(example_sfml)

find_package(SFML REQUIRED graphics)
include_directories(${SFML_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
        Engine.h
        ExampleSystem.cpp
        ExampleSystem.h
        main.cpp
        main.h
        Window.cpp
        Window.h
        )

target_link_libraries(${PROJECT_NAME} PUBLIC goomy sfml-graphics)