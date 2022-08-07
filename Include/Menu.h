#ifndef _H_MENU
#define _H_MENU

#include "GUI.h"

#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"

class Menu : public GUI{
public:
    Menu();
    void update() override;

    ImVec2 size;
    ImVec2 pos;


    int model = 0;
    bool sphere_btn = false;
    bool bumps_btn = false;
    bool output_file_btn = false;


    bool isoChanging = false;
    float iso = 0.01f;

    ImGuiFileDialog load_nrrd_dialog;
    ImGuiFileDialog load_nhdr_dialog;
    std::string nhdr_filename;
    std::string nrrd_filename;
    bool nhdr_loaded = false;
    bool nrrd_loaded = false;


    // Vertex selection buttons for debug cells
    std::vector<int> debug_clicked;
    bool* debug_vertices[8];

    std::vector<int> debug_clicked2;
    bool* debug_vertices2[8];


};

#endif