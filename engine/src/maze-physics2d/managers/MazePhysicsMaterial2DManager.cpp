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
#include "MazePhysics2DHeader.hpp"
#include "maze-physics2d/managers/MazePhysicsMaterial2DManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/gizmos/MazeBoxCollider2DGizmos.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PhysicsMaterial2DManager
    //
    //////////////////////////////////////////
    PhysicsMaterial2DManager* PhysicsMaterial2DManager::s_instance = nullptr;
    
    //////////////////////////////////////////
    PhysicsMaterial2DManager::PhysicsMaterial2DManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DManager::~PhysicsMaterial2DManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void PhysicsMaterial2DManager::Initialize(PhysicsMaterial2DManagerPtr& _physics2DManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(PhysicsMaterial2DManager, _physics2DManager, init());
    }

    //////////////////////////////////////////
    bool PhysicsMaterial2DManager::init()
    {
        m_defaultMaterial = PhysicsMaterial2D::Create(0.25f, 0.35f);

        return true;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr const& PhysicsMaterial2DManager::getMaterial(String const& _materialName)
    {
        static MaterialPtr nullPointer;

        UnorderedMap<String, PhysicsMaterial2DPtr>::const_iterator it = m_materialsByName.find(_materialName);
        if (it != m_materialsByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_materialName);
        if (!assetFile)
        {
            MAZE_ERROR("Undefined material: %s!", _materialName.c_str());
            return m_defaultMaterial;
        }

        PhysicsMaterial2DPtr material = PhysicsMaterial2D::Create(assetFile);
        auto it2 = m_materialsByName.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_materialName),
            std::forward_as_tuple(material));
        return it2.first->second;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr const& PhysicsMaterial2DManager::getMaterial(AssetFilePtr const& _assetFile)
    {
        UnorderedMap<String, PhysicsMaterial2DPtr>::const_iterator it = m_materialsByName.find(_assetFile->getFileName());
        if (it != m_materialsByName.end())
            return it->second;

        PhysicsMaterial2DPtr material = PhysicsMaterial2D::Create(_assetFile);
        auto it2 = m_materialsByName.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_assetFile->getFileName()),
            std::forward_as_tuple(material));
        return it2.first->second;
    }

    //////////////////////////////////////////
    String const& PhysicsMaterial2DManager::getMaterialName(PhysicsMaterial2D const* _material)
    {
        static String nullPointer;

        for (auto const& materialData : m_materialsByName)
        {
            if (materialData.second.get() == _material)
                return materialData.first;
        }

        return nullPointer;
    }
    
} // namespace Maze
//////////////////////////////////////////
