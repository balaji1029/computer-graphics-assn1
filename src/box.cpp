#include "shape.hpp"

void face(std::vector<glm::vec4>& v_positions, std::vector<glm::vec4>& v_colors, int a, int b, int c, int d, glm::vec4* positions, glm::vec4 color) {
    v_colors.push_back(color); v_positions.push_back(positions[a]);
    v_colors.push_back(color); v_positions.push_back(positions[b]);
    v_colors.push_back(color); v_positions.push_back(positions[c]);
    v_colors.push_back(color); v_positions.push_back(positions[a]);
    v_colors.push_back(color); v_positions.push_back(positions[c]);
    v_colors.push_back(color); v_positions.push_back(positions[d]);
}

box_t::box_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = BOX_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;

    // Define the 8 vertices of the box
    vertices = {
        glm::vec4(-0.5, -0.5,  0.5, 1.0),
        glm::vec4( 0.5, -0.5,  0.5, 1.0),
        glm::vec4( 0.5,  0.5,  0.5, 1.0),
        glm::vec4(-0.5,  0.5,  0.5, 1.0),
        glm::vec4(-0.5, -0.5, -0.5, 1.0),
        glm::vec4( 0.5, -0.5, -0.5, 1.0),
        glm::vec4( 0.5,  0.5, -0.5, 1.0),
        glm::vec4(-0.5,  0.5, -0.5, 1.0)
    };

    face(v_positions, v_colors, 0, 1, 2, 3, vertices.data(), color); // Front face
    face(v_positions, v_colors, 1, 5, 6, 2, vertices.data(), color); // Right face
    face(v_positions, v_colors, 5, 4, 7, 6, vertices.data(), color); // Back face
    face(v_positions, v_colors, 4, 0, 3, 7, vertices.data(), color); // Left face
    face(v_positions, v_colors, 3, 2, 6, 7, vertices.data(), color); // Top face
    face(v_positions, v_colors, 4, 5, 1, 0, vertices.data(), color); // Bottom face

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
