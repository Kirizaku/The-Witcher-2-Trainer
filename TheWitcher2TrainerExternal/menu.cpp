/*
* Copyright (c) 2022 Rudeus Kirigaya <https://github.com/kirizaku>
*
* This program is free software: you can redistribute itand /or modify it under the terms of the GNU
* General Public License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.

* You should have received a copy of the GNU General Public License along with this program. If not, see
* <https://www.gnu.org/licenses/>.
*/

#include <filesystem>

#include "menu.h"
#include "stb_image.h"
#include "config/config.h"
#include "../language.h"

#include "license.h"

#pragma comment(lib, "winmm.lib")

bool ImGui::ShowStyleSelector(const char* label) 
{
    settings st; Language lang;

    if (ImGui::Combo(label, &st.style_idx_, lang.kThemeComboSelector))
    {
        switch (st.style_idx_)
        {
            case 0: ImGui::StyleColorsDarkV2(); break;
            case 1: ImGui::StyleColorsLightV2(); break;
        }
        return true;
    }
    return false;
}

void CheckProcess()
{
    Hacks hk;

    while (true)
    {
        globals::pid = mem::get_pid(globals::kProcessName);
        globals::process_check = mem::get_process_name(globals::pid);

        if (globals::process_check == globals::kProcessName) {
            hk.is_running_ = true;
            globals::process_mod = mem::get_module(globals::pid, globals::kModuleWitcher);
            globals::process_mod_hook = mem::get_module(globals::pid, globals::kModuleHook);

            if (!globals::process_mod_hook) {
                std::filesystem::path cwd = std::filesystem::current_path() / "TheWitcher2TrainerHook.dll";
                globals::process_mod_hook = mem::load_module(globals::pid, cwd.string().c_str()); hk.is_hooking_ = true;
                std::ofstream o("config\\hook.json"); Config().SaveHookConfig();
            }
        }
        else { hk.is_running_ = false; }

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

void menu::Initialize()
{
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
 
    window_ = glfwCreateWindow(IMGUI_WINDOW_W, IMGUI_WINDOW_H, "The Witcher 2: Assassins of Kings Enhanced Edition [Current Steam] [29.04.2022]", 0, 0);
    if (window_ == 0) { MessageBox(0, "Failed to create window", "Glfw Error", MB_OK); exit(1); }

    glfwSetWindowSizeLimits(window_, IMGUI_WINDOW_W, IMGUI_WINDOW_H, IMGUI_WINDOW_W, IMGUI_WINDOW_H);
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
    
    GLFWimage icons[1];
    icons[0].pixels = stbi_load("assets/icon.png", &icons[0].width, &icons[0].height, 0, 4);
    glfwSetWindowIcon(window_, 1, icons);
    stbi_image_free(icons[0].pixels);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    menu::Style();
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::thread process_search_thr(CheckProcess);
    process_search_thr.detach();

    Language lang;
    switch (settings::lang_)
    {
        case 0: lang.LangEng(); break;
        case 1: lang.LangRus(); break;
    }

    menu::Draw();
    menu::Shutdown();
}

void menu::Draw() 
{
    settings st; Hacks hk; Language lang;

    std::filesystem::path cwd = std::filesystem::current_path() / "assets\\hotkey.wav";

    while (!glfwWindowShouldClose(window_))
    {
        glfwPollEvents();
        glfwGetWindowSize(window_, &width_, &height_);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(width_, height_));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

        if (!st.is_sound) { waveOutSetVolume(0, 0); } else { waveOutSetVolume(0, -1); }

        ImGui::Begin("Main", 0, flags);
        {
            if (ImGui::BeginTabBar("MainTab"))
            {
                if (ImGui::BeginTabItem(lang.kFunctionsLabel))
                {
                    ImGui::BeginChild("LeftSide", ImVec2(250, ImGui::GetContentRegionAvail().y), true);
                    {
                        if (hk.is_running_ == false) { 
                            ImGui::Text(lang.kWaitForGameLabel); 
                            ImGui::Text(lang.kProcIdLabel, 0); }
                        else { 
                            ImGui::Text(lang.kProcIdLabel, globals::pid); 
                            ImGui::Text(lang.kProcNameLabel, globals::process_check.c_str()); 
                        }

                        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 165);

                        ImGui::PushFont(Ubuntu_Bold);
                        ImGui::Text(lang.kSourceCode);
                        ImGui::PopFont();

                        if (ImGui::Button("Github", ImVec2(ImGui::GetContentRegionAvail().x, 29)))
                        {
                            ShellExecute(0, 0, "https://github.com/Kirizaku/The-Witcher-2-Trainer", 0, 0, SW_SHOW);
                        }

                        ImGui::PushFont(Ubuntu_Bold);
                        ImGui::Text(lang.kSupportProjectLabel);
                        ImGui::PopFont();

                        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0.94f, 0.47f, 0.17f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0.74f, 0.40f, 0.17f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0.74f, 0.35f, 0.17f));
                        if (ImGui::Button("Boosty", ImVec2(ImGui::GetContentRegionAvail().x, 29)))
                        {
                            ShellExecute(0, 0, "https://boosty.to/kirizaku", 0, 0, SW_SHOW);
                        }
                        ImGui::PopStyleColor(3);

                        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(1.0f, 0.549f, 0.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(1.0f, 0.48f, 0.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(1.0f, 0.45f, 0.0f));
                        if (ImGui::Button("QIWI", ImVec2(ImGui::GetContentRegionAvail().x, 29)))
                        {
                            ShellExecute(0, 0, "https://www.qiwi.com/n/KIRIZAKU", 0, 0, SW_SHOW);
                        }
                        ImGui::PopStyleColor(3);
                    }

                    ImGui::EndChild();
                    {
                        ImGui::SameLine(0);
                        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
                        ImGui::SameLine();
                    }

                    ImGui::BeginChild("RightSide", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
                    {
                        //Unlimited Health
                        ImGuiCustom::KeyBind("", &st.key_bind_[0], ImVec2(150, 25), 0); ImGui::SameLine();
                        ImGui::Checkbox(lang.kUnlimitedHealthLabel, &hk.is_unlimited_health_);
                        if (GetAsyncKeyState(st.key_bind_[0]) & 1) 
                        { 
                            hk.is_unlimited_health_ = !hk.is_unlimited_health_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }

                        //Unlimited Vigor
                        ImGuiCustom::KeyBind("", &st.key_bind_[1], ImVec2(150, 25), 1); ImGui::SameLine(); 
                        ImGui::Checkbox(lang.KUnlimitedVigorLabel, &hk.is_unlimited_vigor_);
                        if (GetAsyncKeyState(st.key_bind_[1]) & 1) 
                        { 
                            hk.is_unlimited_vigor_ = !hk.is_unlimited_vigor_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }

                        //Unlimited Talents
                        ImGuiCustom::KeyBind("", &st.key_bind_[2], ImVec2(150, 25), 2); ImGui::SameLine(); 
                        ImGui::Checkbox(lang.kUnlimitedTalentsLabel, &hk.is_unlimited_talents_);
                        if (GetAsyncKeyState(st.key_bind_[2]) & 1) 
                        { 
                            hk.is_unlimited_talents_ = !hk.is_unlimited_talents_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }

                        //Unlimited Medallion
                        ImGuiCustom::KeyBind("", &st.key_bind_[3], ImVec2(150, 25), 3); ImGui::SameLine(); 
                        ImGui::Checkbox(lang.kUnlimitedMedallionLabel, &hk.is_unlimited_medallion_);
                        if (GetAsyncKeyState(st.key_bind_[3]) & 1) 
                        { 
                            hk.is_unlimited_medallion_ = !hk.is_unlimited_medallion_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }

                        //Unlimited Weight
                        ImGuiCustom::KeyBind("", &st.key_bind_[4], ImVec2(150, 25), 4); ImGui::SameLine(); 
                        ImGui::Checkbox(lang.kUnlimitedWeightLabel, &hk.is_unlimited_weight_);
                        if (GetAsyncKeyState(st.key_bind_[4]) & 1) 
                        { 
                            hk.is_unlimited_weight_ = !hk.is_unlimited_weight_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }

                        //Unlimited Adrenaline
                        ImGuiCustom::KeyBind("", &st.key_bind_[5], ImVec2(150, 25), 5); ImGui::SameLine(); 
                        ImGui::Checkbox(lang.kUnlimitedAdrenalineLabel, &hk.is_unlimited_adrenaline_);
                        if (GetAsyncKeyState(st.key_bind_[5]) & 1) 
                        { 
                            hk.is_unlimited_adrenaline_ = !hk.is_unlimited_adrenaline_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }                      

                        //No Toxicity
                       /* ImGuiCustom::KeyBind("", &st.key_bind_[6], ImVec2(150, 25), 6); ImGui::SameLine(); 
                        ImGui::Checkbox(lang.kNoToxicityLabel, &hk.is_toxicity_);
                        if (GetAsyncKeyState(st.key_bind_[6]) & 1)
                        { 
                            hk.is_toxicity_ = !hk.is_toxicity_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }*/

                        //Free Camera
                        ImGui::BeginDisabled(hk.is_game_speed_changer_);
                        
                        ImGuiCustom::KeyBind("", &st.key_bind_[6], ImVec2(150, 25), 6); ImGui::SameLine();
                        ImGui::Checkbox(lang.kFreeCamera, &hk.is_free_camera_);
                        if ((GetAsyncKeyState(st.key_bind_[6]) & 1) && !hk.is_game_speed_changer_)
                        {
                            hk.is_free_camera_ = !hk.is_free_camera_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }

                        ImGui::SetCursorPosX(14.0f);
                        ImGui::SliderFloat("##0", &hk.freecamera_speed_, 0.01f, 1.0f);

                        if (hk.is_game_speed_changer_);
                            ImGui::EndDisabled();

                        //Movement Changer
                        ImGuiCustom::KeyBind("", &st.key_bind_[7], ImVec2(150, 25), 7); ImGui::SameLine(); 
                        ImGui::Checkbox(lang.kMovementChangerLabel, &hk.is_movement_changer_);
                        if (GetAsyncKeyState(st.key_bind_[7]) & 1) 
                        { 
                            hk.is_movement_changer_ = !hk.is_movement_changer_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }

                        ImGui::SetCursorPosX(14.0f);
                        ImGui::SliderFloat("##1", &hk.movement_value_, 0.2f, 10.0f);

                        //FOV Changer
                        ImGuiCustom::KeyBind("", &st.key_bind_[8], ImVec2(150, 25), 8); ImGui::SameLine(); 
                        ImGui::Checkbox(lang.kFovChangerLabel, &hk.is_fov_changer_);
                        if (GetAsyncKeyState(st.key_bind_[8]) & 1) 
                        { 
                            hk.is_fov_changer_ = !hk.is_fov_changer_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        } 

                        ImGui::SetCursorPosX(14.0f);
                        ImGui::SliderFloat("##2", &hk.fov_value_, 0.2f, 1.2f);

                        //Game Speed Changer
                        ImGui::BeginDisabled(hk.is_free_camera_);

                        ImGuiCustom::KeyBind("", &st.key_bind_[9], ImVec2(150, 25), 9); ImGui::SameLine();
                        ImGui::Checkbox(lang.kGameSpeedChangerLabel, &hk.is_game_speed_changer_);
                        if ((GetAsyncKeyState(st.key_bind_[9]) & 1) && !hk.is_free_camera_)
                        {
                            hk.is_game_speed_changer_ = !hk.is_game_speed_changer_;
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }

                        ImGui::SetCursorPosX(14.0f);
                        ImGui::SliderFloat("##3", &hk.game_speed_value, 0.01f, 50.0f);

                        if (hk.is_free_camera_);
                        ImGui::EndDisabled();

                        ImGui::Separator();
                      
                        //MAX LVL
                        ImGuiCustom::KeyBind("", &st.key_bind_[10], ImVec2(150, 25), 10); ImGui::SameLine(); 
                        if (ImGui::Button(lang.kMaxLvlLabel, ImVec2(250, 25)) || GetAsyncKeyState(st.key_bind_[10]) & 1)
                        {
                            int write_level = 35;
                            mem::write(globals::pid, (void*)((uintptr_t)hk.lp_base_[0] + offsets::kLpLevel), &write_level, sizeof(write_level));
                            PlaySound(cwd.string().c_str(), 0, SND_ASYNC);
                        }
                    }
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(lang.kSettingsLabel))
                {
                    ImGui::Checkbox(lang.kTurnOffOnTheSound, &st.is_sound);
                    ImGui::Checkbox(lang.kCheckForUpdatesLabel, &st.is_check_update_);
                    ImGui::ShowStyleSelector(lang.kThemeLabel);
                    lang.ShowLanguageSelector(lang.kLangSelectorLabel);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(lang.kAboutLabel))
                {
                    ImGui::Text(lang.kVersionLabel, TRAINER_VERSION);
                    ImGui::Text(u8"\nCopyright © 2022 Rudeus Kirigaya / Kirizaku");
                    ImGui::Text(lang.kReportLevel);
                    ImGuiCustom::TextURL("https://github.com/Kirizaku/The-Witcher-2-Trainer", "https://github.com/Kirizaku/The-Witcher-2-Trainer");

                    ImGui::PushFont(Ubuntu_Bold);
                    ImGui::Text(lang.kAdditionalCopyrightsLabel);
                    ImGui::PopFont();
                    ImGui::BeginChild("Copyrights", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
                    {
                        ImGui::Text("Dear ImGui: Bloat-free Graphical User interface for C++ with minimal dependencies by ocornut (MIT License)");
                        ImGuiCustom::TextURL("https://github.com/ocornut/imgui", "https://github.com/ocornut/imgui");
                        ImGui::Text("\nGLFW multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop (Zlib License)");
                        ImGuiCustom::TextURL("https://github.com/glfw/glfw", "https://github.com/glfw/glfw");
                        ImGui::Text("\nJSON for Modern C++ by Niels Lohmann (MIT License)");
                        ImGuiCustom::TextURL("https://github.com/nlohmann/json", "https://github.com/nlohmann/json");
                        ImGui::Text("\nUbuntu Font (Ubuntu font licence)");
                        ImGuiCustom::TextURL("https://fonts.google.com/specimen/Ubuntu#standard-styles", "https://fonts.google.com/specimen/Ubuntu#standard-styles");
                        ImGui::Text("\nstb_image by Sean Barrett (public domain)");
                        ImGuiCustom::TextURL("https://github.com/nothings/stb", "https://github.com/nothings/stb");
                    }
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(lang.kLicenseLabel))
                {
                    ImGui::BeginChild("License", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
                    ImGui::TextUnformatted(gplv3);
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
        ImGui::Render();

        glfwGetFramebufferSize(window_, &display_w_, &display_h_);
        glViewport(0, 0, display_w_, display_h_);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
    }
}

void menu::Shutdown() 
{
    Hacks::is_hooking_ = false;
    Config().SaveHookConfig();
    Config().SaveSetiingsConfig();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window_);
    glfwTerminate();
}

void menu::Style() 
{
    ImGuiIO& io = ImGui::GetIO(); (void)io; ImFontConfig cfg{ };
    cfg.FontDataOwnedByAtlas = false; 
    io.Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu-Regular.ttf", 19.0f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    Ubuntu_Bold = io.Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu-Bold.ttf", 19.0f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    switch (settings::style_idx_)
    {
        case 0: ImGui::StyleColorsDarkV2(); break;
        case 1: ImGui::StyleColorsLightV2(); break;
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowMinSize = ImVec2(IMGUI_WINDOW_W, IMGUI_WINDOW_H);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.FrameRounding = 3;
}