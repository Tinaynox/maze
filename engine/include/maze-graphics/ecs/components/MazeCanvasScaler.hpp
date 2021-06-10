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
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(CanvasScaler);
    MAZE_USING_SHARED_PTR(Canvas);


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

        //////////////////////////////////////////
        enum class ScaleMode
        {
            None = 0,
            ConstantPixelSize,
            ScaleWithViewportSize
        };

        //////////////////////////////////////////
        enum class ScreenMatchMode
        {
            MatchWidthOrHeight = 0,
            Expand = 1,
            Shrink = 2,
        };

    public:

        //////////////////////////////////////////
        virtual ~CanvasScaler();

        //////////////////////////////////////////
        static CanvasScalerPtr Create();


        //////////////////////////////////////////
        void updateCanvasScale();


        //////////////////////////////////////////
        inline ScaleMode getScaleMode() const { return m_scaleMode; }

        //////////////////////////////////////////
        void setScaleMode(ScaleMode _scaleMode);


        //////////////////////////////////////////
        inline ScreenMatchMode getScreenMatchMode() const { return m_screenMatchMode; }

        //////////////////////////////////////////
        void setScreenMatchMode(ScreenMatchMode _screenMatchMode);


        //////////////////////////////////////////
        inline Vec2DF getReferenceResolution() const { return m_referenceResolution; }

        //////////////////////////////////////////
        void setReferenceResolution(Vec2DF _referenceResolution);


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
        ScaleMode m_scaleMode;
        ScreenMatchMode m_screenMatchMode;
        Vec2DF m_referenceResolution;
        F32 m_matchWidthOrHeight;

        CanvasPtr m_canvas;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_CPP_ENUMCLASS_SERIALIZATION(CanvasScaler::ScaleMode);
    MAZE_IMPLEMENT_CPP_ENUMCLASS_SERIALIZATION(CanvasScaler::ScreenMatchMode);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCanvasScaler_hpp_
//////////////////////////////////////////
