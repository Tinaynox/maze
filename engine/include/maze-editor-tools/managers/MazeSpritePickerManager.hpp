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
#if (!defined(_MazeSpritePickerManager_hpp_))
#define _MazeSpritePickerManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SpritePickerManager);
    

    //////////////////////////////////////////
    // Class SpritePickerManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API SpritePickerManager
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        using SpritePickerCallback = std::function<void(SpritePtr const& _sprite)>;

    public:

        //////////////////////////////////////////
        virtual ~SpritePickerManager();

        //////////////////////////////////////////
        static void Initialize(SpritePickerManagerPtr& _manager);


        //////////////////////////////////////////
        static inline SpritePickerManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline SpritePickerManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void openSpritePicker(
            SpritePickerCallback _callback,
            SpritePtr const& _texture = SpritePtr());


        //////////////////////////////////////////
        void setSprite(SpritePtr const& _sprite);

        //////////////////////////////////////////
        inline SpritePtr const& getSprite() const { return m_sprite; }



        //////////////////////////////////////////
        bool isSpritePickerEditorWindowOpened();

        //////////////////////////////////////////
        bool isSpritePickerEditorWindowFocused();

    public:

        //////////////////////////////////////////
        MultiDelegate<SpritePtr const&> eventSpriteChanged;

    protected:

        //////////////////////////////////////////
        SpritePickerManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void openSpritePickerEditorWindow();

        //////////////////////////////////////////
        void closeSpritePickerEditorWindow();

        //////////////////////////////////////////
        void openSpritePickerEditorScene();

        //////////////////////////////////////////
        void closeSpritePickerEditorScene();

        //////////////////////////////////////////
        void notifyWindowWillClose(Window* _window);

        //////////////////////////////////////////
        void notifyWindowFocusChanged(Window* _window);

    protected:
        static SpritePickerManager* s_instance;

        RenderWindowPtr m_texturePickerRenderWindow;

        SpritePickerCallback m_callback;

        SpritePtr m_sprite;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSpritePickerManager_hpp_
//////////////////////////////////////////
