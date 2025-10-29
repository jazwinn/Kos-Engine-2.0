/******************************************************************/
/*!
\file      PhysicsLayer.h
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Oct 29, 2024
\brief     This header file defines the PhysicsLayer class, which manages
           layer-based collision settings within the physics pipeline.

The PhysicsLayer class provides functionality for managing collisions
between different layers in the physics system. It maintains a
collision matrix where each entry represents whether collisions are
enabled between two specific layers. The class follows a singleton
pattern to ensure a single instance is accessible throughout the
system. Key methods include setting and retrieving collision statuses
between layers, as well as printing the collision matrix for debugging
purposes.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#ifndef PHYSICSLAYER_H
#define PHYSICSLAYER_H

#include "ECS/Layers.h"
#include "Reflection/Reflection.h"
#include <string>
#include <vector>

namespace physicslayer {


    constexpr int size = layer::LAYERS::MAXLAYER;

    struct LayerData {
        std::string collisionData = "111111111|100000000|100000000|100000000|100000000|100000000|100000000|100000000|100000000|";

		REFLECTABLE(LayerData, collisionData);
    };

   
    class PhysicsLayer {
    public:
        static PhysicsLayer* m_GetInstance() {
            if (!instance) {
                instance = std::make_unique<PhysicsLayer>();
            }
            return instance.get();
        }


        PhysicsLayer();
        PhysicsLayer(const PhysicsLayer&) = delete;
        PhysicsLayer& operator=(const PhysicsLayer&) = delete;


        void m_SetCollision(int layer1, int layer2, bool value);
        bool m_GetCollide(int layer1, int layer2);

        void m_PrintCollisionMatrix() const;

        std::bitset<size>* m_GetMat();
        const std::bitset<size>* m_GetMat() const;
        static const int m_GetSize() {
            return size;
        }
        
        std::string ConvertLayerToString() const;
        void LoadCollisionLayer();
		void SaveCollisionLayer();


    private:
        static std::unique_ptr<PhysicsLayer> instance;
        std::bitset<size> layerCollisions[size];
    };
}


#endif
