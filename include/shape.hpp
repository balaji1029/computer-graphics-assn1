#pragma once

#include <vector>

#include <glm/glm.hpp>

enum shape_type_t {
    SPHERE_SHAPE, 
    CYLINDER_SHAPE, 
    BOX_SHAPE, 
    CONE_SHAPE
};

class shape_t {
    glm::vec4 centroid;
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> colors;
    shape_type_t shapetype;
    uint32_t level;

public:
    shape_t(uint32_t level = 0);
    virtual void draw();
};

class sphere_t : public shape_t {
public:
    sphere_t(uint32_t level = 0);
    void draw() override;
};

class cylinder_t : public shape_t {
public:
    cylinder_t(uint32_t level = 0);
    void draw() override;
};

class box_t : public shape_t {
public:
    box_t(uint32_t level = 0);
    void draw() override;
};

class cone_t : public shape_t {
public:
    cone_t(uint32_t level = 0);
    void draw() override;
};
