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

    debug_clicked2.resize(8);
    debug_clicked.resize(8);

    for(int i=0; i < 8; i++){
        debug_clicked[i] = 0;
        debug_clicked2[i] = 0;

        debug_vertices[i] = new bool;
        *(debug_vertices[i]) = false;

        debug_vertices2[i] = new bool;
        *(debug_vertices2[i]) = false;
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

//    isoChanging = ImGui::SliderFloat("Isovalue", &iso, 0.0f, 200.0f);
    ImGui::InputFloat("Isolevel", &iso, 5.0f, 10.0f);

    ImGui::NewLine();
    // Debug vertices
    debug_clicked[0] = ImGui::Checkbox("V0", debug_vertices[0]);     ImGui::SameLine(); debug_clicked2[0] = ImGui::Checkbox("", debug_vertices2[0]);
    debug_clicked[1] = ImGui::Checkbox("V1", debug_vertices[1]);     ImGui::SameLine(); debug_clicked2[1] = ImGui::Checkbox(" ", debug_vertices2[1]);
    debug_clicked[2] = ImGui::Checkbox("V2", debug_vertices[2]);     ImGui::SameLine(); debug_clicked2[2] = ImGui::Checkbox("  ", debug_vertices2[2]);
    debug_clicked[3] = ImGui::Checkbox("V3", debug_vertices[3]);     ImGui::SameLine(); debug_clicked2[3] = ImGui::Checkbox("   ", debug_vertices2[3]);
    debug_clicked[4] = ImGui::Checkbox("V4", debug_vertices[4]);     ImGui::SameLine(); debug_clicked2[4] = ImGui::Checkbox("    ", debug_vertices2[4]);
    debug_clicked[5] = ImGui::Checkbox("V5", debug_vertices[5]);     ImGui::SameLine(); debug_clicked2[5] = ImGui::Checkbox("     ", debug_vertices2[5]);
    debug_clicked[6] = ImGui::Checkbox("V6", debug_vertices[6]);     ImGui::SameLine(); debug_clicked2[6] = ImGui::Checkbox("      ", debug_vertices2[6]);
    debug_clicked[7] = ImGui::Checkbox("V7", debug_vertices[7]);     ImGui::SameLine(); debug_clicked2[7] = ImGui::Checkbox("       ", debug_vertices2[7]);


    // Debug vertices2


    ImGui::NewLine();
    isoChanging = ImGui::Button("Regenerate"); ImGui::SameLine();
    output_file_btn = ImGui::Button("Output Tris");


    ImGui::End();
}
