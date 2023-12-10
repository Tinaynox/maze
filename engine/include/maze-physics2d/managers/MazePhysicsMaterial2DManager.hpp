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
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PhysicsMaterial2DManager);
    MAZE_USING_SHARED_PTR(PhysicsWorld2D);
    MAZE_USING_SHARED_PTR(PhysicsMaterial2D);


    //////////////////////////////////////////
    // Struct PhysicsMaterial2DLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_PHYSICS2D_API PhysicsMaterial2DLibraryData
    {
        //////////////////////////////////////////
        PhysicsMaterial2DLibraryData(
            PhysicsMaterial2DPtr const& _physicsMaterial2D = nullptr,
            AssetFilePtr const& _assetFile = nullptr)
            : physicsMaterial2D(_physicsMaterial2D)
            , assetFile(_assetFile)
        {}

        PhysicsMaterial2DPtr physicsMaterial2D;
        AssetFilePtr assetFile;
    };


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
        PhysicsMaterial2DLibraryData const* getPhysicsMaterial2DLibraryData(HashedCString _physicsMaterial2DName);

        //////////////////////////////////////////
        PhysicsMaterial2DLibraryData const* getPhysicsMaterial2DLibraryData(String const& _assetFileName) { return getPhysicsMaterial2DLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        PhysicsMaterial2DLibraryData const* getPhysicsMaterial2DLibraryData(CString _assetFileName) { return getPhysicsMaterial2DLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        PhysicsMaterial2DPtr const& getMaterial(HashedCString _materialName);

        //////////////////////////////////////////
        inline PhysicsMaterial2DPtr const& getMaterial(String const& _materialName) { return getMaterial(MAZE_HASHED_CSTRING(_materialName.c_str())); }

        //////////////////////////////////////////
        inline PhysicsMaterial2DPtr const& getMaterial(CString _materialName) { return getMaterial(MAZE_HASHED_CSTRING(_materialName)); }

        //////////////////////////////////////////
        PhysicsMaterial2DPtr const& getMaterial(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        String const& getMaterialName(PhysicsMaterial2D const* _material);


        //////////////////////////////////////////
        PhysicsMaterial2DLibraryData* addFontToLibrary(
            HashedCString _physicsMaterial2DName,
            PhysicsMaterial2DPtr const& _physicsMaterial2D);

        //////////////////////////////////////////
        void removeFontFromLibrary(HashedCString _physicsMaterial2DName);

        //////////////////////////////////////////
        inline void removeFontFromLibrary(CString _physicsMaterial2DName) { removeFontFromLibrary(HashedCString(_physicsMaterial2DName)); }

        //////////////////////////////////////////
        inline void removeFontFromLibrary(String const& _physicsMaterial2DName) { removeFontFromLibrary(HashedCString(_physicsMaterial2DName.c_str())); }


        //////////////////////////////////////////
        void unloadAssetPhysicsMaterial2Ds(Set<String> const& _tags);

    protected:

        //////////////////////////////////////////
        PhysicsMaterial2DManager();

        //////////////////////////////////////////
        bool init();

    protected:
        static PhysicsMaterial2DManager* s_instance;

        PhysicsMaterial2DPtr m_defaultMaterial;

        StringKeyMap<PhysicsMaterial2DLibraryData> m_materialsLibrary;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysicsMaterial2DManager_hpp_
//////////////////////////////////////////
