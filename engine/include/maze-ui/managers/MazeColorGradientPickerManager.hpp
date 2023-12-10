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
#if (!defined(_MazeColorGradientPickerManager_hpp_))
#define _MazeColorGradientPickerManager_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ColorGradientPickerManager);
    


    //////////////////////////////////////////
    // Class ColorGradientPickerManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API ColorGradientPickerManager
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        using ColorPickerCallback = std::function<void(ColorGradient const& _gradient)>;

    public:

        //////////////////////////////////////////
        virtual ~ColorGradientPickerManager();

        //////////////////////////////////////////
        static void Initialize(ColorGradientPickerManagerPtr& _manager);


        //////////////////////////////////////////
        static inline ColorGradientPickerManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline ColorGradientPickerManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void openGradientPicker(
            ColorPickerCallback _callback = nullptr,
            ColorGradient const& _gradient = ColorGradient(0.0f, Vec4F::c_one, 1.0f, Vec4F::c_one));


        //////////////////////////////////////////
        void setGradient(ColorGradient const& _gradient);

        //////////////////////////////////////////
        inline ColorGradient const& getGradient() const { return m_gradient; }


        //////////////////////////////////////////
        bool isColorGradientPickerWindowOpened();

        //////////////////////////////////////////
        bool isColorGradientPickerWindowFocused();

    public:

        //////////////////////////////////////////
        MultiDelegate<ColorGradient const&> eventGradientChanged;

    protected:

        //////////////////////////////////////////
        ColorGradientPickerManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void openColorGradientPickerWindow();

        //////////////////////////////////////////
        void closeColorGradientPickerWindow();

        //////////////////////////////////////////
        void openColorGradientPickerScene();

        //////////////////////////////////////////
        void closeColorGradientPickerScene();

        //////////////////////////////////////////
        void notifyWindowWillClose(Window* _window);

        //////////////////////////////////////////
        void notifyWindowFocusChanged(Window* _window);

    protected:
        static ColorGradientPickerManager* s_instance;

        RenderWindowPtr m_colorPickerRenderWindow;

        ColorPickerCallback m_callback;

        ColorGradient m_gradient;

        bool m_checkClose;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeColorGradientPickerManager_hpp_
//////////////////////////////////////////
