#pragma once
#include "Renderer/Texture.h"
#include "Header.h"

class AssetManager
{
public: // static containers
	//static std::vector<Texture> s_textures;

public: // functions
	static void FindAllFiles(std::string directory);
	static void LoadAllTextures();
};