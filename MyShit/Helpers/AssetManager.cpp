#include "AssetManager.h"

void AssetManager::FindAllFiles(std::string directory)
{	
	// Store the tex directory, you need it in the Texture class so you can load with just the filename
	Texture::s_textureDirectory = directory;

	// Find all textures
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{		
		// Get full fule path found file
		std::stringstream ss;
		ss << entry.path();
		std::string filename = ss.str();

		// Extract filename
		const size_t last_slash_idx = filename.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
			filename.erase(0, last_slash_idx + 1);

		// Remove the trailing quote
		const size_t quote = filename.rfind('"');
		if (std::string::npos != quote)
			filename.erase(quote);

		Texture::LoadTexture(filename);
	}
}

void AssetManager::LoadAllTextures()
{
}
