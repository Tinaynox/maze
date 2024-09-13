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
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderBMP.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2D, Texture,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(TextureFilter, magFilter, TextureFilter::Linear, getMagFilter, setMagFilter),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(TextureFilter, minFilter, TextureFilter::Linear, getMinFilter, setMinFilter),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(TextureWrap, wrapS, TextureWrap::ClampToEdge, getWrapS, setWrapS),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(TextureWrap, wrapT, TextureWrap::ClampToEdge, getWrapT, setWrapT),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, anisotropyLevel, 0.0f, getAnisotropyLevel, setAnisotropyLevel));


    //////////////////////////////////////////
    // Class Texture2D
    //
    //////////////////////////////////////////
    Texture2D::Texture2D()
        : m_size(Vec2S::c_zero)
        , m_magFilter(TextureFilter::Linear)
        , m_minFilter(TextureFilter::Linear)
        , m_wrapS(TextureWrap::ClampToEdge)
        , m_wrapT(TextureWrap::ClampToEdge)
        , m_internalPixelFormat(PixelFormat::None)
    {
        m_type = TextureType::TwoDimensional;
    }

    //////////////////////////////////////////
    Texture2D::~Texture2D()
    {
        
    }

    //////////////////////////////////////////
    Texture2DPtr Texture2D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        return _renderSystem->createTexture2D();
    }

    //////////////////////////////////////////
    Texture2DPtr Texture2D::Create(
        AssetFilePtr const& _assetFile,
        RenderSystem* _renderSystem)
    {
        Texture2DPtr texture2D = Texture2D::Create(_renderSystem);
        if (texture2D)
        {
            texture2D->loadFromAssetFile(_assetFile);
            texture2D->setName(_assetFile->getFileName());
        }

        return texture2D;
    }

    //////////////////////////////////////////
    Texture2DPtr Texture2D::Create(
        String const& _assetFileName,
        RenderSystem* _renderSystem)
    {
        Texture2DPtr texture2D = Texture2D::Create(_renderSystem);
        if (texture2D)
        {
            texture2D->loadFromAssetFile(_assetFileName);
            texture2D->setName(_assetFileName);
        }

        return texture2D;
    }

    //////////////////////////////////////////
    Texture2DPtr Texture2D::Create(
        U32 _width,
        U32 _height,
        PixelFormat::Enum _pixelFormat,
        RenderSystem* _renderSystem)
    {
        Texture2DPtr texture = Texture2D::Create(_renderSystem);
        if (!texture)
            return nullptr;
        texture->loadEmpty({ _width, _height }, _pixelFormat);
        return texture;
    }

    //////////////////////////////////////////
    bool Texture2D::init(RenderSystem* _renderSystem)
    {
        if (!Texture::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void Texture2D::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        MAZE_PROFILE_EVENT("Texture2D::loadFromAssetFile");

        TextureManagerPtr const& textureManager = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager();
        Maze::Vector<Maze::PixelSheet2D> pixelSheets = textureManager->loadPixelSheets2D(_assetFile);
        
        loadTexture(pixelSheets);
    }

    //////////////////////////////////////////
    void Texture2D::loadFromAssetFile(String const& _assetFileName)
    {
        MAZE_PROFILE_EVENT("Texture2D::loadFromAssetFile");

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        loadFromAssetFile(assetFile);
    }

    //////////////////////////////////////////
    bool Texture2D::loadEmpty(
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat)
    {
        MAZE_PROFILE_EVENT("Texture2D::loadEmpty");

        PixelSheet2D pixelSheet(_size, _pixelFormat);
        loadTexture(pixelSheet, _pixelFormat);
        return true;
    }

    //////////////////////////////////////////
    bool Texture2D::loadFromBuffer(
        ByteBufferPtr const& _buffer,
        PixelFormat::Enum _bufferPixelFormat,
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat)
    {
        MAZE_PROFILE_EVENT("Texture2D::loadFromBuffer");

        Vector<ByteBufferPtr> buffers = { _buffer };
        return loadFromBuffers(buffers, _bufferPixelFormat, _size, _pixelFormat);
    }

    //////////////////////////////////////////
    bool Texture2D::loadFromBuffers(
        Vector<ByteBufferPtr> const& _buffers,
        PixelFormat::Enum _bufferPixelFormat,
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat)
    {
        MAZE_PROFILE_EVENT("Texture2D::loadFromBuffers");

        Vector<PixelSheet2D> pixelSheets;
        pixelSheets.resize(_buffers.size());
        for (Size i = 0, in = _buffers.size(); i < in; ++i)
        {
            ByteBufferPtr const& byteBuffer = _buffers[i];
            PixelSheet2D& pixelSheet = pixelSheets[i];
            pixelSheet.copyFrom(byteBuffer->getDataRW(), (Vec2S)_size, _bufferPixelFormat);
        }

        return loadTexture(pixelSheets, _pixelFormat);
    }

    //////////////////////////////////////////
    bool Texture2D::loadTexture(
        PixelSheet2D const& _pixelSheet,
        PixelFormat::Enum _internalPixelFormat)
    {
        MAZE_PROFILE_EVENT("Texture2D::loadTexture");

        Vector<PixelSheet2D> pixelSheets = { _pixelSheet };
        return loadTexture(pixelSheets, _internalPixelFormat);
    }

    //////////////////////////////////////////
    void Texture2D::reload()
    {
        MAZE_PROFILE_EVENT("Texture2D::reload");

        Texture2DLibraryData const* libraryData = m_renderSystem->getTextureManager()->getTexture2DLibraryData(getName().asHashedCString());
        if (libraryData && libraryData->callbacks.requestReload)
            libraryData->callbacks.requestReload(true);
    }

    //////////////////////////////////////////
    void Texture2D::FromString(Texture2DPtr& _value, CString _data, Size _count)
    {
        MAZE_PROFILE_EVENT("Texture2D::FromString");

        if (!_data || strcmp(_data, "") == 0)
        {
            _value.reset();
            return;
        }

        if (_count == 0)
            _count = strlen(_data);

        RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        TextureManagerPtr const& textureManager = renderSystem->getTextureManager();

        if (StringHelper::IsStartsWith(_data, "ptr:"))
        {
            String data = String(_data + 4, _data + _count);
            StringHelper::StringToObjectPtr(_value, data);
        }
        else
        {
            _value = textureManager->getTexture2D(MAZE_HASHED_CSTRING(_data));
        }
    }

    //////////////////////////////////////////
    void Texture2D::ToString(Texture2D const* _value, String& _data)
    {
        MAZE_PROFILE_EVENT("Texture2D::ToString");

        if (!_value)
        {
            _data.clear();
            return;
        }

        RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        TextureManagerPtr const& textureManager = renderSystem->getTextureManager();

        String const& textureName = textureManager->getTextureName(_value);
        if (!textureName.empty())
        {
            _data = textureName;
        }
        else
        {
            StringHelper::FormatString(_data, "ptr:%p", _value);
        }
    }

    //////////////////////////////////////////
    String Texture2D::toString() const
    {
        String result;
        Texture2D::ToString(this, result);
        return result;
    }

    //////////////////////////////////////////
    void Texture2D::setString(CString _data, Size _count)
    {
        MAZE_TODO;
    }

} // namespace Maze
//////////////////////////////////////////
