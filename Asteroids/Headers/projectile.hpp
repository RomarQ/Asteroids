#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

#include "collision.hpp"
using namespace Collision;

const float PROJECTILE_SPEED = 0.07;
const float PROJECTILE_SPAWN_COOLDOWN = 0.4;
const float PROJECTILE_MOVEMENT_COOLDOWN = 0.0003;
const float PROJECTILE_SCALE = 0.05;

namespace Projectiles {
    class Projectile {
        private:
            Shader shader;

        public:
            float angle = 0.0;
            float xOffSet = 0.0;
            float yOffSet = 0.0;
            float projectileMovementTimestamp = 0;

            Projectile(float width, float height, float x, float y, float angle);
            ~Projectile();

            bool render(float width, float height, Camera camera);
            void dislocateProjectile();
    };

    void loadModel(string modelPath);

    vector<Projectile*> * getProjectiles();
    void destroyAll();

    AABB projectileHitbox();
    void renderProjectiles(float width, float height, Camera camera);
    bool readyToFire();
    bool fireProjectile(float width, float height, float x, float y, float angle);
}

#endif