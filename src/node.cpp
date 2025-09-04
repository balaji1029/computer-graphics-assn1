#include "model.hpp"
#include "glm/gtc/matrix_transform.hpp"

node_t::node_t(const std::weak_ptr<node_t>& parent, enum shape_type_t type, uint32_t level, GLuint vPosition, GLuint vColor){
    this->parent = parent;
    if (parent.lock()) {
        auto p = parent.lock();
        this->centroid = p->centroid;
    }
    xrot = yrot = zrot = 0.0f;
    xpos = ypos = zpos = 0.0f;
    xscale = yscale = zscale = 1.0f;
    switch(type) {
        case shape_type_t::SPHERE_SHAPE:
            shape = std::make_shared<sphere_t>(level, vPosition, vColor);
            break;
        case shape_type_t::CYLINDER_SHAPE:
            shape = std::make_shared<cylinder_t>(level, vPosition, vColor);
            break;
        case shape_type_t::BOX_SHAPE:
            shape = std::make_shared<box_t>(level, vPosition, vColor);
            break;
        case shape_type_t::CONE_SHAPE:
            shape = std::make_shared<cone_t>(level, vPosition, vColor);
            break;
    }
}

void node_t::add_child(const std::shared_ptr<node_t>& child) {
    children.push_back(child);
}

void node_t::draw(std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), static_cast<float>(xrot), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(yrot), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(zrot), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos));
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(xscale, yscale, zscale));
    matrixStack.push_back(translation_matrix * rotation_matrix);
    matrixStack.push_back(scale_matrix);
    if (shape) {
        shape->draw(matrixStack, uModelViewMatrix);
    }
    matrixStack.pop_back();
    for (const auto& child : children) {
        if (child) {
            child->draw(matrixStack, uModelViewMatrix);
        }
    }
    matrixStack.pop_back();
}

void node_t::set_color(const glm::vec4& new_color) {
    if (shape) {
        shape->set_color(new_color);
    }
}