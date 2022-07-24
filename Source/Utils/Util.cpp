#include "Util.h"


int output_triangles(Cube** cells, int num_cells, const char* filepath){
    FILE* fp;
    fp = fopen(filepath, "w");

    // Check for file open error
    if(fp == nullptr){
        std::printf("[ERROR] Could not open file %s for writing!\n", filepath);
        return -1;
    }

    // Print triangle points in v0, v1, v2\n format
    for(int c =0; c < num_cells; c++){
        for(int i=0; i < cells[c]->num_tris*3; i++){
            std::fprintf(fp, "%f, %f, %f\n", cells[c]->tris[i].x, cells[c]->tris[i].y, cells[c]->tris[i].z);
        }
    }

    fclose(fp);

    return 1;
}