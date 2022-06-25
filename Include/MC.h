#ifndef _H_MC
#define _H_MC

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>


/*
 * A single marching cube cell
 * Contains vertices of the cube & sampled values at the vertices
 */
struct Cube{
    glm::vec3 vertices[8];
    double samples[8];
};

std::vector<Cube> generate_sphere_samples(int grid_size);
double sample_sphere(glm::vec3 position);


#endif
