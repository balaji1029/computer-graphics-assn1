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
    int shapetype_int;
    switch(node.shape->shapetype) {
        case shape_type_t::SPHERE_SHAPE:
            shapetype_int = 0;
            break;
        case shape_type_t::CYLINDER_SHAPE:
            shapetype_int = 1;
            break;
        case shape_type_t::BOX_SHAPE:
            shapetype_int = 2;
            break;
        case shape_type_t::CONE_SHAPE:
            shapetype_int = 3;
            break;
    }
    os << shapetype_int << " " << node.shape->level << "\n";
    os << node.xrot << " " << node.yrot << " " << node.zrot << "\n";
    os << node.xpos << " " << node.ypos << " " << node.zpos << "\n";
    os << node.xscale << " " << node.yscale << " " << node.zscale << "\n";
    os << node.shape->color.r << " " << node.shape->color.g << " " << node.shape->color.b << " " << node.shape->color.a;
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
        shape_type_t shapetype;
        switch (shapetype_int) {
            case 0:
                shapetype = shape_type_t::SPHERE_SHAPE;
                break;
            case 1:
                shapetype = shape_type_t::CYLINDER_SHAPE;
                break;
            case 2:
                shapetype = shape_type_t::BOX_SHAPE;
                break;
            case 3:
                shapetype = shape_type_t::CONE_SHAPE;
                break;
            default:
                throw std::runtime_error("Invalid shape type in model file");
        }
        nodes[i] = std::make_shared<node_t>(std::weak_ptr<node_t>{}, shapetype, level, vPosition, vColor);
        is >> nodes[i]->xrot >> nodes[i]->yrot >> nodes[i]->zrot;
        is >> nodes[i]->xpos >> nodes[i]->ypos >> nodes[i]->zpos;
        is >> nodes[i]->xscale >> nodes[i]->yscale >> nodes[i]->zscale;
        is >> nodes[i]->shape->color.r >> nodes[i]->shape->color.g >> nodes[i]->shape->color.b >> nodes[i]->shape->color.a;
        nodes[i]->set_color(nodes[i]->shape->color);
        nodes[i]->centroid += glm::vec4(nodes[i]->xpos, nodes[i]->ypos, nodes[i]->zpos, 0);
        nodes[i]->index = i;
    }
    for (int i = 0; i < node_count; ++i) {
        int child_count;
        is >> child_count;
        for (int j = 0; j < child_count; ++j) {
            int child_index;
            is >> child_index;
            nodes[i]->add_child(nodes[child_index]);
            nodes[child_index]->parent = nodes[i];
            nodes[child_index]->centroid += nodes[i]->centroid;
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