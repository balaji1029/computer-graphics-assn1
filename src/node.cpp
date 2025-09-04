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
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos));
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(xscale, yscale, zscale));
    matrixStack.push_back(translation_matrix * rotation_matrix);
    matrixStack.push_back(scale_matrix);
    if (shape) {
        shape->draw(matrixStack, uModelViewMatrix);
    }
    // std::cout << "Centroid: " << centroid.x << ", " << centroid.y << ", " << centroid.z << "\n";
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

void node_t::move_by(const glm::vec4& delta) {
    float factor = 0.1;
    xpos += delta.x * factor;
    ypos += delta.y * factor;
    zpos += delta.z * factor;
    centroid += delta;
}

void node_t::rotate_by(const glm::vec3& axis, float angle) {
    if (axis.x == 1.0f) {
        xrot += angle;
    }
    else if (axis.y == 1.0f) {
        yrot += angle;
    }
    else if (axis.z == 1.0f) {
        zrot += angle;
    }
}

void node_t::scale_by(const glm::vec3& factor) {
    xscale += factor.x;
    if (xscale < 0.1f) xscale = 0.1f;
    yscale += factor.y;
    if (yscale < 0.1f) yscale = 0.1f;
    zscale += factor.z;
    if (zscale < 0.1f) zscale = 0.1f;
}