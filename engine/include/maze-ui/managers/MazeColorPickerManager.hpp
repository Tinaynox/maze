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
#if (!defined(_MazeColorPickerManager_hpp_))
#define _MazeColorPickerManager_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ColorPickerManager);
    

    //////////////////////////////////////////
    // Class ColorPickerManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API ColorPickerManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        using ColorPickerCallback = std::function<void(ColorU32 const& _color)>;
        using ColorHDRPickerCallback = std::function<void(ColorF128 const& _color)>;

    public:

        //////////////////////////////////////////
        virtual ~ColorPickerManager();

        //////////////////////////////////////////
        static void Initialize(ColorPickerManagerPtr& _manager);


        //////////////////////////////////////////
        static inline ColorPickerManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline ColorPickerManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        void openColorPicker(
            ColorPickerCallback _callback,
            ColorU32 const& _color = ColorU32::c_white);


        //////////////////////////////////////////
        void setColor(ColorU32 const& _color);

        //////////////////////////////////////////
        inline ColorU32 const& getColor() const { return m_color; }


        //////////////////////////////////////////
        void openColorHDRPicker(
            ColorHDRPickerCallback _callback,
            ColorF128 const& _color = ColorF128::c_white);


        //////////////////////////////////////////
        void setColorHDR(ColorF128 const& _color);

        //////////////////////////////////////////
        inline ColorF128 const& getColorHDR() const { return m_colorHDR; }


        //////////////////////////////////////////
        bool isColorPickerWindowOpened();

        //////////////////////////////////////////
        bool isColorPickerWindowFocused();

    public:

        //////////////////////////////////////////
        MultiDelegate<ColorU32 const&> eventColorChanged;
        MultiDelegate<ColorF128 const&> eventColorHDRChanged;

    protected:

        //////////////////////////////////////////
        ColorPickerManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void openColorPickerWindow(bool _hdr = false);

        //////////////////////////////////////////
        void closeColorPickerWindow();

        //////////////////////////////////////////
        void openColorPickerScene(bool _hdr);

        //////////////////////////////////////////
        void closeColorPickerScene();

        //////////////////////////////////////////
        void notifyWindowWillClose(Window* _window);

        //////////////////////////////////////////
        void notifyWindowFocusChanged(Window* _window);

    protected:
        static ColorPickerManager* s_instance;

        RenderWindowPtr m_colorPickerRenderWindow;

        ColorPickerCallback m_callback;
        ColorHDRPickerCallback m_callbackHDR;

        ColorU32 m_color;
        ColorF128 m_colorHDR;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeColorPickerManager_hpp_
//////////////////////////////////////////
