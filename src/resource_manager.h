#pragma once

#include "raylib.h"
#include <cassert>
#include <cstdint>

namespace ResourceManager {
	
	enum class EError {
		Ok = 0,
		WrongFormat
	};
	
	EError LoadImageIntoBuffer(const uint8_t* buffer, size_t byteSize, Image* outImageData) {
		assert(outImageData != nullptr && "Cannot load data into an empty out image reference");
		*outImageData = LoadImageFromMemory("png", buffer, byteSize);

		if (outImageData->data == nullptr) {
			return EError::WrongFormat;
		}
		return EError::Ok;
	}

	
};

