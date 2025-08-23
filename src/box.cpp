#include "shape.hpp"
#include "shape_util.hpp"
#include <glm/gtc/matrix_transform.hpp>

void box_t::draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {
    std::unique_ptr<glm::mat4> ms_mult;

    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), static_cast<float>(xrot), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(yrot), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(zrot), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos));
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(xscale, yscale, zscale));
    
    
    for (const auto& mat : matrixStack) {
        if (!ms_mult) {
            ms_mult = std::make_unique<glm::mat4>(mat);
        } else {
            *ms_mult = (*ms_mult) * mat;
        }
    }
    
    *ms_mult = translation_matrix * rotation_matrix * scale_matrix;
    
    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
    glBindVertexArray (vao);
    glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
}

void box_t::set_color(const glm::vec4& new_color) {
    color = new_color;
    v_colors.clear();
    for (size_t i = 0; i < v_positions.size(); ++i) {
        v_colors.push_back(color);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4), v_colors.size() * sizeof(glm::vec4), v_colors.data());
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions.size() * sizeof(glm::vec4)));
}

box_t::box_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = BOX_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;

    vPosition = vPosition;
    vColor = vColor;

    xrot = yrot = zrot = 0.0f;
    xpos = ypos = zpos = 0.0f;
    xscale = yscale = zscale = 1.0f;

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

    face(v_positions, v_colors, 0, 1, 2, 3, vertices, color); // Front face
    face(v_positions, v_colors, 1, 5, 6, 2, vertices, color * 0.80f); // Right face
    face(v_positions, v_colors, 5, 4, 7, 6, vertices, color); // Back face
    face(v_positions, v_colors, 4, 0, 3, 7, vertices, color * 0.80f); // Left face
    face(v_positions, v_colors, 3, 2, 6, 7, vertices, color * 0.90f); // Top face
    face(v_positions, v_colors, 4, 5, 1, 0, vertices, color * 0.90f); // Bottom face

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
