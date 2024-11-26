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
#include "MazeParticlesEditorToolsHeader.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemParameterColor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDropdown2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemParameterColor
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterColor, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerParticleSystemParameterColor);

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterColor::PropertyDrawerParticleSystemParameterColor()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterColor::~PropertyDrawerParticleSystemParameterColor()
    {
        if (m_colorFrom)
        {
            m_colorFrom->eventColorChanged.unsubscribe(this);
        }

        if (m_colorTo)
        {
            m_colorTo->eventColorChanged.unsubscribe(this);
        }

        if (m_gradientFrom)
            m_gradientFrom->eventGradientChanged.unsubscribe(this);

        if (m_gradientTo)
            m_gradientTo->eventGradientChanged.unsubscribe(this);

        if (m_modeDropdown)
        {
            m_modeDropdown->eventValueChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterColorPtr PropertyDrawerParticleSystemParameterColor::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerParticleSystemParameterColorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerParticleSystemParameterColor, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerParticleSystemParameterColor::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterColor::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setAutoWidth(false);
        layout->setExpand(true);
        m_rootEntity = layout->getEntity();

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            EditorToolsHelper::BuildPropertyName(m_dataBlock.getCString(MAZE_HCS("label")), _label).c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(8, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        titleText->setColor(EditorToolsStyles::GetInstancePtr()->getInspectorPropertyColor());

        m_layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_layout->setAutoWidth(true);
        m_layout->setAutoHeight(false);
        m_layout->setExpand(false);

        {
            m_colorFrom = EditorToolsUIHelper::CreateDefaultColorHDREdit(
                ColorF128::c_white,
                EditorToolsStyles::GetInstancePtr()->getDefaultBoldFontMaterial(),
                Vec2F(60, 18),
                Vec2F(0, 0),
                m_layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_colorFrom->eventColorChanged.subscribe(this, &PropertyDrawerParticleSystemParameterColor::notifyColorChanged);

            m_colorTo = EditorToolsUIHelper::CreateDefaultColorHDREdit(
                ColorF128::c_white,
                EditorToolsStyles::GetInstancePtr()->getDefaultBoldFontMaterial(),
                Vec2F(60, 18),
                Vec2F(0, 0),
                m_layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_colorTo->eventColorChanged.subscribe(this, &PropertyDrawerParticleSystemParameterColor::notifyColorChanged);
        }

        {
            m_gradientFrom = EditorToolsUIHelper::CreateDefaultColorGradientEdit(
                ColorGradient(),
                Vec2F(60, 18),
                Vec2F(0, 0),
                m_layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_gradientFrom->eventGradientChanged.subscribe(this, &PropertyDrawerParticleSystemParameterColor::notifyGradientChanged);

            m_gradientTo = EditorToolsUIHelper::CreateDefaultColorGradientEdit(
                ColorGradient(),
                Vec2F(60, 18),
                Vec2F(0, 0),
                m_layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_gradientTo->eventGradientChanged.subscribe(this, &PropertyDrawerParticleSystemParameterColor::notifyGradientChanged);
        }

        createModeDropdown();

        updateModeUI();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterColor::notifyColorChanged(ColorHDREdit2D* _edit, ColorF128 const& _color)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterColor::notifyGradientChanged(ColorGradientEdit2D* _edit, ColorGradient const& _gradient)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterColor::setValue(ParticleSystemParameterColor const& _value)
    {
        m_parameter = _value;
        setMode(_value.getMode());

        switch (_value.getMode())
        {
            case ParticleSystemParameterColorSamplingMode::Color:
            {
                if (!m_colorTo->getSelected())
                    m_colorTo->setColor(ColorF128(_value.getColor0()));
                break;
            }
            case ParticleSystemParameterColorSamplingMode::Gradient:
            {
                if (!m_gradientTo->getSelected())
                    m_gradientTo->setGradient(_value.getGradient0());

                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenColors:
            {
                if (!m_colorFrom->getSelected())
                    m_colorFrom->setColor(ColorF128(_value.getColor0()));

                if (!m_colorTo->getSelected())
                    m_colorTo->setColor(ColorF128(_value.getColor1()));
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenGradients:
            {
                if (!m_gradientFrom->getSelected())
                    m_gradientFrom->setGradient(_value.getGradient0());

                if (!m_gradientTo->getSelected())
                    m_gradientTo->setGradient(_value.getGradient1());

                break;
            }
            default:
            {
                break;
            }
        }

        if (!m_modeDropdown->getSelected())
            m_modeDropdown->setValue(_value.getMode().toString());
    }

    //////////////////////////////////////////
    ParticleSystemParameterColor PropertyDrawerParticleSystemParameterColor::getValue() const
    {
        ParticleSystemParameterColor parameter = m_parameter;

        switch (m_mode)
        {
            case ParticleSystemParameterColorSamplingMode::Color:
            {
                ColorF128 color0 = m_colorTo->getColor();
                parameter.setColor((Vec4F)color0);
                break;
            }
            case ParticleSystemParameterColorSamplingMode::Gradient:
            {
                ColorGradient gradient0 = m_gradientTo->getGradient();

                if (gradient0.empty())
                {
                    gradient0.addKey(0.0f, Vec4F::c_one);
                    gradient0.addKey(1.0f, Vec4F::c_one);
                }
                
                parameter.setGradient(gradient0);
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenColors:
            {
                ColorF128 color0 = m_colorFrom->getColor();
                ColorF128 color1 = m_colorTo->getColor();
                parameter.setRandomBetweenColors((Vec4F)color0, (Vec4F)color1);
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenGradients:
            {
                ColorGradient gradient0 = m_gradientFrom->getGradient();
                ColorGradient gradient1 = m_gradientTo->getGradient();

                if (gradient0.empty())
                {
                    gradient0.addKey(0.0f, Vec4F::c_one);
                    gradient0.addKey(1.0f, Vec4F::c_one);
                }
                if (gradient1.empty())
                {
                    gradient1.addKey(0.0f, Vec4F::c_one);
                    gradient1.addKey(1.0f, Vec4F::c_one);
                }

                parameter.setRandomBetweenGradients(gradient0, gradient1);
                break;
            }
            default:
            {
                break;
            }
        }

        return parameter;
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterColor::setMode(ParticleSystemParameterColorSamplingMode _mode)
    {
        if (m_mode == _mode)
            return;

        m_mode = _mode;

        updateModeUI();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterColor::updateModeUI()
    {
        bool colorFromActive = false;
        bool colorToActive = false;
        bool gradientFromActive = false;
        bool gradientToActive = false;

        switch (m_mode)
        {
            case ParticleSystemParameterColorSamplingMode::Color:
            {
                colorToActive = true;
                break;
            }
            case ParticleSystemParameterColorSamplingMode::Gradient:
            {
                gradientToActive = true;
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenColors:
            {
                colorFromActive = true;
                colorToActive = true;
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenGradients:
            {
                gradientFromActive = true;
                gradientToActive = true;
                break;
            }
            default:
            {
                break;
            }
        }

        m_colorFrom->getEntityRaw()->setActiveSelf(colorFromActive);
        m_colorTo->getEntityRaw()->setActiveSelf(colorToActive);
        m_gradientFrom->getEntityRaw()->setActiveSelf(gradientFromActive);
        m_gradientTo->getEntityRaw()->setActiveSelf(gradientToActive);
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterColor::createModeDropdown()
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        EntityPtr dropdownEntity = m_layout->getEntityRaw()->getEcsScene()->createEntity();
        dropdownEntity->ensureComponent<StaticName>("Dropdown");

        m_modeDropdown = dropdownEntity->createComponent<Dropdown2D>();

        Transform2DPtr const& transform = m_modeDropdown->getTransform();
        transform->setParent(m_layout->getTransform());
        transform->setSize(18.0f, 18.0f);
        
        SpriteRenderer2DPtr spriteRenderer = dropdownEntity->createComponent<SpriteRenderer2D>();
        SpriteRenderer2D* spriteRendererRaw = spriteRenderer.get();
        spriteRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
        spriteRenderer->setMaterialCopy(
            renderSystem->getMaterialManager()->getSpriteMaterial());
        spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
        spriteRenderer->setColor(ColorU32(250, 250, 250, 100));

        auto updateDropdownState =
            [](
                Dropdown2D* _dropdown,
                SpriteRenderer2D* _spriteRenderer,
                Entity* _listEntity)
        {
            _listEntity->setActiveSelf(_dropdown->getSelected());

            Transform2DPtr transform = _listEntity->getComponent<Transform2D>();
            Canvas* rootCanvas = transform->getLastTrunkComponent<Canvas>();
            if (rootCanvas)
            {
                Transform2D* root = rootCanvas->getTransform().get();
                Vec2F const& rootSize = root->getSize();

                
                Vec2F positionOS(
                    transform->getWidth(),
                    transform->getHeight());

                Vec2F positionWS = transform->getWorldTransform().transform(positionOS);
                positionWS.y = Math::Clamp(positionWS.y, transform->getHeight(), rootSize.y);

                Vec2F menuListPositionOS = transform->getParent()->getWorldTransform().inversed().transform(positionWS);

                transform->setPivot(1.0f, 1.0f);
                transform->setAnchor(0.0f, 0.0f);
                transform->setLocalPosition(menuListPositionOS);
            }

            if (_dropdown->getSelected())
            {
                _spriteRenderer->setSprite(
                    UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Selected));
            }
            else
            {
                if (_dropdown->getFocused())
                {
                    _spriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused));
                }
                else
                {
                    _spriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
                }
            }
        };

        SpriteRenderer2DPtr expandButtonSprite = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
            Vec2F(8.0f, 8.0f) * 1.75f,
            Vec2F(0.0f, 0.0f),
            renderSystem->getMaterialManager()->getSpriteMaterial(),
            transform,
            spriteRenderer->getEntityRaw()->getEcsScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F(0.5f, 0.5f));
        expandButtonSprite->setColor(ColorU32::c_black);

        F32 width = 200.0f;

        // List entity
        Entity* listEntity = nullptr;
        {

            SpriteRenderer2DPtr listTemplateSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                Vec2F(width, 140.0f),
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getSpriteMaterial(),
                transform,
                m_layout->getEntityRaw()->getEcsScene(),
                Vec2F(1.0f, 0.0f),
                Vec2F(1.0f, 1.0f));
            listTemplateSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            listEntity = listTemplateSpriteRenderer->getEntityRaw();

            CanvasPtr canvas = listEntity->ensureComponent<Canvas>();

            canvas->setClearColor(ColorU32::c_red);
            canvas->setClearColorFlag(false);
            canvas->setSortOrder(300000);
            canvas->getCanvasScaler()->setScaleMode(CanvasScalerScaleMode::None);
            canvas->setViewportTransformPolicy(ViewportTransformPolicy::TransformToViewport);


            UIElement2DPtr uiElement = listEntity->ensureComponent<UIElement2D>();
            uiElement->setCaptureCursorHits(true);

            m_modeDropdown->setListCanvas(canvas);

            // Item Prefab
            {
                Transform2DPtr itemPrefabTransform = SpriteHelper::CreateTransform2D(
                    Vec2F(width - 2.0f, 20.0f),
                    Vec2F(1.0f, 0.0f),
                    transform,
                    m_layout->getEntityRaw()->getEcsScene(),
                    Vec2F(0.0f, 1.0f),
                    Vec2F(0.0f, 1.0f));

                itemPrefabTransform->getEntityRaw()->setActiveSelf(false);

                SizePolicy2DPtr itemPrefabSizePolicy = itemPrefabTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
                itemPrefabSizePolicy->setFlag(SizePolicy2D::Height, false);
                itemPrefabSizePolicy->setSizeDelta(-4.0f, 0.0f);

                SpriteRenderer2DPtr backgroundSpriteRenderer = SpriteHelper::CreateSprite(
                    ColorU32::c_white,
                    Vec2F(width, 20.0f - 4.0f),
                    Vec2F(0.0f, 2.0f),
                    renderSystem->getMaterialManager()->getSpriteMaterial(),
                    itemPrefabTransform,
                    m_layout->getEntityRaw()->getEcsScene());
                backgroundSpriteRenderer->getEntityRaw()->ensureComponent<StaticName>()->setStaticName("Background");
                backgroundSpriteRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();

                SpriteRenderer2DPtr checkMarkSprite = SpriteHelper::CreateSprite(
                    UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::CheckMark),
                    Vec2F(8.0f, 8.0f) * 1.75f,
                    Vec2F(10.0f, 10.0f),
                    renderSystem->getMaterialManager()->getSpriteMaterial(),
                    itemPrefabTransform,
                    spriteRenderer->getEntityRaw()->getEcsScene(),
                    Vec2F(0.0f, 0.0f),
                    Vec2F(0.5f, 0.5f));
                checkMarkSprite->getEntityRaw()->ensureComponent<StaticName>()->setStaticName("CheckMark");
                checkMarkSprite->setColor(ColorU32::c_black);

                AbstractTextRenderer2DPtr itemTextRenderer = UIHelper::CreateText(
                    "Option 1",
                    12,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F(width, 20.0f),
                    Vec2F(20.0f, 0.0f),
                    itemPrefabTransform,
                    m_layout->getEntityRaw()->getEcsScene(),
                    Vec2F::c_zero,
                    Vec2F::c_zero);
                itemTextRenderer->getEntityRaw()->ensureComponent<StaticName>()->setStaticName("Label");
                itemTextRenderer->setColor(ColorU32::c_black);
                ScissorMask2DPtr scissorMask = itemTextRenderer->getEntityRaw()->createComponent<ScissorMask2D>();
                SizePolicy2DPtr itemTextRendererSizePolicy = itemTextRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
                itemTextRendererSizePolicy->setSizeDelta(-20.0f, -2.0f);

                ClickButton2DPtr button = itemPrefabTransform->getEntityRaw()->createComponent<ClickButton2D>();
                button->getUIElement()->setCaptureCursorHits(false);



                m_modeDropdown->setItemPrefabTransform(itemPrefabTransform);

            }
        }

        m_modeDropdown->eventFocusChanged.subscribe(
            [=](Dropdown2D* _dropdown, bool _value)
            {
                updateDropdownState(_dropdown, spriteRendererRaw, listEntity);
            });

        m_modeDropdown->eventSelectedChanged.subscribe(
            [=](Dropdown2D* _dropdown, bool _value)
            {
                updateDropdownState(_dropdown, spriteRendererRaw, listEntity);
            });

        updateDropdownState(m_modeDropdown.get(), spriteRendererRaw, listEntity);


        for (ParticleSystemParameterColorSamplingMode mode : ParticleSystemParameterColorSamplingMode::All())
        {
            if (mode == ParticleSystemParameterColorSamplingMode::None)
                continue;

            m_modeDropdown->addOption(mode.toString(), false, false);
        }

        m_modeDropdown->updateCaption();
        m_modeDropdown->rebuildOptions();

        m_modeDropdown->eventValueChanged.subscribe(this, &PropertyDrawerParticleSystemParameterColor::notifyDropdownValueChanged);
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterColor::notifyDropdownValueChanged(Dropdown2D* _dropdown, S32 _index)
    {
        ParticleSystemParameterColorSamplingMode mode = ParticleSystemParameterColorSamplingMode(_index + 1);
        setMode(mode);

        eventUIData();
    }


} // namespace Maze
//////////////////////////////////////////
