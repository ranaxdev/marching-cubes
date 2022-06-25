#include "MC.h"

/*
 * Function: x^2 + y^2 + z^2 -1 = 0
 * pair -> vec3 (pos), float (sampled val)
 */
std::vector<Cube> generate_sphere_samples(int grid_size) {

    std::vector<Cube> cells;

    glm::vec3 v0 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 v1 = glm::vec3 (1.0f, 0.0f, 0.0f);
    glm::vec3 v2 = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 v3 = glm::vec3(1.0f, 1.0f, 0.0f);

    glm::vec3 v4 = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 v5 = glm::vec3 (1.0f, 0.0f, 1.0f);
    glm::vec3 v6 = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::vec3 v7 = glm::vec3(1.0f, 1.0f, 1.0f);



    for(int x = 0; x < grid_size; x++){
        Cube c{
            {v0, v1, v2, v3, v4, v5, v6, v7}
        };
        cells.push_back(c);
        v0.y = 0.0f;
        v1.y = 0.0f;
        v2.y = 1.0f;
        v3.y = 1.0f;
        v4.y = 0.0f;
        v5.y = 0.0f;
        v6.y = 1.0f;
        v7.y = 1.0f;

        v0.x += 1.0f;
        v1.x += 1.0f;
        v2.x += 1.0f;
        v3.x += 1.0f;
        v4.x += 1.0f;
        v5.x += 1.0f;
        v6.x += 1.0f;
        v7.x += 1.0f;

        for(int y = 0; y < grid_size; y++){
            v0.y += 1.0f;
            v1.y += 1.0f;
            v2.y += 1.0f;
            v3.y += 1.0f;
            v4.y += 1.0f;
            v5.y += 1.0f;
            v6.y += 1.0f;
            v7.y += 1.0f;

        }

    }



    return cells;




}

double sample_sphere(glm::vec3 position) {
    return std::pow(position.x,2) + std::pow(position.y, 2) + std::pow(position.z, 2) - 1;
}
