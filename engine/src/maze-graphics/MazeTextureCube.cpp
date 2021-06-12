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
#include "maze-graphics/MazeTextureCube.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderBMP.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TextureCube, Texture);


    //////////////////////////////////////////
    // Class TextureCube
    //
    //////////////////////////////////////////
    TextureCube::TextureCube()
    {
        m_type = TextureType::Cube;
    }

    //////////////////////////////////////////
    TextureCube::~TextureCube()
    {
    }

    //////////////////////////////////////////
    TextureCubePtr TextureCube::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        return _renderSystem->createTextureCube();
    }

    //////////////////////////////////////////
    TextureCubePtr TextureCube::Create(
        AssetFilePtr const& _assetFile,
        RenderSystem* _renderSystem)
    {
        TextureCubePtr textureCube = TextureCube::Create(_renderSystem);
        if (textureCube)
        {
            textureCube->loadFromAssetFile(_assetFile);
            textureCube->setName(_assetFile->getFileName());
        }

        return textureCube;
    }

    //////////////////////////////////////////
    TextureCubePtr TextureCube::Create(
        String const& _assetFileName,
        RenderSystem* _renderSystem)
    {
        TextureCubePtr textureCube = TextureCube::Create(_renderSystem);
        if (textureCube)
        {
            textureCube->loadFromAssetFile(_assetFileName);
            textureCube->setName(_assetFileName);
        }

        return textureCube;
    }

    //////////////////////////////////////////
    bool TextureCube::init(RenderSystem* _renderSystem)
    {
        if (!Texture::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void TextureCube::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        m_assetFile = _assetFile;

        Maze::Vector<Maze::PixelSheet2D> pixelSheets[6];

        MAZE_NOT_IMPLEMENTED;

        loadTexture(pixelSheets);
    }

    //////////////////////////////////////////
    void TextureCube::loadFromAssetFile(String const& _assetFileName)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        loadFromAssetFile(assetFile);
    }

    //////////////////////////////////////////
    String const& TextureCube::getAssetFileName() const
    {
        static String nullValue;

        if (!m_assetFile)
            return nullValue;

        return m_assetFile->getFileName();
    }

    //////////////////////////////////////////
    bool TextureCube::loadTexture(
        PixelSheet2D _pixelSheet[6],
        PixelFormat::Enum _internalPixelFormat)
    {
        Vector<PixelSheet2D> pixelSheets[6] = 
            {
                {_pixelSheet[0]},
                {_pixelSheet[1]},
                {_pixelSheet[2]},
                {_pixelSheet[3]},
                {_pixelSheet[4]},
                {_pixelSheet[5]},
            };
        return loadTexture(pixelSheets, _internalPixelFormat);
    }

    //////////////////////////////////////////
    bool TextureCube::loadTexture(
        String const& _rightName,
        String const& _leftName,
        String const& _topName,
        String const& _bottomName,
        String const& _frontName,
        String const& _backName,
        PixelFormat::Enum _internalPixelFormat)
    {
        TextureManagerPtr const& textureManager = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager();

        Vector<PixelSheet2D> pixelSheets[6];
        pixelSheets[0] = textureManager->loadPixelSheets2D(_rightName);
        pixelSheets[1] = textureManager->loadPixelSheets2D(_leftName);
        pixelSheets[2] = textureManager->loadPixelSheets2D(_topName);
        pixelSheets[3] = textureManager->loadPixelSheets2D(_bottomName);
        pixelSheets[4] = textureManager->loadPixelSheets2D(_frontName);
        pixelSheets[5] = textureManager->loadPixelSheets2D(_backName);

        return loadTexture(pixelSheets, _internalPixelFormat);
    }

} // namespace Maze
//////////////////////////////////////////
