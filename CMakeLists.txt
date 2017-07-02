cmake_minimum_required(VERSION 3.6)
project(Cap04)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

set(SFML_INCLUDE_PATH h:/ProyectosCLion/__LIBRERIAS__/SFML-2.4.1/include)
set(SFML_LIBRARY_PATH h:/ProyectosCLion/__LIBRERIAS__/SFML-2.4.1/lib)

set(SOURCE_FILES
        main.cpp Game.cpp Game.h
        Player.cpp Player.h Bullet.cpp Bullet.h)

include_directories(
        ${SFML_INCLUDE_PATH}
        #${GLEW_INCLUDE_PATH}
        #${GLM_INCLUDE_PATH}
)

link_directories(
        ${SFML_LIBRARY_PATH}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        -lsfml-main-d
        -lsfml-window-d
        -lsfml-graphics-d
        -lsfml-audio-d
        -lsfml-network-d
        -lsfml-system-d
        -lopengl32 # Ponerlo al final
        )

#${GLEW_LIBRARY}
#${OPENGL_LIBRARY} # Relleno por "find_package(OpenGL REQUIRED), poner en este orden"