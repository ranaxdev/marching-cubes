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

    debug_clicked.resize(8);


    for(int i=0; i < 8; i++){
        debug_clicked[0] = 0;

        debug_vertices[i] = new bool;
        *(debug_vertices[i]) = false;
    }
}

void Menu::update() {

    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("Sim Menu", &GUI::state, flags);
    ImGui::Text("Select options below \n\n");


    sphere_btn = ImGui::Button("Sphere");    ImGui::SameLine();
    bumps_btn = ImGui::Button("Bumps");
    if(sphere_btn){
        model = 0;
    }
    if(bumps_btn){
        model = 1;
    }

    isoChanging = ImGui::SliderFloat("Isovalue", &iso, -5.0f, 5.0f);

    ImGui::NewLine();
    // Debug vertices
    debug_clicked[0] = ImGui::Checkbox("V0", debug_vertices[0]);
    debug_clicked[1] = ImGui::Checkbox("V1", debug_vertices[1]);
    debug_clicked[2] = ImGui::Checkbox("V2", debug_vertices[2]);
    debug_clicked[3] = ImGui::Checkbox("V3", debug_vertices[3]);
    debug_clicked[4] = ImGui::Checkbox("V4", debug_vertices[4]);
    debug_clicked[5] = ImGui::Checkbox("V5", debug_vertices[5]);
    debug_clicked[6] = ImGui::Checkbox("V6", debug_vertices[6]);
    debug_clicked[7] = ImGui::Checkbox("V7", debug_vertices[7]);



    ImGui::End();
}
