/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: main.
 * Copyright © 2022 DigiPen (USA) LLC. and its owners. All Rights Reserved.
 */


#include <Windows.h>
// Include standard headers
#include <cstdio>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#define GLM_ENABLE_EXPERIMENTAL

// Local / project headers
#include <iostream>

#include "Camera.hpp"
#include "Graphic.h"
#include "Shader.h"

#include "AnimationModel.h"
//////////////////////////////////////////////////////////////////////

int displayIndex = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
//bool camLock = true;
Graphic* graphic;
bool cursorHidden = true;
bool middleMouseButtonPressed = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (scancode);
    (mods);

    if(key == GLFW_KEY_V && action == GLFW_PRESS)
    {
	    if(cursorHidden)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        cursorHidden = !cursorHidden;
    }
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
int main()
{
    bool oneTime = true;
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const int window_width = 1024;//mode->width;
    const int window_height = 768;//mode->height;

    // Setting up OpenGL properties
    glfwWindowHint(GLFW_SAMPLES, 1); // change for anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow* window = glfwCreateWindow(window_width, window_height,
                                          "GAM450 - OpenGL Battle Simulation",
                                          //glfwGetPrimaryMonitor(),
                                          nullptr,
                                          nullptr); 

    glViewport(0, 0, window_width, window_height);

    if (window == nullptr)
    {
        fprintf(stderr,
                "Failed to open GLFW window. Check if your GPU is 4.0 compatible.\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = static_cast<GLboolean>(true); // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, &key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

#if DEBUG
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
#endif

    graphic = new Graphic(window_width, window_height);
    graphic->window = window;

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    int counter = 0;
    std::string fpsDisplay = "";
    do
    {
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;

        if(timeDiff >= 1.0f / 30.0f)
        {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            fpsDisplay = "Animation - FPS : " + FPS; //+ "FPS / " + ms + "ms";
            glfwSetWindowTitle(window, fpsDisplay.c_str());
            prevTime = crntTime;
            counter = 0;
        }
        
#if DEBUG
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("ImGui");
        ImGui::Text("Space  : Toggle cam moving");
        ImGui::Text("W/A/S/D: Cam movement");
        ImGui::Text("Mouse  : Cam rotation");
        ImGui::Text("V      : Enable mouse cursor");
        ImGui::Text("L-Click: Select herd");
        ImGui::Text("R-Click: Move selected herd");
        ImGui::Text(fpsDisplay.c_str());
        ImGui::End();
#endif

        if(oneTime == true)
        {
            graphic->lastFrame = static_cast<float>(glfwGetTime());
            oneTime = false;
        }

        float currentFrame = static_cast<float>(glfwGetTime());
        graphic->deltaTime = currentFrame - graphic->lastFrame;
        graphic->lastFrame = currentFrame;

        graphic->ProcessInput();
        graphic->Draw();

#if DEBUG
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
#if DEBUG
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif
    glfwTerminate();
    delete graphic;

    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    (mods);
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        graphic->GetMousePosInWorldCoord(
            static_cast<float>(xPos), static_cast<float>(yPos));

        graphic->SelectHerd();
	}
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        graphic->GetMousePosInWorldCoord(
            static_cast<float>(xPos), static_cast<float>(yPos));

        graphic->ForwardToPickedPos();
    }

    else if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        middleMouseButtonPressed = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        middleMouseButtonPressed = false;
    }
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    (window);

    if(middleMouseButtonPressed)
    {
	    const float xpos = static_cast<float>(xposIn);
	    const float ypos = static_cast<float>(yposIn);

	    if (firstMouse)
	    {
	        lastX = xpos;
	        lastY = ypos;
	        firstMouse = false;
	    }

	    const float xoffset = xpos - lastX;
	    const float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        graphic->currentCam->ProcessMouseMovement(xoffset, yoffset);

    	lastX = xpos;
	    lastY = ypos;

        //lets check
    }
    else
    {
        lastX = static_cast<float>(xposIn);
        lastY = static_cast<float>(yposIn);
    }
}

