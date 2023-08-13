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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderBuffer.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeTexture2DMS.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/events/MazeGraphicsEvents.hpp"
#include <tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderBuffer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderBuffer, RenderTarget);

    //////////////////////////////////////////
    RenderBuffer::RenderBuffer()
        : m_size(Vec2DU::c_one)
    {
    }

    //////////////////////////////////////////
    RenderBuffer::~RenderBuffer()
    {
        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->generateEventImmediate<RenderBufferDestroyedEvent>(this);
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderBuffer::Create(
        RenderSystem* _renderSystem,
        RenderBufferDeleter const& _deleter)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        return _renderSystem->createRenderBuffer(_deleter);
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderBuffer::Create(
        RenderBufferSpecification const _specification,
        RenderSystem* _renderSystem,
        RenderBufferDeleter const& _deleter)
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(_specification.size.x == 0 || _specification.size.y == 0, RenderBufferPtr(), "Size is zero!");

        RenderBufferPtr renderBuffer = RenderBuffer::Create(_renderSystem, _deleter);
        renderBuffer->setSize(_specification.size);

        for (Size i = 0; i < c_renderBufferColorTexturesMax; ++i)
        {
            RenderBufferSpecification::TextureFormat const& textureFormat = _specification.colorTextureFormats[i];

            if (textureFormat.pixelFormat != PixelFormat::None)
            {
                TexturePtr colorTexture0;
                if (textureFormat.samples == 0)
                {
                    Texture2DPtr texture = Texture2D::Create();
                    texture->loadEmpty(_specification.size, textureFormat.pixelFormat);
                    texture->setMinFilter(TextureFilter::Nearest);
                    texture->setMagFilter(TextureFilter::Nearest);
                    colorTexture0 = texture;
                }
                else
                {
                    Texture2DMSPtr texture = Texture2DMS::Create();
                    if (!texture->loadEmpty(_specification.size, textureFormat.pixelFormat, textureFormat.samples))
                        return nullptr;
                    colorTexture0 = texture;
                }

                renderBuffer->setColorTexture(0, colorTexture0);
            }
        }

        {
            RenderBufferSpecification::TextureFormat const& textureFormat = _specification.depthTextureFormat;
            if (textureFormat.pixelFormat != PixelFormat::None)
            {
                TexturePtr depthTexture;
                if (textureFormat.samples == 0)
                {
                    Texture2DPtr texture = Texture2D::Create();
                    texture->loadEmpty(_specification.size, textureFormat.pixelFormat);
                    texture->setMinFilter(TextureFilter::Nearest);
                    texture->setMagFilter(TextureFilter::Nearest);
                    depthTexture = texture;
                }
                else
                {
                    Texture2DMSPtr texture = Texture2DMS::Create();
                    texture->loadEmpty(_specification.size, textureFormat.pixelFormat, textureFormat.samples);
                    depthTexture = texture;
                }
                renderBuffer->setDepthTexture(depthTexture);
            }
        }

        {
            RenderBufferSpecification::TextureFormat const& textureFormat = _specification.stencilTextureFormat;
            if (textureFormat.pixelFormat != PixelFormat::None)
            {
                TexturePtr stencilTexture;
                if (textureFormat.samples == 0)
                {
                    Texture2DPtr texture = Texture2D::Create();
                    texture->loadEmpty(_specification.size, textureFormat.pixelFormat);
                    texture->setMinFilter(TextureFilter::Nearest);
                    texture->setMagFilter(TextureFilter::Nearest);
                    stencilTexture = texture;
                }
                else
                {
                    Texture2DMSPtr texture = Texture2DMS::Create();
                    texture->loadEmpty(_specification.size, textureFormat.pixelFormat, textureFormat.samples);
                    stencilTexture = texture;
                }
                renderBuffer->setStencilTexture(stencilTexture);
            }
        }

        return renderBuffer;
    }

    //////////////////////////////////////////
    bool RenderBuffer::init(RenderSystem* _renderSystem)
    {
        if (!RenderTarget::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool RenderBuffer::init(RenderBufferPtr const& _renderBuffer)
    {
        MAZE_NOT_IMPLEMENTED;

        return true;
    }

    //////////////////////////////////////////
    bool RenderBuffer::beginDraw()
    {
        if (!RenderTarget::beginDraw())
            return false;

        setViewport(Rect2DF(Vec2DF::c_zero, getSize()));

        return true;
    }

    //////////////////////////////////////////
    void RenderBuffer::endDraw()
    {
        for (Size i = 0; i < c_renderBufferColorTexturesMax; ++i)
        {
            if (getColorTexture((U32)i) && getColorTexture((U32)i)->getType() == TextureType::TwoDimensional)
                getColorTexture((U32)i)->castRaw<Texture2D>()->generateMipmaps();
        }

        RenderTarget::endDraw();
    }

    //////////////////////////////////////////
    bool RenderBuffer::setSize(Vec2DU const& _size)
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(_size.x == 0 || _size.y == 0, false, "Size is zero!");

        if (m_size == _size)
            return false;

        m_size = _size;

        eventRenderTargetResized(this);

        return true;
    }

    //////////////////////////////////////////
    void RenderBuffer::setColorTexture(U32 _index, TexturePtr const& _texture)
    {
        m_colorTextures[_index] = _texture;
    }

    //////////////////////////////////////////
    void RenderBuffer::setDepthTexture(TexturePtr const& _texture)
    {
        m_depthTexture = _texture;
    }

    //////////////////////////////////////////
    void RenderBuffer::setStencilTexture(TexturePtr const& _texture)
    {
        m_stencilTexture = _texture;
    }

    //////////////////////////////////////////
    static inline RenderBufferSpecification::TextureFormat GetTextureFormat(TexturePtr const& _texture)
    {
        switch (_texture->getType())
        {
        case TextureType::TwoDimensional:
            return { _texture->castRaw<Texture2D>()->getInternalPixelFormat(), 0 };
        case TextureType::TwoDimensionalMultisample:
            return {
                _texture->castRaw<Texture2DMS>()->getInternalPixelFormat(),
                _texture->castRaw<Texture2DMS>()->getSamples() };
        default:
            MAZE_NOT_IMPLEMENTED;
            return RenderBufferSpecification::TextureFormat();
        }
    }

    //////////////////////////////////////////
    RenderBufferSpecification RenderBuffer::getSpecification()
    {
        RenderBufferSpecification specification;
        specification.size = getSize();

        for (Size i = 0; i < c_renderBufferColorTexturesMax; ++i)
        {
            if (m_colorTextures[i])
                specification.colorTextureFormats[i] = GetTextureFormat(m_colorTextures[i]);
            else
                specification.colorTextureFormats[i] = PixelFormat::None;

            if (m_depthTexture)
                specification.depthTextureFormat = GetTextureFormat(m_depthTexture);
            else
                specification.depthTextureFormat = PixelFormat::None;

            if (m_stencilTexture)
                specification.stencilTextureFormat = GetTextureFormat(m_stencilTexture);
            else
                specification.stencilTextureFormat = PixelFormat::None;
        }

        specification._hash = specification.calculateHash();

        return specification;
    }

} // namespace Maze
//////////////////////////////////////////
