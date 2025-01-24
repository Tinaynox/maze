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
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(CameraProjectionMode);


    //////////////////////////////////////////
    // Class Camera3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Camera3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Rect2F, viewport, Rect2F(0.0f, 0.0f, 1.0f, 1.0f), getViewport, setViewport),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CameraProjectionMode, projectionMode, CameraProjectionMode::Perspective, getProjectionMode, setProjectionMode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, fov, Maze::Math::DegreesToRadians(60), getFOV, setFOV),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, orthographicSize, 1.0f, getOrthographicSize, setOrthographicSize),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, nearZ, 0.01f, getNearZ, setNearZ),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, farZ, 200.0f, getFarZ, setFarZ),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, clearColorFlag, true, getClearColorFlag, setClearColorFlag),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, clearColor, ColorU32(99, 101, 140, 255), getClearColor, setClearColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, clearDepthFlag, true, getClearDepthFlag, setClearDepthFlag),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, clearSkyBoxFlag, false, getClearSkyBoxFlag, setClearSkyBoxFlag),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, drawFlag, true, getDrawFlag, setDrawFlag),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, clipViewport, false, getClipViewport, setClipViewport),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, renderMask, 0xFFFFFFFF, getRenderMask, setRenderMask),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, sortOrder, 0, getSortOrder, setSortOrder));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Camera3D);

    //////////////////////////////////////////
    Camera3D::Camera3D()
    {

    }

    //////////////////////////////////////////
    Camera3D::~Camera3D()
    {

    }

    //////////////////////////////////////////
    Camera3DPtr Camera3D::Create()
    {
        Camera3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Camera3D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Camera3D::init()
    {
            

        return true;
    }

    //////////////////////////////////////////
    void Camera3D::setProjectionMode(CameraProjectionMode _value)
    {
        if (m_projectionMode == _value)
            return;

        m_projectionMode = _value;
    }

    //////////////////////////////////////////
    void Camera3D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
    }

    //////////////////////////////////////////
    F32 Camera3D::getAspectRatio() const
    {
        if (!m_renderTarget)
            return 1.0f;

        F32 w = m_viewport.size.x * m_renderTarget->getRenderTargetWidth();
        F32 h = m_viewport.size.y * m_renderTarget->getRenderTargetHeight();

        return w / h;
    }

    //////////////////////////////////////////
    LightingSettingsPtr const& Camera3D::getLightingSettings()
    {
        static LightingSettingsPtr const nullPointer;

        EcsScene* cameraScene = getEntityRaw()->getEcsScene();
        if (cameraScene)
        {
            MAZE_DEBUG_ERROR_IF(!cameraScene->getMetaClass()->isInheritedFrom<EcsRenderScene>(), "It is not a render scene!");
            EcsRenderScene* renderScene = cameraScene->castRaw<EcsRenderScene>();

            LightingSettingsPtr const& lightingSettings = renderScene->getLightingSettings();
            return lightingSettings;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    Vec2F Camera3D::convertWorldCoordsToViewportCoords(Vec3F const& _positionWS) const
    {
        if (!m_renderTarget)
            return Vec2F::c_zero;

        TMat const& cameraTransform = getTransform()->getWorldTransform();
        Mat4F projectionMatrix = calculateProjectionMatrix(getRenderTarget());

        Vec4F positionVS = cameraTransform.inversed().transform(_positionWS);
        Vec4F positionCS = positionVS * projectionMatrix;

        Vec2F positionNDC = Vec2F(positionCS.x, positionCS.y) / positionCS.w;
        Vec2U renderTargetSize = m_renderTarget->getRenderTargetSize();
        Vec2F positionV = (positionNDC + 1.0f) * 0.5f * (Vec2F)renderTargetSize * getViewport().size;

        return positionV;
    }

    //////////////////////////////////////////
    Vec3F Camera3D::convertViewportCoordsToWorldCoords(Vec2F const& _positionV) const
    {
        Vec2U renderTargetSize = m_renderTarget->getRenderTargetSize();
        if (renderTargetSize.x == 0u || renderTargetSize.y == 0u ||
            getViewport().size.x == 0.0f || getViewport().size.y == 0.0f)
            return Vec3F::c_zero;

        TMat const& cameraTransform = getTransform()->getWorldTransform();
        Mat4F projectionMatrix = calculateProjectionMatrix(getRenderTarget());

        Vec2F p = _positionV / ((Vec2F)renderTargetSize * getViewport().size);
        Vec4F positionNDC = Vec4F((p * 2.0f - 1.0f), -1.0f, 0.0f);

        Vec4F positionCS = positionNDC;
        
        Vec4F positionVS = positionCS * projectionMatrix.inversed();
        positionVS.w = 1.0;
        Vec4F positionWS = cameraTransform.transform(positionVS);

        return positionWS.xyz();
    }

    //////////////////////////////////////////
    Ray Camera3D::convertViewportCoordsToRay(Vec2F const& _positionV) const
    {
        Vec2U renderTargetSize = m_renderTarget->getRenderTargetSize();
        if (renderTargetSize.x == 0u || renderTargetSize.y == 0u ||
            getViewport().size.x == 0.0f || getViewport().size.y == 0.0f)
            return Ray();

        TMat const& cameraTransform = getTransform()->getWorldTransform();
        Vec3F cameraPosition = cameraTransform.getTranslation();
        Mat4F projectionMatrix = calculateProjectionMatrix(getRenderTarget());

        Vec2F p = _positionV / ((Vec2F)renderTargetSize * getViewport().size);
        Vec4F positionNDC = Vec4F((p * 2.0f - 1.0f), -1.0f, 1.0f);
        Vec4F positionCS = positionNDC;

        Vec4F positionVS = positionCS * projectionMatrix.inversed();
        positionVS.w = 1.0;
        Vec4F positionWS = cameraTransform.transform(positionVS);

        Ray ray;
        ray.setDirection((positionWS.xyz() - cameraPosition).normalizedCopy());
        ray.setPoint(positionWS.xyz() - ray.getDirection());
        

        return ray;
    }

    //////////////////////////////////////////
    Mat4F Camera3D::calculateProjectionMatrix(RenderTargetPtr const& _renderTarget) const
    {
        Vec2U const& renderTargetSize = _renderTarget->getRenderTargetSize();

        F32 aspectRatio = (getViewport().size.x * (F32)renderTargetSize.x) / (getViewport().size.y * (F32)renderTargetSize.y);

        switch (m_projectionMode)
        {
            case CameraProjectionMode::Perspective:
            {
                Mat4F projectionMatrix = Mat4F::CreateProjectionPerspectiveLHMatrix(
                    getFOV(),
                    aspectRatio,
                    getNearZ(),
                    getFarZ());

                return projectionMatrix;
            }
            case CameraProjectionMode::Orthographic:
            {
                F32 top = m_orthographicSize;
                F32 bottom = -top;
                F32 right = m_orthographicSize * aspectRatio;
                F32 left = -right;

                F32 farZ = getFarZ();
                F32 nearZ = getNearZ();

                Mat4F orthographicMatrix = Mat4F::CreateProjectionOrthographicLHMatrix(
                    left,
                    right,
                    bottom,
                    top,
                    getNearZ(),
                    getFarZ());
                return orthographicMatrix;
            }
            default:
                Debug::LogError("Unsupported CameraProjectionMode - %s!", m_projectionMode.toCString());
                return Mat4F::c_identity;
        }
          
    }
    
    
} // namespace Maze
//////////////////////////////////////////
