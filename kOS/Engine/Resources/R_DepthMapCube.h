#pragma once
#include "Resource.h"
#include "Config/pch.h"
#include "Graphics/CubeMap.h"

class R_DepthMapCube:public Resource {
public:
	using Resource::Resource;
	
	void Load()override;
	void Unload()override;
	//Store depth map cube
	DepthCubeMap dcm;
	REFLECTABLE(R_DepthMapCube);
private:

};