#include "shape.hpp"
#include "shape_util.hpp"

void face_box(std::vector<glm::vec4>& v_positions, 
          std::vector<glm::vec4>& v_colors, 
          glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d, 
          glm::vec4 color) 
{
    v_colors.push_back(color); v_positions.push_back(a);
    v_colors.push_back(color); v_positions.push_back(b);
    v_colors.push_back(color); v_positions.push_back(c);
    v_colors.push_back(color); v_positions.push_back(a);
    v_colors.push_back(color); v_positions.push_back(c);
    v_colors.push_back(color); v_positions.push_back(d);
}

void subdivided_face(std::vector<glm::vec4>& v_positions,
        std::vector<glm::vec4>& v_colors,
        glm::vec4 v0, glm::vec4 v1, glm::vec4 v2, glm::vec4 v3,
        glm::vec4 color, int level) {
        // v0 -- v1
        // |     |
        // v3 -- v2
        for (int i = 0; i < level; i++) {
        float ti0 = (float)i / level;
        float ti1 = (float)(i + 1) / level;

        glm::vec4 left0 = (1 - ti0) * v0 + ti0 * v3;
        glm::vec4 right0 = (1 - ti0) * v1 + ti0 * v2;
        glm::vec4 left1 = (1 - ti1) * v0 + ti1 * v3;
        glm::vec4 right1 = (1 - ti1) * v1 + ti1 * v2;


        for (int j = 0; j < level; j++) {
            float tj0 = (float)j / level;
            float tj1 = (float)(j + 1) / level;


            glm::vec4 p00 = (1 - tj0) * left0 + tj0 * right0;
            glm::vec4 p01 = (1 - tj1) * left0 + tj1 * right0;
            glm::vec4 p10 = (1 - tj0) * left1 + tj0 * right1;
            glm::vec4 p11 = (1 - tj1) * left1 + tj1 * right1;


            // Each small quad
            face_box(v_positions, v_colors, p00, p01, p11, p10, color);
        }
    }
}

box_t::box_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = BOX_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;
    this->level = level;


    vPosition = vPosition;
    vColor = vColor;


    // Define the 8 vertices of the box
    vertices = {
        glm::vec4(-0.25, -0.25, 0.25, 1.0),
        glm::vec4( 0.25, -0.25, 0.25, 1.0),
        glm::vec4( 0.25, 0.25, 0.25, 1.0),
        glm::vec4(-0.25, 0.25, 0.25, 1.0),
        glm::vec4(-0.25, -0.25, -0.25, 1.0),
        glm::vec4( 0.25, -0.25, -0.25, 1.0),
        glm::vec4( 0.25, 0.25, -0.25, 1.0),
        glm::vec4(-0.25, 0.25, -0.25, 1.0)
    };


    // Subdivided faces
    subdivided_face(v_positions, v_colors, vertices[0], vertices[1], vertices[2], vertices[3], color, level); // Front
    subdivided_face(v_positions, v_colors, vertices[1], vertices[5], vertices[6], vertices[2], color * 0.80f, level); // Right
    subdivided_face(v_positions, v_colors, vertices[5], vertices[4], vertices[7], vertices[6], color, level); // Back
    subdivided_face(v_positions, v_colors, vertices[4], vertices[0], vertices[3], vertices[7], color * 0.80f, level); // Left
    subdivided_face(v_positions, v_colors, vertices[3], vertices[2], vertices[6], vertices[7], color * 0.90f, level); // Top
    subdivided_face(v_positions, v_colors, vertices[4], vertices[5], vertices[1], vertices[0], color * 0.90f, level); // Bottom
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
