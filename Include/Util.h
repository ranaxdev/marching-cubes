#ifndef _UTIL_H
#define _UTIL_H

#include "MC.h"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

int output_triangles(Cube** cells, int num_cells, const char* filepath);

std::vector<int> parse_nhdr_sizes(const char* filepath);

// Helpers
std::string trim_string(std::string s);
void split_string(std::string s, std::vector<std::string>& v);

#endif
