project(example_sfml)

find_package(SFML REQUIRED graphics)
include_directories(${SFML_INCLUDE_DIR})

add_compile_definitions(EXAMPLE_SFML_RESOURCES_DIR="${PROJECT_SOURCE_DIR}/resources")

add_executable(${PROJECT_NAME}
        systems/AgeSystem.cpp
        systems/AgeSystem.hpp
        systems/Display.cpp
        systems/Display.hpp
        systems/InputEventsDispatcher.cpp
        systems/InputEventsDispatcher.hpp
        systems/ParticleSystem.cpp
        systems/ParticleSystem.hpp
        systems/ResourceManager.cpp
        systems/ResourceManager.hpp
        systems/Window.cpp
        systems/Window.hpp
        Engine.hpp
        main.cpp
        main.hpp
        )

include_directories(${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC goomy sfml-graphics)