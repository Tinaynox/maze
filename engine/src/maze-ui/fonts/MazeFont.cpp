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
#include "MazeUIHeader.hpp"
#include "maze-ui/fonts/MazeFont.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Font
    //
    //////////////////////////////////////////
    Font::Font()
    {
        
    }

    //////////////////////////////////////////
    Font::~Font()
    {
        
    }

    //////////////////////////////////////////
    FontPtr Font::Create()
    {
        FontPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Font, object, init());
        return object;
    }

    //////////////////////////////////////////
    FontPtr Font::Create(
        AssetFilePtr const& _assetFile)
    {
        FontPtr font = Font::Create();

        if (font)
            font->loadFromAssetFile(_assetFile);

        return font;
    }

    //////////////////////////////////////////
    bool Font::init()
    {
        return true;
    }

    //////////////////////////////////////////
    bool Font::loadFromAssetFile(
        AssetFilePtr const& _assetFile)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!_assetFile, false, "Null asset file!");

        setName(_assetFile->getFileName());

        tinyxml2::XMLDocument doc;
        if (!_assetFile->readToXMLDocument(doc))
            return false;

        tinyxml2::XMLNode* rootNode = doc.FirstChild();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File '%s' loading error - empty root node!", _assetFile->getFileName().c_str());
        
        rootNode = rootNode->NextSibling();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File '%s' loading error - empty root node children!", _assetFile->getFileName().c_str());

        m_glyphsData.clear();
        m_glyphsMap.clear();

        tinyxml2::XMLNode* childNode = rootNode->FirstChild();
        while (childNode)
        {
            tinyxml2::XMLElement* childElement = childNode->ToElement();
            if (!childElement)
            {
                childNode = childNode->NextSibling();
                continue;
            }

            if (strcmp(childElement->Name(), "Main") == 0)
            {
                CString fileStr = childElement->Attribute("file");
                if (fileStr)
                {
                    m_defaultGlyphsData.type = FontGlyphStorageType::TrueTypeFont;
                    m_defaultGlyphsData.trueTypeFont = TrueTypeFontManager::GetInstancePtr()->getTrueTypeFont(fileStr);
                }
            }
            else
                if (strcmp(childElement->Name(), "TrueType") == 0)
                {
                    CString fileStr = childElement->Attribute("file");
                    if (fileStr)
                    {
                        const TrueTypeFontPtr& trueTypeFont = TrueTypeFontManager::GetInstancePtr()->getTrueTypeFont(fileStr);
                        if (trueTypeFont)
                        {

                            tinyxml2::XMLNode* symbolsNode = childNode->FirstChild();
                            while (symbolsNode)
                            {
                                tinyxml2::XMLElement* symbolsElement = symbolsNode->ToElement();
                                if (!symbolsElement)
                                {
                                    symbolsNode = symbolsNode->NextSibling();
                                    continue;
                                }

                                CString fromStr = symbolsElement->Attribute("from");
                                CString toStr = symbolsElement->Attribute("to");
                                if (fromStr && toStr)
                                {
                                    U32 from = 0;
                                    U32 to = 0;
                                    {
                                        StringStream ss;
                                        ss << std::hex << fromStr;
                                        ss >> from;
                                    }
                                    {
                                        StringStream ss;
                                        ss << std::hex << toStr;
                                        ss >> to;
                                    }

                                    FontGlyphStorageData glyphsData;
                                    glyphsData.fromCodePoints = from;
                                    glyphsData.toCodePoints = to;
                                    glyphsData.type = FontGlyphStorageType::TrueTypeFont;
                                    glyphsData.trueTypeFont = trueTypeFont;
                                    m_glyphsData.push_back(glyphsData);
                                    m_glyphsMap.clear();
                                }

                                symbolsNode = symbolsNode->NextSibling();
                            }
                        }
                    }
                }
                else
                if (strcmp(childElement->Name(), "Sprite") == 0)
                {
                    CString symbolStr = childElement->Attribute("symbol");
                    CString spriteStr = childElement->Attribute("sprite");

                    if (symbolStr && spriteStr)
                    {
                        SpritePtr const& sprite = SpriteManager::GetCurrentInstance()->getSprite(spriteStr);
                        if (sprite)
                        {
                            U32 symbol = 0;
                            StringStream ss;
                            ss << std::hex << symbolStr;
                            ss >> symbol;

                            FontGlyphStorageData glyphsData;
                            glyphsData.fromCodePoints = symbol;
                            glyphsData.toCodePoints = symbol;
                            glyphsData.type = FontGlyphStorageType::Sprite;
                            glyphsData.spriteData.spriteGlyphFontSize = StringHelper::StringToU32(childElement->Attribute("fontSize"));
                            glyphsData.spriteData.spriteGlyph.advance = StringHelper::StringToF32(childElement->Attribute("advance"));

                            glyphsData.spriteData.spriteGlyph.bounds.position = sprite->getColorOffset() + Vec2DF::FromString(childElement->Attribute("boundsPosition"));
                            glyphsData.spriteData.spriteGlyph.bounds.size = sprite->getColorSize();
                            glyphsData.spriteData.spriteGlyph.texture = sprite->getTexture();

                            glyphsData.spriteData.spriteGlyph.textureCoords.position = sprite->getTextureCoordLB();
                            glyphsData.spriteData.spriteGlyph.textureCoords.size = sprite->getTextureCoordSize();
                            m_glyphsData.push_back(glyphsData);
                            m_glyphsMap.clear();
                        }
                    }
                }
                else
                if (strcmp(childElement->Name(), "Entity") == 0)
                {
                    CString symbolStr = childElement->Attribute("symbol");
                    CString prefabStr = childElement->Attribute("prefab");

                    if (symbolStr && prefabStr)
                    {
                        U32 symbol = 0;
                        StringStream ss;
                        ss << std::hex << symbolStr;
                        ss >> symbol;

                        FontGlyphStorageData glyphsData;
                        glyphsData.fromCodePoints = symbol;
                        glyphsData.toCodePoints = symbol;
                        glyphsData.type = FontGlyphStorageType::Entity;
                        glyphsData.entityData.prefabName = prefabStr;
                        glyphsData.entityData.prefab = nullptr; // prefabStr
                        glyphsData.entityData.prefabGlyphFontSize = StringHelper::StringToU32(childElement->Attribute("fontSize"));
                        glyphsData.entityData.prefabGlyph.advance = StringHelper::StringToF32(childElement->Attribute("advance"));
                        glyphsData.entityData.prefabGlyph.bounds.position = Vec2DF::FromString(childElement->Attribute("boundsPosition"));
                        glyphsData.entityData.prefabGlyph.bounds.size = Vec2DF::FromString(childElement->Attribute("boundsSize"));
                        m_glyphsData.push_back(glyphsData);
                        m_glyphsMap.clear();
                    }
                }


            childNode = childNode->NextSibling();
        }

        return true;
    }

    //////////////////////////////////////////
    FontGlyphStorageData* Font::getGlyphStorageData(U32 _codePoint)
    {
        Map<U32, FontGlyphStorageData*>::iterator it = m_glyphsMap.find(_codePoint);
        if (it != m_glyphsMap.end())
            return it->second;

        Vector<FontGlyphStorageData>::iterator it2 =
            std::find_if(
                m_glyphsData.begin(),
                m_glyphsData.end(),
                [_codePoint](FontGlyphStorageData const& _data) -> bool
            {
                return _data.contains(_codePoint);
            }
        );

        if (it2 == m_glyphsData.end())
        {
            m_glyphsMap[_codePoint] = nullptr;
            return nullptr;
        }

        m_glyphsMap[_codePoint] = &(*it2);
        return &(*it2);
    }

    //////////////////////////////////////////
    FontGlyph const& Font::getGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize)
    {
        static FontGlyph nullGlyph;

        if (!_storage)
        {
            if (m_defaultGlyphsData.trueTypeFont)
                return m_defaultGlyphsData.trueTypeFont->getGlyph(_codePoint, _fontSize);

            return nullGlyph;
        }

        switch (_storage->type)
        {
            case FontGlyphStorageType::None: return nullGlyph;
            case FontGlyphStorageType::TrueTypeFont: return _storage->trueTypeFont->getGlyph(_codePoint, _fontSize);
            case FontGlyphStorageType::Sprite: return _storage->spriteData.getGlyph(_fontSize);
            case FontGlyphStorageType::Entity: return _storage->entityData.getGlyph(_fontSize);
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return nullGlyph;
    }

    //////////////////////////////////////////
    FontGlyph const& Font::getGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, TTFPagePtr<U32>& _ttfPage)
    {
        static FontGlyph nullGlyph;

        if (!_storage)
        {
            if (m_defaultGlyphsData.trueTypeFont)
                return m_defaultGlyphsData.trueTypeFont->getGlyph(_codePoint, _fontSize, _ttfPage);

            return nullGlyph;
        }

        switch (_storage->type)
        {
            case FontGlyphStorageType::None: return nullGlyph;
            case FontGlyphStorageType::TrueTypeFont: return _storage->trueTypeFont->getGlyph(_codePoint, _fontSize, _ttfPage);
            case FontGlyphStorageType::Sprite: return _storage->spriteData.getGlyph(_fontSize);
            case FontGlyphStorageType::Entity: return _storage->entityData.getGlyph(_fontSize);
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return nullGlyph;
    }

    //////////////////////////////////////////
    FontGlyph const& Font::getGlyph(U32 _codePoint, U32 _fontSize)
    {
        FontGlyphStorageData* glyphStorageData = getGlyphStorageData(_codePoint);
        return getGlyphFromStorage(glyphStorageData, _codePoint, _fontSize);
    }

    //////////////////////////////////////////
    FontGlyph const& Font::getOutlinedGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, F32 _outlineThickness)
    {
        static FontGlyph nullGlyph;

        if (!_storage)
        {
            if (m_defaultGlyphsData.trueTypeFont)
                return m_defaultGlyphsData.trueTypeFont->getOutlinedGlyph(_codePoint, _fontSize, _outlineThickness);

            return nullGlyph;
        }

        switch (_storage->type)
        {
            case FontGlyphStorageType::None: return nullGlyph;
            case FontGlyphStorageType::TrueTypeFont: return _storage->trueTypeFont->getOutlinedGlyph(_codePoint, _fontSize, _outlineThickness);
            case FontGlyphStorageType::Sprite: return _storage->spriteData.getGlyph(_fontSize);
            case FontGlyphStorageType::Entity: return _storage->entityData.getGlyph(_fontSize);
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return nullGlyph;
    }

    //////////////////////////////////////////
    FontGlyph const& Font::getOutlinedGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, F32 _outlineThickness, TTFPagePtr<U64>& _ttfPage)
    {
        static FontGlyph nullGlyph;

        if (!_storage)
        {
            if (m_defaultGlyphsData.trueTypeFont)
                return m_defaultGlyphsData.trueTypeFont->getOutlinedGlyph(_codePoint, _fontSize, _outlineThickness, _ttfPage);

            return nullGlyph;
        }

        switch (_storage->type)
        {
            case FontGlyphStorageType::None: return nullGlyph;
            case FontGlyphStorageType::TrueTypeFont: return _storage->trueTypeFont->getOutlinedGlyph(_codePoint, _fontSize, _outlineThickness, _ttfPage);
            case FontGlyphStorageType::Sprite: return _storage->spriteData.getGlyph(_fontSize);
            case FontGlyphStorageType::Entity: return _storage->entityData.getGlyph(_fontSize);
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return nullGlyph;
    }

    //////////////////////////////////////////
    FontGlyph const& Font::getOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness)
    {
        FontGlyphStorageData* glyphStorageData = getGlyphStorageData(_codePoint);
        return getOutlinedGlyphFromStorage(glyphStorageData, _codePoint, _fontSize, _outlineThickness);
    }

    //////////////////////////////////////////
    F32 Font::getLineSpacing(U32 _fontSize) const
    {
        if (!m_defaultGlyphsData.trueTypeFont)
            return _fontSize * 1.5f;

        return m_defaultGlyphsData.trueTypeFont->getLineSpacing(_fontSize);
    }

    //////////////////////////////////////////
    F32 Font::getKerning(U32 _first, U32 _second, U32 _fontSize) const
    {
        if (!m_defaultGlyphsData.trueTypeFont)
            return 0.0f;

        return m_defaultGlyphsData.trueTypeFont->getKerning(_first, _second, _fontSize);
    }

    //////////////////////////////////////////
    F32 Font::getUnderlinePosition(U32 _fontSize) const
    {
        if (!m_defaultGlyphsData.trueTypeFont)
            return 0.0f;

        return m_defaultGlyphsData.trueTypeFont->getUnderlinePosition(_fontSize);
    }

    //////////////////////////////////////////
    F32 Font::getUnderlineThickness(U32 _fontSize) const
    {
        if (!m_defaultGlyphsData.trueTypeFont)
            return 1.0f;

        return m_defaultGlyphsData.trueTypeFont->getUnderlineThickness(_fontSize);
    }

} // namespace Maze
//////////////////////////////////////////
