#include "Config/pch.h"

enum WrapMode  {
	WRAP_CLAMP,
	WRAP_MIRROR
};

enum ColorFormat {
	CS_SRGB,
	CS_SRGBI,
	CS_SRGBO
};

struct TextureCompilerData {
	bool HoriztonalFlip = false;
	bool VerticalFlip = false;
	float Width = 1.0f;
	float Height = 1.0f;
	int MipLevels = 1;
	WrapMode Wrap = WRAP_CLAMP;
	ColorFormat ColorSpace = CS_SRGB;

	REFLECTABLE(TextureCompilerData,
		HoriztonalFlip,
		VerticalFlip,
		Width,
		Height,
		MipLevels,
		Wrap,
		ColorSpace
	);
};