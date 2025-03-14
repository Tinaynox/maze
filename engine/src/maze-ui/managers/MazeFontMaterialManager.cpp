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
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-ui/fonts/MazeFontMaterial.hpp"
#include "maze-ui/assets/MazeAssetUnitFontMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class FontMaterialManager
    //
    //////////////////////////////////////////
    FontMaterialManager* FontMaterialManager::s_instance = nullptr;

    //////////////////////////////////////////
    FontMaterialManager::FontMaterialManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    FontMaterialManager::~FontMaterialManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void FontMaterialManager::Initialize(FontMaterialManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(FontMaterialManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool FontMaterialManager::init()
    {
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitFontMaterial::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
                {
                    return AssetUnitFontMaterial::Create(_file, _data);
                });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
                {
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitFontMaterial>::UID())
                        _assetUnit->castRaw<AssetUnitFontMaterial>()->initFontMaterial();
                });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
                {
                    if (_extension == MAZE_HCS("mzfontmaterial"))
                    {
                        if (!_assetFile->getAssetUnit<AssetUnitFontMaterial>())
                            _assetFile->addAssetUnit(AssetUnitFontMaterial::Create(_assetFile));
                    }
                });

            AssetManager::GetInstancePtr()->eventAssetFileMoved.subscribe(
                [](AssetFilePtr const& _assetFile, Path const& _prevPath)
            {
                if (_assetFile->getExtension() == Path("mzfontmaterial"))
                {
                    if (!FontMaterialManager::GetInstancePtr())
                        return;

                    StringKeyMap<FontMaterialLibraryData>& fontMaterialsLibrary = FontMaterialManager::GetInstancePtr()->m_fontMaterialsLibrary;
                    String prevMaterialName = FileHelper::GetFileNameInPath(_prevPath).toUTF8();
                    StringKeyMap<FontMaterialLibraryData>::iterator it = fontMaterialsLibrary.find(prevMaterialName);
                    if (it != fontMaterialsLibrary.end())
                    {
                        String newAssetName = _assetFile->getFileName().toUTF8();
                        it->second.fontMaterial->setName(HashedString(newAssetName));
                        fontMaterialsLibrary.insert(newAssetName, it->second);
                        fontMaterialsLibrary.erase(it);
                    }
                }
            });
        }
        
        return true;
    }

    //////////////////////////////////////////
    FontMaterialLibraryData const* FontMaterialManager::getFontMaterialLibraryData(HashedCString _fontMaterialName)
    {
        StringKeyMap<FontMaterialLibraryData>::const_iterator it = m_fontMaterialsLibrary.find(_fontMaterialName);
        if (it != m_fontMaterialsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    FontMaterialPtr const& FontMaterialManager::getOrLoadFontMaterial(
        HashedCString _trueTypeFont,
        bool _syncLoad)
    {
        static FontMaterialPtr const nullPointer;

        FontMaterialLibraryData const* libraryData = getFontMaterialLibraryData(_trueTypeFont);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->fontMaterial;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_trueTypeFont);
        if (!assetFile)
            return nullPointer;

        FontMaterialPtr fontMaterial = FontMaterial::Create(assetFile);
        if (!fontMaterial)
            return nullPointer;

        fontMaterial->setName(HashedString(_trueTypeFont));
        FontMaterialLibraryData* data = addFontMaterialToLibrary(fontMaterial);
        if (data)
        {
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)assetFile,
                    fontMaterialWeak = (FontMaterialWPtr)fontMaterial
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    FontMaterialPtr fontMaterial = fontMaterialWeak.lock();
                    if (assetFile && fontMaterial)
                        fontMaterial->loadFromAssetFile(assetFile);
                };
            data->callbacks.hasAnyOfTags =
                [assetFileWeak = (AssetFileWPtr)assetFile](Set<String> const& _tags)
                {
                    if (AssetFilePtr assetFile = assetFileWeak.lock())
                        return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            return data->fontMaterial;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    FontMaterialLibraryData* FontMaterialManager::addFontMaterialToLibrary(
        FontMaterialPtr const& _trueTypeFont,
        FontMaterialLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_fontMaterialsLibrary.insert(
            _trueTypeFont->getName(),
            { _trueTypeFont, _callbacks, _info });
        return it2;
    }

    //////////////////////////////////////////
    void FontMaterialManager::removeFontMaterialFromLibrary(HashedCString _fontMaterialName)
    {
        m_fontMaterialsLibrary.erase(_fontMaterialName);
    }

    //////////////////////////////////////////
    String const& FontMaterialManager::getFontMaterialName(FontMaterial const* _font)
    {
        static String nullPointer;

        for (auto const& fontData : m_fontMaterialsLibrary)
        {
            if (fontData.second.fontMaterial.get() == _font)
                return fontData.first;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    void FontMaterialManager::unloadAssetFontMaterials(Set<String> const& _tags)
    {
        Vector<std::function<void(bool)>> unloadCallbacks;

        m_fontMaterialsLibrary.iterate(
            [&](HashedCString _name, FontMaterialLibraryData const& _data)
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
