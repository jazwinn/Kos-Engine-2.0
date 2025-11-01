#ifndef PARTICLECOMPONENT_H
#define PARTICLECOMPONENT_H

#include "Component.h"


namespace ecs {

	class ParticleComponent : public Component {
	public:
		float duration;
		bool looping;
		float start_Lifetime;
		float start_Velocity;
		float size;
		float rotation;
		glm::vec4 color;
		bool play_On_Awake;
		//max particles
		int max_Particles = 255;	
		//NvFlex 
		void* pointers[5];
		void* library;
		void* solver;

		//change both vector to char
		std::vector<short> freeIndices;               // Pool of available particle slots              
		std::vector<short> alive_Particles;
		float emitterTime;
		float durationCounter;
		float emissionInterval = 0.1f;

		REFLECTABLE(ParticleComponent, duration, looping, start_Lifetime, start_Velocity, size, rotation, color, play_On_Awake, emissionInterval);
	};
}
#endif