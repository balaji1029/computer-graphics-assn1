#pragma once

#include <vector>

#include <memory>
#include <GL/glew.h>
#include "gl_framework.hpp"
#include "glm/gtc/type_ptr.hpp"


#define DEFAULT_COLOR glm::vec4(1.0, 0.0, 0.0, 1.0)

enum shape_type_t {
    SPHERE_SHAPE, 
    CYLINDER_SHAPE, 
    BOX_SHAPE, 
    CONE_SHAPE
};

class shape_t {
protected:
    glm::vec4 centroid;
    glm::vec4 color;
    std::vector<glm::vec4> vertices;
    shape_type_t shapetype;
    uint32_t level;

    GLuint vao, vbo;

    std::vector<glm::vec4> v_positions;
    std::vector<glm::vec4> v_colors;
    
public:
    shape_t(uint32_t level)
    : centroid(0.0f), color(DEFAULT_COLOR), shapetype(BOX_SHAPE), level(level), vao(0), vbo(0) {};
    virtual ~shape_t() {};
    virtual void draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {};
};

class sphere_t : public shape_t {
public:
    sphere_t(uint32_t level = 0);
};

class cylinder_t : public shape_t {
public:
    cylinder_t(uint32_t level = 0, GLuint vPosition = 0, GLuint vColor = 0);
    void draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const override {
        //matrixStack multiply
        std::unique_ptr<glm::mat4> ms_mult;

        for (const auto& mat : matrixStack) {
            if (!ms_mult) {
                ms_mult = std::make_unique<glm::mat4>(mat);
            } else {
                *ms_mult = (*ms_mult) * mat;
            }
        }

        glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
        glBindVertexArray (vao);
        glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
    }
};

class box_t : public shape_t {
public:
    box_t(uint32_t level = 0, GLuint vPosition = 0, GLuint vColor = 0);
    void draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const override {
        //matrixStack multiply
        std::unique_ptr<glm::mat4> ms_mult;

        for (const auto& mat : matrixStack) {
            if (!ms_mult) {
                ms_mult = std::make_unique<glm::mat4>(mat);
            } else {
                *ms_mult = (*ms_mult) * mat;
            }
        }

        glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
        glBindVertexArray (vao);
        glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
    }
};

class cone_t : public shape_t {
public:
    cone_t(uint32_t level = 0);
};
