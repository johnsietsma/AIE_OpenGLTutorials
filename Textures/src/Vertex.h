#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Vertex_Color {
    glm::vec4 position;
    glm::vec4 colour;
};

struct Vertex_TexCoord {
    glm::vec4 position;
    glm::vec2 texCoord;
};


struct Vertex_Normal  {
    glm::vec4 position;
    glm::vec4 normal;
};
