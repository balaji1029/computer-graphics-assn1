#include "shape.hpp"

#include "glm/gtc/matrix_transform.hpp"

void shape_t::draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {
    std::unique_ptr<glm::mat4> ms_mult;

    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), static_cast<float>(xrot), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(yrot), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(zrot), glm::vec3(0.0f, 0.0f, 1.0f));
    // std::cout << xrot << " " << yrot << " " << zrot << std::endl;

    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos));

    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(xscale, yscale, zscale));

    for (const auto& mat : matrixStack) {
        if (!ms_mult) {
            ms_mult = std::make_unique<glm::mat4>(mat);
        }
        else {
            *ms_mult = (*ms_mult) * mat;
        }
    }

    *ms_mult = (*ms_mult) * translation_matrix * rotation_matrix * scale_matrix;

    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
}

void shape_t::set_color(const glm::vec4& new_color) {
    color = new_color;
    for (int i=0; i < color_factors.size(); ++i) {
        v_colors[i] = color * color_factors[i];
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4), v_colors.size() * sizeof(glm::vec4), v_colors.data());
}