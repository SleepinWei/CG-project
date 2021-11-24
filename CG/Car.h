#ifndef CAR_H
#define CAR_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Car_Movement {
    CAR_FORWARD,
    CAR_BACKWARD,
    CAR_LEFT,
    CAR_RIGHT
};

class Car {
public:
    glm::vec3 Position;
    glm::vec3 Direction;

    float Yaw;
    float Speed, Turn;

    void updateDirection() {

    }

    void ProcessKeyboard(Car_Movement direction, float deltaTime) {
        if (direction == CAR_FORWARD)
            Position += Direction * Speed * deltaTime;
        if (direction == CAR_BACKWARD)
            Position -= Direction * Speed * deltaTime;
        if (direction == CAR_LEFT)
            Yaw += Turn * deltaTime;
        if (direction == CAR_RIGHT)
            Yaw -= Turn * deltaTime;
        updateDirection();
    }
};

#endif