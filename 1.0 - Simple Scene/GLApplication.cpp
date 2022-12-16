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
#include "Floor.hpp"
#include "imgui_impl_glfw.h"

#include "Imgui/imgui.h"
#include "imgui_impl_opengl3.h"

//////////////////////////////////////////////////////////////////////


int windowWidth = 1024;
int windowHeight = 768;
int displayIndex = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
bool camLock = true;
Graphic* graphic;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        camLock = !camLock;
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const int window_width = mode->width;
    const int window_height = mode->height;

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
                                          "GAM400 - Massive Animating Objects",
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
    glfwSetScrollCallback(window, scroll_callback);

//#if DEBUG
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
//#endif

    graphic = new Graphic(window_width, window_height);
    graphic->window = window;

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    int counter = 0;
    //Floor* floor = graphic->floor;
    //Camera* cam = graphic->cam;
    int addingNum = 200;
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
        
//#if DEBUG
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("ImGui");

        static int item_current_idx = 0; // Here we store our selection data as an index.
        const char* swatItems[] = { "SWAT_RifleAim", "SWAT_RifleCrouch", "SWAT_RifleIdle",
			"SWAT_RifleWalk", "SWAT_Run", "SWAT_SteppingBackward", "SWAT_Strafing", "SWAT_TurnLeft",
			"SWAT_CrawlBackward", "SWAT_Death", "SWAT_Reload", "SWAT_Reload2", "SWAT_Smash",
        };
        const char* amyItems[] = { "AMY_Excited", "AMY_Floating", "AMY_HappyIdle",
			"AMY_Jogging", "AMY_Singing", "AMY_StandingUp", "AMY_Surprised", "AMY_TalkingOnPhone",
			"AMY_TwistDance"
        };
        const char* knightsItems[] = { "KNIGHT_Attack",  "KNIGHT_Block",
            "KNIGHT_BlockIdle", "KNIGHT_CrouchBlockIdle", "KNIGHT_Death", "KNIGHT_Idle", "KNIGHT_Idle2",
            "KNIGHT_Impact", "KNIGHT_PowerUp", "KNIGHT_Run", "KNIGHT_Slash"
        };
        const char* michelleItems[] = { "MICHELLE_BreakDance",  "MICHELLE_BreakDanceReady",
			"MICHELLE_Dancing", "MICHELLE_DancingTwerk", "MICHELLE_Flair", "MICHELLE_HipHopDance1", "MICHELLE_HipHopDance2",
			"MICHELLE_HipHopDance3", "MICHELLE_HipHopDance4", "MICHELLE_RumbaDance", "MICHELLE_Twist",
            "MICHELLE_WaveHipHop"
        };

        const char* adamItems[] = { "ADAM_ComboPunch",  "ADAM_ElbowPunch",
			"ADAM_Idle", "ADAM_Idle2", "ADAM_Jab", "ADAM_MMAKick", "ADAM_Victory"
        };

        const int swatArraySize = IM_ARRAYSIZE(swatItems);
        const int amyArraySize = IM_ARRAYSIZE(amyItems);
        const int knightArraySize = IM_ARRAYSIZE(knightsItems);
        const int michelleArraySize = IM_ARRAYSIZE(michelleItems);
        const int adamArraySize = IM_ARRAYSIZE(adamItems);

        int arrayOffset = 0;

        if (ImGui::TreeNode("SWAT"))
        {
            if (ImGui::BeginListBox("SWAT Animations"))
            {
                for (int n = 0; n < swatArraySize; n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(swatItems[n], is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            if(ImGui::Button("Add All"))
            {
	            for(int i = 0; i < swatArraySize; ++i)
	            {
                    graphic->PopulateObjs(200, i);
	            }
            }

            ImGui::TreePop();
        }
        arrayOffset += swatArraySize;

        if(ImGui::TreeNode("AMY"))
        {
	        if (ImGui::BeginListBox("AMY Animations"))
	        {
	            for (int n = arrayOffset; n < amyArraySize + arrayOffset; n++)
	            {
	                const bool is_selected = (item_current_idx == n);
	                if (ImGui::Selectable(amyItems[n - arrayOffset], is_selected))
	                    item_current_idx = n;

	                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	                if (is_selected)
	                    ImGui::SetItemDefaultFocus();
	            }
	            ImGui::EndListBox();
	        }
            if (ImGui::Button("Add All"))
            {
                for (int i = arrayOffset; i < amyArraySize + arrayOffset; ++i)
                {
                    graphic->PopulateObjs(200, i);
                }
            }

            ImGui::TreePop();
        }
        arrayOffset += amyArraySize;

        if (ImGui::TreeNode("KNIGHT"))
        {
            if (ImGui::BeginListBox("KNIGHT Animations"))
            {
                for (int n = arrayOffset; n < knightArraySize + arrayOffset; n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(knightsItems[n - arrayOffset], is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            if (ImGui::Button("Add All"))
            {
                for (int i = arrayOffset; i < knightArraySize + arrayOffset; ++i)
                {
                    graphic->PopulateObjs(200, i);
                }
            }

            ImGui::TreePop();
        }

        arrayOffset += knightArraySize;

        if (ImGui::TreeNode("MICHELLE"))
        {
            if (ImGui::BeginListBox("MICHELLE Animations"))
            {
                for (int n = arrayOffset; n < michelleArraySize + arrayOffset; n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(michelleItems[n - arrayOffset], is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            if (ImGui::Button("Add All"))
            {
                for (int i = arrayOffset; i < michelleArraySize + arrayOffset; ++i)
                {
                    graphic->PopulateObjs(200, i);
                }
            }

            ImGui::TreePop();
        }

        arrayOffset += michelleArraySize;

        if (ImGui::TreeNode("ADAM"))
        {
            if (ImGui::BeginListBox("ADAM Animations"))
            {
                for (int n = arrayOffset; n < adamArraySize + arrayOffset; n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(adamItems[n - arrayOffset], is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            if (ImGui::Button("Add All"))
            {
                for (int i = arrayOffset; i < adamArraySize + arrayOffset; ++i)
                {
                    graphic->PopulateObjs(200, i);
                }
            }

            ImGui::TreePop();
        }


        ImGui::SliderInt("Amount", &addingNum, 200, 1000);
        if (ImGui::Button("Add objects"))
            graphic->PopulateObjs(addingNum, item_current_idx);
        if (ImGui::Button("Delete objects"))
            graphic->DeleteObjs(addingNum);

        ImGui::Text("Space  : Toggle cam moving");
        ImGui::Text("W/A/S/D: Cam movement");
        ImGui::Text("Mouse  : Cam rotation");
        ImGui::Text(fpsDisplay.c_str());
        
        ImGui::Text("TotalRendering : %d", graphic->totalRenderingAmount);

        //ImGui::SliderFloat3("Pos", &graphic->floor->pos.x, 600.f, 1000.f);
        //ImGui::SliderFloat3("scale", &graphic->floor->scale.x, 1.f, 1500.f);
        
        ImGui::End();
//#endif

        float currentFrame = glfwGetTime();
        graphic->deltaTime = currentFrame - graphic->lastFrame;
        graphic->lastFrame = currentFrame;

        graphic->ProcessInput();
        graphic->Draw();

//#if DEBUG
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//#endif

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
//#if DEBUG
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
//#endif
    glfwTerminate();
    delete graphic;

    return 0;
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if(!camLock)
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
    }
    else
    {
        lastX = static_cast<float>(xposIn);
        lastY = static_cast<float>(yposIn);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= static_cast<float>(yoffset);
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}