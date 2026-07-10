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
#if (!defined(_MazeTexture2DMSDX11_hpp_))
#define _MazeTexture2DMSDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeTexture2DMS.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Texture2DMSDX11);
    class RenderSystemDX11;


    //////////////////////////////////////////
    // Class Texture2DMSDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API Texture2DMSDX11
        : public Texture2DMS
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture2DMSDX11, Texture2DMS);

    public:

        //////////////////////////////////////////
        virtual ~Texture2DMSDX11();

        //////////////////////////////////////////
        static Texture2DMSDX11Ptr Create(RenderSystemDX11* _renderSystem);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;

        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;

        //////////////////////////////////////////
        inline ID3D11Texture2D* getTextureDX11() const { return m_texture; }

        //////////////////////////////////////////
        inline ID3D11ShaderResourceView* getShaderResourceView() const { return m_shaderResourceView; }

        //////////////////////////////////////////
        ID3D11RenderTargetView* ensureRenderTargetView();

        //////////////////////////////////////////
        ID3D11DepthStencilView* ensureDepthStencilView();


        //////////////////////////////////////////
        virtual bool loadEmpty(
            Vec2U const& _size,
            PixelFormat::Enum _internalPixelFormat,
            S32 _samples) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void copyImageFrom(
            U8 const* _pixels,
            PixelFormat::Enum _pixelFormat,
            U32 _width,
            U32 _height,
            U32 _x,
            U32 _y) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void saveToFileAsTGA(String const& _fileName, Vec2U _size = Vec2U::c_zero) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual PixelSheet2D readAsPixelSheet(PixelFormat::Enum _outputFormat = PixelFormat::None) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        Texture2DMSDX11();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void destroyTexture();

    protected:
        ID3D11Texture2D* m_texture = nullptr;
        ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
        ID3D11RenderTargetView* m_renderTargetView = nullptr;
        ID3D11DepthStencilView* m_depthStencilView = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2DMSDX11_hpp_
//////////////////////////////////////////
