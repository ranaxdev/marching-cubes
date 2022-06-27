#include <string>
#include <iostream>
#include "Logger.h"
#include "Harness.h"
#include "Renderer.h"

// Static initializations
int Renderer::free_buf          = -1;
int Renderer::free_bindpoint    = -1;

Renderer::Renderer(GLuint &VAO, GLuint *buf)
: VAO(VAO), buf(buf) {

    strides.resize(1024);

}

/* ****************************************************************
 *                  RENDER-ABLES INITIALIZATIONS                  *
 ******************************************************************/
/*
 * Initialize axes data
 */
GLuint Renderer::enableAxis() {
    const GLfloat axis_data[] = {
            // Axis 1 (red)
            0.0f, 0.0f, 2.5f,     1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -2.5f,    1.0f, 0.0f, 0.0f,
            // Axis 2 (green)
            0.0f, 2.5f, 0.0f,    0.0f, 1.0f, 0.0f,
            0.0f, -2.5f, 0.0f,   0.0f, 1.0f, 0.0f,
            // Axis 3 (blue)
            2.5f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            -2.5f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    // Prepare buffer
    GLuint loc = prepBuf((GLfloat*)axis_data, sizeof(axis_data));


    // Format data
    formatBuf(loc, 3, {0, 1});

    return loc;

}

GLuint Renderer::create_point_buffer(std::vector<Cube> cells) {
    std::vector<GLfloat> data;
    for(auto& c: cells){
        for(int i=0; i < 8; i++){
            data.push_back(c.vertices[i].x);
            data.push_back(c.vertices[i].y);
            data.push_back(c.vertices[i].z);

            glm::vec3 color;
            if(c.samples[i] > 100)
                color = glm::vec3(0.0f);
            else
                color = glm::vec3(0.0f, 1.0f, 0.0f);

            data.push_back(color.x);
            data.push_back(color.y);
            data.push_back(color.z);


        }
    }

    GLuint loc = prepBuf(data, false);


    formatBuf(loc, 3, {0, 1});

    return loc;
}



GLuint Renderer::create_grid_buffer(std::vector<Cube> cells) {
    std::vector<GLfloat> data;
    for(auto& c : cells){
        for(int e = 0; e < 12; e++){
            data.push_back(c.vertices[edges[e][0]].x); // first vertex of edge
            data.push_back(c.vertices[edges[e][0]].y);
            data.push_back(c.vertices[edges[e][0]].z);

            data.push_back(1.0f); data.push_back(0.0f); data.push_back(0.0f); // color


            data.push_back(c.vertices[edges[e][1]].x); // second vertex of edge
            data.push_back(c.vertices[edges[e][1]].y);
            data.push_back(c.vertices[edges[e][1]].z);

            data.push_back(1.0f); data.push_back(0.0f); data.push_back(0.0f); // color

        }
    }

    GLuint loc = prepBuf(data, false);


    formatBuf(loc, 3, {0, 1});

    return loc;

}



GLuint Renderer::create_tri_buffer(std::vector<Triangle> tris) {
    std::vector<GLfloat> data;
    for(auto& t: tris){
        data.push_back(t.v0.x); data.push_back(t.v0.y); data.push_back(t.v0.z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

        data.push_back(t.v1.x); data.push_back(t.v1.y); data.push_back(t.v1.z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

        data.push_back(t.v2.x); data.push_back(t.v2.y); data.push_back(t.v2.z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

    }
    std::cout << data.size() << std::endl;
    GLuint loc = prepBuf(data, false);


    formatBuf(loc, 3, {0, 1});

    return loc;
}





/* ****************************************************************
 *                        RENDERING ROUTINES                      *
 ******************************************************************/
// temp - just to get oriented in the world real quick
void Renderer::renderAxis(GLuint buffer) {
    shader_axis.bind();
    shader_axis.setMat4(20, qaiser::Harness::VP);
    glLineWidth(3.0f);

    glVertexArrayVertexBuffer(VAO, 0, buf[buffer], 0, strides[buffer]);
    glDrawArrays(GL_LINES , 0, 6);
}

void Renderer::renderPoints(GLuint buffer) {
    shader_axis.bind();
    shader_axis.setMat4(20, qaiser::Harness::VP);
    glPointSize(16.0f);


    glVertexArrayVertexBuffer(VAO, 0, buf[buffer], 0, strides[buffer]);
    glDrawArrays(GL_POINTS, 0, 48000);

}

void Renderer::renderLines(GLuint buffer) {

    shader_axis.bind();
    shader_axis.setMat4(20, qaiser::Harness::VP);
    glPointSize(16.0f);


    glVertexArrayVertexBuffer(VAO, 0, buf[buffer], 0, strides[buffer]);
    glDrawArrays(GL_LINES, 0, 144000);

}


void Renderer::renderTris(GLuint buffer) {


    shader_axis.bind();
    shader_axis.setMat4(20, qaiser::Harness::VP);


    glVertexArrayVertexBuffer(VAO, 0, buf[buffer], 0, strides[buffer]);
    glDrawArrays(GL_TRIANGLES, 0, 8496);

}




void Renderer::renderGUI(Menu &g) {
    g.update();

}




/* ****************************************************************
 *                      BUFFER SETUP ROUTINES                     *
 ******************************************************************/
/*
 * @data - Array of float data
 * @size - Size of the array in bytes
 * Inits a new buffer and returns its index
 */
unsigned int Renderer::prepBuf(GLfloat *data, GLuint size) {
    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], size, data, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);

    return free_buf;
}
unsigned int Renderer::prepBuf(GLushort *data, GLuint size) {
    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], size, data, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);

    return free_buf;
}


/*
 * @data - List of float data
 * Inits a new buffer and returns its index
 */
unsigned int Renderer::prepBuf(std::vector<GLfloat>& data, bool big) {
    int size = (int) data.size();
    int dat_size = 4*size;

    // Either allocate a big buffer (for large meshes and such)
    // Or just as large as your data
    int to_allocate;
    if(big)
        to_allocate = ONE_MB;
    else
        to_allocate = dat_size;


    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], to_allocate, nullptr, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT); // 1mb buffer

    float* ptr = (float*) glMapNamedBufferRange(buf[free_buf], 0, dat_size, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);
    for(int i=0; i<size; i++){
        ptr[i] = data[i];
    }
    glUnmapNamedBuffer(buf[free_buf]);

    return free_buf;
}

/*
 * @buf - Buffer to edit
 * Edits buffer data
 */
unsigned int Renderer::editBuf(std::vector<GLfloat>& data, GLuint i) {
    int size = (int) data.size();
    int dat_size = 4*size;

    // Buffer overflowed
    if(dat_size >= ONE_MB){
        Logger::log(ERROR, "Buffer overflowed, buffer ID: "+ std::to_string(i), __FILENAME__);
    }

    float* ptr = (float*) glMapNamedBufferRange(buf[i], 0, dat_size, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);
    for(int x=0; x<size; x++){
        ptr[x] = data[x];
    }
    glUnmapNamedBuffer(buf[i]);

    return i;
}


/*
 * @loc                     - Active buffer location
 * @comps_per_elem          - Number of components in an element
 * @names                   - List of attribute names used in GLSL
 * @s                       - Shader to get the attribute names from
 *
 * Formats the buffer for the VAO
 */

void Renderer::formatBuf(GLuint loc, GLint comps_per_elem, std::vector<int> attribs) {
    auto num_attribs = attribs.size();

    for(int i=0; i < num_attribs; i++){

        glVertexArrayAttribFormat(VAO, attribs[i], comps_per_elem, GL_FLOAT, GL_FALSE, (i*comps_per_elem)*sizeof(float));
        glVertexArrayAttribBinding(VAO, attribs[i], 0);
        glEnableVertexArrayAttrib(VAO, attribs[i]);
    }


    strides[loc] = (GLsizei)((num_attribs*comps_per_elem) * sizeof(float));

    // put this above every draw call with appropriate buffer
    glVertexArrayVertexBuffer(VAO, 0, buf[loc], 0, strides[loc]);
}












