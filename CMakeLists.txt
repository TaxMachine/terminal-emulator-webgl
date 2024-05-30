cmake_minimum_required(VERSION 3.28)
project(terminal_animation)

set(CMAKE_CXX_STANDARD 23)
file(GLOB_RECURSE SOURCES "src/**")

find_package(glm CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)

set(GLFW_DIR ${CMAKE_SOURCE_DIR}/libs/glfw)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)
set(STB_DIR ${CMAKE_SOURCE_DIR}/libs/stb)
set(IMTERM_DIR ${CMAKE_SOURCE_DIR}/libs/ImTerm)

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
include_directories(${GLFW_DIR}/deps)
include_directories(${STB_DIR})
include_directories(${IMTERM_DIR}/include)

add_executable(terminal_animation
        ${SOURCES}
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

if (EMSCRIPTEN)
    message("Building for web")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s USE_WEBGL2=1 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s FETCH=1 -s NO_DISABLE_EXCEPTION_CATCHING --bind")
    file(COPY ${CMAKE_SOURCE_DIR}/assets/imgui.html DESTINATION ${CMAKE_BINARY_DIR})
endif ()

target_link_libraries(terminal_animation PRIVATE glfw glm::glm freetype opengl32)