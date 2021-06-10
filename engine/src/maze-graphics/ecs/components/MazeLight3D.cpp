//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(Light3DType);


    //////////////////////////////////////////
    // Class Light3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Light3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Light3DType, lightType, Light3DType::Directional, getLightType, setLightType),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorF128, color, ColorF128::c_white, getColor, setColor));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Light3D);

    //////////////////////////////////////////
    Light3D::Light3D()
        : m_renderSystem(nullptr)
        , m_lightType(Light3DType::Directional)
        , m_color(ColorF128::c_white)
    {
    }

    //////////////////////////////////////////
    Light3D::~Light3D()
    {

    }

    //////////////////////////////////////////
    Light3DPtr Light3D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        Light3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Light3D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool Light3D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool Light3D::init(
        Component* _component,
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<Light3D>()->m_renderSystem;

        if (!Component::init(
            _component,
            _world,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void Light3D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
        m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();
    }
    
    
} // namespace Maze
//////////////////////////////////////////
