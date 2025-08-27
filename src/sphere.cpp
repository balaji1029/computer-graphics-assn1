#include "shape.hpp"
#include "shape_util.hpp"

sphere_t::sphere_t(uint32_t level, GLuint vPosition, GLuint vColor) : shape_t(level) {
    shapetype = SPHERE_SHAPE;
    centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);
    color = DEFAULT_COLOR;
    float radius = 0.5f;

    vPosition = vPosition;
    vColor = vColor;
    int num_lat = 5 * (level + 2);  // latitude divisions
    int num_lng = 5 * (level + 2);  // longitude divisions

    // Bottom pole
    vertices.push_back(glm::vec4(0.0, 0.0, -radius, 1.0));

    // Intermediate rings
    for (int lat = 1; lat < num_lat; ++lat) {
        double phi = -M_PI/2.0 + M_PI * lat / num_lat; // -90° to +90°
        for (int lng = 0; lng < num_lng; ++lng) {
            double theta = 2.0 * M_PI * lng / num_lng; // 0° to 360°
            double x = radius * cos(phi) * cos(theta);
            double y = radius * cos(phi) * sin(theta);
            double z = radius * sin(phi);
            vertices.push_back(glm::vec4(x, y, z, 1.0));
        }
    }

    // Top pole
    vertices.push_back(glm::vec4(0.0, 0.0, radius, 1.0));

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

            face(v_positions, v_colors, curr, next, aboveNext, above, vertices, color);
        }
    }

    // Top fan
    int startLastRing = 1 + (num_lat - 2) * num_lng;
    for (int lng = 0; lng < num_lng; ++lng) {
        int curr = startLastRing + lng;
        int next = startLastRing + (lng + 1) % num_lng;
        triangle(v_positions, v_colors, topPole, curr, next, vertices, color);
    }

    color_factors.assign(v_positions.size(), 1.0f);

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
