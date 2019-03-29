#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

const float PROJECTILE_SPEED = 0.07;
const float PROJECTILE_COOLDOWN = 0.4;

namespace Projectiles {
    class Projectile {
        private:
            Shader shader;

        public:
            float angle = 0;
            float xOffSet = 0;
            float yOffSet = 0;

            Projectile(float width, float height, float x, float y, float angle);
            ~Projectile();

            bool render(float width, float height);
            void dislocateProjectile();
    };

    void loadModel(string modelPath);

    vector<Projectile*> * getProjectiles();

    void renderProjectiles(float width, float height);
    bool readyToFire();
    bool fireProjectile(float width, float height, float x, float y, float angle);
}

#endif