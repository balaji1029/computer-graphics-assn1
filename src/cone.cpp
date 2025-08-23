#include "shape.hpp"
#include "shape_util.hpp"

void cone_t::draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {
    std::unique_ptr<glm::mat4> ms_mult;

    for (const auto& mat : matrixStack) {
        if (!ms_mult) {
            ms_mult = std::make_unique<glm::mat4>(mat);
        } else {
            *ms_mult = (*ms_mult) * mat;
        }
    }

    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
    glBindVertexArray (vao);
    glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
}

void cone_t::set_color(const glm::vec4& new_color) {
    color = new_color;
    v_colors.clear();
    for (size_t i = 0; i < v_positions.size(); ++i) {
        v_colors.push_back(color);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4), v_colors.size() * sizeof(glm::vec4), v_colors.data());
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions.size() * sizeof(glm::vec4)));
}

cone_t::cone_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = BOX_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;

    vPosition = vPosition;
    vColor = vColor;

    // Define the 8 vertices of the box
    int num_vertices = (level + 1) * 10;

    for (int i=0; i < num_vertices; ++i) {
        double angle = (2 * M_PI * i) / num_vertices;
        double x = 0.5 * cos(angle);
        double y = 0.5 * sin(angle);
        
        vertices.push_back(glm::vec4(x, y, -0.5, 1.0)); // Bottom circle
    }

    // Top and bottom centers
    vertices.push_back(glm::vec4(0.0, 0.0, 1.5, 1.0)); // Top center
    vertices.push_back(glm::vec4(0.0, 0.0, -0.5, 1.0)); // Bottom center

    for (int i = 0; i < num_vertices; ++i) {
        int next = (i + 1) % num_vertices;
        triangle(v_positions, v_colors, i, next, vertices.size() - 2, vertices, color); // Side face
        triangle(v_positions, v_colors, i, next, vertices.size() - 1, vertices, color); // Bottom face
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
