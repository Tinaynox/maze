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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderSystem);
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(AssetFile);
    

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
        inline UnorderedMap<String, ShaderPtr> const& getShadersCache() const { return m_shadersCache; }

        //////////////////////////////////////////
        ShaderPtr const& getShaderFromCache(String const& _shaderName);

        //////////////////////////////////////////
        ShaderPtr const& getErrorShader() const { return m_errorShader; }

        //////////////////////////////////////////
        ShaderPtr const& getUVShader() const { return m_uvShader; }

        //////////////////////////////////////////
        ShaderPtr const& getColorShader() const { return m_colorShader; }

        //////////////////////////////////////////
        ShaderPtr const& getColorTextureShader() const { return m_colorTextureShader; }

        //////////////////////////////////////////
        ShaderPtr const& getColorTextureCustomUVShader() const { return m_colorTextureCustomUVShader; }

        //////////////////////////////////////////
        ShaderPtr const& getColorHDRShader() const { return m_colorHDRShader; }

        //////////////////////////////////////////
        ShaderPtr const& getHSVRectShader() const { return m_hsvRectShader; }

        //////////////////////////////////////////
        ShaderPtr const& getHSVBandShader() const { return m_hsvBandShader; }

        //////////////////////////////////////////
        ShaderPtr const& getColorPickerChannelShader() const { return m_colorPickerChannelShader; }

        //////////////////////////////////////////
        ShaderPtr const& getDebugGridShader() const { return m_debugGridShader; }

        //////////////////////////////////////////
        ShaderPtr const& getDebugAxisShader() const { return m_debugAxisShader; }


        //////////////////////////////////////////
        inline Size stringToHash(String const& _string) { return m_hashGenerator.stringToHash(_string); }

    public:

        MultiDelegate<> eventSystemInited;

    protected:

        //////////////////////////////////////////
        ShaderSystem();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);    
    

        //////////////////////////////////////////
        bool addShaderToCache(ShaderPtr const& _shader);

        //////////////////////////////////////////
        bool removeShaderFromCache(ShaderPtr const& _shader);

        //////////////////////////////////////////
        void processSystemInited();

    protected:
        bool m_systemInited;

        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        UnorderedMap<String, ShaderPtr> m_shadersCache;

        ShaderPtr m_errorShader;
        ShaderPtr m_uvShader;
        ShaderPtr m_colorShader;
        ShaderPtr m_colorTextureShader;
        ShaderPtr m_colorTextureCustomUVShader;
        ShaderPtr m_colorHDRShader;
        ShaderPtr m_hsvRectShader;
        ShaderPtr m_hsvBandShader;
        ShaderPtr m_colorPickerChannelShader;
        ShaderPtr m_debugGridShader;
        ShaderPtr m_debugAxisShader;

        StringPerfectHashGenerator m_hashGenerator;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderSystem_hpp_
//////////////////////////////////////////
