#include "shape.hpp"

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

cylinder_t::cylinder_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = BOX_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;

    // Define the 8 vertices of the box
    int num_vertices = (level + 1) * 10;

    for (int i=0; i < num_vertices; ++i) {
        double angle = (2 * M_PI * i) / num_vertices;
        double x = 0.5 * cos(angle);
        double y = 0.5 * sin(angle);
        
        vertices.push_back(glm::vec4(x, y, 0.5, 1.0)); // Top circle
        vertices.push_back(glm::vec4(x, y, -0.5, 1.0)); // Bottom circle
    }

    // Top and bottom centers
    vertices.push_back(glm::vec4(0.0, 0.0, 0.5, 1.0)); // Top center
    vertices.push_back(glm::vec4(0.0, 0.0, -0.5, 1.0)); // Bottom center

    for (int i = 0; i < num_vertices; ++i) {
        int next = (i + 1) % num_vertices;
        face(v_positions, v_colors, i * 2, next * 2, next * 2 + 1, i * 2 + 1, vertices, color); // Side face
    }

    // Top face
    for (int i = 0; i < num_vertices; ++i) {
        int next = (i + 1) % num_vertices;
        triangle(v_positions, v_colors, i * 2, next * 2, vertices.size() - 2, vertices, color);
    }
    // Bottom face
    for (int i = 0; i < num_vertices; ++i) {
        int next = (i + 1) % num_vertices;
        triangle(v_positions, v_colors, i * 2 + 1, next * 2 + 1, vertices.size() - 1, vertices, color);
    }

    // Ask GL for a vao
    glGenVertexArrays(1, &vao);
    // Set it as the current array to be used by binding it
    glBindVertexArray(vao);

    // Ask GL for a vbo
    glGenBuffers(1, &vbo);
    // Set it as the current buffer to be used by binding it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    
    // Copy the points into the current buffer
    glBufferData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4) + v_colors.size() * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, v_positions.size() * sizeof(glm::vec4), v_positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4), v_colors.size() * sizeof(glm::vec4), v_colors.data());

    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions.size() * sizeof(glm::vec4)));
}
