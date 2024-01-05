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
#if (!defined(_MazeCanvasScaler_hpp_))
#define _MazeCanvasScaler_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(CanvasScaler);
    MAZE_USING_SHARED_PTR(Canvas);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2_API(MAZE_GRAPHICS_API, CanvasScalerScaleMode,
        ConstantPixelSize,
        ScaleWithViewportSize)

    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_GRAPHICS_API, CanvasScalerScreenMatchMode,
        MatchWidthOrHeight,
        Expand,
        Shrink)


    //////////////////////////////////////////
    // Class CanvasScaler
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API CanvasScaler
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(CanvasScaler, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(CanvasScaler);

    public:

        //////////////////////////////////////////
        virtual ~CanvasScaler();

        //////////////////////////////////////////
        static CanvasScalerPtr Create();


        //////////////////////////////////////////
        void updateCanvasScale();


        //////////////////////////////////////////
        inline CanvasScalerScaleMode getScaleMode() const { return m_scaleMode; }

        //////////////////////////////////////////
        void setScaleMode(CanvasScalerScaleMode _scaleMode);


        //////////////////////////////////////////
        inline CanvasScalerScreenMatchMode getScreenMatchMode() const { return m_screenMatchMode; }

        //////////////////////////////////////////
        void setScreenMatchMode(CanvasScalerScreenMatchMode _screenMatchMode);


        //////////////////////////////////////////
        inline Vec2F getReferenceResolution() const { return m_referenceResolution; }

        //////////////////////////////////////////
        void setReferenceResolution(Vec2F _referenceResolution);


        //////////////////////////////////////////
        inline F32 getMatchWidthOrHeight() const { return m_matchWidthOrHeight; }

        //////////////////////////////////////////
        void setMatchWidthOrHeight(F32 _matchWidthOrHeight);

    protected:

        //////////////////////////////////////////
        CanvasScaler();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void processEntityAwakened() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void setCanvasScale(F32 _scale);

        //////////////////////////////////////////
        void handleConstantPixelSize();

        //////////////////////////////////////////
        void handleScaleWithViewportSize();

    protected:
        CanvasScalerScaleMode m_scaleMode;
        CanvasScalerScreenMatchMode m_screenMatchMode;
        Vec2F m_referenceResolution;
        F32 m_matchWidthOrHeight;

        CanvasPtr m_canvas;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCanvasScaler_hpp_
//////////////////////////////////////////
