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
#if (!defined(_MazeTexture2DDX11_hpp_))
#define _MazeTexture2DDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Texture2DDX11);
    class RenderSystemDX11;


    //////////////////////////////////////////
    // Class Texture2DDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API Texture2DDX11
        : public Texture2D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture2DDX11, Texture2D);

    public:

        //////////////////////////////////////////
        virtual ~Texture2DDX11();

        //////////////////////////////////////////
        static Texture2DDX11Ptr Create(RenderSystemDX11* _renderSystem);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;

        //////////////////////////////////////////
        inline ID3D11Texture2D* getTextureDX11() const { return m_texture; }

        //////////////////////////////////////////
        inline ID3D11ShaderResourceView* getShaderResourceView() const { return m_shaderResourceView; }

        //////////////////////////////////////////
        // Lazily creates the render target view (color formats only)
        ID3D11RenderTargetView* ensureRenderTargetView();

        //////////////////////////////////////////
        // Lazily creates the depth stencil view (depth formats only)
        ID3D11DepthStencilView* ensureDepthStencilView();

        //////////////////////////////////////////
        ID3D11SamplerState* ensureSamplerState();


        //////////////////////////////////////////
        virtual bool setMagFilter(TextureFilter _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setMinFilter(TextureFilter _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapS(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapT(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setBorderColor(ColorU32 _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setAnisotropyLevel(F32 _value) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void copyImageFrom(
            Texture2DPtr const& _texture,
            U32 _x = 0,
            U32 _y = 0) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void copyImageFrom(
            U8 const* _pixels,
            PixelFormat::Enum _pixelFormat,
            U32 _width,
            U32 _height,
            U32 _x,
            U32 _y) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void saveToFileAsTGA(
            String const& _fileName,
            Vec2U _size = Vec2U::c_zero,
            bool _resetAlpha = false) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool readAsPixelSheet(
            PixelSheet2D& _outResult,
            PixelFormat::Enum _outputFormat = PixelFormat::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void generateMipmaps() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        Texture2DDX11();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadTextureImpl(
            Vector<PixelSheet2D> const& _pixelSheets,
            PixelFormat::Enum _internalPixelFormat) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void destroyTexture();

    protected:
        ID3D11Texture2D* m_texture = nullptr;
        ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
        ID3D11RenderTargetView* m_renderTargetView = nullptr;
        ID3D11DepthStencilView* m_depthStencilView = nullptr;

        U32 m_mipLevels = 1u;
        bool m_hasMipmapsGenerationSupport = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2DDX11_hpp_
//////////////////////////////////////////
