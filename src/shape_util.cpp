#include "shape_util.hpp"

void face(std::vector<glm::vec4>& v_positions, std::vector<glm::vec4>& v_colors, int a, int b, int c, int d, const std::vector<glm::vec4>& positions, glm::vec4 color) {
    v_colors.push_back(color); v_positions.push_back(positions[a]);
    v_colors.push_back(color); v_positions.push_back(positions[b]);
    v_colors.push_back(color); v_positions.push_back(positions[c]);
    v_colors.push_back(color); v_positions.push_back(positions[a]);
    v_colors.push_back(color); v_positions.push_back(positions[c]);
    v_colors.push_back(color); v_positions.push_back(positions[d]);
}

void triangle(std::vector<glm::vec4>& v_positions, std::vector<glm::vec4>& v_colors, int a, int b, int c, const std::vector<glm::vec4>& positions, glm::vec4 color) {
    v_colors.push_back(color); v_positions.push_back(positions[a]);
    v_colors.push_back(color); v_positions.push_back(positions[b]);
    v_colors.push_back(color); v_positions.push_back(positions[c]);
}