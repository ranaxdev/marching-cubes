#include "Util.h"

// Generate .OBJ file out of triangles from isosurface
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


// Get dimension sizes from NHDR file
std::vector<int> parse_nhdr_sizes(const char* filepath){
    std::vector<int> dims;
    std::ifstream header(filepath);
    std::string line;

    while(true){
        std::getline(header, line);
        // End
        if(line.length() < 1)
            break;

        // Ignore comment
        if(line[0] == '#')
            continue;

        size_t delim = line.find(": "); // delimiter
        if (delim != std::string::npos){
            std::string field = line.substr(0, delim);
            std::string desc = trim_string(line.substr(delim + 2));

            // Care about dimensions field NX*NY*NZ
            if(field == "sizes"){
                std::vector<std::string> sizes;
                split_string(desc, sizes);

                dims.push_back(std::stoi(sizes[0]));
                dims.push_back(std::stoi(sizes[1]));
                dims.push_back(std::stoi(sizes[2]));

                break;
            }
        }
    }

    return dims;
}


// Create 3D buffer from parsing NRRD file data
std::uint8_t*** parse_nrrd_file(const char* filepath, int NX, int NY, int NZ){
    FILE* fileptr;
    std::uint8_t*** buffer = nullptr;
    long fsize;

    fileptr = fopen(filepath, "rb");

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

    return buffer;
}


// Helper functions
std::string trim_string(std::string s)
{
    size_t begin = 0;
    while ((begin < s.length()) && (isspace(s[begin])))
        begin++;

    size_t end = s.length();
    while ((end > begin) && (isspace(s[end - 1])))
        end--;
    return s.substr(begin, end - begin);
}



void split_string(std::string s, std::vector<std::string>& v){
    std::string t = "";
    for(char i : s){
        if(i==' '){
            v.push_back(t);
            t = "";
        }
        else{
            t.push_back(i);
        }
    }
    v.push_back(t);
}
