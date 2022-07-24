#ifndef _H_MENU
#define _H_MENU

#include "GUI.h"
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

    std::vector<int> debug_clicked;
    bool* debug_vertices[8];


};

#endif