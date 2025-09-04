#include "model.hpp"

model_t::model_t(GLuint vPosition, GLuint vColor) : vPosition{ vPosition }, vColor{ vColor } {
    node_count = 0;
    root_node = nullptr;
    selected_node = std::weak_ptr<node_t>{};
}

void model_t::update_centroid() {
    glm::vec4 new_centroid, stack_centroid;
    auto dfs = [] (auto&& self, std::shared_ptr<node_t> curr, glm::vec4& new_centroid, glm::vec4& stack_centroid) -> void {
        if (!curr) return;
        stack_centroid += glm::vec4(curr->xpos, curr->ypos, curr->zpos, 0);
        new_centroid += stack_centroid;
        for(auto child:curr->children) {
            self(self, child, new_centroid, stack_centroid);
        }
        stack_centroid -= glm::vec4(curr->xpos, curr->ypos, curr->zpos, 0);
    };
    if (!root_node) {
        centroid = glm::vec4{0, 0, 0, 0};
        return;
    }
    dfs(dfs, root_node, new_centroid, stack_centroid);
    new_centroid /= node_count;
    centroid = new_centroid;
}

void model_t::addNode(shape_type_t type, uint32_t level) {
    node_count++;
    if (root_node) {
        std::shared_ptr<node_t> new_node = std::make_shared<node_t>(selected_node, type, level, vPosition, vColor);
        auto parent_node = selected_node.lock();
        new_node->shape->centroid = parent_node->shape->centroid;
        update_centroid();
        parent_node->add_child(new_node);
        selected_node = new_node;
    }
    else {
        root_node = std::make_shared<node_t>(std::weak_ptr<node_t>{}, type, level, vPosition, vColor);
        selected_node = root_node;
    }
}

void model_t::duplicateNode() {
    node_count++;
    std::shared_ptr<node_t> current_node = selected_node.lock();
    std::shared_ptr<node_t> new_node = std::make_shared<node_t>(selected_node, current_node->shape->shapetype, current_node->shape->level, vPosition, vColor);
    new_node->xscale = current_node->xscale;
    new_node->yscale = current_node->yscale;
    new_node->zscale = current_node->zscale;
    new_node->set_color(current_node->shape->color);
    new_node->shape->centroid = current_node->shape->centroid;
    update_centroid();
    current_node->add_child(new_node);
    selected_node = new_node;
}

void model_t::removeSelectedNode() {
    if (selected_node.lock()) {
        auto node = this->selected_node.lock();
        if (node->children.size() > 0) {
            std::cerr << "Cannot delete node with children\n";
            return;
        }
        else {
            node_count--;
            auto parent = node->parent.lock();
            if (parent) {
                auto& siblings = parent->children;
                for (auto it = siblings.begin(); it != siblings.end(); ++it) {
                    if ((*it)->index == node->index) {
                        siblings.erase(it);
                        break;
                    }
                }
                selected_node = parent;
            }
            else {
                root_node = nullptr;
                selected_node = std::weak_ptr<node_t>{};
            }
            node_count--;
        }
    }
}

void model_t::draw(std::vector<glm::mat4, std::allocator<glm::mat4>>& matrixStack, GLuint uModelViewMatrix, bool inspection_mode) {
    this->update_centroid();
    glm::mat4 neg_centroid = glm::translate(glm::mat4(1.0f), -glm::vec3(centroid));
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), static_cast<float>(xrot), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(yrot), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(zrot), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 pos_centroid = glm::translate(glm::mat4(1.0f), glm::vec3(centroid));
    // std::cout << "Model Centroid: " << centroid.x << ", " << centroid.y << ", " << centroid.z << "\n";
    if (inspection_mode) {
        matrixStack.push_back(rotation_matrix);
        matrixStack.push_back(neg_centroid);
    }
    if (root_node) {
        root_node->draw(matrixStack, uModelViewMatrix);
    }
    // int x;
    // std::cin >> x;
}