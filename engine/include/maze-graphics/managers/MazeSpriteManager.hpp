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
#pragma once
#if (!defined(_MazeSpriteManager_hpp_))
#define _MazeSpriteManager_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(SpriteManager);
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Enum BuiltinSpriteType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_GRAPHICS_API, BuiltinSpriteType,
        White,
        Black,
        Error);


    //////////////////////////////////////////
    // Struct SpriteLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API SpriteLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct SpriteLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API SpriteLibraryData
    {
        //////////////////////////////////////////
        SpriteLibraryData(
            SpritePtr const& _sprite = nullptr,
            SpriteLibraryDataCallbacks const& _callbacks = SpriteLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : sprite(_sprite)
            , callbacks(_callbacks)
            , data(_data)
        {}

        SpritePtr sprite;
        SpriteLibraryDataCallbacks callbacks;
        DataBlock data;
    };


    //////////////////////////////////////////
    // Class SpriteManager
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SpriteManager 
    {
    public:

        //////////////////////////////////////////
        virtual ~SpriteManager();

        //////////////////////////////////////////
        static void Initialize(SpriteManagerPtr& _textureManager, RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        static SpriteManagerPtr const& GetCurrentInstance();


        //////////////////////////////////////////
        inline StringKeyMap<SpriteLibraryData> const& getSpritesLibrary() const { return m_spritesLibrary; }


        //////////////////////////////////////////
        SpriteLibraryData const* getSpriteLibraryData(HashedCString _spriteName);

        //////////////////////////////////////////
        SpriteLibraryData const* getSpriteLibraryData(String const& _assetFileName) { return getSpriteLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        SpriteLibraryData const* getSpriteLibraryData(CString _assetFileName) { return getSpriteLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        SpritePtr const& getOrLoadSprite(HashedCString _imageName, bool _syncLoad = true);

        //////////////////////////////////////////
        inline SpritePtr const& getOrLoadSprite(String const& _imageName, bool _syncLoad = true) { return getOrLoadSprite(MAZE_HASHED_CSTRING(_imageName.c_str()), _syncLoad); }

        //////////////////////////////////////////
        inline SpritePtr const& getOrLoadSprite(CString _imageName, bool _syncLoad = true) { return getOrLoadSprite(MAZE_HASHED_CSTRING(_imageName), _syncLoad); }


        //////////////////////////////////////////
        void setDefaultSpriteMaterial(MaterialPtr const& _defaultSpriteMaterial) { m_defaultSpriteMaterial = _defaultSpriteMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getDefaultSpriteMaterial() const { return m_defaultSpriteMaterial; }


        //////////////////////////////////////////
        void loadSpriteMetaData(SpritePtr const& _sprite, DataBlock const& _metaData);


        //////////////////////////////////////////
        SpriteLibraryData* addSpriteToLibrary(
            SpritePtr const& _sprite,
            SpriteLibraryDataCallbacks const& _callbacks = SpriteLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

        //////////////////////////////////////////
        void removeSpriteFromLibrary(HashedCString _spriteName);

        //////////////////////////////////////////
        inline void removeSpriteFromLibrary(CString _spriteName) { removeSpriteFromLibrary(HashedCString(_spriteName)); }

        //////////////////////////////////////////
        inline void removeSpriteFromLibrary(String const& _spriteName) { removeSpriteFromLibrary(HashedCString(_spriteName.c_str())); }


        //////////////////////////////////////////
        void unloadAssetSprites(Set<String> const& _tags);


        //////////////////////////////////////////
        HashedCString getSpriteName(Sprite const* _sprite);


        //////////////////////////////////////////
        inline SpritePtr const& getBuiltinSprite(BuiltinSpriteType _spriteType) const { return m_builtinSprites[(Size)_spriteType]; }

        //////////////////////////////////////////
        SpritePtr const& createBuiltinSprite(BuiltinSpriteType _spriteType);

        //////////////////////////////////////////
        SpritePtr const& ensureBuiltinSprite(BuiltinSpriteType _spriteType);

        //////////////////////////////////////////
        void createBuiltinSprites();

    public:
        //////////////////////////////////////////
        MultiDelegate<> eventBuiltinSpritesCreated;

    protected:

        //////////////////////////////////////////
        SpriteManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);

    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        StringKeyMap<SpriteLibraryData> m_spritesLibrary;
        SpritePtr m_builtinSprites[BuiltinSpriteType::MAX];

        MaterialPtr m_defaultSpriteMaterial;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSpriteManager_hpp_
//////////////////////////////////////////
