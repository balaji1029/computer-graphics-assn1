#include "shape.hpp"

#include "glm/gtc/matrix_transform.hpp"

void shape_t::draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {
    std::unique_ptr<glm::mat4> ms_mult;

    for (const auto& mat : matrixStack) {
        if (!ms_mult) {
            ms_mult = std::make_unique<glm::mat4>(mat);
        }
        else {
            *ms_mult = (*ms_mult) * mat;
        }
    }

    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
}

void shape_t::set_color(const glm::vec4& new_color) {
    color = new_color;
    v_colors.assign(v_colors.size(), color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4), v_colors.size() * sizeof(glm::vec4), v_colors.data());
}

shape_t::~shape_t() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}