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
#include "maze-ui/managers/MazeFontManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-ui/fonts/MazeFont.hpp"
#include "maze-ui/assets/MazeAssetUnitFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class FontManager
    //
    //////////////////////////////////////////
    FontManager* FontManager::s_instance = nullptr;

    //////////////////////////////////////////
    FontManager::FontManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    FontManager::~FontManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void FontManager::Initialize(FontManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(FontManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool FontManager::init()
    {
        TrueTypeFontManager::Initialize(m_trueTypeFontManager);
        if (!m_trueTypeFontManager)
            return false;

        FontMaterialManager::Initialize(m_fontMaterialManager);
        if (!m_fontMaterialManager)
            return false;
        

        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitFont::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
                {
                    return AssetUnitFont::Create(_file, _data);
                });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
                {
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitFont>::UID())
                        _assetUnit->castRaw<AssetUnitFont>()->initFont();
                });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
                {
                    if (_extension == MAZE_HCS("mzfont"))
                    {
                        if (!_assetFile->getAssetUnit<AssetUnitFont>())
                            _assetFile->addAssetUnit(AssetUnitFont::Create(_assetFile));
                    }
                });
        }

        return true;
    }

    //////////////////////////////////////////
    FontLibraryData const* FontManager::getFontLibraryData(HashedCString _fontName)
    {
        StringKeyMap<FontLibraryData>::const_iterator it = m_fontsLibrary.find(_fontName);
        if (it != m_fontsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    FontPtr const& FontManager::getOrLoadFont(
        HashedCString _font,
        bool _syncLoad)
    {
        static FontPtr const nullPointer;

        FontLibraryData const* libraryData = getFontLibraryData(_font);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->font;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_font);
        if (!assetFile)
            return nullPointer;

        FontPtr font = Font::Create(assetFile);
        if (!font)
            return nullPointer;

        font->setName(HashedString(_font));
        FontLibraryData* data = addFontToLibrary(font);
        if (data)
        {
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)assetFile,
                    fontWeak = (FontWPtr)font
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    FontPtr font = fontWeak.lock();
                    if (assetFile && font)
                        font->loadFromAssetFile(assetFile);
                };
            data->callbacks.hasAnyOfTags =
                [assetFileWeak = (AssetFileWPtr)assetFile](Set<String> const& _tags)
                {
                    if (AssetFilePtr assetFile = assetFileWeak.lock())
                        return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            return data->font;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    FontLibraryData* FontManager::addFontToLibrary(
        FontPtr const& _font,
        FontLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_fontsLibrary.insert(
            _font->getName(),
            { _font, _callbacks, _info });
        return it2;
    }

    //////////////////////////////////////////
    void FontManager::removeFontFromLibrary(HashedCString _fontName)
    {
        m_fontsLibrary.erase(_fontName);
    }

    //////////////////////////////////////////
    String const& FontManager::getFontName(Font const* _font)
    {
        static String nullPointer;

        for (auto const& fontData : m_fontsLibrary)
        {
            if (fontData.second.font.get() == _font)
                return fontData.first;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    void FontManager::unloadAssetFontMaterials(Set<String> const& _tags)
    {
        Vector<std::function<void(bool)>> unloadCallbacks;

        m_fontsLibrary.iterate(
            [&](HashedCString _name, FontLibraryData const& _data)
            {
                if (_data.callbacks.hasAnyOfTags && _data.callbacks.hasAnyOfTags(_tags) && _data.callbacks.requestUnload)
                    unloadCallbacks.push_back(_data.callbacks.requestUnload);

                return true;
            });

        for (std::function<void(bool)> const& unloadCallback : unloadCallbacks)
            unloadCallback(true);
    }


} // namespace Maze
//////////////////////////////////////////
