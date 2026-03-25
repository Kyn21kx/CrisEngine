#pragma once
#include <cstdint>
#include <string.h>
#include <cstdio>
#include <string_view>
#include <vector>

constexpr uint32_t Fnv1a(const char *data, const uint32_t length)
{
	// NOLINTNEXTLINE
	uint32_t hash = 0x811c9dc5;
	const uint32_t prime = 0x1000193;

	for (uint32_t i = 0; i < length; ++i)
	{
		uint8_t value = data[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

constexpr uint32_t Fnv1a(std::string_view data)
{
	return Fnv1a(data.data(), data.size());
}

// Necesitamos la abstracción del asset

enum class EAssetType : uint8_t {
	None = 0,
	Texture,
	Font,
	Audio,
	Shader,
	Model
};

enum class EFileType : uint32_t {
	PNG = Fnv1a(".png"),
	JPG = Fnv1a(".jpg"),
	TTF = Fnv1a(".ttf"),
	WAV = Fnv1a(".wav"),
	MP3 = Fnv1a(".mp3"),
	GLSL = Fnv1a(".glsl"),
	SPV = Fnv1a(".spv"),
	OBJ = Fnv1a(".obj"),
	GLB = Fnv1a(".glb"),
};

constexpr size_t MAX_PATH = 128;

struct Asset {
	// Id del asset (path hasheado)
	uint32_t id = 0;
	EAssetType type = EAssetType::None;
	
	// Size in bytes (file)
	size_t byteSize = 0;

	char path[MAX_PATH] = {0};
};

class AssetManager {
public:
	enum class EError {
		Ok = 0,
		FileNotFound,
		CannotReadFile
	};
	
	// Leer todos los archivos en un directorio
	void Scan(const char* projectDirectory);

	EError LoadResource(const Asset& asset, uint8_t* buffer, size_t bufferSize);

	const Asset& GetAssetAt(size_t index) const {
		return this->m_scannedAssets[index];
	}

private:
	std::vector<Asset> m_scannedAssets;
};



