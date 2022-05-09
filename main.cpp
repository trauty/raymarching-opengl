#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include "shader.h"
#include "time.h"

int screenWidth = 1280;
int screenHeight = 720;

GLFWwindow* window;
int keyX = 0, keyY = 0;

void reactToFrameResize(GLFWwindow* window, int newWidth, int newHeight)
{
    glViewport(0, 0, newWidth, newHeight);
    screenWidth = newWidth;
    screenHeight = newHeight;
}

void calculateKeyPress()
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        keyY = 1;
        std::cout << keyY;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        keyY = -1;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        keyX = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        keyX = -1;
    }
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

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mainShader.activate();
        glUniform1f(glGetUniformLocation(mainShader.id, "iTime"), (float)glfwGetTime());
        glUniform1f(glGetUniformLocation(mainShader.id, "iDeltaTime"), Time::deltaTime);
        glUniform2f(glGetUniformLocation(mainShader.id, "iKeyboard"), (float)keyX, (float)keyY);

        glBindVertexArray(fakeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        calculateKeyPress();

        Time::calculateDeltaTime();

        glfwPollEvents();
    }


    mainShader.dispose();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}