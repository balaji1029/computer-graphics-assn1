#pragma once

#include "shape.hpp"
#include "model.hpp"

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class WindowManager {
    GLFWwindow* window;
    std::vector<glm::mat4> matrixStack;
    
    GLuint shaderProgram;
    GLuint vbo, vao;

    
    std::string vertex_shader_file = "shaders/vshader.glsl";
    std::string fragment_shader_file = "shaders/fshader.glsl";

    glm::mat4 c_rotation_matrix;
    glm::mat4 c_view_matrix;
    glm::mat4 c_ortho_matrix;

    bool enable_culling;
    bool enable_wireframe;

    
    public:
    // Translation Parameters
    GLfloat xpos = 0.0, ypos = 0.0, zpos = 0.0;
    // Rotation Parameters
    GLfloat xrot = 0.0, yrot = 0.0, zrot = 0.0;
    GLuint vPosition, vColor, uModelViewProjectMatrix;
    // std::vector<std::shared_ptr<node_t>> nodes;
    std::weak_ptr<node_t> selected_node;
    std::shared_ptr<node_t> root_node;
    WindowManager(int width, int height, const char* title);
    void addNode(shape_type_t type, uint32_t level);
    void initBuffersGL();
    void renderGL();
    void mainLoop();
    ~WindowManager();
};