#include "Menu.h"
#include <iostream>

Menu::Menu() {
    Menu::size = ImVec2(360.0f, 640.0f);
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

    ImGui::Checkbox("Show grid", &show_grid);
    ImGui::NewLine();

    ImGui::InputFloat("Isovalue", &iso, 5.0f, 10.0f);

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


    ImGui::NewLine();
    ImGui::SliderFloat("Scale", &scale, 0.01f, 1.0f);


    ImGui::NewLine();

    // NHDR header load file dialog & Button
    if(ImGui::Button("Load NHDR")){
        load_nhdr_dialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".nhdr", ".");
    }
    if (load_nhdr_dialog.Display("ChooseFileDlgKey"))
    {
        // OK clicked on file
        if (load_nhdr_dialog.IsOk())
        {
            nhdr_filename = load_nhdr_dialog.GetFilePathName();
            nhdr_loaded = true;
        }
        // close dialog after loading it in
        load_nhdr_dialog.Close();
    }
    ImGui::SameLine();
    // NRRD data load file dialog & Button
    if(ImGui::Button("Load Data")){
        load_nrrd_dialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".raw", ".");
    }
    if (load_nrrd_dialog.Display("ChooseFileDlgKey"))
    {
        // OK clicked on file
        if (load_nrrd_dialog.IsOk())
        {
            nrrd_filename = load_nrrd_dialog.GetFilePathName();
            nrrd_loaded = true;
            mesh_active = true;
        }
        // close dialog after loading it in
        load_nrrd_dialog.Close();
    }




    ImGui::NewLine();
    isoChanging = ImGui::Button("Regenerate"); ImGui::SameLine(); // Regenerate isosurface with current isovalue
    output_file_btn = ImGui::Button("Output Tris"); // Output triangles to .obj file


    ImGui::End();
}
