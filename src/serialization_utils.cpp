#include "serialization_utils.hpp"

#include <map>

void dfs_print_nodes(const std::shared_ptr<node_t>& node, std::ostream& os, std::vector<std::vector<int>>& adj, std::map<std::shared_ptr<node_t>, int>& node_to_index, int& index) {
    if (node_to_index.find(node) == node_to_index.end()) {
        node_to_index[node] = index++;
        os << *(node) << "\n";
    }
    for (const auto& child : node->children) {
        if (node_to_index.find(child) == node_to_index.end()) {
            node_to_index[child] = index++;
            os << *(child) << "\n";
        }
        adj[node_to_index[node]].push_back(node_to_index[child]);
        dfs_print_nodes(child, os, adj, node_to_index, index);
    }
}

std::ostream& operator<<(std::ostream& os, const node_t& node) {
    int shapetype_int = static_cast<int>(node.shape->shapetype);
    os << shapetype_int << " " << node.shape->level << "\n";
    os << node.xrot << " " << node.yrot << " " << node.zrot << "\n";
    os << node.xpos << " " << node.ypos << " " << node.zpos << "\n";
    os << node.xscale << " " << node.yscale << " " << node.zscale;
    return os;
}

std::ostream& operator<<(std::ostream& os, const model_t& model) {
    if (!model.root_node) {
        os << "0\n";
        return os;
    }
    int index = 0;
    std::map<std::shared_ptr<node_t>, int> node_to_index;
    os << model.node_count << "\n"; // Number of nodes
    std::vector<std::vector<int>> adj(model.node_count);
    dfs_print_nodes(model.root_node, os, adj, node_to_index, index);
    for (const auto& neighbors : adj) {
        os << neighbors.size();
        for (int neighbor : neighbors) {
            os << " " << neighbor;
        }
        os << "\n";
    }
    return os;
}

std::shared_ptr<model_t> read_model(std::istream& is, GLuint vPosition, GLuint vColor) {
    int node_count;
    is >> node_count;
    if (node_count == 0) {
        return std::make_shared<model_t>(vPosition, vColor);
    }
    std::vector<std::shared_ptr<node_t>> nodes(node_count);
    for (int i = 0; i < node_count; ++i) {
        int shapetype_int, level;
        is >> shapetype_int >> level;
        shape_type_t shapetype = static_cast<shape_type_t>(shapetype_int);
        nodes[i] = std::make_shared<node_t>(std::weak_ptr<node_t>{}, shapetype, level, vPosition, vColor);
        is >> nodes[i]->xrot >> nodes[i]->yrot >> nodes[i]->zrot;
        is >> nodes[i]->xpos >> nodes[i]->ypos >> nodes[i]->zpos;
        is >> nodes[i]->xscale >> nodes[i]->yscale >> nodes[i]->zscale;
    }
    for (int i = 0; i < node_count; ++i) {
        int child_count;
        is >> child_count;
        for (int j = 0; j < child_count; ++j) {
            int child_index;
            is >> child_index;
            nodes[i]->add_child(nodes[child_index]);
            nodes[child_index]->parent = nodes[i];
        }
    }
    auto model = std::make_shared<model_t>(vPosition, vColor);
    model->root_node = nodes[0];
    model->node_count = node_count;
    model->selected_node = nodes[node_count - 1];
    return model;
}

std::istream& operator>>(std::istream& is, model_t& model) {
    auto new_model = read_model(is, model.vPosition, model.vColor);
    model.root_node = new_model->root_node;
    model.node_count = new_model->node_count;
    model.selected_node = new_model->selected_node;
    return is;
}