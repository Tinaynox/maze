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
#if (!defined(_MazeTextureCubeDX11_hpp_))
#define _MazeTextureCubeDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeTextureCube.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(TextureCubeDX11);
    class RenderSystemDX11;


    //////////////////////////////////////////
    // Class TextureCubeDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API TextureCubeDX11
        : public TextureCube
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TextureCubeDX11, TextureCube);

    public:

        //////////////////////////////////////////
        virtual ~TextureCubeDX11();

        //////////////////////////////////////////
        static TextureCubeDX11Ptr Create(RenderSystemDX11* _renderSystem);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;

        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;

        //////////////////////////////////////////
        inline ID3D11Texture2D* getTextureDX11() const { return m_texture; }

        //////////////////////////////////////////
        inline ID3D11ShaderResourceView* getShaderResourceView() const { return m_shaderResourceView; }

        //////////////////////////////////////////
        ID3D11SamplerState* ensureSamplerState();


        //////////////////////////////////////////
        virtual bool loadTexture(
            Vector<PixelSheet2D> const _pixelSheets[6],
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setMagFilter(TextureFilter _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setMinFilter(TextureFilter _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapS(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapT(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapR(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void generateMipmaps() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void reload() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        TextureCubeDX11();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void destroyTexture();

    protected:
        ID3D11Texture2D* m_texture = nullptr;
        ID3D11ShaderResourceView* m_shaderResourceView = nullptr;

        bool m_hasMipmapsGenerationSupport = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextureCubeDX11_hpp_
//////////////////////////////////////////
