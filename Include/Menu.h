#ifndef _H_MENU
#define _H_MENU

#include "GUI.h"
class Menu : public GUI{
public:
    Menu();
    void update() override;

    ImVec2 size;
    ImVec2 pos;

    bool start = false;
    bool reset = false;


    bool isoChanging = false;
    float iso = 0.01f;


};

#endif