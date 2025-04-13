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
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(GlobalShaderUniform);
    

    //////////////////////////////////////////
    // Enum BuiltinShaderType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_17_API(MAZE_GRAPHICS_API, BuiltinShaderType,
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
        MeshPreview,
        Font,
        ShadowCaster);


    //////////////////////////////////////////
    // Struct ShaderLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API ShaderLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct ShaderLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API ShaderLibraryData
    {
        //////////////////////////////////////////
        ShaderLibraryData(
            ShaderPtr const& _shader = nullptr,
            ShaderLibraryDataCallbacks const& _callbacks = ShaderLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : shader(_shader)
            , callbacks(_callbacks)
            , data(_data)
        {}

        ShaderPtr shader;
        ShaderLibraryDataCallbacks callbacks;
        DataBlock data;
    };


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
        static ShaderSystemPtr const& GetCurrentInstancePtr();


        //////////////////////////////////////////
        inline bool getSystemInited() const { return m_systemInited; }


        //////////////////////////////////////////
        void findAssetShadersAndAddToLibrary();

        //////////////////////////////////////////
        void addAssetFileShadersToLibrary(Vector<AssetFilePtr> const& _files);

        //////////////////////////////////////////
        void removeAssetFileShadersFromLibrary(Vector<AssetFilePtr> const& _files);

        //////////////////////////////////////////
        inline RenderSystemPtr getRenderSystem() const { return m_renderSystem.lock(); }

        //////////////////////////////////////////
        inline RenderSystem* getRenderSystemRaw() const { return m_renderSystemRaw; }


        //////////////////////////////////////////
        virtual ShaderPtr createShader() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual ShaderPtr createShader(AssetFilePtr const& _shaderFile) MAZE_ABSTRACT;


        //////////////////////////////////////////
        inline StringKeyMap<ShaderLibraryData> const& getShadersLibrary() const { return m_shadersLibrary; }

        //////////////////////////////////////////
        ShaderLibraryData const* getShaderLibraryData(HashedCString _shaderName);

        //////////////////////////////////////////
        ShaderPtr const& getShaderFromLibrary(HashedCString _shaderName);

        //////////////////////////////////////////
        inline ShaderPtr const& getShaderFromLibrary(String const& _shaderName) { return getShaderFromLibrary(MAZE_HASHED_CSTRING(_shaderName.c_str())); }

        //////////////////////////////////////////
        inline ShaderPtr const& getShaderFromLibrary(CString _shaderName) { return getShaderFromLibrary(MAZE_HASHED_CSTRING(_shaderName)); }


        //////////////////////////////////////////
        ShaderPtr const& getOrLoadShader(HashedCString _shaderName, bool _syncLoad = true);

        //////////////////////////////////////////
        inline ShaderPtr const& getOrLoadShader(String const& _shaderName, bool _syncLoad = true) { return getOrLoadShader(MAZE_HASHED_CSTRING(_shaderName.c_str()), _syncLoad); }

        //////////////////////////////////////////
        inline ShaderPtr const& getOrLoadShader(CString _shaderName, bool _syncLoad = true) { return getOrLoadShader(MAZE_HASHED_CSTRING(_shaderName), _syncLoad); }

        //////////////////////////////////////////
        ShaderPtr const& getOrLoadShader(AssetFilePtr const& _shaderName, bool _syncLoad = true);


        //////////////////////////////////////////
        ShaderPtr const& getBuiltinShader(BuiltinShaderType _shaderType) const { return m_builtinShaders[(Size)_shaderType]; }

        //////////////////////////////////////////
        ShaderPtr const& ensureBuiltinShader(BuiltinShaderType _shaderType);

        //////////////////////////////////////////
        virtual ShaderPtr const& createBuiltinShader(BuiltinShaderType _shaderType) MAZE_ABSTRACT;

        //////////////////////////////////////////
        void createBuiltinShaders();


        //////////////////////////////////////////
        ShaderLibraryData* addShaderToLibrary(
            ShaderPtr const& _shader,
            ShaderLibraryDataCallbacks const& _callbacks = ShaderLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

        //////////////////////////////////////////
        bool removeShaderFromLibrary(ShaderPtr const& _shader);


        //////////////////////////////////////////
        void addGlobalFeature(CString _name, CString _value = "");

        //////////////////////////////////////////
        void removeGlobalFeature(CString _name);

        //////////////////////////////////////////
        String const& ensureGlobalFeaturesString();


        //////////////////////////////////////////
        void reloadShaders();


        //////////////////////////////////////////
        inline StringKeyMap<GlobalShaderUniformPtr> const& getGlobalShaderUniforms() const { return m_globalShaderUniforms; }

        //////////////////////////////////////////
        GlobalShaderUniformPtr const& ensureGlobalShaderUniform(HashedCString _name);

        //////////////////////////////////////////
        GlobalShaderUniformPtr const& getGlobalShaderUniform(HashedCString _name) const;

    public:

        MultiDelegate<> eventSystemInited;

    protected:

        //////////////////////////////////////////
        ShaderSystem();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);    
   

        //////////////////////////////////////////
        void processSystemInited();

        //////////////////////////////////////////
        String buildGlobalShaderFeatures();

        //////////////////////////////////////////
        void updateGlobalShaderFeaturesString();

    protected:
        bool m_systemInited;

        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        StringKeyMap<ShaderLibraryData> m_shadersLibrary;

        ShaderPtr m_builtinShaders[(Size)BuiltinShaderType::MAX];

        StringKeyMap<String> m_globalFeatures;
        bool m_globalFeaturesStringDirty = false;
        String m_globalFeaturesString;

        StringKeyMap<GlobalShaderUniformPtr> m_globalShaderUniforms;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderSystem_hpp_
//////////////////////////////////////////
