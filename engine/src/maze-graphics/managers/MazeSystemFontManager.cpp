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
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinSystemFontType);


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
            m_renderSystemRaw->getMaterialManager()->eventBuiltinMaterialsCreated.unsubscribe(this);
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

        m_renderSystemRaw->getMaterialManager()->eventBuiltinMaterialsCreated.subscribe(
            this, &SystemFontManager::notifyBuiltinMaterialsCreated);

        return true;
    }

    //////////////////////////////////////////
    SystemFontManagerPtr const& SystemFontManager::GetCurrentInstancePtr()
    {
        return RenderSystem::GetCurrentInstancePtr()->getSystemFontManager();
    }

    //////////////////////////////////////////
    void SystemFontManager::notifyBuiltinMaterialsCreated()
    {
        // createBuiltinSystemFonts();
    }

    //////////////////////////////////////////
    SystemFontPtr const& SystemFontManager::getSystemFont(HashedCString _fontName)
    {
        static SystemFontPtr const nullPointer;

        SystemFontPtr* it = m_systemFontsByName.tryGet(_fontName);
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
    SystemFontPtr const& SystemFontManager::createBuiltinSystemFont(BuiltinSystemFontType _fontType)
    {
        SystemFontPtr& systemFont = m_builtinSystemFonts[_fontType];

        switch (_fontType)
        {
            case BuiltinSystemFontType::Default:
            case BuiltinSystemFontType::Default3D:
            {
                S32 const extrude = 1;
                S32 const upscale = 2;
                Texture2DPtr texture = Texture2D::Create(m_renderSystemRaw);
                texture->setName(_fontType.toCString());
                PixelSheet2D systemFontSheet = GraphicsUtilsHelper::GenerateSystemFontExtrude(
                    GraphicsUtilsHelper::GetAsciiSymbolsSheet8x8(), 16, 6, 8, 8);

                Vector<PixelSheet2D> pixelSheets;
                pixelSheets.emplace_back(systemFontSheet.upscaledCopy(upscale));
                pixelSheets.emplace_back(systemFontSheet);
                Vec2S size = systemFontSheet.getSize();
                while (size.x > 1 && size.y > 1)
                {
                    pixelSheets.emplace_back(pixelSheets.back().downscaledCopy(2, false));
                    size = pixelSheets.back().getSize();
                }

                texture->loadTexture(pixelSheets);
                m_renderSystemRaw->getTextureManager()->addTextureToLibrary(texture);
                systemFont = createSystemFont(
                    texture,
                    Vec2S(8, 8) * upscale,
                    (Vec2S(8, 8) + extrude * 2) * upscale,
                    Vec2S(extrude, extrude) * upscale);

                if (_fontType == BuiltinSystemFontType::Default3D)
                {
                    systemFont->texture->setMagMinFilters(TextureFilter::Linear, TextureFilter::LinearMipmapLinear);
                    systemFont->material->getFirstRenderPass()->setDepthTestCompareFunction(CompareFunction::LessEqual);
                }

                break;
            }
            case BuiltinSystemFontType::DefaultOutlined:
            case BuiltinSystemFontType::Default3DOutlined:
            {
                systemFont = createSystemFontOutlined(_fontType.toCString(), ColorU32::c_black);

                if (_fontType == BuiltinSystemFontType::Default3DOutlined)
                {
                    systemFont->texture->setMagMinFilters(TextureFilter::Linear, TextureFilter::LinearMipmapLinear);
                    systemFont->material->getFirstRenderPass()->setDepthTestCompareFunction(CompareFunction::LessEqual);
                }

                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }


        if (systemFont)
        {
            registerSystemFont(_fontType.toCString(), systemFont);
        }

        return systemFont;
    }

    //////////////////////////////////////////
    SystemFontPtr const& SystemFontManager::ensureBuiltinSystemFont(BuiltinSystemFontType _fontType)
    {
        SystemFontPtr const& systemFont = getBuiltinSystemFont(_fontType);
        if (systemFont)
            return systemFont;

        return createBuiltinSystemFont(_fontType);
    }

    //////////////////////////////////////////
    void SystemFontManager::createBuiltinSystemFonts()
    {
        MAZE_PROFILE_EVENT("SystemFontManager::createBuiltinSystemFonts");

        for (BuiltinSystemFontType t = BuiltinSystemFontType(1); t < BuiltinSystemFontType::MAX; ++t)
            ensureBuiltinSystemFont(t);        
    }

    //////////////////////////////////////////
    SystemFontPtr SystemFontManager::createSystemFont(
        Texture2DPtr const& _texture,
        Vec2S const& _charSize,
        Vec2S const& _stroke,
        Vec2S const& _offset)
    {
        SystemFontPtr systemFont = std::make_shared<SystemFont>();

        systemFont->texture = _texture;
        systemFont->texture->setMagFilter(TextureFilter::Linear);
        systemFont->texture->setMinFilter(TextureFilter::NearestMipmapNearest);

        systemFont->charSize = _charSize;
        systemFont->stroke = _stroke;
        systemFont->offset = _offset;

        systemFont->material = MaterialManager::GetCurrentInstance()->getColorTextureMaterial()->createCopy();
        systemFont->material->ensureUniform(MAZE_HS("u_baseMap"))->set(systemFont->texture);
        systemFont->material->ensureUniform(MAZE_HS("u_baseMapTexelSize"))->set(1.0f / (Vec2F)systemFont->texture->getSize());

        RenderPassPtr const& renderPass = systemFont->material->getFirstRenderPass();
        renderPass->setRenderQueueIndex(3000);
        renderPass->setDepthWriteEnabled(false);
        renderPass->setDepthTestCompareFunction(CompareFunction::Always);
        ShaderPtr shader = renderPass->getShader()->createCopy();
        shader->addLocalFeature("MAZE_COLOR_STREAM", "(1)");
        shader->addLocalFeature("MAZE_UV0_STREAM", "(1)");
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
        Vec2S size = systemFontSheet.getSize();
        while (size.x > 1 && size.y > 1)
        {
            pixelSheets.emplace_back(pixelSheets.back().downscaledCopy(2, false));
            size = pixelSheets.back().getSize();
        }

        texture->loadTexture(pixelSheets);
        m_renderSystemRaw->getTextureManager()->addTextureToLibrary(texture);
        SystemFontPtr systemFont = createSystemFont(
            texture,
            (Vec2S(8, 8) + outline * 2) * upscale,
            (Vec2S(8, 8) + outline * 2 + extrude * 2) * upscale,
            Vec2S(extrude, extrude) * upscale);
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
