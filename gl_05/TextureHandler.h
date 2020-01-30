#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
unsigned int loadTexture(string textureName) {
	// prepare textures 
	GLuint texture;
	glGenTextures(1, &texture);

	// texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture and generate mipmaps
	cout << "Nazwa tekstury: " << textureName.c_str() << endl;
	int width, height;
	unsigned char* image = SOIL_load_image(textureName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr) {
		throw exception("Failed to load texture file");
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}