#include "asset_manager.h"
#include <cassert>
#include <cstdio>
#include <filesystem>

void AssetManager::Scan(const char* projectDirectory) {
	// Fill up scanned assets
 	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	for (const std::filesystem::directory_entry& dirEntry : recursive_directory_iterator(projectDirectory)) {
		std::filesystem::path dirPath = dirEntry.path();
		if (!dirPath.has_extension()) {
			continue;
		}

		// Small string optimization
		std::string extension = dirPath.extension().string();
		EFileType extensionType = (EFileType)Fnv1a(extension);

		
		switch (extensionType) {
            case EFileType::PNG: {
				Asset resultToPushBack{};
            	resultToPushBack.type = EAssetType::Texture;
            	resultToPushBack.byteSize = std::filesystem::file_size(dirPath);
            	std::string pathStr = dirPath.string();
            	resultToPushBack.id = Fnv1a(pathStr);
            	strncpy(resultToPushBack.path, pathStr.c_str(), pathStr.size());
				this->m_scannedAssets.emplace_back(resultToPushBack);
            	break;
        	}
            case EFileType::JPG:
            case EFileType::TTF:
            case EFileType::WAV:
            case EFileType::MP3:
            case EFileType::GLSL:
            case EFileType::SPV:
            case EFileType::OBJ:
            case EFileType::GLB:
            default:
            	break;
	    }

	}
	
}


AssetManager::EError AssetManager::LoadResource(const Asset& asset, uint8_t* buffer, size_t bufferSize) {
	assert(buffer != nullptr && "Cannot read file into a null buffer");
	assert(asset.byteSize <= bufferSize && "Buffer is not big enough to store the desired asset!");
	
	FILE* file = fopen(asset.path, "r");

	if (file == nullptr) {
		return EError::FileNotFound;
	}

	fread(buffer, sizeof(uint8_t), asset.byteSize, file);

	if (ferror(file) != 0) {
		return EError::CannotReadFile;
	}
	
	fclose(file);

	return EError::Ok;
}

