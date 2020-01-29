#pragma once

#include <GL/glew.h>
#include <string>
#include "Object.h"
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <iostream>


using namespace std;

class Primitive : public Object
{
	virtual void initVerticies() = 0;
	virtual void initIndicies() = 0;

	glm::vec3 positionVec;
	glm::vec3 rotationVec;
	glm::vec3 scaleVec;
	GLuint VAO, VBO, EBO;

	bool textured;
	GLuint textureId;
	vec4 color;

protected:
	// te wartosci musza zostac przypisane przez klase potomna w metodach wirtualnych
	vector<GLfloat> vertices;
	vector<GLuint> indices;

	string textureName;

public:

	Primitive() :
		positionVec(glm::vec3(0.0f, 0.0f, 0.0f)),
		rotationVec(glm::vec3(0.0f, 0.0f, 0.0f)),
		scaleVec(glm::vec3(1.0f, 1.0f, 1.0f))
	{}

	Primitive(string textureName) : Primitive() {
		this->textureName = textureName;
		this->textured = true;
	}

	Primitive(glm::vec4 color) : Primitive() {
		this->color = color;
		this->textured = false;
	}

	virtual ~Primitive()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	
	// nie moge uzyc metody wirtualnej w konstruktorze
	void init() {
		initVerticies();
		initIndicies();
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// normal 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		if (textured) {
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

			textureId = texture;
		}
	}

	void draw(int shaderId, Camera camera) override
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, positionVec);
		model = glm::rotate(model, glm::radians(rotationVec.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotationVec.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotationVec.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scaleVec);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, &model[0][0]);

		if (textured) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glUniform1i(glGetUniformLocation(shaderId, "Texture"), 0);
		}
		else {
			glUniform4fv(glGetUniformLocation(shaderId, "Color"), 1, &color[0]);
		}


		glBindVertexArray(VAO);
		
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void move(const glm::vec3& vector) override
	{
		this->positionVec += vector;
	}

	void scale(const glm::vec3& vector) override
	{
		this->scaleVec += vector;
	}

	void rotate(const glm::vec3& vector) override
	{
		this->rotationVec += vector;
	}


};
