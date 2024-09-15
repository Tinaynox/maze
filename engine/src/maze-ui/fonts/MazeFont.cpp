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
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-ui/managers/MazeFontManager.hpp"


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
        unsubscribeGlyphsData();
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
        MAZE_PROFILE_EVENT("Font::loadFromAssetFile");

        MAZE_ERROR_RETURN_VALUE_IF(!_assetFile, false, "Null asset file!");

        setName(_assetFile->getFileName());

        ByteBufferPtr assetFileHeader = _assetFile->readHeaderAsByteBuffer(6);
        assetFileHeader->setByte(5, 0);

        if (strstr((CString)assetFileHeader->getDataRO(), "xml") != nullptr)
        {
            Debug::LogWarning("Obsolete Material format - %s", _assetFile->getFileName().toUTF8().c_str());
            tinyxml2::XMLDocument doc;
            _assetFile->readToXMLDocument(doc);
            return loadFromXMLDocument(doc);
        }
        else
        {
            DataBlock dataBlock;
            ByteBufferPtr byteBuffer = _assetFile->readAsByteBuffer();
            dataBlock.loadFromByteBuffer(*byteBuffer.get());
            return loadFromDataBlock(dataBlock);
        }
        
    }

    //////////////////////////////////////////
    bool Font::loadFromXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        tinyxml2::XMLNode* rootNode = _doc.FirstChild();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File '%s' loading error - empty root node!", m_name.c_str());

        rootNode = rootNode->NextSibling();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File '%s' loading error - empty root node children!", m_name.c_str());

        unsubscribeGlyphsData();

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
                    m_defaultGlyphsData.setTrueTypeFont(TrueTypeFontManager::GetInstancePtr()->getOrLoadTrueTypeFont(fileStr));
                }
            }
            else
            if (strcmp(childElement->Name(), "TrueType") == 0)
            {
                CString fileStr = childElement->Attribute("file");
                if (fileStr)
                {
                    const TrueTypeFontPtr& trueTypeFont = TrueTypeFontManager::GetInstancePtr()->getOrLoadTrueTypeFont(fileStr);
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
                                glyphsData.setTrueTypeFont(trueTypeFont);
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
                    SpritePtr const& sprite = SpriteManager::GetCurrentInstance()->getOrLoadSprite(spriteStr);
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

                        glyphsData.spriteData.spriteGlyph.bounds.position = sprite->getColorOffset() + Vec2F::FromString(childElement->Attribute("boundsPosition"));
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
                    glyphsData.entityData.prefabGlyph.bounds.position = Vec2F::FromString(childElement->Attribute("boundsPosition"));
                    glyphsData.entityData.prefabGlyph.bounds.size = Vec2F::FromString(childElement->Attribute("boundsSize"));
                    m_glyphsData.push_back(glyphsData);
                    m_glyphsMap.clear();
                }
            }

            childNode = childNode->NextSibling();
        }

        subscribeGlyphsData();

        return true;
    }

    //////////////////////////////////////////
    bool Font::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        unsubscribeGlyphsData();

        m_glyphsData.clear();
        m_glyphsMap.clear();

        for (DataBlock::DataBlockIndex i = 0; i < _dataBlock.getDataBlocksCount(); ++i)
        {
            DataBlock const* subBlock = _dataBlock.getDataBlock(i);
            if (subBlock->getName() == MAZE_HCS("main"))
            {
                CString fileStr = subBlock->getCString(MAZE_HCS("file"));
                if (fileStr)
                {
                    m_defaultGlyphsData.type = FontGlyphStorageType::TrueTypeFont;
                    m_defaultGlyphsData.setTrueTypeFont(TrueTypeFontManager::GetInstancePtr()->getOrLoadTrueTypeFont(fileStr));
                }
            }
            else
            if (subBlock->getName() == MAZE_HCS("trueType"))
            {
                CString fileStr = subBlock->getCString(MAZE_HCS("file"));
                if (fileStr)
                {
                    const TrueTypeFontPtr& trueTypeFont = TrueTypeFontManager::GetInstancePtr()->getOrLoadTrueTypeFont(fileStr);
                    if (trueTypeFont)
                    {
                        for (DataBlock::DataBlockIndex j = 0; j < subBlock->getDataBlocksCount(); ++j)
                        {
                            DataBlock const* symbolsBlock = subBlock->getDataBlock(j);

                            if (symbolsBlock->isParamExists(MAZE_HCS("from")) && symbolsBlock->isParamExists(MAZE_HCS("to")))
                            {
                                U32 from = symbolsBlock->getU32(MAZE_HCS("from"));
                                U32 to = symbolsBlock->getU32(MAZE_HCS("to"));

                                FontGlyphStorageData glyphsData;
                                glyphsData.fromCodePoints = from;
                                glyphsData.toCodePoints = to;
                                glyphsData.type = FontGlyphStorageType::TrueTypeFont;
                                glyphsData.setTrueTypeFont(trueTypeFont);
                                m_glyphsData.push_back(glyphsData);
                                m_glyphsMap.clear();
                            }
                        }
                    }
                }
            }
            else
            if (subBlock->getName() == MAZE_HCS("sprite"))
            {
                if (subBlock->isParamExists(MAZE_HCS("symbol")) && subBlock->isParamExists(MAZE_HCS("sprite")))
                {
                    U32 symbol = subBlock->getU32(MAZE_HCS("symbol"));
                    CString spriteStr = subBlock->getCString(MAZE_HCS("sprite"));

                    SpritePtr const& sprite = SpriteManager::GetCurrentInstance()->getOrLoadSprite(spriteStr);
                    if (sprite)
                    {
                        FontGlyphStorageData glyphsData;
                        glyphsData.fromCodePoints = symbol;
                        glyphsData.toCodePoints = symbol;
                        glyphsData.type = FontGlyphStorageType::Sprite;
                        glyphsData.spriteData.spriteGlyphFontSize = subBlock->getU32(MAZE_HCS("fontSize"));
                        glyphsData.spriteData.spriteGlyph.advance = subBlock->getF32(MAZE_HCS("advance"));

                        glyphsData.spriteData.spriteGlyph.bounds.position = sprite->getColorOffset() + subBlock->getVec2F(MAZE_HCS("boundsPosition"));
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
            if (subBlock->getName() == MAZE_HCS("entity"))
            {
                if (subBlock->isParamExists(MAZE_HCS("symbol")) && subBlock->isParamExists(MAZE_HCS("prefab")))
                {
                    U32 symbol = subBlock->getU32(MAZE_HCS("symbol"));
                    CString prefabStr = subBlock->getCString(MAZE_HCS("prefab"));

                    FontGlyphStorageData glyphsData;
                    glyphsData.fromCodePoints = symbol;
                    glyphsData.toCodePoints = symbol;
                    glyphsData.type = FontGlyphStorageType::Entity;
                    glyphsData.entityData.prefabName = prefabStr;
                    glyphsData.entityData.prefab = nullptr; // prefabStr
                    glyphsData.entityData.prefabGlyphFontSize = subBlock->getU32(MAZE_HCS("fontSize"));
                    glyphsData.entityData.prefabGlyph.advance = subBlock->getF32(MAZE_HCS("advance"));
                    glyphsData.entityData.prefabGlyph.bounds.position = subBlock->getVec2F(MAZE_HCS("boundsPosition"));
                    glyphsData.entityData.prefabGlyph.bounds.size = subBlock->getVec2F(MAZE_HCS("boundsSize"));
                    m_glyphsData.push_back(glyphsData);
                    m_glyphsMap.clear();
                }
            }
        }

        subscribeGlyphsData();

        return true;
    }

    //////////////////////////////////////////
    void Font::toDataBlock(DataBlock& _dataBlock) const
    {
        MAZE_TODO;
    }

    //////////////////////////////////////////
    String Font::toString() const
    {
        String value;
        Font::ToString(this, value);
        return value;
    }

    //////////////////////////////////////////
    void Font::setString(CString _data, Size _count)
    {
        MAZE_TODO;
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
    FontGlyph const& Font::ensureGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize)
    {
        static FontGlyph nullGlyph;

        if (!_storage)
        {
            if (m_defaultGlyphsData.getTrueTypeFont())
                return m_defaultGlyphsData.getTrueTypeFont()->ensureGlyph(_codePoint, _fontSize);

            return nullGlyph;
        }

        switch (_storage->type)
        {
            case FontGlyphStorageType::None: return nullGlyph;
            case FontGlyphStorageType::TrueTypeFont: return _storage->getTrueTypeFont()->ensureGlyph(_codePoint, _fontSize);
            case FontGlyphStorageType::Sprite: return _storage->spriteData.ensureGlyph(_fontSize);
            case FontGlyphStorageType::Entity: return _storage->entityData.ensureGlyph(_fontSize);
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return nullGlyph;
    }

    //////////////////////////////////////////
    FontGlyph const& Font::ensureGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, TTFPagePtr const& _ttfPage)
    {
        static FontGlyph nullGlyph;

        if (!_storage)
        {
            if (m_defaultGlyphsData.getTrueTypeFont())
                return m_defaultGlyphsData.getTrueTypeFont()->ensureGlyph(_codePoint, _fontSize, _ttfPage);

            return nullGlyph;
        }

        switch (_storage->type)
        {
            case FontGlyphStorageType::None: return nullGlyph;
            case FontGlyphStorageType::TrueTypeFont: return _storage->getTrueTypeFont()->ensureGlyph(_codePoint, _fontSize, _ttfPage);
            case FontGlyphStorageType::Sprite: return _storage->spriteData.ensureGlyph(_fontSize);
            case FontGlyphStorageType::Entity: return _storage->entityData.ensureGlyph(_fontSize);
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return nullGlyph;
    }

    //////////////////////////////////////////
    FontGlyph const& Font::ensureGlyph(U32 _codePoint, U32 _fontSize)
    {
        FontGlyphStorageData* glyphStorageData = getGlyphStorageData(_codePoint);
        return ensureGlyphFromStorage(glyphStorageData, _codePoint, _fontSize);
    }

    //////////////////////////////////////////
    FontGlyph const& Font::ensureOutlinedGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, F32 _outlineThickness)
    {
        static FontGlyph nullGlyph;

        if (!_storage)
        {
            if (m_defaultGlyphsData.getTrueTypeFont())
                return m_defaultGlyphsData.getTrueTypeFont()->ensureOutlinedGlyph(_codePoint, _fontSize, _outlineThickness);

            return nullGlyph;
        }

        switch (_storage->type)
        {
            case FontGlyphStorageType::None: return nullGlyph;
            case FontGlyphStorageType::TrueTypeFont: return _storage->getTrueTypeFont()->ensureOutlinedGlyph(_codePoint, _fontSize, _outlineThickness);
            case FontGlyphStorageType::Sprite: return _storage->spriteData.ensureGlyph(_fontSize);
            case FontGlyphStorageType::Entity: return _storage->entityData.ensureGlyph(_fontSize);
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return nullGlyph;
    }

    //////////////////////////////////////////
    FontGlyph const& Font::ensureOutlinedGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, F32 _outlineThickness, TTFPagePtr const& _ttfPage)
    {
        static FontGlyph nullGlyph;

        if (!_storage)
        {
            if (m_defaultGlyphsData.getTrueTypeFont())
                return m_defaultGlyphsData.getTrueTypeFont()->ensureOutlinedGlyph(_codePoint, _fontSize, _outlineThickness, _ttfPage);

            return nullGlyph;
        }

        switch (_storage->type)
        {
            case FontGlyphStorageType::None: return nullGlyph;
            case FontGlyphStorageType::TrueTypeFont: return _storage->getTrueTypeFont()->ensureOutlinedGlyph(_codePoint, _fontSize, _outlineThickness, _ttfPage);
            case FontGlyphStorageType::Sprite: return _storage->spriteData.ensureGlyph(_fontSize);
            case FontGlyphStorageType::Entity: return _storage->entityData.ensureGlyph(_fontSize);
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return nullGlyph;
    }

    //////////////////////////////////////////
    FontGlyph const& Font::ensureOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness)
    {
        FontGlyphStorageData* glyphStorageData = getGlyphStorageData(_codePoint);
        return ensureOutlinedGlyphFromStorage(glyphStorageData, _codePoint, _fontSize, _outlineThickness);
    }

    //////////////////////////////////////////
    F32 Font::getLineSpacing(U32 _fontSize) const
    {
        if (!m_defaultGlyphsData.getTrueTypeFont())
            return _fontSize * 1.5f;

        return m_defaultGlyphsData.getTrueTypeFont()->getLineSpacing(_fontSize);
    }

    //////////////////////////////////////////
    F32 Font::getKerning(U32 _first, U32 _second, U32 _fontSize) const
    {
        if (!m_defaultGlyphsData.getTrueTypeFont())
            return 0.0f;

        return m_defaultGlyphsData.getTrueTypeFont()->getKerning(_first, _second, _fontSize);
    }

    //////////////////////////////////////////
    F32 Font::getUnderlinePosition(U32 _fontSize) const
    {
        if (!m_defaultGlyphsData.getTrueTypeFont())
            return 0.0f;

        return m_defaultGlyphsData.getTrueTypeFont()->getUnderlinePosition(_fontSize);
    }

    //////////////////////////////////////////
    F32 Font::getUnderlineThickness(U32 _fontSize) const
    {
        if (!m_defaultGlyphsData.getTrueTypeFont())
            return 1.0f;

        return m_defaultGlyphsData.getTrueTypeFont()->getUnderlineThickness(_fontSize);
    }

    //////////////////////////////////////////
    void Font::FromString(FontPtr& _value, CString _data, Size _count)
    {
        MAZE_PROFILE_EVENT("Font::FromString");

        if (!_data || strcmp(_data, "") == 0)
        {
            _value.reset();
            return;
        }

        if (_count == 0)
            _count = strlen(_data);

        FontManager* fontManager = FontManager::GetInstancePtr();

        if (StringHelper::IsStartsWith(_data, "ptr:"))
        {
            String data = String(_data + 4, _data + _count);
            StringHelper::StringToObjectPtr(_value, data);
        }
        else
        {
            _value = fontManager->getOrLoadFont(MAZE_HCS(_data));
        }
    }

    //////////////////////////////////////////
    void Font::ToString(Font const* _value, String& _data)
    {
        MAZE_PROFILE_EVENT("Font::ToString");

        if (!_value)
        {
            _data.clear();
            return;
        }

        FontManager* fontManager = FontManager::GetInstancePtr();

        String const& fontName = fontManager->getFontName(_value);
        if (!fontName.empty())
        {
            _data = fontName;
        }
        else
        {
            StringHelper::FormatString(_data, "ptr:%p", _value);
        }
    }

    //////////////////////////////////////////
    void Font::updateMaterialTextures()
    {
        TrueTypeFontPtr const& defaultFont = getDefaultFont();
    }

    //////////////////////////////////////////
    void Font::subscribeGlyphsData()
    {
        for (FontGlyphStorageData& glyphsData : m_glyphsData)
            glyphsData.eventTexturesChanged.subscribe(this, &Font::notifyTexturesChanged);

        m_defaultGlyphsData.eventTexturesChanged.subscribe(this, &Font::notifyTexturesChanged);
    }

    //////////////////////////////////////////
    void Font::unsubscribeGlyphsData()
    {
        for (FontGlyphStorageData& glyphsData : m_glyphsData)
            glyphsData.eventTexturesChanged.unsubscribe(this, &Font::notifyTexturesChanged);

        m_defaultGlyphsData.eventTexturesChanged.unsubscribe(this, &Font::notifyTexturesChanged);
    }

    //////////////////////////////////////////
    void Font::collectAllTextures(U32 _fontSize, Vector<Texture2DPtr>& _result) const
    {
        for (FontGlyphStorageData const& data : m_glyphsData)
            data.collectAllTextures(_fontSize, _result);

        m_defaultGlyphsData.collectAllTextures(_fontSize, _result);
    }

    //////////////////////////////////////////
    void Font::notifyTexturesChanged()
    {
        eventTexturesChanged();
    }

} // namespace Maze
//////////////////////////////////////////
