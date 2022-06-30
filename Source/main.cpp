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

    std::vector<Cube> samples;
    std::vector<Triangle> t;

    // MC buffers
    GLuint axis_buffer;
    GLuint lines_buffer;
    GLuint points_buffer;
    GLuint tri_buffer;

    // debug buffers
    GLuint debug_grid_buffer;
    GLuint debug_points_buffer;
    GLuint debug_tri_buffer;


    void startup() override {

        menu = new Menu();


        // Cell debugging sample
        R->enableDebug();
        R->setDebugCell(generate_debug_sample());
        debug_points_buffer = R->create_debug_point_buffer();
        debug_grid_buffer = R->create_debug_grid_buffer();
        debug_tri_buffer = R->create_debug_tri_buffer();


        // Main sample
        samples = generate_sphere_samples(10);
        R->setCells(samples);

        axis_buffer = R->enableAxis();
        points_buffer = R->create_point_buffer();
        lines_buffer = R->create_grid_buffer();
        tri_buffer = R->create_tri_buffer();
    };


    void render(float delta) override {
        VP = camera->calc_VP(delta);
        V = camera->getView();
        campos = camera->getCamPos();

        // GUI
        R->renderGUI(*menu, points_buffer, tri_buffer, debug_points_buffer, debug_tri_buffer);

        // Axis
        R->renderAxis(axis_buffer);

        // Main sample
        R->renderPoints(points_buffer);
        R->renderLines(lines_buffer);
        R->renderTris(tri_buffer);

        // Debug sample
        R->renderPoints(debug_points_buffer);
        R->renderLines(debug_grid_buffer);
        R->renderTris(debug_tri_buffer);
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