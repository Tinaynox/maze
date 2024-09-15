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
#if (!defined(_MazeAssetUnitShader_hpp_))
#define _MazeAssetUnitShader_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetUnitShader);   
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Shader);


    //////////////////////////////////////////
    // Class AssetUnitShader
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API AssetUnitShader
        : public AssetUnit
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnitShader, AssetUnit);
        
    public:

        //////////////////////////////////////////
        virtual ~AssetUnitShader();

        //////////////////////////////////////////
        static AssetUnitShaderPtr Create(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data = DataBlock::c_empty);


        //////////////////////////////////////////
        inline ShaderPtr const& getShader() const { return m_shader; }

        //////////////////////////////////////////
        ShaderPtr const& loadShader(bool _syncLoad = false);


        //////////////////////////////////////////
        ShaderPtr const& initShader();

    protected:

        //////////////////////////////////////////
        AssetUnitShader();

        //////////////////////////////////////////
        virtual bool init(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadNowImpl() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool unloadNowImpl() MAZE_OVERRIDE;
    
    protected:
        ShaderPtr m_shader;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetUnitShader_hpp_
//////////////////////////////////////////
