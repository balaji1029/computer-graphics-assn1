#include "model.hpp"

model_t::model_t(GLuint vPosition, GLuint vColor) : vPosition{ vPosition }, vColor{ vColor } {
    node_count = 0;
    root_node = nullptr;
    selected_node = std::weak_ptr<node_t>{};
}

void model_t::update_centroid() {
    glm::vec4 new_centroid;
    auto dfs = [&new_centroid] (auto&& self, std::shared_ptr<node_t> curr) -> void {
        new_centroid += curr->shape->centroid;
        for(auto child:curr->children) {
            self(self, child);
        }
    };
    dfs(dfs, root_node);
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

void model_t::draw(std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {
    if (root_node) {
        root_node->draw(matrixStack, uModelViewMatrix);
    }
}