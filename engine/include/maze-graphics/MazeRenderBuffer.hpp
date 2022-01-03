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
#if (!defined(_MazeRenderBuffer_hpp_))
#define _MazeRenderBuffer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/MazeRenderBufferSpecification.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/hash/MazeHashSuperFast.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderBuffer);
    MAZE_USING_SHARED_PTR(RenderBufferUniform);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Texture);
    MAZE_USING_SHARED_PTR(Texture2D);
  

    //////////////////////////////////////////
    // Class RenderBuffer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderBuffer
        : public RenderTarget
        , public ISharedCopyable<RenderBuffer>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderBuffer, RenderTarget);

        //////////////////////////////////////////
        using RenderBufferDeleter = std::function<void(RenderBuffer* _ptr)>;

    public:

        //////////////////////////////////////////
        virtual ~RenderBuffer();


        //////////////////////////////////////////
        static RenderBufferPtr Create(
            RenderSystem* _renderSystem = nullptr,
            RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>());

        //////////////////////////////////////////
        static RenderBufferPtr Create(
            RenderBufferSpecification const _specification,
            RenderSystem* _renderSystem = nullptr,
            RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>());


        //////////////////////////////////////////
        virtual bool beginDraw() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void endDraw() MAZE_OVERRIDE;


        //////////////////////////////////////////
        using RenderTarget::blit;

        //////////////////////////////////////////
        virtual void blit(RenderBufferPtr const& _srcBuffer) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual Vec2DU getRenderTargetSize() const MAZE_OVERRIDE { return getSize(); }


        //////////////////////////////////////////
        inline Vec2DU const& getSize() const { return m_size; }

        //////////////////////////////////////////
        inline U32 getWidth() const { return m_size.x; }

        //////////////////////////////////////////
        inline U32 getHeight() const { return m_size.y; }


        //////////////////////////////////////////
        virtual bool setSize(Vec2DU const& _size);

        
        //////////////////////////////////////////
        inline Texture2DPtr getColorTexture2D(U32 _index = 0) const
        {
            return std::static_pointer_cast<Texture2D>(m_colorTextures[_index]);
        }

        //////////////////////////////////////////
        inline TexturePtr const& getColorTexture(U32 _index = 0) const { return m_colorTextures[_index]; }

        //////////////////////////////////////////
        virtual void setColorTexture(U32 _index, TexturePtr const& _texture);

        //////////////////////////////////////////
        inline TexturePtr const& getDepthTexture() const { return m_depthTexture; }

        //////////////////////////////////////////
        virtual void setDepthTexture(TexturePtr const& _texture);

        //////////////////////////////////////////
        inline TexturePtr const& getStencilTexture() const { return m_stencilTexture; }

        //////////////////////////////////////////
        virtual void setStencilTexture(TexturePtr const& _texture);


        //////////////////////////////////////////
        RenderBufferSpecification getSpecification();

    public:

        //////////////////////////////////////////
        MultiDelegate<RenderBufferPtr const&> eventRenderBufferSizeChanged;
        MultiDelegate<RenderBuffer*> eventRenderBufferEndDraw;

    protected:

        //////////////////////////////////////////
        RenderBuffer();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool init(RenderBufferPtr const& _renderBuffer);


    protected:

        Vec2DU m_size;
        TexturePtr m_colorTextures[c_renderBufferColorTexturesMax];
        TexturePtr m_depthTexture;
        TexturePtr m_stencilTexture;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderBuffer_hpp_
//////////////////////////////////////////
