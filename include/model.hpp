#pragma once

#include "shape.hpp"

#include <memory>

class node_t {
    std::weak_ptr<node_t> parent;
    std::vector<std::shared_ptr<node_t>> children;
public:
    std::shared_ptr<shape_t> shape;
    node_t(const std::shared_ptr<shape_t>& parent = nullptr, const std::shared_ptr<shape_t>& shape);
    void new_child(shape_type_t type, uint32_t level = 0, GLuint vPosition = 0, GLuint vColor = 0);
};

class model_t {
    node_t root_node;
};