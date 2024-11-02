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
#if (!defined(_MazeCanvas_hpp_))
#define _MazeCanvas_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(CanvasScaler);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2_API(MAZE_GRAPHICS_API, ViewportTransformPolicy,
        ViewportToTransform,
        TransformToViewport);


    //////////////////////////////////////////
    // Class Canvas
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Canvas
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Canvas, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Canvas);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Canvas();

        //////////////////////////////////////////
        static CanvasPtr Create();


        //////////////////////////////////////////
        void prepareFrame(F32 _dt);


        //////////////////////////////////////////
        void setRenderTarget(RenderTargetPtr const& _renderTarget);

        //////////////////////////////////////////
        inline RenderTargetPtr const& getRenderTarget() const { return m_renderTarget; }


        //////////////////////////////////////////
        void setViewport(Rect2F const& _viewport);

        //////////////////////////////////////////
        inline void setViewport(F32 _x, F32 _y, F32 _w, F32 _h) { setViewport(Rect2F(_x, _y, _w, _h)); }

        //////////////////////////////////////////
        inline Rect2F const& getViewport() const { return m_viewport; }
    
        //////////////////////////////////////////
        Rect2F getRenderTargetRect() const;

        //////////////////////////////////////////
        AABB2D getRenderTargetAABB() const;


        //////////////////////////////////////////
        inline bool getClearColorFlag() const { return m_clearColorFlag; }

        //////////////////////////////////////////
        inline void setClearColorFlag(bool _clearColorFlag) { m_clearColorFlag = _clearColorFlag; }

        //////////////////////////////////////////
        inline ColorU32 const& getClearColor() const { return m_clearColor; }

        //////////////////////////////////////////
        inline void setClearColor(ColorU32 const& _clearColor) { m_clearColor = _clearColor; }

        //////////////////////////////////////////
        inline void setClearColor(U8 _r, U8 _g, U8 _b) { setClearColor(ColorU32{ _r, _g, _b }); }


        //////////////////////////////////////////
        inline bool getClearDepthFlag() const { return m_clearDepthFlag; }

        //////////////////////////////////////////
        inline void setClearDepthFlag(bool _clearDepthFlag) { m_clearDepthFlag = _clearDepthFlag; }


        //////////////////////////////////////////
        inline bool getClipViewport() const { return m_clipViewport; }

        //////////////////////////////////////////
        inline void setClipViewport(bool _value) { m_clipViewport = _value; }


        //////////////////////////////////////////
        inline S32 getSortOrder() const { return m_sortOrder; }

        //////////////////////////////////////////
        inline void setSortOrder(S32 _sortOrder) { m_sortOrder = _sortOrder; }


        //////////////////////////////////////////
        inline ViewportTransformPolicy getViewportTransformPolicy() const { return m_viewportTransformPolicy; }

        //////////////////////////////////////////
        inline void setViewportTransformPolicy(ViewportTransformPolicy _viewportTransformPolicy) { m_viewportTransformPolicy = _viewportTransformPolicy; }


        //////////////////////////////////////////
        inline Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline CanvasScalerPtr getCanvasScaler() const { return m_canvasScaler.lock(); }

        //////////////////////////////////////////
        inline Vec2F convertRenderTargetCoordsToViewportCoords(Vec2F const& _renderTargetPosition) const
        {
            return _renderTargetPosition - m_originPosition;
        }

        //////////////////////////////////////////
        inline Vec2F convertViewportCoordsToRenderTargetCoords(Vec2F const& _viewportPosition) const
        {
            return _viewportPosition + m_originPosition;
        }

    public:

        //////////////////////////////////////////
        MultiDelegate<Canvas*, RenderTargetPtr const&> eventRenderTargetChanged;

    protected:

        //////////////////////////////////////////
        Canvas();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void notifyRenderTargetResized(RenderTarget* _renderTarget);


        //////////////////////////////////////////
        void updateCanvasTransform();


        //////////////////////////////////////////
        void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        Transform2DPtr m_transform;
        CanvasScalerWPtr m_canvasScaler;

        RenderTargetPtr m_renderTarget;
        Rect2F m_viewport;

        bool m_clearColorFlag;
        ColorU32 m_clearColor;

        bool m_clearDepthFlag;

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
        bool m_clipViewport = true;
#else
        bool m_clipViewport = false;
#endif

        S32 m_sortOrder;

        Vec2F m_originPosition;

        ViewportTransformPolicy m_viewportTransformPolicy;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCanvas_hpp_
//////////////////////////////////////////
