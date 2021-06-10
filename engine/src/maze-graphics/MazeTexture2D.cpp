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
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderBMP.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Texture2D
    //
    //////////////////////////////////////////
    Texture2D::Texture2D()
        : m_size(Vec2DS::c_zero)
        , m_magFilter(TextureFilter::Linear)
        , m_minFilter(TextureFilter::Linear)
        , m_wrapS(TextureWrap::ClampToEdge)
        , m_wrapT(TextureWrap::ClampToEdge)
        , m_internalPixelFormat(PixelFormat::None)
    {
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
    bool Texture2D::init(RenderSystem* _renderSystem)
    {
        if (!Texture::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void Texture2D::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        m_assetFile = _assetFile;

        Maze::Vector<Maze::PixelSheet2D> pixelSheets;

        UnorderedMap<String, String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);

        if (metaData.empty())
        {
            if (Maze::IsBMPFile(_assetFile))
            {
                MAZE_ERROR_IF(!Maze::LoadBMP(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            if (Maze::IsPNGFile(_assetFile))
            {
                MAZE_ERROR_IF(!Maze::LoadPNG(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported texture format!");
            }
        }
        else
        {
            String fileExtension = StringHelper::ToLower(metaData["ext"]);
            if (fileExtension == "bmp")
            {
                MAZE_ERROR_IF(!Maze::LoadBMP(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            if (fileExtension == "png")
            {
                MAZE_ERROR_IF(!Maze::LoadPNG(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported texture format!");
            }
        }
        
        loadTexture(pixelSheets);
    }

    //////////////////////////////////////////
    void Texture2D::loadFromAssetFile(String const& _assetFileName)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        loadFromAssetFile(assetFile);
    }

    //////////////////////////////////////////
    bool Texture2D::loadEmpty(
        Vec2DU const& _size,
        PixelFormat::Enum _pixelFormat)
    {
        PixelSheet2D pixelSheet(_size, _pixelFormat);
        loadTexture(pixelSheet, _pixelFormat);
        return true;
    }

    //////////////////////////////////////////
    String const& Texture2D::getAssetFileName() const
    {
        static String nullValue;

        if (!m_assetFile)
            return nullValue;

        return m_assetFile->getFileName();
    }

    //////////////////////////////////////////
    bool Texture2D::loadTexture(
        PixelSheet2D const& pixelSheet,
        PixelFormat::Enum internalPixelFormat)
    {
        Vector<PixelSheet2D> pixelSheets = { pixelSheet };
        return loadTexture(pixelSheets, internalPixelFormat);
    }

    //////////////////////////////////////////
    void Texture2D::reload()
    {
        if (m_assetFile)
        {
            loadFromAssetFile(m_assetFile);
        }
    }


} // namespace Maze
//////////////////////////////////////////
