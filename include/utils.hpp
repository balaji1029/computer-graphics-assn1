#pragma once

#include "shape.hpp"

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class WindowManager {
    GLFWwindow* window;
    std::vector<std::shared_ptr<shape_t>> shapes;
    std::vector<glm::mat4> matrixStack;

    GLuint shaderProgram;
    GLuint vbo, vao;

    GLuint vPosition, vColor, uModelViewProjectMatrix;

    std::string vertex_shader_file = "shaders/vshader.glsl";
    std::string fragment_shader_file = "shaders/fshader.glsl";

    glm::mat4 c_rotation_matrix;
    glm::mat4 c_view_matrix;
    glm::mat4 c_ortho_matrix;

    bool enable_culling = false;
    bool enable_wireframe = false;

    // Translation Parameters
    GLfloat xpos = 0.0, ypos = 0.0, zpos = 0.0;
    // Rotation Parameters
    GLfloat xrot = 0.0, yrot = 0.0, zrot = 0.0;

    
public:
    std::weak_ptr<shape_t> selected_shape;
    WindowManager(int width, int height, const char* title);
    void initBuffersGL();
    void renderGL();
    void mainLoop();
    ~WindowManager();
};