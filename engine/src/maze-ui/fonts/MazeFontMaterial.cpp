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
#include "maze-ui/fonts/MazeFontMaterial.hpp"
#include "maze-ui/fonts/MazeFont.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-ui/managers/MazeFontManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    void FontMaterialRenderData::updateMaterialUniforms()
    {
        if (!material)
            return;

        if (textures.empty())
        {
            Texture2D** errorTexture = (Texture2D**)TextureManager::GetCurrentInstancePtr()->getErrorTexture().get();
            material->ensureUniform(MAZE_HASHED_CSTRING("u_baseMaps"))->set(errorTexture, 1);
        }
        else
        {
            material->ensureUniform(MAZE_HASHED_CSTRING("u_baseMaps"))->set(&textures[0], (U32)textures.size());
        }
    }


    //////////////////////////////////////////
    // Class FontMaterial
    //
    //////////////////////////////////////////
    FontMaterial::FontMaterial()
    {
        
    }

    //////////////////////////////////////////
    FontMaterial::~FontMaterial()
    {
        setFont(nullptr);
    }

    //////////////////////////////////////////
    FontMaterialPtr FontMaterial::Create()
    {
        FontMaterialPtr fontMaterial;
        MAZE_CREATE_AND_INIT_SHARED_PTR(FontMaterial, fontMaterial, init());
        return fontMaterial;
    }

    //////////////////////////////////////////
    FontMaterialPtr FontMaterial::Create(
        AssetFilePtr const& _assetFile)
    {
        FontMaterialPtr fontMaterial = FontMaterial::Create();
        if (!fontMaterial)
            return nullptr;

        if (!fontMaterial->loadFromAssetFile(_assetFile))
            return nullptr;

        return fontMaterial;
    }

    //////////////////////////////////////////
    bool FontMaterial::init()
    {
        return true;
    }

    //////////////////////////////////////////
    bool FontMaterial::loadFromAssetFile(
        AssetFilePtr const& _assetFile)
    {
        MAZE_PROFILE_EVENT("FontMaterial::loadFromAssetFile");

        MAZE_ERROR_RETURN_VALUE_IF(!_assetFile, false, "Null asset file!");

        setName(_assetFile->getFileName());

        ByteBufferPtr assetFileHeader = _assetFile->readHeaderAsByteBuffer(6);
        assetFileHeader->setByte(5, 0);

        if (strstr((CString)assetFileHeader->getDataRO(), "xml") != nullptr)
        {
            Debug::LogWarning("Obsolete Font Material format - %s", _assetFile->getFileName().toUTF8().c_str());
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
    bool FontMaterial::loadFromXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        tinyxml2::XMLNode* rootNode = _doc.FirstChild();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File '%s' loading error - empty root node!", m_name.c_str());

        rootNode = rootNode->NextSibling();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File '%s' loading error - empty root node children!", m_name.c_str());

        tinyxml2::XMLElement* element = rootNode->ToElement();
        if (!element)
            return false;

        CString font = element->Attribute("font");
        CString assetMaterial = element->Attribute("assetMaterial");

        setFont(font ? FontManager::GetInstancePtr()->getOrLoadFont(font) : FontPtr());
        setAssetMaterial(assetMaterial ? MaterialManager::GetCurrentInstance()->getOrLoadMaterial(assetMaterial) : MaterialPtr());

        return true;
    }

    //////////////////////////////////////////
    bool FontMaterial::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        CString font = _dataBlock.getCString(MAZE_HCS("font"));
        CString assetMaterial = _dataBlock.getCString(MAZE_HCS("assetMaterial"));

        setFont(font ? FontManager::GetInstancePtr()->getOrLoadFont(font) : FontPtr());
        setAssetMaterial(assetMaterial ? MaterialManager::GetCurrentInstance()->getOrLoadMaterial(assetMaterial) : MaterialPtr());

        return true;
    }

    //////////////////////////////////////////
    void FontMaterial::toDataBlock(DataBlock& _dataBlock) const
    {
        MAZE_TODO;
    }

    //////////////////////////////////////////
    String FontMaterial::toString() const
    {
        String value;
        FontMaterial::ToString(this, value);
        return value;
    }

    //////////////////////////////////////////
    void FontMaterial::setString(CString _data, Size _count)
    {
        MAZE_TODO;
    }

    //////////////////////////////////////////
    void FontMaterial::FromString(FontMaterialPtr& _value, CString _data, Size _count)
    {
        MAZE_PROFILE_EVENT("FontMaterial::FromString");

        if (!_data || strcmp(_data, "") == 0)
        {
            _value.reset();
            return;
        }

        if (_count == 0)
            _count = strlen(_data);

        FontMaterialManager* fontManager = FontMaterialManager::GetInstancePtr();

        if (StringHelper::IsStartsWith(_data, "ptr:"))
        {
            String data = String(_data + 4, _data + _count);
            StringHelper::StringToObjectPtr(_value, data);
        }
        else
        {
            _value = fontManager->getOrLoadFontMaterial(MAZE_HASHED_CSTRING(_data));
        }
    }

    //////////////////////////////////////////
    void FontMaterial::ToString(FontMaterial const* _value, String& _data)
    {
        MAZE_PROFILE_EVENT("FontMaterial::ToString");

        if (!_value)
        {
            _data.clear();
            return;
        }

        FontMaterialManager* fontMaterialManager = FontMaterialManager::GetInstancePtr();

        String const& fontName = fontMaterialManager->getFontMaterialName(_value);
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
    void FontMaterial::setFont(FontPtr const& _value)
    {
        if (m_font)
        {
            m_font->eventTexturesChanged.unsubscribe(this, &FontMaterial::notifyTexturesChanged);
        }

        m_font = _value;

        if (m_font)
        {
            m_font->eventTexturesChanged.subscribe(this, &FontMaterial::notifyTexturesChanged);
        }
    }

    //////////////////////////////////////////
    void FontMaterial::setAssetMaterial(MaterialPtr const& _value)
    {
        m_assetMaterial = _value;

        if (m_assetMaterial)
        {
            for (auto& renderData : m_renderData)
            {
                renderData.second->material = m_assetMaterial->createCopy();
                renderData.second->updateMaterialUniforms();
            }
        }
        else
        {
            for (auto& renderData : m_renderData)
                renderData.second->material.reset(); 
        }

        eventMaterialChanged();
    }

    //////////////////////////////////////////
    FontMaterialRenderData const& FontMaterial::fetchRenderData(U32 _fontSize)
    {
        auto it = m_renderData.find(_fontSize);
        if (it != m_renderData.end())
        {
            if (it->second->texturesDirty)
                updateFontTextures(_fontSize);

            return *it->second;
        }

        FontMaterialRenderDataPtr& data = m_renderData[_fontSize];
        if (!data)
            data = std::make_shared<FontMaterialRenderData>();

        if (m_assetMaterial)
            data->material = m_assetMaterial->createCopy();
        updateFontTextures(_fontSize);
        return *data;
    }

    //////////////////////////////////////////
    MaterialPtr const& FontMaterial::fetchMaterial(U32 _fontSize)
    {
        return fetchRenderData(_fontSize).material;
    }

    //////////////////////////////////////////
    void FontMaterial::updateFontTextures(U32 _fontSize)
    {
        FontMaterialRenderDataPtr& data = m_renderData[_fontSize];
        if (!data)
            data = std::make_shared<FontMaterialRenderData>();

        data->textures.clear();
        data->textureIndices.clear();

        if (!m_font)
            return;

        data->texturesDirty = false;

        Vector<Texture2DPtr> textures;
        m_font->collectAllTextures(_fontSize, textures);
        data->textures.resize(textures.size());
        for (S32 i = 0, in = (S32)data->textures.size(); i < in; ++i)
            data->textures[i] = textures[i].get();

        for (S32 i = 0, in = (S32)data->textures.size(); i < in; ++i)
            data->textureIndices[data->textures[i]] = i;

        data->updateMaterialUniforms();
    }

    //////////////////////////////////////////
    void FontMaterial::notifyTexturesChanged()
    {
        for (auto& renderData : m_renderData)
            renderData.second->texturesDirty = true;

        eventTexturesChanged();
    }

    //////////////////////////////////////////
    S32 FontMaterial::getTextureIndex(U32 _fontSize, Texture2D* _texture)
    {
        FontMaterialRenderData const& renderData = fetchRenderData(_fontSize);
        auto it = renderData.textureIndices.find(_texture);
        if (it == renderData.textureIndices.end())
            return -1;

        return it->second;
    }


    //////////////////////////////////////////
    // Class FontMaterialAssetRef
    //
    //////////////////////////////////////////
    String FontMaterialAssetRef::toString() const
    {
        if (!m_fontMaterial)
            return String();

        return FontMaterialManager::GetInstancePtr()->getFontMaterialName(m_fontMaterial.get());
    }

    //////////////////////////////////////////
    void FontMaterialAssetRef::setString(CString _data, Size _count)
    {
        FontMaterialPtr const& fontMaterial = FontMaterialManager::GetInstancePtr()->getOrLoadFontMaterial(_data);
        setFontMaterial(fontMaterial);
    }


} // namespace Maze
//////////////////////////////////////////
