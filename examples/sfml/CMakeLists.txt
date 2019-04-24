project(example_sfml)

find_package(SFML REQUIRED graphics)
include_directories(${SFML_INCLUDE_DIR})

add_compile_definitions(EXAMPLE_SFML_RESOURCES_DIR="${PROJECT_SOURCE_DIR}/resources")

add_executable(${PROJECT_NAME}
        systems/AgeSystem.cpp
        systems/AgeSystem.h
        systems/Overlay.cpp
        systems/Overlay.h
        systems/ParticleSystem.cpp
        systems/ParticleSystem.h
        systems/ResourceManager.cpp
        systems/ResourceManager.h
        systems/Window.cpp
        systems/Window.h
        Engine.h
        main.cpp
        main.h
        )

include_directories(${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC goomy sfml-graphics)