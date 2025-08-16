#include <bits/stdc++.h>
#include "glm/vec4.hpp"

enum class ShapeType {
    SPHERE_SHAPE,
    CYLINDER_SHAPE,
    BOX_SHAPE,
    CONE_SHAPE
};

class shape_t {
protected:
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> colors;
    ShapeType shapetype;
    uint32_t level;
public:
    virtual void draw() {}
    shape_t(uint32_t level) {
        this->level = level;
    }
    virtual ~shape_t()=0;
};