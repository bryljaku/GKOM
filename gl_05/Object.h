#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

class Object {
public:
	
	virtual void draw(int shaderId, Camera camera) = 0;
	virtual void move(const glm::vec3& vector) = 0;
	virtual void move2(const glm::vec3& vector) = 0;
	virtual void scale(const glm::vec3& vector) = 0;
	virtual void scale2(const glm::vec3& vector) = 0;
	virtual void rotate(const glm::vec3& vector) = 0;
	virtual void rotate2(const glm::vec3& vector) = 0;

	virtual ~Object() {}
};
