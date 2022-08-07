#ifndef _UTIL_H
#define _UTIL_H
#include "MC.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

int output_triangles(Cube** cells, int num_cells, const char* filepath);

int output_pointcloud(std::uint8_t*** data, int res, const char* filepath);

#endif
