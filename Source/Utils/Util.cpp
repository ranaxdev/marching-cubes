#include "Util.h"


int output_triangles(Cube** cells, int num_cells, const char* filepath){
    FILE* fp;
    fp = fopen(filepath, "w");

    // Check for file open error
    if(fp == nullptr){
        std::printf("[ERROR] Could not open file %s for writing!\n", filepath);
        return -1;
    }

    std::vector<glm::vec3> vertices;
    std::vector<int> faces;
    bool used;

    // Print triangle points in v0, v1, v2\n format
    for(int c =0; c < num_cells; c++){
        for(int i=0; i < cells[c]->num_tris*3; i++){
            glm::vec3 vertex = glm::vec3(cells[c]->tris[i].x, cells[c]->tris[i].y, cells[c]->tris[i].z);
            used = false;
            // Search if vertex already exists
            for(const auto v: vertices){
                if(v == vertex)
                {
                    used = true;
                    break;
                }
            }
            if(used)
                continue;

            vertices.push_back(vertex);

            std::fprintf(fp, "v %f %f %f\n", cells[c]->tris[i].x, cells[c]->tris[i].y, cells[c]->tris[i].z);
        }
    }

    glm::vec3 v0, v1, v2;
    int fv0, fv1, fv2;

    for(int c=0; c < num_cells; c++){
        for(int i=0; i < cells[c]->num_tris*3; i+=3){
            v0 = glm::vec3(cells[c]->tris[i].x, cells[c]->tris[i].y, cells[c]->tris[i].z);
            v1 = glm::vec3(cells[c]->tris[i+1].x, cells[c]->tris[i+1].y, cells[c]->tris[i+1].z);
            v2 = glm::vec3(cells[c]->tris[i+2].x, cells[c]->tris[i+2].y, cells[c]->tris[i+2].z);

            if(v0 == v1 || v0 == v2 || v1 == v2){
                continue;
            }

            for(int j=0; j < vertices.size(); j++){
                if(v0 == vertices[j])
                    fv0 = j;
                if(v1 == vertices[j])
                    fv1 = j;
                if(v2 == vertices[j])
                    fv2 = j;
            }

            faces.push_back(fv0);
            faces.push_back(fv1);
            faces.push_back(fv2);
        }
        for(int i=0; i < faces.size(); i+=3){
            std::fprintf(fp, "f %d %d %d\n", faces[i], faces[i+1], faces[i+2]);

        }

        faces.clear();
    }


    fclose(fp);

    return 1;
}


int output_pointcloud(std::uint8_t*** data, int res, const char* filepath){
    FILE* fp;
    fp = fopen(filepath, "w");

    // Check for file open error
    if(fp == nullptr){
        std::printf("[ERROR] Could not open file %s for writing!\n", filepath);
        return -1;
    }

    // Print point cloud in x,y,z\n format
    for(int x =0; x < res; x++){
        for(int y =0; y < res; y++){
            for(int z=0; z < res; z++){

            }
        }
    }
}