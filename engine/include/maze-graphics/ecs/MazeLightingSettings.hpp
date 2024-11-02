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
#if (!defined(_MazeLightingSettings_hpp_))
#define _MazeLightingSettings_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/ecs/MazeLightingSettings.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(LightingSettings);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class LightingSettings
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API LightingSettings
        : public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(LightingSettings);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~LightingSettings();

        //////////////////////////////////////////
        static LightingSettingsPtr Create();


        //////////////////////////////////////////
        inline MaterialAssetRef const& getSkyBoxMaterialRef() const { return m_skyBoxMaterialRef; }

        //////////////////////////////////////////
        void setSkyBoxMaterialRef(MaterialAssetRef const& _material) { m_skyBoxMaterialRef.setMaterial(_material.getMaterial()); }

        //////////////////////////////////////////
        inline MaterialPtr const& getSkyBoxMaterial() const { return m_skyBoxMaterialRef.getMaterial(); }

        //////////////////////////////////////////
        inline void setSkyBoxMaterial(MaterialPtr const& _material) { setSkyBoxMaterialRef(MaterialAssetRef(_material)); }



        //////////////////////////////////////////
        void setSkyBoxMaterial(String const& _id);

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
        {
            DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            return true;
        }

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
        {
            DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
        }

    protected:

        //////////////////////////////////////////
        LightingSettings();
        
        //////////////////////////////////////////
        bool init();

    protected:
        MaterialAssetRef m_skyBoxMaterialRef;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLightingSettings_hpp_
//////////////////////////////////////////
