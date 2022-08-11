#ifndef _H_RENDERER
#define _H_RENDERER

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "MC.h"
#include "Globals.h"
#include "Shader.h"
#include "Menu.h"
#include "Util.h"

class Renderer{
public:
    Renderer(GLuint& VAO, GLuint buf[]);
    ~Renderer() = default;

    GLuint enableAxis();
    GLuint create_point_buffer(Cube** target_cells, int amount, double isovalue, int update=-1);
    GLuint create_grid_buffer(Cube** target_cells, int amount, int update=-1);
    GLuint create_tri_buffer(Cube** target_cells, int amount, bool manual_size=false, int update=-1);





    void renderGUI(Menu& g, GLuint debug_points_buffer,
                   GLuint debug_tri_buffer, GLuint debug_points_buffer2,
                   GLuint debug_tri_buffer2);
    // Render primitives
    void renderAxis(GLuint buffer);
    void renderPoints(GLuint buffer);
    void renderLines(GLuint buffer);
    void renderTris(GLuint buffer, bool debug_tris=false, glm::vec3 scale=glm::vec3(1.0f));
    void renderTris2(GLuint buffer);

    // Buffer allocation
    unsigned int prepBuf(GLfloat data[], GLuint size);
    unsigned int prepBuf(GLushort data[], GLuint size);
    unsigned int prepBuf(std::vector<GLfloat>& data, bool big);

    // Edits existing buffer
    unsigned int editBuf(std::vector<GLfloat>& data, GLuint i);

    void formatBuf(GLuint loc, GLint comps_per_elem, std::vector<int> attribs);

    void setCells(Cube** c, int num_cell);


    // Debugging cell
    void setDebugCell(Cube* cell);
    void setDebugCell2(Cube* cell);

    void enableDebug();
    GLuint create_debug_point_buffer(Cube* cell);
    GLuint create_debug_grid_buffer(Cube* cell);
    GLuint create_debug_tri_buffer(Cube* cell);
    void update_debug_points(GLuint buffer, Cube* cell);
    void update_debug_tris(GLuint buffer, Cube* cell);


private:
    // Memory mgmt
    static int free_buf;
    GLuint VAO;
    GLuint* buf;
    std::vector<GLsizei> strides;
    std::vector<GLsizei> sizes;

    // Marching cubes data
    // Main (sampled data from files)
    int NX, NY, NZ;
    GLuint points_buffer, grid_buffer, tri_buffer; // main buffers
    Cube** cells;
    std::uint8_t*** mc_buffer;
    int num_cells;

    // Secondary (math implicit functions)
    GLuint math_points_buffer, math_grid_buffer, math_tri_buffer; // live f(x,y,z) generated function buffers
    Cube** math_cells;
    int math_res = 10;
    int math_num_cells;

    // Debug cells
    Cube* debug_cell;
    Cube* debug_cell2;
    bool debug = false;

    // Shaders
    Shader shader_axis = Shader(SRC+"Shaders/vert.glsl", SRC+"Shaders/frag.glsl");
    Shader shader_phong = Shader(SRC+"Shaders/vert0.glsl", SRC+"Shaders/frag0.glsl");
    Shader shader_light = Shader(SRC+"Shaders/vert1.glsl", SRC+"Shaders/frag1.glsl");


    // Colors
    glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

    // Other
    glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 0.0f);

};
#endif