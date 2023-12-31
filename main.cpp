// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#define MAX_STRING_LENGTH 260 //max string length for directory defined in CLR (win32)


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "fmod.h"
#include <string>
#include <stdio.h>
#include <filesystem>
#include <windows.h>
namespace fs = std::filesystem;
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif
#include "imgui/imgui.h"
#include "imgui/examples/libs/glfw/include/GLFW/glfw3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include <iostream>
#include "fmod_audio.h"


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    //bool show_demo_window = true;
    bool show_demo_window = true;
    bool show_another_window = false;
    bool show_file_browser_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    fmodAudio AudioPlayer;

    //Variables for when we want to look through the directory
    std::filesystem::path directory = "D:\\Tu\\imgui-music-player\\media";



    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        
        //ImGui::ShowDemoWindow(&show_demo_window);


        // DIRECTORY ACCESS
        {

            //variables for this scope
            char tmpCharArray[MAX_STRING_LENGTH];
            char drives[MAX_STRING_LENGTH];

            // Specifying initial position and size of window.
            ImGui::SetNextWindowPos(ImVec2(20,20), 2, ImVec2(0, 0)); //x,y , 2 means only at start, changeable
            ImGui::SetNextWindowSize(ImVec2(500,400),2); // width, height, 2 means only at start, changeable

            ImGui::Begin("Directory access");

            ImGui::Text("Directory:");
            ImGui::SameLine();
            int test = GetLogicalDriveStrings(MAX_STRING_LENGTH,drives);
            //Set default content of input text to default value of directory, 
            //the string copy is work around for the requirement of having to use a char* instead of const char*
            strcpy(tmpCharArray, directory.string().c_str());
            if (ImGui::InputText(" ", tmpCharArray, MAX_STRING_LENGTH)) {
                directory = tmpCharArray;
            }

            if (ImGui::Button("Back")) {
                if (directory.has_parent_path()) directory = directory.parent_path();
            }

            //Why plus 4 ? strings obtained are of length for, because C:\ or D:\ and an end string character after that. Why does it work weird, as in every character goes in a slot and not a string a slot ? cuz char buffer and we use address (see & symbols)
            ImGui::Text("Drives :");
            for (int i = 0; i < test; i=i+4) {
                ImGui::SameLine();
                if (ImGui::Button(&drives[i])) {
                    directory = std::string(&drives[i]);
                }
            }
            if (ImGui::Button("Stop audio"))
            {   
                bool isPlaying;
                AudioPlayer.channelGroup->isPlaying(&isPlaying);
                if (isPlaying)
                {
                    AudioPlayer.channelGroup->stop();
                    
                }
            }
            ImGui::BeginChild("directoryContent");
            try {
                for (const auto& entry : fs::directory_iterator(directory)) {

                    if (ImGui::Selectable(entry.path().string().c_str(), true)) {
                        //If its a directory, go into it, files are to be dealt with later
                        if (entry.is_directory()) {
                            directory = entry.path();
                        }
                        else
                        {
                            try
                            {
                                std::string path_string{ entry.path().string() };
                                bool isPlaying;
                                AudioPlayer.channelGroup->isPlaying(&isPlaying);
								if (isPlaying)
								{
									AudioPlayer.channelGroup->stop();
								}
                                AudioPlayer.playAudioFromAudioFilePath(path_string);
                            }
                            catch (const std::exception& e)
                            {
                                std::cout << e.what();
                            }
                        }
                    }

                }
            }
            catch (const std::overflow_error& e)
            {
            } // this executes if f() throws std::overflow_error (same type rule)
            catch (const std::runtime_error& e)
            {
            } // this executes if f() throws std::underflow_error (base class rule)
            catch (const std::exception& e)
            {
            } // this executes if f() throws std::logic_error (base class rule)
            catch (...)
            {
            }
            ImGui::EndChild();

            //PROGRESS BAR
            try {
                int index=0;
                unsigned int totalLength = 0;
                std::string tmpLength = " ";
                std::string tmpPosition = " ";
                unsigned int position = 0;
                bool isPlaying;
                float percentageFilled = 0.0;
                AudioPlayer.channelGroup->isPlaying(&isPlaying);
                if (isPlaying) {
                    AudioPlayer.sound->getLength(&totalLength, FMOD_TIMEUNIT_MS);
                    AudioPlayer.channel->getPosition(&position, FMOD_TIMEUNIT_MS);

                    //Prepare the string so overlay displayed on bar is proper
                    percentageFilled = position / (float)totalLength;
                    tmpLength = std::to_string((float)totalLength / 1000);
                    for (int i = 0; i < tmpLength.length(); i++) {
                        if (tmpLength[i] == '.') {
                            index = i + 1;
                            break;
                        }
                    }
                    tmpLength = tmpLength.substr(0, index + 2);
                    tmpPosition = std::to_string((float)position / 1000);
                    for (int i = 0; i < tmpPosition.length(); i++) {
                        if (tmpPosition[i] == '.') {
                            index = i + 1;
                            break;
                        }
                    }
                    tmpPosition = tmpPosition.substr(0, index + 2);
                    
                }
                std::string stringOverlay = tmpPosition + ":" + tmpLength;
                
                //first parameter is how much the bar is filled up, receives a float, below 1 (0.3 means 30%)
                //second parameter is the allignment of the bar, 0 means auto alligned
                //3rd is the text on the bar, if none is specified, percentage number according to first parameter will be had.
                ImGui::ProgressBar(percentageFilled, ImVec2(0, 0), stringOverlay.c_str());
            }
            catch (std::exception& e) {
                std::cout << e.what();
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
