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
int iterations = 0;
float scale = 0.0f;
float exponent = 0.0f;
bool firstClick = true;
double mouseX = 0.0;
double mouseY = 0.0;

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
    glUniform2f(glGetUniformLocation(mainShader.id, "iResolution"), (float)screenWidth, (float)screenHeight);

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

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            if (firstClick)
            {
                glfwSetCursorPos(window, (screenWidth / 2), (screenHeight / 2));
                firstClick = false;
            }

            glfwGetCursorPos(window, &mouseX, &mouseY);

            mouseY = (float)(mouseY - (screenHeight / 2)) / screenHeight;
            mouseX = (float)(mouseX - (screenWidth / 2)) / screenWidth;

           // glfwSetCursorPos(window, (screenWidth / 2), (screenHeight / 2));
        }

        mainShader.activate();
        glUniform1f(glGetUniformLocation(mainShader.id, "iTime"), (float)glfwGetTime());
        glUniform1f(glGetUniformLocation(mainShader.id, "iDeltaTime"), Time::deltaTime);
        glUniform1f(glGetUniformLocation(mainShader.id, "scale"), scale);
        glUniform1f(glGetUniformLocation(mainShader.id, "exponent"), exponent);
        glUniform1i(glGetUniformLocation(mainShader.id, "iterations"), iterations);
        glUniform2f(glGetUniformLocation(mainShader.id, "iResolution"), (float)screenWidth, (float)screenHeight);
        glUniform2f(glGetUniformLocation(mainShader.id, "iMouse"), (float)mouseX, (float)mouseY);

        glBindVertexArray(fakeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGui::SetNextWindowSize(ImVec2(350, 100));
        ImGui::Begin("Variablen");
        ImGui::SliderInt("Iterationen", &iterations, 1, 50);
        ImGui::SliderFloat("Skalierung", &scale, 0.0f, 20.0f);
        ImGui::SliderFloat("Exponent", &exponent, 0.0f, 10.0f);
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