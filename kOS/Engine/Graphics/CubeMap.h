/********************************************************************/
/*!
\file      CubeMap.h
\author    Gabe Ng 2301290 
\par       gabe.ng@digipen.edu
\date      Oct 03 2025
\brief     This file houses the cube map class, used for construction of
		   - The skybox
		   - The Shadow map
		   - The irradiance map

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "Texture.h"
#include "Config/pch.h"
#include "Shader.h"

class CubeMap {
public:
	virtual void InitializeMap(std::vector<std::string>);
	void LoadCubeTexture(std::vector<std::string>);
	void LoadCubeTextureDDS(std::vector<std::string>);

	void LoadCubeModel();
	int RetrieveID();
protected:
	GLuint VAO{}, VBO{};
	GLuint texID{};
};
class Skybox :public CubeMap {

public:
	void Render(Shader*, glm::mat4 const& view, glm::mat4 const& projection);
};

//class ShadowBox :public CubeMap {
//	void InitializeShadowMap();
//
//};
class IrradianceMap :public CubeMap {
	public:
		void InitializeMap();
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[6] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		void Render(Shader* irradianceShader, Skybox sb);
		void RenderCube(Shader*, glm::mat4 const& view, glm::mat4 const& projection);

};

class DepthCubeMap :public CubeMap {
public:
	void InitializeMap();
	void FillMap(glm::vec3& lightPos);
	glm::mat4 shadowTransforms[6];
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	GLuint GetFBO() { return VBO;; };
};