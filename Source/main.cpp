#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Harness.h"
#include "Camera.h"
#include "Menu.h"
#include "MC.h"

using namespace qaiser;
class App : public qaiser::Harness{
public:
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    Menu* menu;

    Cube** cells;
    std::vector<Triangle> t;

    // MC buffers
    GLuint axis_buffer;

    // debug buffers
    Cube* debug_cell;
    Cube* debug_cell2;

    GLuint debug_grid_buffer;
    GLuint debug_points_buffer;
    GLuint debug_tri_buffer;

    GLuint debug_grid_buffer2;
    GLuint debug_points_buffer2;
    GLuint debug_tri_buffer2;

    // Light buffer
    GLuint light_tri_buffer;
    Cube** light_cells;


    void startup() override {
        // Init GUI
        menu = new Menu();

        // Light
//        light_cells = generate_math_samples(20, sample_sphere, 2.0);
//        light_tri_buffer = R->create_tri_buffer(light_cells, 20*20*20);

        // Cell debugging sample
        R->enableDebug();

        debug_cell = generate_debug_sample(glm::vec3(15.0f, 0.0f ,0.0f));
        debug_cell2 = generate_debug_sample(glm::vec3(15.0f, 0.0f ,1.01f));

        R->setDebugCell(debug_cell);
        R->setDebugCell2(debug_cell2);

        debug_points_buffer = R->create_debug_point_buffer(debug_cell);
        debug_grid_buffer = R->create_debug_grid_buffer(debug_cell);
        debug_tri_buffer = R->create_debug_tri_buffer(debug_cell);

        debug_points_buffer2 = R->create_debug_point_buffer(debug_cell2);
        debug_grid_buffer2 = R->create_debug_grid_buffer(debug_cell2);
        debug_tri_buffer2 = R->create_debug_tri_buffer(debug_cell2);



        axis_buffer = R->enableAxis();

    };


    void render(float delta) override {
        VP = camera->calc_VP(delta);
        V = camera->getView();
        campos = camera->getCamPos();

        // GUI
        R->renderGUI(*menu, debug_points_buffer, debug_tri_buffer, debug_points_buffer2, debug_tri_buffer2);

        // Axis
        R->renderAxis(axis_buffer);

        // Debug sample
        R->renderPoints(debug_points_buffer);
        R->renderLines(debug_grid_buffer);
        R->renderTris(debug_tri_buffer, true);

        // Debug sample
        R->renderPoints(debug_points_buffer2);
        R->renderLines(debug_grid_buffer2);
        R->renderTris(debug_tri_buffer2, true);

        // Render light
//        R->renderTris2(light_tri_buffer);

    }
};


#if !DEBUG
int main(){
    qaiser::Window window = qaiser::Window(1920, 1080, "Marching Cubes", 1.0f, 1.0f, 0.9f);
    App* a = new App;
    a->setWindow(window);
    a->run(a);

    delete a;
    
    return 0;
}


#else
// empty
#endif