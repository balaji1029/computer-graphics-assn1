#include "shape.hpp"
#include "shape_util.hpp"

cone_t::cone_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = CONE_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;
    this->level = level;

    vPosition = vPosition;
    vColor = vColor;

    // Define the 8 vertices of the box
    int num_vertices = (level + 1) * 10;

    for (int i=0; i < num_vertices; ++i) {
        double angle = (2 * M_PI * i) / num_vertices;
        double x = 0.5 * cos(angle);
        double z = 0.5 * sin(angle);
        vertices.push_back(glm::vec4(x, -0.5, z, 1.0)); // Bottom circle
    }

    // Top and bottom centers
    vertices.push_back(glm::vec4(0.0, 0.5, 0.0, 1.0)); // Top center
    vertices.push_back(glm::vec4(0.0, -0.5, 0.0, 1.0)); // Bottom center

    for (int i = 0; i < num_vertices; ++i) {
        int next = (i + 1) % num_vertices;
        triangle(v_positions, v_colors, i, next, vertices.size() - 2, vertices, color * 0.80f); // Side face
    }

    for (int i = 0; i < num_vertices; ++i) {
        int next = (i + 1) % num_vertices;
        triangle(v_positions, v_colors, i, vertices.size() - 1, next, vertices, color); // Bottom face
    }

    color_factors = std::vector<float>(num_vertices * 3, 0.80f);
    std::vector<float> bottom_factors(num_vertices * 3, 1.0f);
    color_factors.insert(color_factors.end(), bottom_factors.begin(), bottom_factors.end());


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
