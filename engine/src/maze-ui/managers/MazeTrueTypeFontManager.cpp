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
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-ui/assets/MazeAssetUnitTrueTypeFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TrueTypeFontManager
    //
    //////////////////////////////////////////
    TrueTypeFontManager* TrueTypeFontManager::s_instance = nullptr;

    //////////////////////////////////////////
    TrueTypeFontManager::TrueTypeFontManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    TrueTypeFontManager::~TrueTypeFontManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void TrueTypeFontManager::Initialize(TrueTypeFontManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(TrueTypeFontManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool TrueTypeFontManager::init()
    {
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitTrueTypeFont::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
                {
                    return AssetUnitTrueTypeFont::Create(_file, _data);
                });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
                {
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitTrueTypeFont>::UID())
                    {
                        if (TrueTypeFontManager::GetInstancePtr() && TrueTypeFontManager::GetInstancePtr()->isTrueTypeFontSupported())
                            _assetUnit->castRaw<AssetUnitTrueTypeFont>()->initTrueTypeFont();
                    }
                });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
                {
                    if (_extension == MAZE_HCS("ttf"))
                    {
                        if (!_assetFile->getAssetUnit<AssetUnitTrueTypeFont>())
                            _assetFile->addAssetUnit(AssetUnitTrueTypeFont::Create(_assetFile));
                    }
                });
        }
        
        return true;
    }

    //////////////////////////////////////////
    TrueTypeFontLibraryData const* TrueTypeFontManager::getTrueTypeFontLibraryData(HashedCString _fontName)
    {
        StringKeyMap<TrueTypeFontLibraryData>::const_iterator it = m_trueTypeFontsLibrary.find(_fontName);
        if (it != m_trueTypeFontsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    TrueTypeFontPtr const& TrueTypeFontManager::getOrLoadTrueTypeFont(
        HashedCString _trueTypeFont,
        bool _syncLoad)
    {
        static TrueTypeFontPtr const nullPointer;

        TrueTypeFontLibraryData const* libraryData = getTrueTypeFontLibraryData(_trueTypeFont);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->trueTypeFont;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_trueTypeFont);
        if (!assetFile)
            return nullPointer;

        TrueTypeFontPtr font = createTrueTypeFont();
        if (!font)
            return nullPointer;

        font->loadFromAssetFile(assetFile);
        font->setName(_trueTypeFont.str);

        TrueTypeFontLibraryData* data = addTrueTypeFontToLibrary(font);
        if (data)
        {
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)assetFile,
                    fontWeak = (TrueTypeFontWPtr)font
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    TrueTypeFontPtr font = fontWeak.lock();
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

            return data->trueTypeFont;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    TrueTypeFontLibraryData* TrueTypeFontManager::addTrueTypeFontToLibrary(
        TrueTypeFontPtr const& _trueTypeFont,
        TrueTypeFontLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_trueTypeFontsLibrary.insert(
            _trueTypeFont->getName(),
            { _trueTypeFont, _callbacks, _info });
        return it2;
    }

    //////////////////////////////////////////
    void TrueTypeFontManager::removeTrueTypeFontFromLibrary(HashedCString _fontName)
    {
        m_trueTypeFontsLibrary.erase(_fontName);
    }

    //////////////////////////////////////////
    bool TrueTypeFontManager::isTrueTypeFontSupported()
    {
        return m_trueTypeFontLoader.createTrueTypeFontFunc != nullptr;
    }

    //////////////////////////////////////////
    TrueTypeFontPtr TrueTypeFontManager::createTrueTypeFont()
    {
        MAZE_WARNING_RETURN_VALUE_IF(!isTrueTypeFontSupported(), nullptr, "There is no TTF loader. Loading is not possible.");
        return m_trueTypeFontLoader.createTrueTypeFontFunc();
    }

    //////////////////////////////////////////
    void TrueTypeFontManager::unloadAssetTrueTypeFonts(Set<String> const& _tags)
    {
        Vector<std::function<void(bool)>> unloadCallbacks;

        m_trueTypeFontsLibrary.iterate(
            [&](HashedCString _name, TrueTypeFontLibraryData const& _data)
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
