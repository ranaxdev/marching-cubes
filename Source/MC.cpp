#include "MC.h"

std::vector<Cube> generate_samples2(int grid_size, double buffer[65][65][65]){

    std::vector<Cube> cells;

    glm::vec3 v = glm::vec3(-grid_size/2.0f, -grid_size/2.0f, -grid_size/2.0f);

    for(int y = 0; y < grid_size; y++){

        for(int x =0 ; x < grid_size; x++){
            Cube cell{
                {
                    v+baseVertices[0],
                    v+baseVertices[1],
                    v+baseVertices[2],
                    v+baseVertices[3],
                    v+baseVertices[4],
                    v+baseVertices[5],
                    v+baseVertices[6],
                    v+baseVertices[7]
                    }
                    ,
                    {
                    static_cast<double>(buffer[x][y][0]),
                    static_cast<double>(buffer[x+1][y][0]),
                    static_cast<double>(buffer[x+1][y+1][0]),
                    static_cast<double>(buffer[x][y+1][0]),
                    static_cast<double>(buffer[x][y][1]),
                    static_cast<double>(buffer[x+1][y][1]),
                    static_cast<double>(buffer[x+1][y+1][1]),
                    static_cast<double>(buffer[x][y+1][1])
                    }
            };
            cells.push_back(cell);

            for(int z =0; z < grid_size; z++){
                v.z += 1.0f;

                Cube cell2{
                    {
                        v+baseVertices[0],
                        v+baseVertices[1],
                        v+baseVertices[2],
                        v+baseVertices[3],
                        v+baseVertices[4],
                        v+baseVertices[5],
                        v+baseVertices[6],
                        v+baseVertices[7]
                        }
                        ,
                        {
                        static_cast<double>(buffer[x][y][z]),
                        static_cast<double>(buffer[x+1][y][z]),
                        static_cast<double>(buffer[x+1][y+1][z]),
                        static_cast<double>(buffer[x][y+1][z]),
                        static_cast<double>(buffer[x][y][z+1]),
                        static_cast<double>(buffer[x+1][y][z+1]),
                        static_cast<double>(buffer[x+1][y+1][z+1]),
                        static_cast<double>(buffer[x][y+1][z+1])
                        }
                };

                cells.push_back(cell2);

            }

            v.x += 1.0f;
            v.z = -grid_size/2.0f;

        }

        v.x = -grid_size/2.0f;
        v.z = -grid_size/2.0f;
        v.y += 1.0f;

    }
    return cells;
}


/*
 * Function: x^2 + y^2 + z^2 -1 = 0
 * pair -> vec3 (pos), float (sampled val)
 */
std::vector<Cube> generate_samples(int grid_size, double (*func)(glm::vec3)) {

    std::vector<Cube> cells;

    glm::vec3 v = glm::vec3(-grid_size/2.0f, -grid_size/2.0f, -grid_size/2.0f);

    for(int y = 0; y < grid_size; y++){

        for(int x = 0; x < grid_size; x++){
            Cube cell{
                {
                    v+baseVertices[0],
                    v+baseVertices[1],
                    v+baseVertices[2],
                    v+baseVertices[3],
                    v+baseVertices[4],
                    v+baseVertices[5],
                    v+baseVertices[6],
                    v+baseVertices[7]
                    }
                    ,
                    {
                    func(v+baseVertices[0]),
                    func(v+baseVertices[1]),
                    func(v+baseVertices[2]),
                    func(v+baseVertices[3]),
                    func(v+baseVertices[4]),
                    func(v+baseVertices[5]),
                    func(v+baseVertices[6]),
                    func(v+baseVertices[7])

                    }
            };

            cells.push_back(cell);

            for(int z = 0; z < grid_size-1; z++){
                v.z += 1.0f;
                Cube cell2{
                    {
                        v+baseVertices[0],
                        v+baseVertices[1],
                        v+baseVertices[2],
                        v+baseVertices[3],
                        v+baseVertices[4],
                        v+baseVertices[5],
                        v+baseVertices[6],
                        v+baseVertices[7]
                        }
                        ,
                        {
                        func(v+baseVertices[0]),
                        func(v+baseVertices[1]),
                        func(v+baseVertices[2]),
                        func(v+baseVertices[3]),
                        func(v+baseVertices[4]),
                        func(v+baseVertices[5]),
                        func(v+baseVertices[6]),
                        func(v+baseVertices[7])

                        }
                };

                cells.push_back(cell2);

            }

            v.x += 1.0f;
            v.z = -grid_size/2.0f;

        }


        v.x = -grid_size/2.0f;
        v.z = -grid_size/2.0f;
        v.y += 1.0f;
    }


    return cells;
}

Cube generate_debug_sample(){
    glm::vec3 v = glm::vec3(15.0f, 0.0f, 0.0f);
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
            11,
            11,
            11,
            11,
            11,
            11,
            11,
            11
            }
    };

    return cell;
}


double sample_sphere(glm::vec3 position) {

    return std::pow(position.x,2) + std::pow(position.y, 2) + std::pow(position.z, 2) - 1;
}

double sample_bumps(glm::vec3 position){
    return 20* (cos(position.x) + cos(position.y) + cos(position.z));
}



/*
 * Vertex interpolation between 2 positions
 */
glm::vec3 vertex_lerp(glm::vec3 pos1, glm::vec3 pos2, double sample1, double sample2, double isovalue) {
    glm::vec3 pos;
    double mu;

    if(std::abs(isovalue - sample1) < 0.00001)
        return pos1;
    if(std::abs(isovalue - sample2) < 0.00001)
        return pos2;
    if(std::abs(sample1 - sample2) < 0.00001)
        return pos1;

    mu = (isovalue - sample1) / (sample2 - sample1);

    pos.x = pos1.x + mu * (pos2.x - pos1.x);
    pos.y = pos1.y + mu * (pos2.y - pos1.y);
    pos.z = pos1.z + mu * (pos2.z - pos1.z);


    return pos;
}




std::vector<Triangle> march(Cube cell, double isovalue) {
    int cube_index = 0;

    glm::vec3 vertices[12] = {glm::vec3(0.0f)};

    // Index into edge table (8-bit number)
    if(cell.samples[0] < isovalue)
        cube_index |= 1;
    if(cell.samples[1] < isovalue)
        cube_index |= 2;
    if(cell.samples[2] < isovalue)
        cube_index |= 4;
    if(cell.samples[3] < isovalue)
        cube_index |= 8;
    if(cell.samples[4] < isovalue)
        cube_index |= 16;
    if(cell.samples[5] < isovalue)
        cube_index |= 32;
    if(cell.samples[6] < isovalue)
        cube_index |= 64;
    if(cell.samples[7] < isovalue)
        cube_index |= 128;


    // If cube is fully outside or inside surface, skip
    if(edgeTable[cube_index] == 0)
        return {};


    // Vertices where surface intersects cube
    if(edgeTable[cube_index] & 1)
        vertices[0] = vertex_lerp(cell.vertices[0], cell.vertices[1], cell.samples[0], cell.samples[1], isovalue);

    if(edgeTable[cube_index] & 2)
        vertices[1] = vertex_lerp(cell.vertices[1], cell.vertices[2], cell.samples[1], cell.samples[2], isovalue);

    if(edgeTable[cube_index] & 4)
        vertices[2] = vertex_lerp(cell.vertices[2], cell.vertices[3], cell.samples[2], cell.samples[3], isovalue);

    if(edgeTable[cube_index] & 8)
        vertices[3] = vertex_lerp(cell.vertices[3], cell.vertices[0], cell.samples[3], cell.samples[0], isovalue);

    if(edgeTable[cube_index] & 16)
        vertices[4] = vertex_lerp(cell.vertices[4], cell.vertices[5], cell.samples[4], cell.samples[5], isovalue);

    if(edgeTable[cube_index] & 32)
        vertices[5] = vertex_lerp(cell.vertices[5], cell.vertices[6], cell.samples[5], cell.samples[6], isovalue);

    if(edgeTable[cube_index] & 64)
        vertices[6] = vertex_lerp(cell.vertices[6], cell.vertices[7], cell.samples[6], cell.samples[7], isovalue);

    if(edgeTable[cube_index] & 128)
        vertices[7] = vertex_lerp(cell.vertices[7], cell.vertices[4], cell.samples[7], cell.samples[4], isovalue);

    if(edgeTable[cube_index] & 256)
        vertices[8] = vertex_lerp(cell.vertices[0], cell.vertices[4], cell.samples[0], cell.samples[4], isovalue);

    if(edgeTable[cube_index] & 512)
        vertices[9] = vertex_lerp(cell.vertices[1], cell.vertices[5], cell.samples[1], cell.samples[5], isovalue);

    if(edgeTable[cube_index] & 1024)
        vertices[10] = vertex_lerp(cell.vertices[2], cell.vertices[6], cell.samples[2], cell.samples[6], isovalue);

    if(edgeTable[cube_index] & 2048)
        vertices[11] = vertex_lerp(cell.vertices[3], cell.vertices[7], cell.samples[3], cell.samples[7], isovalue);



    std::vector<Triangle> triangles;
    for(int i=0; triTable[cube_index][i] != -1; i+=3){
        Triangle t{};
        t.v0 = vertices[triTable[cube_index][i]];
        t.v1 = vertices[triTable[cube_index][i+1]];
        t.v2 = vertices[triTable[cube_index][i+2]];
        triangles.push_back(t);
    }

    return triangles;

}





