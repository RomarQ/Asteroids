#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

#include "mesh.hpp"

typedef struct aabb
{
    glm::vec3 min;
    glm::vec3 max;
} AABB;

namespace Collision {
    AABB getAABB(const Mesh& mesh, double scale);
}

#endif