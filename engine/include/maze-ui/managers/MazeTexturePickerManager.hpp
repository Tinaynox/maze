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
#if (!defined(_MazeTexturePickerManager_hpp_))
#define _MazeTexturePickerManager_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-graphics/MazeTexture.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TexturePickerManager);
    

    //////////////////////////////////////////
    // Class TexturePickerManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API TexturePickerManager
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        using TexturePickerCallback = std::function<void(Texture2DPtr const& _texture)>;

    public:

        //////////////////////////////////////////
        virtual ~TexturePickerManager();

        //////////////////////////////////////////
        static void Initialize(TexturePickerManagerPtr& _manager);


        //////////////////////////////////////////
        static inline TexturePickerManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline TexturePickerManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void openTexturePicker(
            TexturePickerCallback _callback,
            Texture2DPtr const& _texture = Texture2DPtr());


        //////////////////////////////////////////
        void setTexture(Texture2DPtr const& _material);

        //////////////////////////////////////////
        inline Texture2DPtr const& getTexture() const { return m_texture; }



        //////////////////////////////////////////
        bool isTexturePickerEditorWindowOpened();

        //////////////////////////////////////////
        bool isTexturePickerEditorWindowFocused();

    public:

        //////////////////////////////////////////
        MultiDelegate<Texture2DPtr const&> eventTextureChanged;

    protected:

        //////////////////////////////////////////
        TexturePickerManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void openTexturePickerEditorWindow();

        //////////////////////////////////////////
        void closeTexturePickerEditorWindow();

        //////////////////////////////////////////
        void openTexturePickerEditorScene();

        //////////////////////////////////////////
        void closeTexturePickerEditorScene();

        //////////////////////////////////////////
        void notifyWindowWillClose(Window* _window);

        //////////////////////////////////////////
        void notifyWindowFocusChanged(Window* _window);

    protected:
        static TexturePickerManager* s_instance;

        RenderWindowPtr m_texturePickerRenderWindow;

        TexturePickerCallback m_callback;

        Texture2DPtr m_texture;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexturePickerManager_hpp_
//////////////////////////////////////////
