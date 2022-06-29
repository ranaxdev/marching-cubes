#include "Menu.h"
#include <iostream>

Menu::Menu() {
    Menu::size = ImVec2(360.0f, 480.0f);
    Menu::pos = ImVec2(SCREEN_W-size.x, 0.0f);


    // Init flags
    Menu::flag_list = {
            ImGuiWindowFlags_MenuBar
    };
    updateFlags();
}

void Menu::update() {

    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("Sim Menu", &GUI::state, flags);
    ImGui::Text("Select options below \n\n");


    if(ImGui::Button("Begin")){
        start = true;
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset")){
        reset = true;
    }

    isoChanging = ImGui::SliderFloat("Isovalue", &iso, 0.01f, 200.0f);

    ImGui::End();
}
