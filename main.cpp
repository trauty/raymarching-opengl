#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include "shader.h"
#include "time.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int screenWidth = 1280;
int screenHeight = 720;

GLFWwindow* window;
int keyX = 0, keyY = 0;
double mouseX = 0.0, mouseY = 0.0;
int iterations = 0;
float scale = 0.0f;

void reactToFrameResize(GLFWwindow* window, int newWidth, int newHeight)
{
    glViewport(0, 0, newWidth, newHeight);
    screenWidth = newWidth;
    screenHeight = newHeight;
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screenWidth, screenHeight, "raymarching", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Error whilst creating window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, reactToFrameResize);
    gladLoadGL();

    glViewport(0, 0, screenWidth, screenHeight);

    Shader mainShader("main.vert", "main.frag");
    mainShader.activate();
    glUniform2f(glGetUniformLocation(mainShader.id, "iResolution"), screenWidth, screenHeight);

    GLuint fakeVAO;
    glGenVertexArrays(1, &fakeVAO);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glfwGetCursorPos(window, &mouseX, &mouseY);

        mainShader.activate();
        glUniform1f(glGetUniformLocation(mainShader.id, "iTime"), (float)glfwGetTime());
        glUniform1f(glGetUniformLocation(mainShader.id, "iDeltaTime"), Time::deltaTime);
        glUniform1f(glGetUniformLocation(mainShader.id, "scale"), scale);
        glUniform1i(glGetUniformLocation(mainShader.id, "iterations"), iterations);
        glUniform2f(glGetUniformLocation(mainShader.id, "iResolution"), screenWidth, screenHeight);
        glUniform2f(glGetUniformLocation(mainShader.id, "iMouse"), mouseX, mouseY);

        glBindVertexArray(fakeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGui::SetNextWindowSize(ImVec2(350, 100));
        ImGui::Begin("Variablen");
        ImGui::SliderInt("Iterationen", &iterations, 1, 50);
        ImGui::SliderFloat("Skalierung", &scale, 0.0f, 5.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        Time::calculateDeltaTime();

        glfwPollEvents();
    }


    mainShader.dispose();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}