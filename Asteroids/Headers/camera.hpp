#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <vector>

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    /*
    *   Used to destroy objects when they are out of user POV
    */
    bool isInsideFrustum(glm::mat4 viewMatrix)
    {
        glm::vec4 rowX = glm::row(viewMatrix, 0);
        glm::vec4 rowY = glm::row(viewMatrix, 1);
        glm::vec4 rowZ = glm::row(viewMatrix, 2);
        glm::vec4 rowW = glm::row(viewMatrix, 3);

        glm::vec4 planes[6];
        planes[0] = normalize(rowW + rowX);
        planes[1] = normalize(rowW - rowX);
        planes[2] = normalize(rowW + rowY);
        planes[3] = normalize(rowW - rowY);
        planes[4] = normalize(rowW + rowZ);
        planes[5] = normalize(rowW - rowZ);
   
        for(int i = 0; i<6; i++) {
            // Normalizing the planes.
            glm::vec3 normal(planes[i].x, planes[i].y, planes[i].z);
            float length = glm::length(normal);
            planes[i] = -planes[i] / length; // Notice the negation. I don't know why I needed that!!
        }

        /*
        *   Verify if object is inside the frustum
        */
        for (glm::vec4 plane : planes)
        {
            
            if (
                -plane.w < plane.x && plane.x < plane.w &&
                -plane.w < plane.y && plane.y < plane.w &&
                -plane.w < plane.z && plane.z < plane.w
            )
            {
                return false;
            }
        }
        
        return true;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif