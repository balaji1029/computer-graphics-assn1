#pragma once

#include <vector>

#include <memory>
#include <GL/glew.h>
#include "gl_framework.hpp"
#include "glm/gtc/type_ptr.hpp"


#define DEFAULT_COLOR glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)

enum shape_type_t {
    SPHERE_SHAPE, 
    CYLINDER_SHAPE, 
    BOX_SHAPE, 
    CONE_SHAPE
};

class shape_t {
protected:
    glm::vec4 centroid;
    std::vector<glm::vec4> vertices;
    shape_type_t shapetype;
    uint32_t level;
    glm::vec4 color;
    std::vector<float> color_factors;
    
    GLuint vao, vbo;
    GLuint vPosition, vColor;
    
    std::vector<glm::vec4> v_positions;
    std::vector<glm::vec4> v_colors;
    
public:
    GLfloat xrot = 0, yrot = 0, zrot = 0;
    GLfloat xpos = 0, ypos = 0, zpos = 0;
    GLfloat xscale = 1.0f, yscale = 1.0f, zscale = 1.0f;
    shape_t(uint32_t level, GLuint vPosition = 0, GLuint vColor = 0) {};
    virtual ~shape_t() {};
    void draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const;
    void set_color(const glm::vec4& new_color);
};

class sphere_t : public shape_t {
public:
    sphere_t(uint32_t level = 0, GLuint vPosition = 0, GLuint vColor = 0);
    void draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const override;
    void set_color(const glm::vec4& new_color) override;
};

class cylinder_t : public shape_t {
public:
    cylinder_t(uint32_t level = 0, GLuint vPosition = 0, GLuint vColor = 0);
};

class box_t : public shape_t {
public:
    box_t(uint32_t level = 0, GLuint vPosition = 0, GLuint vColor = 0);
};

class cone_t : public shape_t {
public:
    cone_t(uint32_t level = 0, GLuint vPosition = 0, GLuint vColor = 0);
};
