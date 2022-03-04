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
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SystemFontManager
    //
    //////////////////////////////////////////
    SystemFontManager::SystemFontManager()
    {
        
    }

    //////////////////////////////////////////
    SystemFontManager::~SystemFontManager()
    {
        if (m_renderSystemRaw && m_renderSystemRaw->getMaterialManager())
            m_renderSystemRaw->getMaterialManager()->eventSpecialMaterialsCreated.unsubscribe(this);
    }

    //////////////////////////////////////////
    void SystemFontManager::Initialize(SystemFontManagerPtr& _manager, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SystemFontManager, _manager, init(_renderSystem));
    }

    //////////////////////////////////////////
    bool SystemFontManager::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        m_renderSystemRaw->getMaterialManager()->eventSpecialMaterialsCreated.subscribe(
            this, &SystemFontManager::notifySpecialMaterialsCreated);

        return true;
    }

    //////////////////////////////////////////
    SystemFontManagerPtr const& SystemFontManager::GetCurrentInstancePtr()
    {
        return RenderSystem::GetCurrentInstancePtr()->getSystemFontManager();
    }

    //////////////////////////////////////////
    void SystemFontManager::notifySpecialMaterialsCreated()
    {
        createSystemFont();
    }

    //////////////////////////////////////////
    SystemFontPtr const& SystemFontManager::getSystemFont(String const& _fontName)
    {
        static SystemFontPtr nullPointer;

        SystemFontPtr* it = m_systemFontsByName.find(_fontName);
        if (it != nullptr)
            return *it;

        return nullPointer;
    }

    //////////////////////////////////////////
    CString SystemFontManager::getSystemFontName(SystemFont const* _font)
    {
        for (auto it = m_systemFontsByName.begin(), end = m_systemFontsByName.end(); it != end; ++it)
        {
            if (it.value().get() == _font)
                return it.key().str;
        }

        return nullptr;
    }

    //////////////////////////////////////////
    void SystemFontManager::createSystemFont()
    {
        // System Font
        S32 const extrude = 1;
        S32 const upscale = 2;
        Texture2DPtr texture = Texture2D::Create(m_renderSystemRaw);
        texture->setName("system_font");
        PixelSheet2D systemFontSheet = GraphicsUtilsHelper::GenerateSystemFontExtrude(
            GraphicsUtilsHelper::GetAsciiSymbolsSheet8x8(), 16, 6, 8, 8);
        
        Vector<PixelSheet2D> pixelSheets;
        pixelSheets.emplace_back(systemFontSheet.upscaledCopy(upscale));
        pixelSheets.emplace_back(systemFontSheet);
        Vec2DS size = systemFontSheet.getSize();
        while (size.x > 1 && size.y > 1)
        {
            pixelSheets.emplace_back(pixelSheets.back().downscaledCopy(2, false));
            size = pixelSheets.back().getSize();
        }

        texture->loadTexture(pixelSheets);
        m_renderSystemRaw->getTextureManager()->addTexture(texture);
        m_systemFontDefault = createSystemFont(
            texture,
            Vec2DS(8, 8) * upscale,
            (Vec2DS(8, 8) + extrude * 2) * upscale,
            Vec2DS(extrude, extrude) * upscale);
        registerSystemFont("system_font", m_systemFontDefault);


        // System Font Outlined
        m_systemFontDefaultOutlined = createSystemFontOutlined("system_font_outline", ColorU32::c_black);
        registerSystemFont("system_font_outline", m_systemFontDefaultOutlined);
    }

    //////////////////////////////////////////
    SystemFontPtr SystemFontManager::createSystemFont(
        Texture2DPtr const& _texture,
        Vec2DS const& _charSize,
        Vec2DS const& _stroke,
        Vec2DS const& _offset)
    {
        SystemFontPtr systemFont = std::make_shared<SystemFont>();

        systemFont->texture = _texture;
        systemFont->texture->setMagFilter(TextureFilter::Linear);
        systemFont->texture->setMinFilter(TextureFilter::NearestMipmapNearest);

        systemFont->charSize = _charSize;
        systemFont->stroke = _stroke;
        systemFont->offset = _offset;

        systemFont->material = MaterialManager::GetCurrentInstance()->getColorTextureMaterial()->createCopy();
        systemFont->material->ensureUniform("u_baseMap")->set(systemFont->texture);
        systemFont->material->ensureUniform("u_baseMapTexelSize")->set(1.0f / (Vec2DF)systemFont->texture->getSize());

        RenderPassPtr const& renderPass = systemFont->material->getFirstRenderPass();
        renderPass->setRenderQueueIndex(3000);
        renderPass->setDepthWriteEnabled(false);
        renderPass->setDepthTestCompareFunction(CompareFunction::Always);
        ShaderPtr shader = renderPass->getShader()->createCopy();
        shader->addLocalFeature("MAZE_COLOR_STREAM", "(1)");
        shader->addLocalFeature("MAZE_UV_STREAM", "(1)");
        shader->recompile();
        renderPass->setShader(shader);

        return systemFont;
    }

    //////////////////////////////////////////
    SystemFontPtr SystemFontManager::createSystemFontOutlined(
        String const& _name,
        ColorU32 const& _outlineColor)
    {
        S32 const extrude = 1;
        S32 const outline = 1;
        S32 const upscale = 2;
        Texture2DPtr texture = Texture2D::Create(m_renderSystemRaw);
        texture->setName(_name);
        PixelSheet2D systemFontSheet = GraphicsUtilsHelper::GenerateSystemFontExtrudeOutlined(
            GraphicsUtilsHelper::GetAsciiSymbolsSheet8x8(), 16, 6, 8, 8, _outlineColor);

        Vector<PixelSheet2D> pixelSheets;
        pixelSheets.emplace_back(systemFontSheet.upscaledCopy(upscale));
        pixelSheets.emplace_back(systemFontSheet);
        Vec2DS size = systemFontSheet.getSize();
        while (size.x > 1 && size.y > 1)
        {
            pixelSheets.emplace_back(pixelSheets.back().downscaledCopy(2, false));
            size = pixelSheets.back().getSize();
        }

        texture->loadTexture(pixelSheets);
        m_renderSystemRaw->getTextureManager()->addTexture(texture);
        SystemFontPtr systemFont = createSystemFont(
            texture,
            (Vec2DS(8, 8) + outline * 2) * upscale,
            (Vec2DS(8, 8) + outline * 2 + extrude * 2) * upscale,
            Vec2DS(extrude, extrude) * upscale);
        systemFont->outline = outline * upscale;

        systemFont->texture->setMagFilter(TextureFilter::Linear);
        systemFont->texture->setMinFilter(TextureFilter::LinearMipmapNearest);

        return systemFont;
    }

    //////////////////////////////////////////
    void SystemFontManager::registerSystemFont(String const& _name, SystemFontPtr const& _font)
    {
        m_systemFontsByName.insert(_name, _font);
    }

} // namespace Maze
//////////////////////////////////////////
