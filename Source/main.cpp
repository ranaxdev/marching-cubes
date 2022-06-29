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

    // buffers
    GLuint axis_buffer;
    GLuint lines_buffer;
    GLuint points_buffer;
    GLuint tri_buffer;

    void startup() override {

        menu = new Menu();

//        samples = generate_sphere_samples(10);

        glm::vec3 v = glm::vec3(2.0f, 0.0f, 0.0f);
        Cube cell{
            {
                v+ baseVertices[0],
                v+ baseVertices[1],
                v+ baseVertices[2],
                v+baseVertices[3],
                v+baseVertices[4],
                v+baseVertices[5],
                v+baseVertices[6],
                v+baseVertices[7]
            },
            {
                250,
                56,
                128,
                176,
                189,
                111,
                65,
                23
            }
        };


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
        R->renderGUI(*menu, points_buffer, tri_buffer);

        R->renderAxis(axis_buffer);

        R->renderPoints(points_buffer);

        R->renderLines(lines_buffer);

        R->renderTris(tri_buffer);

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