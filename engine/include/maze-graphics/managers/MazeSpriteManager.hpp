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
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Struct SpriteLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API SpriteLibraryData
    {
        //////////////////////////////////////////
        SpriteLibraryData(
            SpritePtr const& _sprite = nullptr)
            : sprite(_sprite)
        {}

        SpritePtr sprite;
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
        SpritePtr const& getSprite(HashedCString _imageName);

        //////////////////////////////////////////
        inline SpritePtr const& getSprite(String const& _imageName) { return getSprite(MAZE_HASHED_CSTRING(_imageName.c_str())); }

        //////////////////////////////////////////
        inline SpritePtr const& getSprite(CString _imageName) { return getSprite(MAZE_HASHED_CSTRING(_imageName)); }


        //////////////////////////////////////////
        void setDefaultSpriteMaterial(MaterialPtr const& _defaultSpriteMaterial) { m_defaultSpriteMaterial = _defaultSpriteMaterial; }

        //////////////////////////////////////////
        MaterialPtr const& getDefaultSpriteMaterial() const { return m_defaultSpriteMaterial; }


        //////////////////////////////////////////
        SpriteLibraryData* addSpriteToLibrary(SpritePtr const& _sprite);

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

    protected:

        //////////////////////////////////////////
        SpriteManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);

    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        StringKeyMap<SpriteLibraryData> m_spritesLibrary;

        MaterialPtr m_defaultSpriteMaterial;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSpriteManager_hpp_
//////////////////////////////////////////
