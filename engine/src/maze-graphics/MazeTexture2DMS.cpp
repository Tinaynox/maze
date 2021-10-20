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
#include "maze-graphics/MazeTexture2DMS.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderBMP.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DMS, Texture);


    //////////////////////////////////////////
    // Class Texture2DMS
    //
    //////////////////////////////////////////
    Texture2DMS::Texture2DMS()
        : m_size(Vec2DS::c_zero)
        , m_internalPixelFormat(PixelFormat::None)
    {
        m_type = TextureType::TwoDimensionalMultisample;
    }

    //////////////////////////////////////////
    Texture2DMS::~Texture2DMS()
    {
        
    }

    //////////////////////////////////////////
    Texture2DMSPtr Texture2DMS::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        return _renderSystem->createTexture2DMS();
    }

    //////////////////////////////////////////
    Texture2DMSPtr Texture2DMS::Create(
        Vec2DU const& _size,
        PixelFormat::Enum _internalPixelFormat,
        S32 _samples,
        RenderSystem* _renderSystem)
    {
        Texture2DMSPtr texture2D = Texture2DMS::Create(_renderSystem);
        if (texture2D)
            texture2D->loadEmpty(_size, _internalPixelFormat, _samples);

        return texture2D;
    }

    //////////////////////////////////////////
    bool Texture2DMS::init(RenderSystem* _renderSystem)
    {
        if (!Texture::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    String const& Texture2DMS::getAssetFileName() const
    {
        static String nullValue;

        if (!m_assetFile)
            return nullValue;

        return m_assetFile->getFileName();
    }

    //////////////////////////////////////////
    void Texture2DMS::reload()
    {
        loadEmpty(m_size, m_internalPixelFormat, m_samples);
    }

    //////////////////////////////////////////
    String Texture2DMS::toString() const
    {
        MAZE_NOT_IMPLEMENTED;
        return String();
    }

    //////////////////////////////////////////
    void Texture2DMS::setString(CString _data, Size _count)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    Texture2DMSPtr const& Texture2DMS::FromString(CString _data, Size _count)
    {
        static Texture2DMSPtr nullPointer;

        MAZE_NOT_IMPLEMENTED;

        return nullPointer;
    }


} // namespace Maze
//////////////////////////////////////////
