#include "MC.h"

#include <cmath>

Cube::Cube() {}

Cube::Cube(glm::vec3 start, glm::vec3 *tris, glm::vec3* normals, int num_tris)
: v_start(start), tris(tris), normals(normals), num_tris(num_tris) {

}



/* Sampling mathematical implicit functions */
double sample_sphere(glm::vec3 position) {
    return std::sqrt(std::pow(position.x,2) + std::pow(position.y, 2) + std::pow(position.z, 2)) - 1;

}


double sample_bumps(glm::vec3 position){
    return 20* (cos(position.x) + cos(position.y) + cos(position.z));
}

double sample_surf(glm::vec3 position){
    return std::pow(3* position.x, 4) + std::pow(3*position.y, 4) + std::pow(3*position.z,4)-
    (45 * std::pow(position.x, 2)) - (45 * std::pow(position.y, 2)) - (45 * std::pow(position.z, 2));
}


/*
 * Vertex interpolation between 2 positions
 */
glm::vec3 vertex_lerp(glm::vec3 pos1, glm::vec3 pos2, double sample1, double sample2, double isovalue) {
    glm::vec3 pos;
    double mu;

    if(std::abs(sample1) < 0.000001)
        return pos1;
    if(std::abs(sample2) < 0.000001)
        return pos2;
    if(std::abs(sample1 - sample2) < 0.000001)
        return pos1;

    mu = (isovalue - sample1) / (sample2 - sample1);

    pos.x = pos1.x + mu * (pos2.x - pos1.x);
    pos.y = pos1.y + mu * (pos2.y - pos1.y);
    pos.z = pos1.z + mu * (pos2.z - pos1.z);


    return pos;
}

glm::vec3 calc_normal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2){
    glm::vec3 normal;

    glm::vec3 n_v0 = v1 - v0;
    glm::vec3 n_v1 = v2 - v0;

    normal = glm::cross(v0, v1);
//    normal = glm::normalize(normal);
    return normal;
}


/*
 * Creates cube indexer from samples
 */
void make_cube_index(int& cube_index, double* samples, double isovalue){

    if(samples[0] < isovalue)
        cube_index |= 1;
    if(samples[4] < isovalue)
        cube_index |= 2;
    if(samples[5] < isovalue)
        cube_index |= 4;
    if(samples[1] < isovalue)
        cube_index |= 8;
    if(samples[2] < isovalue)
        cube_index |= 16;
    if(samples[6] < isovalue)
        cube_index |= 32;
    if(samples[7] < isovalue)
        cube_index |= 64;
    if(samples[3] < isovalue)
        cube_index |= 128;
}


/*
 * This routine marches a single cube and returns information about the cell
 * Such as the cube's corner vertex, the triangulated vertices and the normals
 */
Cube* march(glm::vec3 cube_start, float cube_length, std::uint8_t*** sdf, glm::vec3*** gradients, double isovalue, bool store_grid){

    // One of the cube's corners needed to look into its other corners by offsetting
    int x = static_cast<int>(cube_start.x);
    int y = static_cast<int>(cube_start.y);
    int z = static_cast<int>(cube_start.z);


    // Get the vertices, samples and gradients at the cube
    glm::vec3 vertices[8];
    glm::vec3 grads[8];
    double samples[8];
    int vertexID = 0;

    for(int i=0; i<=1; i++){
        for(int j=0; j<=1; j++){
            for(int k=0; k<=1; k++){
                vertices[vertexID] = cube_length * glm::vec3(x+i, y+j, z+k);
                grads[vertexID] = gradients[x+i][y+j][z+k];
                samples[vertexID] = sdf[x + i][y + j][z + k];
                vertexID++;
            }
        }
    }

    // Index into edge table to get an 8-bit number where each bit corresponds to a vertex
    // The edge table has 12-bit numbers where each bit corresponds to an edge
    int cube_index = 0;
    make_cube_index(cube_index, samples, isovalue);

    // Count the number of triangles that will be generated in this cube
    int num_tris = 0;
    for(int i=0; triTable[cube_index][i] != -1; i+=3)
        num_tris++;

    glm::vec3* triangle_points = new glm::vec3[num_tris*3];
    glm::vec3* triangle_normals = new glm::vec3[num_tris*3];

    // If cube fully inside or outside surface, return empty cube (no triangles insides)
    if(edgeTable[cube_index] == 0){
        Cube* c = new Cube(cube_start, triangle_points, triangle_normals, num_tris);

        if(store_grid){
            for(int i=0; i < 8; i++){
                c->vertices[i] = vertices[i];
                c->samples[i] = samples[i];
            }
        }

        return c;
    }

    // Vertices where cube intersects the surface.
    // If edge is intersecting surface, linearly interpolate new vertices and their normals
    glm::vec3 t_v[12]; // triangle vertices
    glm::vec3 t_n[12]; // triangle normals

    if(edgeTable[cube_index] & 1){
        t_v[0] = vertex_lerp(vertices[0], vertices[4], samples[0], samples[4], isovalue);
        t_n[0] = vertex_lerp(grads[0], grads[4], samples[0], samples[4], isovalue);

    }
    if(edgeTable[cube_index] & 2){
        t_v[1] = vertex_lerp(vertices[4], vertices[5], samples[4], samples[5], isovalue);
        t_n[1] = vertex_lerp(grads[4], grads[5], samples[4], samples[5], isovalue);

    }
    if(edgeTable[cube_index] & 4){
        t_v[2] = vertex_lerp(vertices[5], vertices[1], samples[5], samples[1], isovalue);
        t_n[2] = vertex_lerp(grads[5], grads[1], samples[5], samples[1], isovalue);

    }
    if(edgeTable[cube_index] & 8){
        t_v[3] = vertex_lerp(vertices[1], vertices[0], samples[1], samples[0], isovalue);
        t_n[3] = vertex_lerp(grads[1], grads[0], samples[1], samples[0], isovalue);

    }
    if(edgeTable[cube_index] & 16){
        t_v[4] = vertex_lerp(vertices[2], vertices[6], samples[2], samples[6], isovalue);
        t_n[4] = vertex_lerp(grads[2], grads[6], samples[2], samples[6], isovalue);
    }
    if(edgeTable[cube_index] & 32){
        t_v[5] = vertex_lerp(vertices[6], vertices[7], samples[6], samples[7], isovalue);
        t_n[5] = vertex_lerp(grads[6], grads[7], samples[6], samples[7], isovalue);
    }
    if(edgeTable[cube_index] & 64){
        t_v[6] = vertex_lerp(vertices[7], vertices[3], samples[7], samples[3], isovalue);
        t_n[6] = vertex_lerp(grads[7], grads[3], samples[7], samples[3], isovalue);
    }
    if(edgeTable[cube_index] & 128){
        t_v[7] = vertex_lerp(vertices[3], vertices[2], samples[3], samples[2], isovalue);
        t_n[7] = vertex_lerp(grads[3], grads[2], samples[3], samples[2], isovalue);
    }
    if(edgeTable[cube_index] & 256){
        t_v[8] = vertex_lerp(vertices[0], vertices[2], samples[0], samples[2], isovalue);
        t_n[8] = vertex_lerp(grads[0], grads[2], samples[0], samples[2], isovalue);
    }
    if(edgeTable[cube_index] & 512){
        t_v[9] = vertex_lerp(vertices[4], vertices[6], samples[4], samples[6], isovalue);
        t_n[9] = vertex_lerp(grads[4], grads[6], samples[4], samples[6], isovalue);
    }
    if(edgeTable[cube_index] & 1024){
        t_v[10] = vertex_lerp(vertices[5], vertices[7], samples[5], samples[7], isovalue);
        t_n[10] = vertex_lerp(grads[5], grads[7], samples[5], samples[7], isovalue);
    }
    if(edgeTable[cube_index] & 2048){
        t_v[11] = vertex_lerp(vertices[1], vertices[3], samples[1], samples[3], isovalue);
        t_n[11] = vertex_lerp(grads[1], grads[3], samples[1], samples[3], isovalue);
    }


    // Build the triangles from the triangle table (getting vertices of each triangle)
    // Triangle point list will be linear where each 3 elements represent vertices of 1 triangle
    // ie v0 v1 v2 v3 v4 v5 ...
    //      tri1    tri2
    // Normal list mirrors the triangle point list
    for(int i=0; triTable[cube_index][i] != -1; i += 3){
        triangle_points[i] = t_v[triTable[cube_index][i]];
        triangle_points[i + 1] = t_v[triTable[cube_index][i + 1]];
        triangle_points[i + 2] = t_v[triTable[cube_index][i + 2]];

        // Calculate face normal (without gradients)
//        glm::vec3 normal = glm::cross(triangle_points[i+1]-triangle_points[i], triangle_points[i+2]-triangle_points[i]);
//        triangle_normals[i] = normal;
//        triangle_normals[i+1] = normal;
//        triangle_normals[i+2] = normal;
//
        // Calculate face normal (with gradients)
        triangle_normals[i] = glm::normalize(t_n[triTable[cube_index][i]]) * -1.0f;
        triangle_normals[i+1] = glm::normalize(t_n[triTable[cube_index][i+1]]) * -1.0f;
        triangle_normals[i+2] = glm::normalize(t_n[triTable[cube_index][i+2]]) * -1.0f;

    }

    Cube* c = new Cube(cube_start, triangle_points, triangle_normals, num_tris);
    if(store_grid){
        for(int i=0; i < 8; i++){
            c->vertices[i] = vertices[i];
            c->samples[i] = samples[i];
        }
    }

    return c;
}




/*
 * Routine to generate a marching cubes grid, sample the points and march each cell
 */
Cube** generate_samples(glm::vec3 grid_start, int resX, int resY, int resZ, float grid_size, std::uint8_t*** sdf, double isovalue, bool store_grid){


    glm::vec3 ***gradients = new glm::vec3**[resX+1];
    for (int i = 0; i < resX+1; i++) {
        gradients[i] = new glm::vec3*[resY+1];
        for (int j = 0; j < resY+1; j++) {
            gradients[i][j] = new glm::vec3[resZ+1];
        }
    }

    // Calculate gradients at each point in space using central differencing
    glm::vec3 p_x1, p_x2, p_y1, p_y2, p_z1, p_z2;
    float grad_x, grad_y, grad_z;

    for(int x =0; x < resX; x++){
        for(int y=0; y<resY; y++){
            for(int z=0; z<resZ; z++){
                p_x1 = (glm::vec3(x+1, y, z) + grid_start);
                p_x2 = (glm::vec3(x-1, y, z) + grid_start);
                p_y1 = (glm::vec3(x, y+1, z) + grid_start);
                p_y2 = (glm::vec3(x, y-1, z) + grid_start);
                p_z1 = (glm::vec3(x, y, z+1) + grid_start);
                p_z2 = (glm::vec3(x, y, z-1) + grid_start);

                if(x == 0 || y == 0 || z == 0){
                    grad_x = grad_y = grad_z = 0.0f;
                }
                else{
                grad_x = sdf[(int)p_x1.x][(int)p_x1.y][(int)p_x1.z] - sdf[(int)p_x2.x][(int)p_x2.y][(int)p_x2.z];
                grad_y = sdf[(int)p_y1.x][(int)p_y1.y][(int)p_y1.z] - sdf[(int)p_y2.x][(int)p_y2.y][(int)p_y2.z];
                grad_z = sdf[(int)p_z1.x][(int)p_z1.y][(int)p_z1.z] - sdf[(int)p_z2.x][(int)p_z2.y][(int)p_z2.z];
                }


                gradients[x][y][z] = glm::vec3(grad_x, grad_y, grad_z);
            }
        }
    }

    Cube** cubes = new Cube*[resX*resY*resZ];

    // March each cube
    float cube_length = grid_size /2;
    for(int x =0; x < resX; x++){
        for(int y =0; y < resY; y++){
            for(int z =0; z < resZ; z++){
                int idx = x + resX * (y + resY * z); // flatten 3d array index

                glm::vec3 cube_start = glm::vec3(x,y,z) + grid_start;

                cubes[idx] = march(cube_start, cube_length, sdf, gradients, isovalue);
            }
        }
    }



    return cubes;
}







/*
 * Routine to generate a res^3 marching cubes grid, sample the points mathematically and march each cell
 */
Cube** generate_math_samples(int res, double (*func)(glm::vec3), double isovalue){

    Cube** cubes = new Cube*[262144];

    glm::vec3 grid_start = glm::vec3(-res/2, -res/2, -res/2);

    for(int x =0; x < res; x++){
        for(int y =0; y < res; y++){
            for(int z =0; z < res; z++){
                int idx = x + res * (y + res * z); // flatten 3d array index

                glm::vec3 cube_start = glm::vec3(x,y,z) + grid_start;

                Cube* cell = new Cube(cube_start, nullptr, nullptr, 0);

                for(int i=0; i < 8; i++){
                    cell->vertices[i] = cube_start + baseVertices[i];
                    cell->samples[i] = func(cube_start + baseVertices[i]);
                }

                std::cout << idx << std::endl;

                march_debug_cell(cell, isovalue);

                cubes[idx] = cell;
            }
        }
    }

    return cubes;

}
















/*
 * This routine generates a single cell for debug purposes
 * Its purpose is to test the different triangulation cases
 */
Cube* generate_debug_sample(glm::vec3 pos){

    // Get the vertices, samples and gradients at the cube
    glm::vec3 vertices[8];
    double samples[8];

    vertices[0] = pos;
    vertices[1] = pos + glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[2] = pos + glm::vec3(1.0f, 1.0f, 0.0f);
    vertices[3] = pos + glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[4] = pos + glm::vec3(0.0f, 0.0f, 1.0f);
    vertices[5] = pos + glm::vec3(1.0f, 0.0f ,1.0f);
    vertices[6] = pos + glm::vec3(1.0f ,1.0f ,1.0f);
    vertices[7] = pos + glm::vec3(0.0f, 1.0f ,1.0f);

    for(int i=0; i< 8; i++)
        samples[i] = 11;

    Cube* cell = new Cube(pos, nullptr, nullptr, 0);
    for(int i=0; i < 8; i++){
        cell->vertices[i] = vertices[i];
        cell->samples[i] = samples[i];
    }


    march_debug_cell(cell, 10);

    return cell;

}


void march_debug_cell(Cube* cell, double isovalue){

    // Clean-up previous data if it has
    if(cell->tris != nullptr){
        delete[] cell->tris;
        delete[] cell->normals;

        cell->tris = nullptr;
        cell->normals = nullptr;
        cell->num_tris = 0;
    }


    int cube_index = 0;
    if(cell->samples[0] < isovalue)
        cube_index |= 1;
    if(cell->samples[1] < isovalue)
        cube_index |= 2;
    if(cell->samples[2] < isovalue)
        cube_index |= 4;
    if(cell->samples[3] < isovalue)
        cube_index |= 8;
    if(cell->samples[4] < isovalue)
        cube_index |= 16;
    if(cell->samples[5] < isovalue)
        cube_index |= 32;
    if(cell->samples[6] < isovalue)
        cube_index |= 64;
    if(cell->samples[7] < isovalue)
        cube_index |= 128;


    int num_tris = 0;
    for(int i=0; triTable[cube_index][i] != -1; i+=3)
        num_tris++;

    cell->num_tris = num_tris;
    cell->tris = new glm::vec3[num_tris*3];
    cell->normals = new glm::vec3[num_tris*3];

    if(edgeTable[cube_index] == 0){
        return;
    }

    glm::vec3 t_v[12]; // triangle vertices
    if(edgeTable[cube_index] & 1)
        t_v[0] = vertex_lerp(cell->vertices[0], cell->vertices[1], cell->samples[0], cell->samples[1], isovalue);
    if(edgeTable[cube_index] & 2)
        t_v[1] = vertex_lerp(cell->vertices[1], cell->vertices[2], cell->samples[1], cell->samples[2], isovalue);
    if(edgeTable[cube_index] & 4)
        t_v[2] = vertex_lerp(cell->vertices[2], cell->vertices[3], cell->samples[2], cell->samples[3], isovalue);
    if(edgeTable[cube_index] & 8)
        t_v[3] = vertex_lerp(cell->vertices[3], cell->vertices[0], cell->samples[3], cell->samples[0], isovalue);
    if(edgeTable[cube_index] & 16)
        t_v[4] = vertex_lerp(cell->vertices[4], cell->vertices[5], cell->samples[4], cell->samples[5], isovalue);
    if(edgeTable[cube_index] & 32)
        t_v[5] = vertex_lerp(cell->vertices[5], cell->vertices[6], cell->samples[5], cell->samples[6], isovalue);
    if(edgeTable[cube_index] & 64)
        t_v[6] = vertex_lerp(cell->vertices[6], cell->vertices[7], cell->samples[6], cell->samples[7], isovalue);
    if(edgeTable[cube_index] & 128)
        t_v[7] = vertex_lerp(cell->vertices[7], cell->vertices[4], cell->samples[7], cell->samples[4], isovalue);
    if(edgeTable[cube_index] & 256)
        t_v[8] = vertex_lerp(cell->vertices[0], cell->vertices[4], cell->samples[0], cell->samples[4], isovalue);
    if(edgeTable[cube_index] & 512)
        t_v[9] = vertex_lerp(cell->vertices[1], cell->vertices[5], cell->samples[1], cell->samples[5], isovalue);
    if(edgeTable[cube_index] & 1024)
        t_v[10] = vertex_lerp(cell->vertices[2], cell->vertices[6], cell->samples[2], cell->samples[6], isovalue);
    if(edgeTable[cube_index] & 2048)
        t_v[11] = vertex_lerp(cell->vertices[3], cell->vertices[7], cell->samples[3], cell->samples[7], isovalue);

    for(int i=0; triTable[cube_index][i] != -1; i += 3){
        cell->tris[i] = t_v[triTable[cube_index][i]];
        cell->tris[i + 1] = t_v[triTable[cube_index][i + 1]];
        cell->tris[i + 2] = t_v[triTable[cube_index][i + 2]];

        glm::vec3 normal = glm::cross(cell->tris[i+1]-cell->tris[i], cell->tris[i+2]-cell->tris[i]);
        cell->normals[i] = normal;
        cell->normals[i+1] = normal;
        cell->normals[i+2] = normal;

    }
}