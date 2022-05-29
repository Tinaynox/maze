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
#if (!defined(_MazeShaderSystem_hpp_))
#define _MazeShaderSystem_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeStringPerfectHashGenerator.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderSystem);
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(AssetFile);
    

    //////////////////////////////////////////
    // Enum BuiltinShaderType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_15_API(MAZE_GRAPHICS_API, BuiltinShaderType,
        Error,
        UV,
        Normal,
        Color,
        ColorTexture,
        ColorTextureCustomUV,
        ColorHDR,
        HSVRect,
        HSVBand,
        ColorPickerChannel,
        DebugGrid,
        DebugAxis,
        Skybox,
        Specular,
        MeshPreview);


    //////////////////////////////////////////
    // Class ShaderSystem
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ShaderSystem
        : public SharedObject<ShaderSystem>
    {
    public:

        //////////////////////////////////////////
        virtual ~ShaderSystem();


        //////////////////////////////////////////
        inline bool getSystemInited() const { return m_systemInited; }


        //////////////////////////////////////////
        void findAssetShadersAndAddToCache();

        //////////////////////////////////////////
        void addAssetFileShadersToCache(Vector<AssetFilePtr> const& _files);

        //////////////////////////////////////////
        void removeAssetFileShadersFromCache(Vector<AssetFilePtr> const& _files);

        //////////////////////////////////////////
        inline RenderSystemPtr getRenderSystem() const { return m_renderSystem.lock(); }

        //////////////////////////////////////////
        inline RenderSystem* getRenderSystemRaw() const { return m_renderSystemRaw; }


        //////////////////////////////////////////
        virtual ShaderPtr createShader(AssetFilePtr const& _shaderFile) MAZE_ABSTRACT;


        //////////////////////////////////////////
        inline StringKeyMap<ShaderPtr> const& getShadersCache() const { return m_shadersCache; }

        //////////////////////////////////////////
        ShaderPtr const& getShaderFromCache(HashedCString _shaderName);

        //////////////////////////////////////////
        inline ShaderPtr const& getShaderFromCache(String const& _shaderName) { return getShaderFromCache(MAZE_HASHED_CSTRING(_shaderName.c_str())); }

        //////////////////////////////////////////
        inline ShaderPtr const& getShaderFromCache(CString _shaderName) { return getShaderFromCache(MAZE_HASHED_CSTRING(_shaderName)); }


        //////////////////////////////////////////
        ShaderPtr const& getShader(HashedCString _shaderName);

        //////////////////////////////////////////
        inline ShaderPtr const& getShader(String const& _shaderName) { return getShader(MAZE_HASHED_CSTRING(_shaderName.c_str())); }

        //////////////////////////////////////////
        inline ShaderPtr const& getShader(CString _shaderName) { return getShader(MAZE_HASHED_CSTRING(_shaderName)); }


        //////////////////////////////////////////
        ShaderPtr const& getBuiltinShader(BuiltinShaderType _shaderType) const { return m_builtinShaders[(Size)_shaderType]; }

        //////////////////////////////////////////
        ShaderPtr const& ensureBuiltinShader(BuiltinShaderType _shaderType);

        //////////////////////////////////////////
        virtual ShaderPtr const& createBuiltinShader(BuiltinShaderType _shaderType) MAZE_ABSTRACT;

        //////////////////////////////////////////
        void createBuiltinShaders();


        //////////////////////////////////////////
        ShaderPtr const& addShaderToCache(ShaderPtr const& _shader);

        //////////////////////////////////////////
        bool removeShaderFromCache(ShaderPtr const& _shader);

    public:

        MultiDelegate<> eventSystemInited;

    protected:

        //////////////////////////////////////////
        ShaderSystem();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);    
   

        //////////////////////////////////////////
        void processSystemInited();

    protected:
        bool m_systemInited;

        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        StringKeyMap<ShaderPtr> m_shadersCache;

        ShaderPtr m_builtinShaders[(Size)BuiltinShaderType::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderSystem_hpp_
//////////////////////////////////////////
