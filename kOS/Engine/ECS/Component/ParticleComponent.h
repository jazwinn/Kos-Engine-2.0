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
		int max_Particles = 1000;	
		int particles_Alive = 0;
		//NvFlex 
		void* pointers[4];
		void* library;
		void* solver;

		std::vector<float> particleLifetimes;  // Lifetime remaining for each particle
		std::vector<int> freeIndices;               // Pool of available particle slots              
		std::vector<float> emitterTimers;   // Emission timer per entity
		std::vector<float> durationTimers;  // Duration timer per entity

		REFLECTABLE(ParticleComponent, duration, looping, start_Lifetime, start_Velocity, size, rotation, color, play_On_Awake);
	};
}
#endif