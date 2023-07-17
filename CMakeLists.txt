cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project("raymarching-opengl")

add_executable(raymarching-opengl src/main.cpp src/glad.c src/shader.cpp src/time.cpp lib/imgui/imgui.cpp lib/imgui/imgui_draw.cpp lib/imgui/imgui_impl_glfw.cpp
									lib/imgui/imgui_impl_opengl3.cpp lib/imgui/imgui_tables.cpp lib/imgui/imgui_widgets.cpp)

target_link_options(raymarching-opengl PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})

target_link_libraries(raymarching-opengl ${CMAKE_SOURCE_DIR}/lib/libglfw3.a)

set_property(TARGET raymarching-opengl PROPERTY CXX_STANDARD 17)

target_include_directories(raymarching-opengl PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_custom_command(TARGET raymarching-opengl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/shaders"
        $<TARGET_FILE_DIR:raymarching-opengl>)