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
#include "ExampleFPSCameraController.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(UpdateExampleFPSCameraControllerES,
        {},
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        ExampleFPSCameraController* _someObject)
    {
        _someObject->update(_event.getDt());
    }


    //////////////////////////////////////////
    // Class ExampleFPSCameraController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ExampleFPSCameraController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ExampleFPSCameraController);

    //////////////////////////////////////////
    ExampleFPSCameraController::ExampleFPSCameraController()
    {

    }

    //////////////////////////////////////////
    ExampleFPSCameraController::~ExampleFPSCameraController()
    {

    }

    //////////////////////////////////////////
    ExampleFPSCameraControllerPtr ExampleFPSCameraController::Create()
    {
        ExampleFPSCameraControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ExampleFPSCameraController, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ExampleFPSCameraController::init()
    {
            

        return true;
    }

    //////////////////////////////////////////
    void ExampleFPSCameraController::processEntityAwakened()
    {
        getEntityRaw()->ensureComponent<Name>()->setName("FPSController");
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();

        createElements();
    }

    //////////////////////////////////////////
    void ExampleFPSCameraController::update(F32 _dt)
    {
        if (!Example::GetInstancePtr() || !Example::GetInstancePtr()->isWindowFocused())
            return;

        Rect2DF viewportRect = m_camera3D->getViewport();
        viewportRect.position *= (Vec2F32)m_camera3D->getRenderTarget()->getRenderTargetSize();
        viewportRect.size *= (Vec2F32)m_camera3D->getRenderTarget()->getRenderTargetSize();

        AABB2D aabb = AABB2D::FromRect2D(viewportRect);
        Vec2F32 cursorPositionRWS = InputManager::GetInstancePtr()->getCursorPosition(0);

        if (m_camera3D && aabb.contains(cursorPositionRWS))
        {
            Vec3F32 cameraForwardDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3F32::c_unitZ;
            cameraForwardDirection.y = 0.0f;
            cameraForwardDirection.normalize();
            Vec3F32 cameraRightDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3F32::c_unitX;
            cameraRightDirection.y = 0.0f;
            cameraRightDirection.normalize();

            F32 speed = m_speed;

            if (InputManager::GetInstancePtr()->getKeyState(KeyCode::LShift) ||
                InputManager::GetInstancePtr()->getKeyState(KeyCode::RShift))
            {
                speed *= 3.0f;
            }

            if (m_forward)
            {
                m_targetPosition += cameraForwardDirection * _dt * speed;
            }
            else
            if (m_backward)
            {
                m_targetPosition += -cameraForwardDirection * _dt * speed;
            }

            if (m_right)
            {
                m_targetPosition += -cameraRightDirection * _dt * speed;
            }
            else
            if (m_left)
            {
                m_targetPosition += cameraRightDirection * _dt * speed;
            }

            Vec2F32 clampedSize = m_levelSize - Vec2F32(m_radius);
            m_targetPosition.x = Math::Clamp(m_targetPosition.x, -clampedSize.x * 0.5f, +clampedSize.x * 0.5f);
            m_targetPosition.z = Math::Clamp(m_targetPosition.z, -clampedSize.y * 0.5f, +clampedSize.y * 0.5f);


            getTransform()->setLocalPosition(
                Math::Lerp(
                    getTransform()->getLocalPosition(),
                    m_targetPosition,
                    _dt * 16.0f));

            Quaternion q = Quaternion::Slerp(
                36.0f * _dt,
                m_camera3D->getTransform()->getLocalRotation(),
                Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
            m_camera3D->getTransform()->setLocalRotation(q);
        }
    }

    //////////////////////////////////////////
    void ExampleFPSCameraController::createElements()
    {
        EntityPtr cameraEntity = getEntityRaw()->getEcsScene()->createEntity();
        Transform3DPtr cameraTransform =  cameraEntity->ensureComponent<Transform3D>();
        cameraTransform->setLocalY(1.5f);
        cameraTransform->setParent(m_transform);

        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->setClearColorFlag(false);
        m_camera3D->setClearSkyBoxFlag(true);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
        m_camera3D->getEntityRaw()->ensureComponent<Name>("Camera");
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(500.0f);

        m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
    }

    
    
} // namespace Maze
//////////////////////////////////////////
