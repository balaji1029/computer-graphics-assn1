#include "shape.hpp"
#include "shape_util.hpp"
#include <glm/gtc/matrix_transform.hpp>

void cylinder_t::draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {
    std::unique_ptr<glm::mat4> ms_mult;

    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), static_cast<float>(xrot), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(yrot), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(zrot), glm::vec3(0.0f, 0.0f, 1.0f));
    rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos));

    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(xscale, yscale, zscale));

    for (const auto& mat : matrixStack) {
        if (!ms_mult) {
            ms_mult = std::make_unique<glm::mat4>(mat);
        } else {
            *ms_mult = (*ms_mult) * mat;
        }
    }

    *ms_mult = (*ms_mult) * translation_matrix * rotation_matrix * scale_matrix;

    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
    glBindVertexArray (vao);
    glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
}

void cylinder_t::set_color(const glm::vec4& new_color) {
    color = new_color;
    v_colors.resize(v_positions.size(), color);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4), v_colors.size() * sizeof(glm::vec4), v_colors.data());
    std::cout << "Color set to (" << new_color.r << ", " << new_color.g << ", " << new_color.b << ", " << new_color.a << ")\n";
}   

cylinder_t::cylinder_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
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
