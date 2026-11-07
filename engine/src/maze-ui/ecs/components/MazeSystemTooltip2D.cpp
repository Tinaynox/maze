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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeSystemTooltip2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeOverlayLayout2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static S32 const c_tooltipZ = 1000000;
    static F32 const c_tooltipGap = 3.0f;


    //////////////////////////////////////////
    // Class SystemTooltip2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SystemTooltip2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, text, String(), getText, setText),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, delay, 0.5f, getDelay, setDelay));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SystemTooltip2D);


    //////////////////////////////////////////
    SystemTooltip2D::SystemTooltip2D()
    {
    }

    //////////////////////////////////////////
    SystemTooltip2D::~SystemTooltip2D()
    {
        destroyView();
    }

    //////////////////////////////////////////
    SystemTooltip2DPtr SystemTooltip2D::Create()
    {
        SystemTooltip2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SystemTooltip2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SystemTooltip2D::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void SystemTooltip2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_element = getEntityRaw()->getComponent<UIElement2D>();
    }

    //////////////////////////////////////////
    void SystemTooltip2D::processComponentRemoved()
    {
        destroyView();
    }

    //////////////////////////////////////////
    void SystemTooltip2D::setText(String const& _text)
    {
        if (m_text == _text)
            return;

        m_text = _text;

        if (m_viewText)
            m_viewText->setText(m_text);
    }

    //////////////////////////////////////////
    void SystemTooltip2D::update(F32 _dt)
    {
        bool hovered = m_element && m_element->getFocused() && !m_element->getPressed() && !m_text.empty();
        if (!hovered)
        {
            m_hoverTime = 0.0f;
            hide();
            return;
        }

        m_hoverTime += _dt;
        if (m_hoverTime >= m_delay)
            show();
    }

    //////////////////////////////////////////
    void SystemTooltip2D::show()
    {
        if (!m_viewEntity && !createView())
            return;

        m_viewEntity->setActiveSelf(true);
        updateViewPosition();
    }

    //////////////////////////////////////////
    void SystemTooltip2D::hide()
    {
        if (m_viewEntity && m_viewEntity->getActiveSelf())
            m_viewEntity->setActiveSelf(false);
    }

    //////////////////////////////////////////
    bool SystemTooltip2D::createView()
    {
        Canvas* canvas = m_transform ? m_transform->getLastTrunkComponent<Canvas>() : nullptr;
        EcsScene* scene = getEntityRaw()->getEcsScene();
        if (!canvas || !scene)
            return false;

        Transform2DPtr canvasTransform = canvas->getEntityRaw()->getComponent<Transform2D>();

        SpriteRenderer2DPtr background = SpriteHelper::CreateSprite(
            SpritePtr(),
            Vec2F::c_zero,
            Vec2F::c_zero,
            MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
            canvasTransform,
            scene,
            Vec2F::c_zero,
            Vec2F(0.0f, 1.0f));
        background->setColor(ColorU32{ 35, 35, 40, 240 });

        m_viewEntity = background->getEntity();
        m_viewEntity->ensureComponent<Name>("Tooltip");
        m_viewTransform = background->getTransform();
        m_viewTransform->setZ(c_tooltipZ);

        if (RenderMask* renderMask = getEntityRaw()->getComponentRaw<RenderMask>())
            m_viewEntity->ensureComponent<RenderMask>()->setMask(renderMask->getMask());

        OverlayLayout2DPtr layout = m_viewEntity->ensureComponent<OverlayLayout2D>();
        layout->setPadding(4.0f, 4.0f, 2.0f, 2.0f);
        layout->setAutoWidth(true);
        layout->setAutoHeight(true);
        layout->setUseChildrenPreferredSize(true);

        m_viewText = SystemUIHelper::CreateSystemText(
            m_text.c_str(),
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2F::c_zero,
            Vec2F::c_zero,
            m_viewTransform,
            scene,
            Vec2F::c_zero,
            Vec2F::c_zero);
        m_viewText->setColor(ColorU32{ 235, 235, 235 });

        if (RenderMask* renderMask = getEntityRaw()->getComponentRaw<RenderMask>())
            m_viewText->getEntityRaw()->ensureComponent<RenderMask>()->setMask(renderMask->getMask());

        return true;
    }

    //////////////////////////////////////////
    // Below the element, flipped above it or shifted left if it doesn't fit into the canvas
    void SystemTooltip2D::updateViewPosition()
    {
        Transform2D* canvasTransform = m_viewTransform ? m_viewTransform->getParent().get() : nullptr;
        if (!canvasTransform || !m_transform)
            return;

        TMat toCanvas = canvasTransform->getWorldTransform().inversed().transform(m_transform->getWorldTransform());
        Vec2F elementBottomLeft = toCanvas.transform(Vec2F::c_zero);
        Vec2F elementTopLeft = toCanvas.transform(Vec2F(0.0f, m_transform->getHeight()));

        Vec2F const& viewSize = m_viewTransform->getSize();
        Vec2F const& canvasSize = canvasTransform->getSize();

        Vec2F position(elementBottomLeft.x, elementBottomLeft.y - c_tooltipGap);
        if (position.y - viewSize.y < 0.0f)
            position.y = elementTopLeft.y + c_tooltipGap + viewSize.y;

        position.x = Math::Clamp(position.x, 0.0f, Math::Max(0.0f, canvasSize.x - viewSize.x));

        m_viewTransform->setLocalPosition(Math::Round(position));
    }

    //////////////////////////////////////////
    void SystemTooltip2D::destroyView()
    {
        if (m_viewEntity && m_viewEntity->getEcsWorld())
            m_viewEntity->removeFromEcsWorld();

        m_viewEntity.reset();
        m_viewTransform.reset();
        m_viewText.reset();
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SystemTooltip2DUpdate,
        {},
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        SystemTooltip2D* _tooltip)
    {
        _tooltip->update(_event.getDt());
    }

} // namespace Maze
//////////////////////////////////////////
