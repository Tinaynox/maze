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
#if (!defined(_MazeViewport_hpp_))
#define _MazeViewport_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-core/math/MazeMat3.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Viewport);
    MAZE_USING_SHARED_PTR(RenderTarget);


    //////////////////////////////////////////
    // Class Viewport
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Viewport 
        : public SharedObject<Viewport>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~Viewport();

        //////////////////////////////////////////
        static ViewportPtr Create(RenderTargetPtr const& _renderTarget, Rect2F const& _relativeRect);


        //////////////////////////////////////////
        void setRenderTarget(RenderTargetPtr const& _renderTarget);

        //////////////////////////////////////////
        void setRelativeRect(Rect2F const& _relativeRect);

        //////////////////////////////////////////
        inline void setRelativeRect(F32 _x, F32 _y, F32 _width, F32 _height) { setRelativeRect(Rect2F(_x, _y, _width, _height)); }

        //////////////////////////////////////////
        inline Rect2F const& getRelativeRect() const { return m_relativeRect; }

        //////////////////////////////////////////
        inline Rect2U const& getAbsoluteRect() const { return m_absoluteRect; }

        //////////////////////////////////////////
        inline U32 getAbsoluteX() const { return getAbsoluteRect().position.x; }

        //////////////////////////////////////////
        inline U32 getAbsoluteY() const { return getAbsoluteRect().position.y; }

        //////////////////////////////////////////
        inline U32 getAbsoluteWidth() const { return getAbsoluteRect().size.x; }

        //////////////////////////////////////////
        inline U32 getAbsoluteHeight() const { return getAbsoluteRect().size.y; }

        //////////////////////////////////////////
        inline Vec2U const& getAbsoluteSize() const { return getAbsoluteRect().size; }

        //////////////////////////////////////////
        inline Mat3F const& getExternalTransform() const { return m_externalTransform; }

        //////////////////////////////////////////
        void setExternalTransform(Mat3F const& _matrix);

        //////////////////////////////////////////
        inline Mat3F const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        inline RenderTargetPtr getRenderTarget() const { return m_renderTarget.lock(); }


        
        //////////////////////////////////////////
        Vec2F convertRenderTargetCoordsToViewportCoords(Vec2F const& _renderTargetCoords);


        //////////////////////////////////////////
        bool setZ(S32 z);

        //////////////////////////////////////////
        inline S32 getZ() const { return m_z; }
        
        
    protected:

        //////////////////////////////////////////
        Viewport();

        //////////////////////////////////////////
        bool init(RenderTargetPtr const& _renderTarget, Rect2F const& _relativeRect);


        //////////////////////////////////////////
        void updateAbsoluteRect();

        //////////////////////////////////////////
        void notifyRenderTargetResized(RenderTarget* _renderTarget);



        //////////////////////////////////////////
        Viewport(Viewport const&) = delete;
        
        //////////////////////////////////////////
        Viewport(Viewport&&) = delete;
        
        //////////////////////////////////////////
        Viewport& operator=(Viewport const&) = delete;
        
        //////////////////////////////////////////
        Viewport& operator=(Viewport&&) = delete;

    public:

        MultiDelegate<ViewportPtr const&> eventViewportRectChanged;
        MultiDelegate<> eventZOrderChanged;

    protected:
        RenderTargetWPtr m_renderTarget;

        Rect2F m_relativeRect;
        Rect2U m_absoluteRect;
        Mat3F m_transform;
        Mat3F m_externalTransform;

        S32 m_z;
        U32 m_orderOfArrival;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeViewport_hpp_
//////////////////////////////////////////
