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
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-ui/managers/MazeFontManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
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
            return false;

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
        MAZE_ERROR_RETURN_VALUE_IF(!_assetFile, false, "Null asset file!");

        setName(_assetFile->getFileName());

        tinyxml2::XMLDocument doc;
        if (!_assetFile->readToXMLDocument(doc))
            return false;

        tinyxml2::XMLNode* rootNode = doc.FirstChild();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File '%s' loading error - empty root node!", _assetFile->getFileName().c_str());

        rootNode = rootNode->NextSibling();
        MAZE_ERROR_RETURN_VALUE_IF(!rootNode, false, "File '%s' loading error - empty root node children!", _assetFile->getFileName().c_str());

        tinyxml2::XMLElement* element = rootNode->ToElement();
        if (!element)
            return false;

        CString font = element->Attribute("font");
        CString material = element->Attribute("material");

        setFont(font ? FontManager::GetInstancePtr()->getFont(font) : FontPtr());
        setMaterial(material ? MaterialManager::GetCurrentInstance()->getMaterial(material) : MaterialPtr());

        return true;
    }

    //////////////////////////////////////////
    void FontMaterial::FromString(FontMaterialPtr& _value, CString _data, Size _count)
    {
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
            _value = fontManager->getFontMaterial(MAZE_HASHED_CSTRING(_data));
        }
    }

    //////////////////////////////////////////
    void FontMaterial::ToString(FontMaterial const* _value, String& _data)
    {
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


} // namespace Maze
//////////////////////////////////////////
