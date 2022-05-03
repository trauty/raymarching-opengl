#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include "shader.h"
#include "time.h"

int screenWidth = 1280;
int screenHeight = 720;

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

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "raymarching", NULL, NULL);
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


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mainShader.activate();
        glUniform1f(glGetUniformLocation(mainShader.id, "iTime"), (float)glfwGetTime());

        glBindVertexArray(fakeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        Time::calculateDeltaTime();

        glfwPollEvents();
    }


    mainShader.dispose();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}