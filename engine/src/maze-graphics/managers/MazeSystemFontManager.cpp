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
    void SystemFontManager::createSystemFont()
    {
        S32 const extrude = 1;

        // System Font
        {
            SystemFontPtr systemFont = std::make_shared<SystemFont>();

            systemFont->texture = Texture2D::Create(m_renderSystemRaw);
            systemFont->texture->setName("system_font");
            systemFont->charSize = { 8, 8 };
            systemFont->stroke = systemFont->charSize + extrude * 2;
            systemFont->offset = { extrude, extrude };

            PixelSheet2D systemFontSheet = GraphicsUtilsHelper::GenerateSystemFontExtrude(
                GraphicsUtilsHelper::GetAsciiSymbolsSheet8x8(), 16, 6, 8, 8);

            systemFont->texture->loadTexture(systemFontSheet);
            m_renderSystemRaw->getTextureManager()->addTexture(systemFont->texture);

            finalizeSystemFont(systemFont);

            m_systemFont = systemFont;
        }

        // System Font Outlined
        m_systemFontOutlined = createSystemFontOutlined("system_font_outline", ColorU32::c_black);
    }

    //////////////////////////////////////////
    SystemFontPtr SystemFontManager::createSystemFontOutlined(
        String const& _name,
        ColorU32 const& _outlineColor)
    {
        S32 const extrude = 1;

        SystemFontPtr systemFont = std::make_shared<SystemFont>();

        systemFont->texture = Texture2D::Create(m_renderSystemRaw);
        systemFont->texture->setName(_name);
        systemFont->outline = 1;
        systemFont->charSize = Vec2DS(8, 8) + systemFont->outline * 2;
        systemFont->stroke = systemFont->charSize + (extrude) * 2;
        systemFont->offset = { extrude, extrude };

        PixelSheet2D systemFontSheet = GraphicsUtilsHelper::GenerateSystemFontExtrudeOutlined(
            GraphicsUtilsHelper::GetAsciiSymbolsSheet8x8(), 16, 6, 8, 8, _outlineColor);

        systemFont->texture->loadTexture(systemFontSheet);
        m_renderSystemRaw->getTextureManager()->addTexture(systemFont->texture);

        finalizeSystemFont(systemFont);

        return systemFont;
    }

    //////////////////////////////////////////
    void SystemFontManager::finalizeSystemFont(SystemFontPtr const& _systemFont)
    {
        _systemFont->texture->setMagFilter(TextureFilter::Nearest);
        _systemFont->texture->setMinFilter(TextureFilter::LinearMipmapLinear);

        _systemFont->material = MaterialManager::GetCurrentInstance()->getColorTextureMaterial()->createCopy();
        _systemFont->material->ensureUniform("u_baseMap")->set(_systemFont->texture);
        _systemFont->material->ensureUniform("u_baseMapTexelSize")->set(1.0f / (Vec2DF)_systemFont->texture->getSize());

        RenderPassPtr const& renderPass = _systemFont->material->getFirstRenderPass();
        renderPass->setRenderQueueIndex(3000);
        renderPass->setDepthWriteEnabled(false);
        renderPass->setDepthTestCompareFunction(CompareFunction::Always);
        ShaderPtr shader = renderPass->getShader()->createCopy();
        shader->addLocalFeature("MAZE_COLOR_STREAM", "(1)");
        shader->addLocalFeature("MAZE_UV_STREAM", "(1)");
        shader->recompile();
        renderPass->setShader(shader);
    }

} // namespace Maze
//////////////////////////////////////////
