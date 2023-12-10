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
#if (!defined(_MazeAnimationCurveManager_hpp_))
#define _MazeAnimationCurveManager_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-ui/ecs/components/MazeAnimationCurveEdit2D.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AnimationCurveManager);
    

    //////////////////////////////////////////
    // Class AnimationCurveManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API AnimationCurveManager
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        using AnimationCurveCallback = std::function<void(AnimationCurve const& _curve)>;

    public:

        //////////////////////////////////////////
        virtual ~AnimationCurveManager();

        //////////////////////////////////////////
        static void Initialize(AnimationCurveManagerPtr& _manager);


        //////////////////////////////////////////
        static inline AnimationCurveManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline AnimationCurveManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void openCurveEditor(
            AnimationCurveCallback _callback = nullptr,
            AnimationCurve const& _curve = AnimationCurve(0.0f, 0.0f, 1.0f, 1.0f));


        //////////////////////////////////////////
        void setCurve(AnimationCurve const& _animationCurve);

        //////////////////////////////////////////
        inline AnimationCurve const& getCurve() const { return m_curve; }


        //////////////////////////////////////////
        bool isAnimationCurveEditorWindowOpened();

        //////////////////////////////////////////
        bool isAnimationCurveEditorWindowFocused();

    public:

        //////////////////////////////////////////
        MultiDelegate<AnimationCurve const&> eventCurveChanged;

    protected:

        //////////////////////////////////////////
        AnimationCurveManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void openAnimationCurveEditorWindow();

        //////////////////////////////////////////
        void closeAnimationCurveEditorWindow();

        //////////////////////////////////////////
        void openAnimationCurveEditorScene();

        //////////////////////////////////////////
        void closeAnimationCurveEditorScene();

        //////////////////////////////////////////
        void notifyWindowWillClose(Window* _window);

        //////////////////////////////////////////
        void notifyWindowFocusChanged(Window* _window);

    protected:
        static AnimationCurveManager* s_instance;

        RenderWindowPtr m_animationCurveEditorRenderWindow;

        AnimationCurveCallback m_callback;

        AnimationCurve m_curve;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAnimationCurveManager_hpp_
//////////////////////////////////////////
