#ifndef _H_RENDERER
#define _H_RENDERER

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Globals.h"
#include "Shader.h"
#include "Menu.h"

class Renderer{
public:
    Renderer(GLuint& VAO, GLuint buf[]);
    ~Renderer() = default;

    GLuint enableAxis();
    GLuint create_point_buffer(std::vector<glm::vec3> points);


    void renderGUI(Menu& g);
    void renderAxis(GLuint buffer);
    void renderPoints(GLuint buffer);

    unsigned int prepBuf(GLfloat data[], GLuint size);
    unsigned int prepBuf(GLushort data[], GLuint size);
    unsigned int prepBuf(std::vector<GLfloat>& data, bool big);
    unsigned int editBuf(std::vector<GLfloat>& data, GLuint i);

    void formatBuf(GLuint loc, GLint comps_per_elem, std::vector<int> attribs);

private:
    // Memory mgmt
    static int free_buf;
    static int free_bindpoint;
    GLuint VAO;
    GLuint* buf;
    std::vector<GLsizei> strides;


    // Shaders
    // REMINDER: change environment variable SRC in globals to setup shaders correctly
    Shader shader_axis = Shader(SRC+"Shaders/vert.glsl", SRC+"Shaders/frag.glsl");

    // Colors
    glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);


};
#endif