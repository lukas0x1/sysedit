cmake_minimum_required(VERSION 3.26)
project(saveeditor)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /Zc:strictStrings-")
add_executable(saveeditor main.cpp
        include/imgui/imgui.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/backends/imgui_impl_dx9.cpp
        include/imgui/backends/imgui_impl_win32.cpp

        gui.cpp
        gui.h
        include/imgui/backends/imgui_impl_win32.cpp
        main.h
        systemdata.cpp
        systemdata.h)


set_target_properties(saveeditor PROPERTIES WIN32_EXECUTABLE TRUE)
target_link_libraries(saveeditor d3d9.lib dwmapi)
