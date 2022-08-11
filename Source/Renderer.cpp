#include <string>
#include <iostream>
#include "Logger.h"
#include "Harness.h"
#include "Renderer.h"

// Static initializations
int Renderer::free_buf          = -1;

Renderer::Renderer(GLuint &VAO, GLuint *buf)
: VAO(VAO), buf(buf) {

    strides.resize(1024);
    sizes.resize(1024);

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
            0.0f, 0.0f, 12.5f,     1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -12.5f,    1.0f, 0.0f, 0.0f,
            // Axis 2 (green)
            0.0f, 12.5f, 0.0f,    0.0f, 1.0f, 0.0f,
            0.0f, -12.5f, 0.0f,   0.0f, 1.0f, 0.0f,
            // Axis 3 (blue)
            12.5f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            -12.5f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    // Prepare buffer
    GLuint loc = prepBuf((GLfloat*)axis_data, sizeof(axis_data));


    // Format data
    formatBuf(loc, 3, {0, 1});


    return loc;
}


GLuint Renderer::create_point_buffer(Cube** target_cells, int amount, double isovalue, int update) {
    std::vector<GLfloat> data;

    for(int c=0; c < amount; c++){
        for(int i=0; i < 8; i++){
            data.push_back(target_cells[c]->vertices[i].x);
            data.push_back(target_cells[c]->vertices[i].y);
            data.push_back(target_cells[c]->vertices[i].z);

            glm::vec3 color;
            if(target_cells[c]->samples[i] > isovalue)
                color = glm::vec3(0.0f);
            else
                color = glm::vec3(0.0f, 1.0f, 0.0f);

            data.push_back(color.x);
            data.push_back(color.y);
            data.push_back(color.z);
        }
    }

    if(update == -1){
        GLuint loc = prepBuf(data, false);
        formatBuf(loc, 3, {0, 1});
        // Save size
        sizes[loc] = data.size();

        return loc;
    }

    // Otherwise, update has specified a buffer to edit
    // Save size
    editBuf(data, update);
    sizes[update] = data.size();


    return update;
}




/*
 * Display marching cubes grid for debug purposes
 */
GLuint Renderer::create_grid_buffer(Cube** target_cells, int amount, int update) {
    std::vector<GLfloat> data;
    for(int c =0; c < amount; c++){
        for(int e = 0; e < 12; e++){
            data.push_back(target_cells[c]->vertices[edges[e][0]].x); // first vertex of edge
            data.push_back(target_cells[c]->vertices[edges[e][0]].y);
            data.push_back(target_cells[c]->vertices[edges[e][0]].z);

            data.push_back(0.878f); data.push_back(0.623f); data.push_back(0.678f); // color


            data.push_back(target_cells[c]->vertices[edges[e][1]].x); // second vertex of edge
            data.push_back(target_cells[c]->vertices[edges[e][1]].y);
            data.push_back(target_cells[c]->vertices[edges[e][1]].z);

            data.push_back(0.878f); data.push_back(0.623f); data.push_back(0.678f); // color

        }
    }

    if(update == -1){
        GLuint loc = prepBuf(data, false);
        formatBuf(loc, 3, {0, 1});
        // Save size
        sizes[loc] = data.size();

        return loc;
    }

    // Otherwise, update has specified a buffer to edit
    // Save size
    editBuf(data, update);
    sizes[update] = data.size();


    return update;

}





/*
 * Takes triangle data generated from marching cubes algorithm and puts it in a buffer
 */
GLuint Renderer::create_tri_buffer(Cube** target_cells, int amount, bool manual_size, int update) {
    std::vector<GLfloat> data;
    for(int c=0; c < amount; c++){
        for(int t=0; t < target_cells[c]->num_tris*3; t+=3){
            // push vertices -> colors -> normals
            // V0
            data.push_back(target_cells[c]->tris[t].x); data.push_back(target_cells[c]->tris[t].y); data.push_back(target_cells[c]->tris[t].z);
            data.push_back(target_cells[c]->normals[t].x); data.push_back(target_cells[c]->normals[t].y); data.push_back(target_cells[c]->normals[t].z);

            // V1
            data.push_back(target_cells[c]->tris[t+1].x); data.push_back(target_cells[c]->tris[t+1].y); data.push_back(target_cells[c]->tris[t+1].z);
            data.push_back(target_cells[c]->normals[t+1].x); data.push_back(target_cells[c]->normals[t+1].y); data.push_back(target_cells[c]->normals[t+1].z);

            // V2
            data.push_back(target_cells[c]->tris[t+2].x); data.push_back(target_cells[c]->tris[t+2].y); data.push_back(target_cells[c]->tris[t+2].z);
            data.push_back(target_cells[c]->normals[t+2].x); data.push_back(target_cells[c]->normals[t+2].y); data.push_back(target_cells[c]->normals[t+2].z);

        }
    }

    if(update == -1){
        GLuint loc = prepBuf(data, manual_size);
        formatBuf(loc, 3, {0, 2});
        // Save size
        sizes[loc] = data.size();

        return loc;
    }

    // Otherwise, update has specified a buffer to edit
    // Save size
    editBuf(data, update);
    sizes[update] = data.size();


    return update;
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

    glDrawArrays(GL_POINTS, 0, sizes[buffer]/6);

}

void Renderer::renderLines(GLuint buffer) {

    shader_axis.bind();
    shader_axis.setMat4(20, qaiser::Harness::VP);

    glPointSize(16.0f);


    glVertexArrayVertexBuffer(VAO, 0, buf[buffer], 0, strides[buffer]);
    glDrawArrays(GL_LINES, 0, sizes[buffer]);

}
void Renderer::renderTris2(GLuint buffer){
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, light_pos);
    trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));

    shader_light.bind();
    shader_light.setMat4(20, qaiser::Harness::VP);
    shader_light.setMat4(21, trans);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // regular

    glVertexArrayVertexBuffer(VAO, 0, buf[buffer], 0, strides[buffer]);
    glDrawArrays(GL_TRIANGLES, 0, sizes[buffer]);

}

void Renderer::renderTris(GLuint buffer, bool debug_tris, glm::vec3 scale) {

    if(!debug_tris){
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, scale);

        shader_phong.bind();
        shader_phong.setMat4(20, qaiser::Harness::VP);
        shader_phong.setMat4(21, trans);
        shader_phong.setVec3(22, qaiser::Harness::campos);

        shader_phong.setVec3(23, light_pos);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // regular
    }
    else{ // debug tris
        shader_axis.bind();
        shader_axis.setMat4(20, qaiser::Harness::VP);
        shader_axis.setVec3(22, qaiser::Harness::campos);
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // wireframe
    }


    glVertexArrayVertexBuffer(VAO, 0, buf[buffer], 0, strides[buffer]);
    glDrawArrays(GL_TRIANGLES, 0, sizes[buffer]);

}



/*
 * Buffer adaption (later):
 *  - Pass array of buffer locations e.g. point buffers, then update them all accordingly
 */
void Renderer::renderGUI(Menu &g, GLuint debug_points_buffer,
                         GLuint debug_tri_buffer, GLuint debug_points_buffer2,
                         GLuint debug_tri_buffer2) {
    g.update();


    // Loading NHDR header file requested
    if(g.nhdr_loaded){

        std::vector<int> dimensions = parse_nhdr_sizes(g.nhdr_filename.c_str());
        NX = dimensions[0];
        NY = dimensions[1];
        NZ = dimensions[2];

        // Reset
        g.nhdr_filename = "";
        g.nhdr_loaded = false;
    }


    // Loading NRRD file data into buffer and generating mesh
    if(g.nrrd_loaded){
        mc_buffer = parse_nrrd_file(g.nrrd_filename.c_str(), NX, NY, NZ);

        cells = generate_samples(glm::vec3(0.0f), NX-1, NY-1, NZ-1, 2.0, mc_buffer, g.iso, false);
        setCells(cells, (NX-1)*(NY-1)*(NZ-1));

        tri_buffer = create_tri_buffer(cells, num_cells);


        // Reset
        g.nrrd_filename = "";
        g.nrrd_loaded = false;
    }


    // Triangle output file requested
    if(g.output_file_btn){
        output_triangles(cells, num_cells, std::string(SRC+"triangles.obj").c_str());
    }


    // Regenerating mathematical implicit functions
    if(g.sphere_btn || g.bumps_btn){
        math_num_cells = 10*10*10;

        if(g.model == 0)
            math_cells = generate_math_samples(10, sample_sphere, g.iso);
        if(g.model == 1)
            math_cells = generate_math_samples(10 , sample_bumps, g.iso);


        math_tri_buffer = create_tri_buffer(math_cells, math_num_cells, true);
        math_points_buffer = create_point_buffer(math_cells, math_num_cells, g.iso);
        math_grid_buffer = create_grid_buffer(math_cells, math_num_cells);



    }

    // Debug vertices controller
    for(int i=0; i < 8; i++){
        if(g.debug_clicked[i])
        {
            if(*g.debug_vertices[i])
                debug_cell->samples[i] = 9;
            else
                debug_cell->samples[i] = 11;

            march_debug_cell(debug_cell, 10);

            update_debug_points(debug_points_buffer, debug_cell);
            update_debug_tris(debug_tri_buffer, debug_cell);
        }

        if(g.debug_clicked2[i])
        {
            if(*g.debug_vertices2[i])
                debug_cell2->samples[i] = 9;
            else
                debug_cell2->samples[i] = 11;

            march_debug_cell(debug_cell2, 10);

            update_debug_points(debug_points_buffer2, debug_cell2);
            update_debug_tris(debug_tri_buffer2, debug_cell2);
        }

    }

    // Light position was updated
    if(g.light_pos_changed){
        light_pos = qaiser::Harness::campos;
        g.light_pos_changed = false;
    }


    // Data-based mesh is active
    if(g.mesh_active){
        // Isovalue has been edited, regenerate mesh and update buffers
        if(g.isoChanging)
        {
            // Free previous cells
            for(int i=0; i < num_cells; i++){
                delete cells[i];
            }
            delete[] cells;

            // Regenerate
            cells = generate_samples(glm::vec3(0.0f), NX-1, NY-1, NZ-1, 1.0, mc_buffer, g.iso);

            // Update buffers
            create_tri_buffer(cells, num_cells, false, tri_buffer);
        }

        renderTris(tri_buffer, false, glm::vec3(g.scale));
    }

    // Math-based mesh is active
    if(g.math_active){
        // Isovalue has been edited, regenerate mesh and update buffers
        if(g.isoChanging)
        {
            // Free previous cells
            for(int i=0; i < math_num_cells; i++){
                delete math_cells[i];
            }
            delete[] math_cells;

            // Regenerate
            if(g.model == 0)
                math_cells = generate_math_samples(20, sample_sphere, g.iso);
            if(g.model == 1)
                math_cells = generate_math_samples(20 , sample_bumps, g.iso);

            // Update buffers
            create_tri_buffer(math_cells, math_num_cells, true, math_tri_buffer);
            create_point_buffer(math_cells, math_num_cells, g.iso, math_points_buffer);
            create_grid_buffer(math_cells, math_num_cells, math_grid_buffer);
        }

        renderTris(math_tri_buffer, true, glm::vec3(g.scale));

        if(g.show_grid){
            renderPoints(math_points_buffer);
            renderLines(math_grid_buffer);
        }
    }

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
        to_allocate = TEN_MB;
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


    int previous_size = 4*sizes[i];


    std::cout << "new size: " << dat_size  << " / " << previous_size << std::endl;


    // Buffer overflowed
    if(dat_size >= TEN_MB){
        std::cout << dat_size << " / " << TEN_MB << std::endl;
        Logger::log(ERROR, "Buffer overflowed, buffer ID: "+ std::to_string(i), __FILENAME__);
    }

    // Unmap previous data to null
    float* ptr0 = (float*) glMapNamedBufferRange(buf[i], 0, previous_size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    for(int x=0; x < sizes[i]; x++)
        ptr0[x] = 0;
    glUnmapNamedBuffer(buf[i]);


    // Map new data
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



void Renderer::setCells(Cube** c, int num_cell) {

    cells = c;
    num_cells = num_cell;
}













/*
 * DEBUGGING METHODS
 */

void Renderer::setDebugCell(Cube* cell) {
    this->debug_cell = cell;
}
void Renderer::setDebugCell2(Cube *cell) {
    this->debug_cell2 = cell;
}

void Renderer::enableDebug() {
    this->debug = true;
}

GLuint Renderer::create_debug_point_buffer(Cube* cell) {
    std::vector<GLfloat> data;
    for(int i=0; i < 8; i++){
        data.push_back(cell->vertices[i].x);
        data.push_back(cell->vertices[i].y);
        data.push_back(cell->vertices[i].z);

        glm::vec3 color;
        if(cell->samples[i] > 10)
            color = glm::vec3(0.0f);
        else
            color = glm::vec3(0.0f, 1.0f, 0.0f);

        data.push_back(color.x);
        data.push_back(color.y);
        data.push_back(color.z);

    }
    GLuint loc = prepBuf(data, false);


    formatBuf(loc, 3, {0, 1});

    // Save size
    sizes[loc] = data.size();

    return loc;
}

void Renderer::update_debug_points(GLuint buffer, Cube* cell) {
    std::vector<GLfloat> data;
    for(int i=0; i < 8; i++){
        data.push_back(cell-> vertices[i].x);
        data.push_back(cell->vertices[i].y);
        data.push_back(cell->vertices[i].z);

        glm::vec3 color;
        if(cell->samples[i] > 10)
            color = glm::vec3(0.0f);
        else
            color = glm::vec3(0.0f, 1.0f, 0.0f);

        data.push_back(color.x);
        data.push_back(color.y);
        data.push_back(color.z);

    }
    editBuf(data, buffer);

    sizes[buffer] = data.size();
}





GLuint Renderer::create_debug_grid_buffer(Cube* cell) {
    std::vector<GLfloat> data;
    for(int e = 0; e < 12; e++){
        data.push_back(cell->vertices[edges[e][0]].x); // first vertex of edge
        data.push_back(cell->vertices[edges[e][0]].y);
        data.push_back(cell->vertices[edges[e][0]].z);

        data.push_back(0.878f); data.push_back(0.623f); data.push_back(0.678f); // color


        data.push_back(cell->vertices[edges[e][1]].x); // second vertex of edge
        data.push_back(cell->vertices[edges[e][1]].y);
        data.push_back(cell->vertices[edges[e][1]].z);

        data.push_back(0.878f); data.push_back(0.623f); data.push_back(0.678f); // color

    }
    GLuint loc = prepBuf(data, false);


    formatBuf(loc, 3, {0, 1});

    // Save size
    sizes[loc] = data.size();

    return loc;
}

GLuint Renderer::create_debug_tri_buffer(Cube* cell) {
    std::vector<GLfloat> data;

    for(int t=0; t < cell->num_tris*3; t+=3){
        // push vertices -> colors
        // V0
        data.push_back(cell->tris[t].x); data.push_back(cell->tris[t].y); data.push_back(cell->tris[t].z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

        // V1
        data.push_back(cell->tris[t+1].x); data.push_back(cell->tris[t+1].y); data.push_back(cell->tris[t+1].z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

        // V2
        data.push_back(cell->tris[t+2].x); data.push_back(cell->tris[t+2].y); data.push_back(cell->tris[t+2].z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

    }
    GLuint loc = prepBuf(data, true);


    formatBuf(loc, 3, {0, 1});

    // Save size
    sizes[loc] = data.size();

    return loc;

}

void Renderer::update_debug_tris(GLuint buffer, Cube* cell) {
    std::vector<GLfloat> data;

    for(int t=0; t < cell->num_tris*3; t+=3){
        // push vertices -> colors
        // V0
        data.push_back(cell->tris[t].x); data.push_back(cell->tris[t].y); data.push_back(cell->tris[t].z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

        // V1
        data.push_back(cell->tris[t+1].x); data.push_back(cell->tris[t+1].y); data.push_back(cell->tris[t+1].z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

        // V2
        data.push_back(cell->tris[t+2].x); data.push_back(cell->tris[t+2].y); data.push_back(cell->tris[t+2].z);
        data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f);

    }


    editBuf(data, buffer);

    sizes[buffer] = data.size();
}











