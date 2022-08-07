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
    GLuint lines_buffer;
    GLuint points_buffer;
    GLuint tri_buffer;

    // debug buffers
    Cube* debug_cell;
    Cube* debug_cell2;

    GLuint debug_grid_buffer;
    GLuint debug_points_buffer;
    GLuint debug_tri_buffer;

    GLuint debug_grid_buffer2;
    GLuint debug_points_buffer2;
    GLuint debug_tri_buffer2;


    void startup() override {

        // TEMP - read nrrd file
        FILE* fileptr;
        std::uint8_t*** buffer = nullptr;
        long fsize;

        fileptr = fopen(std::string(SRC+"Res/fuel.raw").c_str(), "rb");

        // Get size and reset
        fseek(fileptr, 0, SEEK_END);
        fsize = ftell(fileptr);
        rewind(fileptr);


        int NX = 64;
        int NY = 64;
        int NZ = 64;
        int c;
        buffer = static_cast<uint8_t ***>(malloc(NX * sizeof(std::uint8_t **)));

        for(int i=0; i < NX; i++)
            buffer[i] = static_cast<uint8_t **>(malloc(NY * sizeof(std::uint8_t *)));
        for(int i=0; i < NX; i++)
            for(int j=0; j < NY; j++)
                buffer[i][j] = static_cast<uint8_t *>(malloc(NZ * sizeof(std::uint8_t)));

        for(int k=0; k < NZ; k++)
        {
            for(int j=0; j < NY; j++){
                for(int i=0; i < NX; i++){
                    if((c = fgetc(fileptr)) == EOF){
                        break;
                    }
                    buffer[i][j][k] = c;
                }
            }
        }




        menu = new Menu();


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





        // Main sample
//        samples = generate_samples2(64, buffer);
//        samples = generate_samples3(64, buffer);

        cells = generate_samples(glm::vec3(0.0f), 63, 2.0, buffer, 0.010);
        R->setCells(cells, buffer, 63*63*63);

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
        R->renderGUI(*menu, points_buffer, tri_buffer, lines_buffer, debug_points_buffer, debug_tri_buffer, debug_points_buffer2, debug_tri_buffer2);

        // Axis
        R->renderAxis(axis_buffer);

        // Main sample
//        R->renderPoints(points_buffer);
//        R->renderLines(lines_buffer);
        R->renderTris(tri_buffer);

        // Debug sample
        R->renderPoints(debug_points_buffer);
        R->renderLines(debug_grid_buffer);
        R->renderTris(debug_tri_buffer, true);

        // Debug sample
        R->renderPoints(debug_points_buffer2);
        R->renderLines(debug_grid_buffer2);
        R->renderTris(debug_tri_buffer2, true);

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