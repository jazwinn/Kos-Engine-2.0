/********************************************************************/
/*!
\file      MeshFilterComponent.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the MeshFilterComponent class, which stores
           information about which mesh asset an entity should use
           for rendering.

           This component holds the reference (GUID or path) to
           the mesh resource assigned to an entity. It acts as a
           link between the ECS entity and the mesh data used by
           rendering systems.

           Typically paired with MeshRendererComponent for defining
           how the mesh should appear visually.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef MESHFILTER_H
#define MESHFILTER_H

#include "Component.h"

namespace ecs {

    class MeshFilterComponent : public Component {

    public:
        std::string meshGUID{};   // Path or ID for mesh asset

        REFLECTABLE(MeshFilterComponent, meshGUID);
    };

}

#endif // MESHFILTER_H

