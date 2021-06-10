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
#pragma once
#if (!defined(_MazePhysicsMaterial2DManager_hpp_))
#define _MazePhysicsMaterial2DManager_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PhysicsMaterial2DManager);
    MAZE_USING_SHARED_PTR(PhysicsWorld2D);
    MAZE_USING_SHARED_PTR(PhysicsMaterial2D);


    //////////////////////////////////////////
    // Class PhysicsMaterial2DManager
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API PhysicsMaterial2DManager
    {
    public:

        //////////////////////////////////////////
        virtual ~PhysicsMaterial2DManager();

        //////////////////////////////////////////
        static void Initialize(PhysicsMaterial2DManagerPtr& _physics2DManager);


        //////////////////////////////////////////
        static inline PhysicsMaterial2DManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline PhysicsMaterial2DManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        inline PhysicsMaterial2DPtr const& getDefaultMaterial() const { return m_defaultMaterial; }


        //////////////////////////////////////////
        PhysicsMaterial2DPtr const& getMaterial(String const& _materialName);

        //////////////////////////////////////////
        PhysicsMaterial2DPtr const& getMaterial(AssetFilePtr const& _assetFile);

    protected:

        //////////////////////////////////////////
        PhysicsMaterial2DManager();

        //////////////////////////////////////////
        bool init();

    protected:
        static PhysicsMaterial2DManager* s_instance;

        PhysicsMaterial2DPtr m_defaultMaterial;

        UnorderedMap<String, PhysicsMaterial2DPtr> m_materialsByName;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysicsMaterial2DManager_hpp_
//////////////////////////////////////////
