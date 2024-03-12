cmake_minimum_required(VERSION 3.7)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
project(opengl_shooter)

add_subdirectory(lib/glfw)
add_subdirectory(lib/cglm)

include_directories(lib/glad/include)
include_directories(lib/stb)
add_executable(
    ${PROJECT_NAME} 
    lib/glad/src/glad.c
    src/main.c
    src/gfx/window.c
    src/gfx/vao.c
    src/gfx/buffer.c
    src/gfx/vertex.c
    src/gfx/shader.c
    src/gfx/texture.c
    src/gfx/text.c
    src/entity/player.c
    src/entity/rock.c
    src/entity/collider.c)

add_compile_options(${PROJECT_NAME} PUBLIC "-std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wpointer-arith -Wstrict-aliasing -Wno-unused-parameter -Wno-missing-field-initializers")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw cglm)
