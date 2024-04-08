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
#include "maze-graphics/MazeViewport.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Viewport
    //
    //////////////////////////////////////////
    Viewport::Viewport()
        : m_transform(Mat3F::c_identity)
        , m_externalTransform(Mat3F::c_identity)
        , m_z(0)
        , m_orderOfArrival(0)
    {
    }
    

    //////////////////////////////////////////
    Viewport::~Viewport()
    {
        setRenderTarget(RenderTargetPtr());
    }

    //////////////////////////////////////////
    ViewportPtr Viewport::Create(RenderTargetPtr const& renderTarget, Rect2DF const& relativeRect)
    {
        ViewportPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Viewport, object, init(renderTarget, relativeRect));
        return object;
    }

    //////////////////////////////////////////
    bool Viewport::init(RenderTargetPtr const& renderTarget, Rect2DF const& relativeRect)
    {
        setRenderTarget(renderTarget);
        setRelativeRect(relativeRect);

        return true;
    }

    //////////////////////////////////////////
    void Viewport::setRenderTarget(RenderTargetPtr const& renderTarget)
    {
        RenderTargetPtr prevRenderTarget = m_renderTarget.lock();

        if (prevRenderTarget == renderTarget)
            return;

        if (prevRenderTarget)
        {
            prevRenderTarget->eventRenderTargetResized.unsubscribe(this);
        }

        m_renderTarget = renderTarget;

        if (renderTarget)
        {
            renderTarget->eventRenderTargetResized.subscribe(this, &Viewport::notifyRenderTargetResized);
            updateAbsoluteRect();
        }
    }

    //////////////////////////////////////////
    void Viewport::setRelativeRect(Rect2DF const& relativeRect)
    {
        m_relativeRect = relativeRect;
        updateAbsoluteRect();
    }

    //////////////////////////////////////////
    void Viewport::setExternalTransform(Mat3F const& matrix)
    {
        m_externalTransform = matrix;
        updateAbsoluteRect();
    }

    //////////////////////////////////////////
    Vec2F Viewport::convertRenderTargetCoordsToViewportCoords(Vec2F const& renderTargetCoords)
    {
        Vec2F result = getTransform().inverseAffine().transformAffine(Vec2F(renderTargetCoords.x, renderTargetCoords.y));
        return Vec2F(result.x, result.y);
    }


    //////////////////////////////////////////
    bool Viewport::setZ(S32 z)
    {
        if (m_z == z)
            return false;

        m_z = z;

        eventZOrderChanged();

        return true;
    }

    //////////////////////////////////////////
    void Viewport::updateAbsoluteRect()
    {
        RenderTargetPtr renderTarget = m_renderTarget.lock();
        if (!renderTarget)
            return;

        const Vec2U& renderTargetSize = renderTarget->getRenderTargetSize();

        m_absoluteRect.position = Vec2U(
            (U32)Math::Round(m_relativeRect.position.x * renderTargetSize.x),
            (U32)Math::Round(m_relativeRect.position.y * renderTargetSize.y));

        m_absoluteRect.size = Vec2U(
            (U32)Math::Round(m_relativeRect.size.x * renderTargetSize.x),
            (U32)Math::Round(m_relativeRect.size.y * renderTargetSize.y));

        m_transform = Mat3F::CreateTranslationMatrix((Vec2F)m_absoluteRect.position);
        m_transform = m_transform.concatenatedAffineCopy(m_externalTransform);

        eventViewportRectChanged(getSharedPtr());
    }

    //////////////////////////////////////////
    void Viewport::notifyRenderTargetResized(RenderTarget* renderTarget)
    {
        updateAbsoluteRect();
    }


} // namespace Maze
//////////////////////////////////////////
