#pragma once

#include "shape.hpp"

#include <memory>

class node_t {
    std::shared_ptr<shape_t> shape;
    glm::mat4 model_matrix;
    glm::mat4 rotation_matrix;
    glm::mat4 translation_matrix;
    glm::mat4 scaling_matrix;
    std::weak_ptr<node_t> parent;
    std::vector<std::shared_ptr<node_t>> children;

};

class model_t {
    node_t root_node;
};