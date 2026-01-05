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
#if (!defined(_MazeRenderTarget_hpp_))
#define _MazeRenderTarget_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeRenderBufferSpecification.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeManagedSharedPtr.hpp"
#include "maze-core/utils/MazeManagedSharedObject.hpp"
#include "maze-core/utils/MazeIndexedResource.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(RenderQueue);
    MAZE_USING_MANAGED_SHARED_PTR(RenderTarget);
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_MANAGED_SHARED_PTR(RenderBuffer);
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_UNIQUE_PTR(RenderBufferPool);
    MAZE_USING_UNIQUE_PTR(RenderMeshPool);


    //////////////////////////////////////////
    // Class RenderTarget
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderTarget 
        : public ManagedSharedObject<RenderTarget>
        , public IndexedResource<RenderTarget>
        , public MultiDelegateCallbackReceiver
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(RenderTarget);

    public:

        //////////////////////////////////////////
        virtual ~RenderTarget();

        //////////////////////////////////////////
        RenderTarget(RenderTarget const&) = delete;
        
        //////////////////////////////////////////
        RenderTarget(RenderTarget&&) = delete;
        
        //////////////////////////////////////////
        RenderTarget& operator=(RenderTarget const&) = delete;
        
        //////////////////////////////////////////
        RenderTarget& operator=(RenderTarget&&) = delete;


        //////////////////////////////////////////
        virtual Vec2U getRenderTargetSize() const MAZE_ABSTRACT;


        //////////////////////////////////////////
        inline U32 getRenderTargetWidth() const { return getRenderTargetSize().x; }

        //////////////////////////////////////////
        inline U32 getRenderTargetHeight() const { return getRenderTargetSize().y; }


        //////////////////////////////////////////
        inline void setName(String const& name) { m_name = name; }

        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }


        //////////////////////////////////////////
        void setOrder(S32 _order);

        //////////////////////////////////////////
        inline S32 getOrder() const { return m_order; }
        


        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }


        //////////////////////////////////////////
        inline ColorU32 const& getClearColor() const { return m_clearColor; }

        //////////////////////////////////////////
        inline void setClearColor(ColorU32 const& _clearColor) { m_clearColor = _clearColor; }

        //////////////////////////////////////////
        inline F32 getClearDepth() const { return m_clearDepth; }

        //////////////////////////////////////////
        inline void setClearDepth(F32 _clearDepth) { m_clearDepth = _clearDepth; }

        //////////////////////////////////////////
        virtual bool isReadyToRender() const { return true; }

        //////////////////////////////////////////
        virtual bool beginDraw();

        //////////////////////////////////////////
        virtual void endDraw();

        //////////////////////////////////////////
        virtual void swapBuffers();

        //////////////////////////////////////////
        void blit(MaterialPtr const& _material, Size _renderPassIndex = 0);



        //////////////////////////////////////////
        RenderQueuePtr const& getRenderQueue() const { return m_renderQueue; }


        //////////////////////////////////////////
        virtual bool processRenderTargetWillSet() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processRenderTargetSet() MAZE_ABSTRACT;
        
        //////////////////////////////////////////
        virtual void processRenderTargetWillReset() MAZE_ABSTRACT;
        

        //////////////////////////////////////////
        virtual void* getRenderContext() { return nullptr; }


        //////////////////////////////////////////
        inline Rect2F const& getViewport() const { return m_viewport; }

        //////////////////////////////////////////
        inline void setViewport(Rect2F const& _viewport) { m_viewport = _viewport; }

        //////////////////////////////////////////
        inline void setViewport(F32 _x, F32 _y, F32 _w, F32 _h) { setViewport(Rect2F(_x, _y, _w, _h)); }


        //////////////////////////////////////////
        inline Vec3F const& getViewPosition() const { return m_viewPosition; }

        //////////////////////////////////////////
        inline void setViewPosition(Vec3F const& _viewPosition) { m_viewPosition = _viewPosition; }

        //////////////////////////////////////////
        inline TMat const& getViewMatrix() const { return m_viewMatrix; }

        //////////////////////////////////////////
        inline void setViewMatrix(TMat const& _viewMatrix) { m_viewMatrix = _viewMatrix; }

        //////////////////////////////////////////
        inline Mat4F const& getProjectionMatrix() const { return m_projectionMatrix; }

        //////////////////////////////////////////
        inline void setProjectionMatrix(Mat4F const& _projectionMatrix) { m_projectionMatrix = _projectionMatrix; }

        //////////////////////////////////////////
        void setProjectionMatrixPerspective(
            F32 _fieldOfViewY,
            F32 _nearZ,
            F32 _farZ);

        //////////////////////////////////////////
        void setProjectionMatrix2D(
            F32 _x,
            F32 _y,
            F32 _w,
            F32 _h);

        //////////////////////////////////////////
        inline void setNear(F32 _value) { m_near = _value; }

        //////////////////////////////////////////
        inline F32 getNear() const { return m_near; }

        //////////////////////////////////////////
        inline void setFar(F32 _value) { m_far = _value; }

        //////////////////////////////////////////
        inline F32 getFar() const { return m_far; }


        //////////////////////////////////////////
        void clearDrawCalls();

        //////////////////////////////////////////
        S32 getDrawCalls();


        //////////////////////////////////////////
        RenderBufferPtr createRenderBufferFromPool(RenderBufferSpecification const& _specification);

        //////////////////////////////////////////
        RenderMeshPtr createRenderMeshFromPool(S32 _subMeshCount = 1);

    public:

        MultiDelegate<RenderTarget*> eventRenderTargetResized;
        MultiDelegate<RenderTarget*, S32> eventRenderTargetOrderChanged;
        MultiDelegate<RenderTarget*> eventRenderTargetDestroyed;

    protected:

        //////////////////////////////////////////
        RenderTarget();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem);
        

    protected:

        String m_name;
        RenderSystem* m_renderSystem;

        S32 m_order;

        ColorU32 m_clearColor;
        F32 m_clearDepth;

        RenderQueuePtr m_renderQueue;

        Rect2F m_viewport;
        Vec3F m_viewPosition;
        TMat m_viewMatrix;

        Mat4F m_projectionMatrix;
        F32 m_near;
        F32 m_far;

        RenderMeshPoolUPtr m_renderMeshPool;
        RenderBufferPoolUPtr m_renderBufferPool;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderTarget_hpp_
//////////////////////////////////////////
