#include "MC.h"

/*
 * Function: x^2 + y^2 + z^2 -1 = 0
 * pair -> vec3 (pos), float (sampled val)
 */
std::vector<std::pair<glm::vec3, float>> generate_sphere_samples(int grid_size) {
    std::vector<std::pair<glm::vec3, float>> samples;

    for(int x = 0; x < grid_size; x++){
        for(int y = 0; y < grid_size; y++){
            for(int z = 0; z < grid_size; z++){
                double s = std::pow(x,2) + std::pow(y, 2) + std::pow(z, 2) -1;
                auto sample = (float) s;

                std::pair<glm::vec3, float> curr = std::make_pair(glm::vec3(x,y,z), sample);
                samples.push_back(curr);
            }
        }
    }

    return samples;




}
