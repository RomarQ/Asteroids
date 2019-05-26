#include "collision.hpp"

namespace Collision {
    AABB getAABB(const Mesh& mesh, double scale)
    {
        AABB hitBox;
        for (Vertex v : mesh.vertices) {
            if (v.Position.x*scale < hitBox.min.x) hitBox.min.x = v.Position.x*scale;
            if (v.Position.y*scale < hitBox.min.y) hitBox.min.y = v.Position.y*scale;
            if (v.Position.z*scale < hitBox.min.z) hitBox.min.z = v.Position.z*scale;

            if (v.Position.x*scale > hitBox.max.x) hitBox.max.x = v.Position.x*scale;
            if (v.Position.y*scale > hitBox.max.y) hitBox.max.y = v.Position.y*scale;
            if (v.Position.z*scale > hitBox.max.z) hitBox.max.z = v.Position.z*scale;
        }

        return hitBox;
    }
}