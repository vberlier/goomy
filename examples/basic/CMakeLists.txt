project(example_basic)

add_executable(${PROJECT_NAME}
        main.cpp
        Configuration.cpp
        Configuration.h
        Engine.h
        Foo.cpp
        Foo.h
        Bar.cpp
        Bar.h
        example.h
        )

target_link_libraries(${PROJECT_NAME} PUBLIC goomy)