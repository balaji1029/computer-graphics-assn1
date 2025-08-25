#include "shape.hpp"
#include "shape_util.hpp"
#include <glm/gtc/matrix_transform.hpp>

void sphere_t::draw(const std::vector<glm::mat4>& matrixStack, GLuint uModelViewMatrix) const {
    std::unique_ptr<glm::mat4> ms_mult;

    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), static_cast<float>(xrot), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(yrot), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, static_cast<float>(zrot), glm::vec3(0.0f, 0.0f, 1.0f));
    // std::cout << xrot << " " << yrot << " " << zrot << std::endl;

    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos));

    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(xscale, yscale, zscale));

    for (const auto& mat : matrixStack) {
        if (!ms_mult) {
            ms_mult = std::make_unique<glm::mat4>(mat);
        } else {
            *ms_mult = (*ms_mult) * mat;
        }
    }

    *ms_mult = (*ms_mult) * translation_matrix * rotation_matrix * scale_matrix;

    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
    glBindVertexArray (vao);
    glDrawArrays(GL_TRIANGLES, 0, v_positions.size());
}

void sphere_t::set_color(const glm::vec4& new_color) {
    color = new_color;
    v_colors.resize(v_positions.size(), color);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4), v_colors.size() * sizeof(glm::vec4), v_colors.data());
    std::cout << "Color set to (" << new_color.r << ", " << new_color.g << ", " << new_color.b << ", " << new_color.a << ")\n";
}   

sphere_t::sphere_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = SPHERE_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;

    vPosition = vPosition;
    vColor = vColor;
    int num_lat = 5 * (level + 2);  // latitude divisions
    int num_lng = 5 * (level + 2);  // longitude divisions

        // Bottom pole
    vertices.push_back(glm::vec4(0.0, 0.0, -1.0, 1.0));

    // Intermediate rings
    for (int lat = 1; lat < num_lat; ++lat) {
        double phi = -M_PI/2.0 + M_PI * lat / num_lat; // -90° to +90°
        for (int lng = 0; lng < num_lng; ++lng) {
            double theta = 2.0 * M_PI * lng / num_lng; // 0° to 360°
            double x = cos(phi) * cos(theta);
            double y = cos(phi) * sin(theta);
            double z = sin(phi);
            vertices.push_back(glm::vec4(x, y, z, 1.0));
        }
    }

    // Top pole
    vertices.push_back(glm::vec4(0.0, 0.0, 1.0, 1.0));

    int bottomPole = 0;
    int topPole = (int)vertices.size() - 1;

    // --- Build faces ---
    
    // Bottom fan
    for (int lng = 0; lng < num_lng; ++lng) {
        int curr = 1 + lng;
        int next = 1 + (lng + 1) % num_lng;
        triangle(v_positions, v_colors, bottomPole, next, curr, vertices, color);
    }

    // Middle quads (lat rings)
    for (int lat = 0; lat < num_lat - 2; ++lat) {
        for (int lng = 0; lng < num_lng; ++lng) {
            int curr     = 1 + lat * num_lng + lng;
            int next     = 1 + lat * num_lng + (lng + 1) % num_lng;
            int above    = 1 + (lat + 1) * num_lng + lng;
            int aboveNext= 1 + (lat + 1) * num_lng + (lng + 1) % num_lng;

            // Two triangles per quad
            triangle(v_positions, v_colors, curr, next, above, vertices, color);
            triangle(v_positions, v_colors, next, aboveNext, above, vertices, color);
        }
    }

    // Top fan
    int startLastRing = 1 + (num_lat - 2) * num_lng;
    for (int lng = 0; lng < num_lng; ++lng) {
        int curr = startLastRing + lng;
        int next = startLastRing + (lng + 1) % num_lng;
        triangle(v_positions, v_colors, topPole, curr, next, vertices, color);
    }


    // int num_lines = 2*(level + 2);
    // vertices.push_back(glm::vec4(0.0, 0.0, -1.0, 1.0)); // Bottom center

    // for(int lat = -num_lines; lat <= num_lines; ++lat) {
    //     for(int lng = -num_lines; lng <= num_lines; ++lng) {
    //         double theta = 2*M_PI*lat/(2*num_lines+1);
    //         double phi = M_PI*lng/(2*num_lines+1);
    //         vertices.push_back(glm::vec4(cos(phi)*cos(theta), cos(phi)*sin(theta), sin(phi), 1.0));
    //     }
    // }

    // vertices.push_back(glm::vec4(0.0, 0.0, 1.0, 1.0)); // Top center

    // // Bottom of the sphere
    // for(int i=1; i<=2*num_lines; ++i) {
    //     triangle(v_positions, v_colors, 0, i+1, i, vertices, color);
    // }
    // triangle(v_positions, v_colors, 0, 2*num_lines+1, 1, vertices, color);

    // // Top of the sphere
    // for(int i=vertices.size()-2*num_lines-2; i<=vertices.size()-3; ++i) {
    //     triangle(v_positions, v_colors, vertices.size()-1, i, i+1, vertices, color);
    // }
    // triangle(v_positions, v_colors, vertices.size()-1, vertices.size()-2, vertices.size()-2*num_lines-2, vertices, color);

    // // Rest of the sphere
    // int count = 1;
    // for(int lat=-num_lines; lat<num_lines; ++lat) {
    //     for(int lng=-num_lines; lng<num_lines; ++lng) {
    //         face(v_positions, v_colors, count, count+1, count+2*num_lines+2, count+2*num_lines+1, vertices, color);
    //         ++count;
    //     }
    //     face(v_positions, v_colors, count, count-2*num_lines, count+1, count+2*num_lines+1, vertices, color);
    //     ++count;
    // }

    // Ask GL for a vao
    glGenVertexArrays(1, &vao);
    // Set it as the current array to be used by binding it
    glBindVertexArray(vao);

    // Ask GL for a vbo
    glGenBuffers(1, &vbo);
    // Set it as the current buffer to be used by binding it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    
    // Copy the points into the current buffer
    glBufferData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4) + v_colors.size() * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, v_positions.size() * sizeof(glm::vec4), v_positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(glm::vec4), v_colors.size() * sizeof(glm::vec4), v_colors.data());

    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(v_positions.size() * sizeof(glm::vec4)));
}
