#pragma once
#include "Config/pch.h"
#include "Resources/ResourceHeader.h"


struct MeshCompiler {
	std::string type = R_Model::classname();
	std::string path;
	std::string outputExtension;
	std::vector<std::string> inputExtensions;
	REFLECTABLE(MeshCompiler, path, outputExtension, inputExtensions);
};

struct AnimationCompiler {
	std::string type = R_Animation::classname();
	std::string path;
	std::string outputExtension;
	std::vector<std::string> inputExtensions;
	REFLECTABLE(AnimationCompiler, path, outputExtension, inputExtensions);
};
struct TextureCompiler {
	std::string type = R_Texture::classname();
	std::string path;
	std::string outputExtension;
	std::vector<std::string> inputExtensions;
	REFLECTABLE(TextureCompiler, path, outputExtension, inputExtensions);

};
struct FontCompiler {
	std::string type = R_Font::classname();
	std::string path;
	std::string outputExtension;
	std::vector<std::string> inputExtensions;
	REFLECTABLE(TextureCompiler, path, outputExtension, inputExtensions);

};
struct SceneCompiler {
	std::string type = R_Scene::classname();
	std::string path;
	std::string outputExtension;
	std::vector<std::string> inputExtensions;
	REFLECTABLE(SceneCompiler, path, outputExtension, inputExtensions);

};

struct AudioCompiler {
	std::string type = R_Audio::classname();
	std::string path;
	std::string outputExtension;
	std::vector<std::string> inputExtensions;
	REFLECTABLE(AudioCompiler, path, outputExtension, inputExtensions);

};

struct AudioStudioCompiler {
	std::string type = R_AudioStudio::classname();
	std::string path;
	std::string outputExtension;
	std::vector<std::string> inputExtensions;
	REFLECTABLE(AudioCompiler, path, outputExtension, inputExtensions);

};

struct MaterialCompiler {
	std::string type = R_Material::classname();
	std::string path;
	std::string outputExtension;
	std::vector<std::string> inputExtensions;
	REFLECTABLE(R_Material, path, outputExtension, inputExtensions);

};

struct CompilerData {
	MeshCompiler meshCompiler;
	TextureCompiler textureCompiler;
	FontCompiler fontCompiler;
	SceneCompiler sceneCompiler;
	AnimationCompiler animationCompiler;
	AudioCompiler audioCompiler;
	MaterialCompiler materialCompiler;

	REFLECTABLE(CompilerData, meshCompiler,textureCompiler, fontCompiler, sceneCompiler, animationCompiler, audioCompiler, materialCompiler);
};


