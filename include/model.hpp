#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "shape.hpp"
#include "glm/glm.hpp"

class node_t {
public:
    std::vector<std::shared_ptr<node_t>> children;
    std::weak_ptr<node_t> parent;
    std::shared_ptr<shape_t> shape;
    GLfloat xrot, yrot, zrot;
    GLfloat xpos, ypos, zpos;
    GLfloat xscale, yscale, zscale;
    node_t(const std::weak_ptr<node_t>& parent, enum shape_type_t type = shape_type_t::SPHERE_SHAPE, uint32_t level = 0, GLuint vPosition = 0, GLuint vColor = 0);
    void add_child(const std::shared_ptr<node_t>& child);
    void draw(std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const;
    void set_color(const glm::vec4& new_color);
};

class model_t {
public:
    node_t root_node;
};