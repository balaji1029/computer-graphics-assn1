#pragma once
#include <vector>
#include <GL/glew.h>
#include "glm/vec4.hpp"
#include "glm/gtc/type_ptr.hpp"

void face(std::vector<glm::vec4>& v_positions, std::vector<glm::vec4>& v_colors, 
    int a, int b, int c, int d, const std::vector<glm::vec4>& positions, glm::vec4 color);

void triangle(std::vector<glm::vec4>& v_positions, std::vector<glm::vec4>& v_colors, 
    int a, int b, int c, const std::vector<glm::vec4>& positions, glm::vec4 color);