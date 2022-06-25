#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
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

    // buffers
    GLuint points_buffer;
    GLuint axis_buffer;

    void startup() override {

        menu = new Menu();

        samples = generate_sphere_samples(10);


        std::vector<glm::vec3> data;
        for(auto& s: samples)
        {
            data.push_back(s.vertices[0]);
            data.push_back(s.vertices[1]);
            data.push_back(s.vertices[2]);
            data.push_back(s.vertices[3]);
            data.push_back(s.vertices[4]);
            data.push_back(s.vertices[5]);
            data.push_back(s.vertices[6]);
            data.push_back(s.vertices[7]);

        }

        axis_buffer = R->enableAxis();
        points_buffer = R->create_point_buffer(data);
    };


    void render(float delta) override {
        VP = camera->calc_VP(delta);
        V = camera->getView();
        campos = camera->getCamPos();

        // GUI
        R->renderGUI(*menu);

        R->renderAxis(axis_buffer);
        R->renderPoints(points_buffer);



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