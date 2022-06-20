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

    std::vector<std::pair<glm::vec3, float>> samples;

    void startup() override {

        menu = new Menu();

        samples = generate_sphere_samples(10);

//        for(auto& s : samples){
//            std::printf("%2.1f %2.1f %2.1f -> %2.1f\n", s.first.x, s.first.y, s.first.z, s.second);
//        }

        std::vector<glm::vec3> data;
        for(auto& s: samples)
            data.push_back(s.first);


        R->enableAxis();
        R->create_point_buffer(data);
    };


    void render(float delta) override {
        VP = camera->calc_VP(delta);
        V = camera->getView();
        campos = camera->getCamPos();

        // GUI
        R->renderGUI(*menu);

        R->renderAxis();
        R->renderPoints();



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