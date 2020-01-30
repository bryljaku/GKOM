#pragma once

#include "Primitive.h"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Cone : public Primitive {

public:
	GLfloat reps;

	Cone(glm::vec4 color) : Primitive(color) {
		this->reps = 0.0f; // to i tak jest nieuzywane
		init();
	}


private:
	void initVerticies() override {
		vector <GLfloat> vertices = coneVertices(30, 1.0f, 1.0f);
		this->vertices = std::move(vertices);
	}

	void initIndicies() override {
		vector<GLuint>  indices = coneIndices(30);
		this->indices = std::move(indices);
	}
	vector<GLfloat> coneVertices(int nrSides, GLfloat radius, GLfloat height) {
		vector<GLfloat> vertices =  vector<GLfloat>((nrSides + 2) * 8);

		for (int i = 0; i < (nrSides + 2) * 8; i++) {
			vertices[i] = 0.0f;
		}
		//points 0 and 1
		vertices[1] = height;
		//texture
		vertices[6] = 0.5f;
		vertices[7] = 1.0f;
		//normal
		vertices[5] = -1.0f;//5

		//tex
		vertices[14] = 0.5f;//14
		vertices[15] = 1.0f;//15
		//normal
		vertices[13] = 1.0f;//12

		//point 2
		vertices[16] = radius;//16

		//obracanie - macierz
		glm::mat4 rotationZ;
		rotationZ = glm::rotate(rotationZ, glm::radians(360.0f / nrSides), glm::vec3(0.0f, 1.0f, 0.0f));

		//wektor z 2. wierzcho³kiem
		glm::vec4 currentV = glm::vec4(radius, 0.0f, 0.0f, 1.0f);
		for (int i = -1; i < nrSides - 1; i++) {
			currentV = rotationZ * currentV;

			vertices[24 + i * 8] = currentV[0];
			vertices[25 + i * 8] = currentV[1];
			vertices[26 + i * 8] = currentV[2];

			//normals
			vertices[27 + i * 8] = -currentV[0];
			vertices[29 + i * 8] = -currentV[2];
			//texture
			if (i % 2 == 0)
				vertices[30 + i * 8] = (1.0f * 6) / nrSides;
		}
		return vertices;
	}

	vector<GLuint> coneIndices(int nrSides) {
		vector<GLuint> indices = vector<GLuint>(nrSides * 2 * 3);

		GLuint face = 2;
		indices[0] = nrSides + 1;
		indices[1] = 0;
		indices[2] = 2;

		for (int i = 3; i < (nrSides) * 3; i += 3) {
			indices[i] = face;
			indices[i + 1] = 0;
			face++;
			indices[i + 2] = face;
		}

		face = 2;
		indices[(nrSides) * 3] = nrSides + 1;
		indices[(nrSides) * 3 + 1] = 1;
		indices[(nrSides) * 3 + 2] = 2;

		for (int i = (nrSides) * 3 + 3; i < (nrSides) * 3 * 2; i += 3) {
			indices[i] = face;
			indices[i + 1] = 1;
			face++;
			indices[i + 2] = face;
		}

		return indices;
	}
};/**/