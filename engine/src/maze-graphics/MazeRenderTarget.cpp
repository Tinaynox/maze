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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/MazeRenderBufferPool.hpp"
#include "maze-graphics/MazeRenderMeshPool.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderTarget
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(RenderTarget);

    //////////////////////////////////////////
    RenderTarget::RenderTarget()
        : m_renderSystem(nullptr)
        , m_order(0)
        , m_clearColor(ColorU32::c_lightGray)
        , m_clearDepth(1.0f)
        , m_viewport(Rect2DF(0.0f, 0.0f, 1.0f, 1.0f))
        , m_viewPosition(Vec3F::c_zero)
        , m_viewMatrix(TMat::c_identity)
        , m_projectionMatrix(Mat4F::c_identity)
        , m_near(0.0f)
        , m_far(0.0f)
    {
    }
    

    //////////////////////////////////////////
    RenderTarget::~RenderTarget()
    {
        eventRenderTargetDestroyed(this);

        if (m_renderSystem)
        {
            m_renderSystem->processRenderTargetDestroyed(this);
        }

        m_renderMeshPool.reset();
        m_renderBufferPool.reset();
    }

    //////////////////////////////////////////
    bool RenderTarget::init(RenderSystem* _renderSystem)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!_renderSystem, false, "RenderTarget cannot be created without RenderSystem!");

        m_renderSystem = _renderSystem;

        m_renderBufferPool = std::make_shared<RenderBufferPool>(m_renderSystem);
        m_renderMeshPool = std::make_shared<RenderMeshPool>(m_renderSystem);

        return true;
    }

    //////////////////////////////////////////
    void RenderTarget::setOrder(S32 _order)
    {
        if (m_order == _order)
            return;

        m_order = _order;

        eventRenderTargetOrderChanged(this, m_order);
    }

    //////////////////////////////////////////
    bool RenderTarget::beginDraw()
    {
        return m_renderSystem->setCurrentRenderTarget(this);
    }

    //////////////////////////////////////////
    void RenderTarget::endDraw()
    {

    }

    //////////////////////////////////////////
    void RenderTarget::swapBuffers()
    {

    }

    //////////////////////////////////////////
    void RenderTarget::blit(MaterialPtr const& _material, Size _renderPassIndex)
    {
        if (beginDraw())
        {
            Rect2DF const viewport(0.0f, 0.0f, 1.0f, 1.0f);

            RenderTarget* renderTarget = this;
            RenderQueuePtr const& renderQueue = renderTarget->getRenderQueue();

            renderQueue->clear();
            renderQueue->addPushScissorRectCommand(viewport);

            bool const clearColorFlag = true;
            bool const clearDepthFlag = false;

            if (clearColorFlag)
                renderTarget->setClearColor(ColorU32::c_transparent);

            if (clearColorFlag || clearDepthFlag)
            {
                renderQueue->addClearCurrentRenderTargetCommand(
                    clearColorFlag,
                    clearDepthFlag);
            }

            renderTarget->setViewport(viewport);

            F32 viewportWidth = viewport.size.x * (F32)renderTarget->getRenderTargetWidth();
            F32 viewportHeight = viewport.size.y * (F32)renderTarget->getRenderTargetHeight();

            // Projection matrix
            renderTarget->setProjectionMatrix2D(
                0.0f,
                0.0f,
                viewportWidth,
                viewportHeight);
                    
            Vec3F canvasCameraPosition = Vec3F(
                (F32)renderTarget->getRenderTargetWidth() * 0.5f,
                (F32)renderTarget->getRenderTargetHeight() * 0.5f,
                0.0f);
                    
            // View matrix
            renderTarget->setViewMatrix(TMat::c_identity);

            // View position
            renderTarget->setViewPosition(canvasCameraPosition);

            RenderMeshPtr const& quad = m_renderSystem->getRenderMeshManager()->getDefaultQuadMesh();
            VertexArrayObject* vao = quad->getVertexArrayObjects().front().get();
            renderQueue->addSelectRenderPassCommand(_material->getRenderPass(RenderPassType::Default, _renderPassIndex));
            renderQueue->addDrawVAOInstancedCommand(
                vao, 
                TMat(
                    viewportWidth,        0.0f,                  0.0f,
                    0.0f,                 viewportHeight,        0.0f,
                    0.0f,                 0.0f,                  0.0f,
                    viewportWidth * 0.5f, viewportHeight * 0.5f, 0.0f));
            

            renderQueue->addPopScissorRectCommand();
            renderQueue->draw();

            endDraw();
        }
    }

    //////////////////////////////////////////
    void RenderTarget::setProjectionMatrixPerspective(
        F32 _fieldOfViewY,
        F32 _nearZ,
        F32 _farZ)
    {
        setNear(_nearZ);
        setFar(_farZ);

        Vec2U const& renderTargetSize = getRenderTargetSize();

        F32 aspectRatio = (m_viewport.size.x * (F32)renderTargetSize.x) / (m_viewport.size.y * (F32)renderTargetSize.y);

        setProjectionMatrix(
            Mat4F::CreateProjectionPerspectiveLHMatrix(
                _fieldOfViewY,
                aspectRatio,
                _nearZ,
                _farZ));
    }

    //////////////////////////////////////////
    void RenderTarget::setProjectionMatrix2D(
        F32 _x,
        F32 _y,
        F32 _w,
        F32 _h)
    {
        setNear(0.0f);
        setFar(1.0f);

        setProjectionMatrix(
            Mat4F::CreateProjection2DMatrix(_x, _y, _w, _h));
    }

    //////////////////////////////////////////
    void RenderTarget::clearDrawCalls()
    {
        m_renderQueue->clearDrawCalls();
    }

    //////////////////////////////////////////
    S32 RenderTarget::getDrawCalls()
    {
        return m_renderQueue->getDrawCalls();
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderTarget::createRenderBufferFromPool(RenderBufferSpecification const& _specification)
    {
        return m_renderBufferPool->createRenderBuffer(_specification);
    }

    //////////////////////////////////////////
    RenderMeshPtr RenderTarget::createRenderMeshFromPool(S32 _subMeshCount)
    {
        return m_renderMeshPool->createRenderMeshFromPool(_subMeshCount);
    }


} // namespace Maze
//////////////////////////////////////////
