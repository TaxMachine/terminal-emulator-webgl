//
// Created by TaxMachine on 2024-05-28.
//

#include "gui.hpp"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <imgui_internal.h>
#include "imterm/terminal.hpp"

#include "commands/Command.hpp"

GLFWwindow *window;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

#ifdef EMSCRIPTEN

EM_JS(int, canvas_get_width, (), {
  return Module.canvas.width;
});

// Function used by c++ to get the size of the html canvas
EM_JS(int, canvas_get_height, (), {
  return Module.canvas.height;
});

// Function called by javascript
EM_JS(void, resizeCanvas, (), {
  js_resizeCanvas();
});

#endif

void on_size_changed() {
    glfwSetWindowSize(window, g_width, g_height);

    ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void createImGuiWindow() {
    glfwGetWindowSize(window, &g_width, &g_height);

    ImTerm::terminal<Command> terminal_log;
    terminal_log.set_min_log_level(ImTerm::message::severity::info);
    terminal_log.set_flags(ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoMove |
                           ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_NoBringToFrontOnFocus |
                           ImGuiWindowFlags_NoNavFocus |
                           ImGuiWindowFlags_NoDocking |
                           ImGuiWindowFlags_NoSavedSettings);
    terminal_log.set_size(ImVec2((float)g_width, (float)g_height - 20));
    terminal_log.show();
}

void GUI::loop() {
    if (!glfwInit())
        return;
    glfwSetErrorCallback(glfw_error_callback);
    window = glfwCreateWindow(600, 600, "terminal emulator", nullptr, nullptr);
    if (window == nullptr)
        return;
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigWindowsResizeFromEdges = false;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        createImGuiWindow();
        ImGui::Render();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        int app_width, app_height;
        glfwGetFramebufferSize(window, &app_width, &app_height);
        glViewport(0, 0, app_width, app_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

void GUI::quit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}