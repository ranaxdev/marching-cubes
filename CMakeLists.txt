cmake_minimum_required(VERSION 2.8)
project(MarchingCubes)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES Include/dirent.h Include/ImGuiFileDialogConfig.h Include/ImGuiFileDialog.h Source/Utils/ImGuiFileDialog.cpp Source/main.cpp Source/Harness.cpp Include/Harness.h Source/Shader.cpp Include/Shader.h Source/Camera.cpp Include/Camera.h Include/KeyListener.h Source/Utils/Logger.cpp Include/Logger.h Include/Globals.h Source/Renderer.cpp Include/Renderer.h Source/GUI/GUI.cpp Include/GUI.h Source/Utils/KeyListener.cpp)

set(IMGUI_SOURCE_FILES
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_impl_glfw.cpp
        external/imgui/imgui_impl_opengl3.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp)


#set(GLFW_DIR external/glfw/build)
#add_subdirectory(${GLFW_DIR})

set(glfw_DIR external/glfw/glfw-3.2.1)
add_subdirectory(${glfw_DIR})
#find_package(glfw3 REQUIRED)

set(GLAD_SOURCE_FILES external/glad/glad.c Include/Menu.h Source/GUI/Menu.cpp Include/MC.h Source/MC.cpp Include/Util.h Source/Utils/Util.cpp)


add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${IMGUI_SOURCE_FILES} ${GLAD_SOURCE_FILES})

include_directories(Include)

target_link_libraries(${PROJECT_NAME} glfw dl pthread)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/external/glfw/include")