#include "shape.hpp"
#include "shape_util.hpp"

box_t::box_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = BOX_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;
    this->level = level;

    vPosition = vPosition;
    vColor = vColor;

    // Define the 8 vertices of the box
    vertices = {
        glm::vec4(-0.25, -0.25,  0.25, 1.0),
        glm::vec4( 0.25, -0.25,  0.25, 1.0),
        glm::vec4( 0.25,  0.25,  0.25, 1.0),
        glm::vec4(-0.25,  0.25,  0.25, 1.0),
        glm::vec4(-0.25, -0.25, -0.25, 1.0),
        glm::vec4( 0.25, -0.25, -0.25, 1.0),
        glm::vec4( 0.25,  0.25, -0.25, 1.0),
        glm::vec4(-0.25,  0.25, -0.25, 1.0)
    };

    face(v_positions, v_colors, 0, 1, 2, 3, vertices, color); // Front face
    face(v_positions, v_colors, 1, 5, 6, 2, vertices, color * 0.80f); // Right face
    face(v_positions, v_colors, 5, 4, 7, 6, vertices, color); // Back face
    face(v_positions, v_colors, 4, 0, 3, 7, vertices, color * 0.80f); // Left face
    face(v_positions, v_colors, 3, 2, 6, 7, vertices, color * 0.90f); // Top face
    face(v_positions, v_colors, 4, 5, 1, 0, vertices, color * 0.90f); // Bottom face

    std::vector<float> front = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    std::vector<float> side = {0.80f, 0.80f, 0.80f, 0.80f, 0.80f, 0.80f};
    std::vector<float> top_bottom = {0.90f, 0.90f, 0.90f, 0.90f, 0.90f, 0.90f};
    color_factors.insert(color_factors.end(), front.begin(), front.end());
    color_factors.insert(color_factors.end(), side.begin(), side.end());
    color_factors.insert(color_factors.end(), front.begin(), front.end());
    color_factors.insert(color_factors.end(), side.begin(), side.end());
    color_factors.insert(color_factors.end(), top_bottom.begin(), top_bottom.end());
    color_factors.insert(color_factors.end(), top_bottom.begin(), top_bottom.end());


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
