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
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemParameterF32.hpp"
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
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
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
    // Class PropertyDrawerParticleSystemParameterF32
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterF32, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerParticleSystemParameterF32);

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterF32::PropertyDrawerParticleSystemParameterF32()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterF32::~PropertyDrawerParticleSystemParameterF32()
    {
        if (m_editBoxFrom)
        {
            m_editBoxFrom->eventTextInput.unsubscribe(this);
            m_editBoxFrom->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxTo)
        {
            m_editBoxTo->eventTextInput.unsubscribe(this);
            m_editBoxTo->eventSelectedChanged.unsubscribe(this);
        }

        if (m_curveFrom)
            m_curveFrom->eventCurveChanged.unsubscribe(this);

        if (m_curveTo)
            m_curveTo->eventCurveChanged.unsubscribe(this);

        if (m_modeDropdown)
        {
            m_modeDropdown->eventValueChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterF32Ptr PropertyDrawerParticleSystemParameterF32::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerParticleSystemParameterF32Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerParticleSystemParameterF32, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerParticleSystemParameterF32::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32::buildUI(
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
            m_editBoxFrom = UIHelper::CreateDefaultEditBox(
                "",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                Vec2F(60, 18),
                Vec2F(0, 0),
                m_layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_editBoxFrom->eventTextInput.subscribe(this, &PropertyDrawerParticleSystemParameterF32::notifyTextInput);
            m_editBoxFrom->eventSelectedChanged.subscribe(this, &PropertyDrawerParticleSystemParameterF32::notifySelectedChanged);

            m_editBoxTo = UIHelper::CreateDefaultEditBox(
                "",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                Vec2F(60, 18),
                Vec2F(0, 0),
                m_layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_editBoxTo->eventTextInput.subscribe(this, &PropertyDrawerParticleSystemParameterF32::notifyTextInput);
            m_editBoxTo->eventSelectedChanged.subscribe(this, &PropertyDrawerParticleSystemParameterF32::notifySelectedChanged);
        }

        {
            m_curveFrom = EditorToolsUIHelper::CreateDefaultAnimationCurveEdit(
                AnimationCurve(),
                Vec2F(60, 18),
                Vec2F(0, 0),
                m_layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_curveFrom->eventCurveChanged.subscribe(this, &PropertyDrawerParticleSystemParameterF32::notifyCurveChanged);

            m_curveTo = EditorToolsUIHelper::CreateDefaultAnimationCurveEdit(
                AnimationCurve(),
                Vec2F(60, 18),
                Vec2F(0, 0),
                m_layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_curveTo->eventCurveChanged.subscribe(this, &PropertyDrawerParticleSystemParameterF32::notifyCurveChanged);
        }

        createModeDropdown();

        updateModeUI();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32::notifyTextInput(EditBox2D* _editBox)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32::notifyCurveChanged(AnimationCurveEdit2D* _edit, AnimationCurve const& _curve)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32::setValue(ParticleSystemParameterF32 const& _value)
    {
        m_parameter = _value;
        setMode(_value.getMode());

        switch (_value.getMode())
        {
            case ParticleSystemParameterF32SamplingMode::Constant:
            {
                if (!m_editBoxTo->getSelected())
                    m_editBoxTo->setText(
                        StringHelper::F32ToStringFormatted(_value.getConst0(), 4));
                break;
            }
            case ParticleSystemParameterF32SamplingMode::Curve:
            {
                if (!m_curveTo->getSelected())
                    m_curveTo->setCurve(_value.getCurve0().normalized());

                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenConstants:
            {
                if (!m_editBoxFrom->getSelected())
                    m_editBoxFrom->setText(
                        StringHelper::F32ToStringFormatted(_value.getConst0(), 4));

                if (!m_editBoxTo->getSelected())
                    m_editBoxTo->setText(
                        StringHelper::F32ToStringFormatted(_value.getConst1(), 4));
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenCurves:
            {
                if (!m_curveFrom->getSelected())
                    m_curveFrom->setCurve(_value.getCurve0().normalized());

                if (!m_curveTo->getSelected())
                    m_curveTo->setCurve(_value.getCurve1().normalized());

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
    ParticleSystemParameterF32 PropertyDrawerParticleSystemParameterF32::getValue() const
    {
        ParticleSystemParameterF32 parameter = m_parameter;

        switch (m_mode)
        {
            case ParticleSystemParameterF32SamplingMode::Constant:
            {
                F32 const0 = StringHelper::StringToF32(m_editBoxTo->getText());
                parameter.setConstant(const0);
                break;
            }
            case ParticleSystemParameterF32SamplingMode::Curve:
            {
                AnimationCurve curve0 = m_curveTo->getCurve();

                curve0.setMinMaxMode(m_curveMinMaxMode);

                if (curve0.empty())
                {
                    if (Math::Abs(m_parameter.getConst0()) <= 1.0f)
                    {
                        curve0.addKey(0.0f, m_parameter.getConst0());
                        curve0.addKey(1.0f, m_parameter.getConst0());
                    }
                    else
                    {
                        curve0.setScalar(Math::Abs(m_parameter.getConst0()));
                        curve0.addKey(0.0f, Math::Sign(m_parameter.getConst0()));
                        curve0.addKey(1.0f, Math::Sign(m_parameter.getConst0()));
                    }
                }
                
                parameter.setCurve(curve0.normalized());
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenConstants:
            {
                F32 const0 = StringHelper::StringToF32(m_editBoxFrom->getText());
                F32 const1 = StringHelper::StringToF32(m_editBoxTo->getText());
                parameter.setRandomBetweenConstants(const0, const1);
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenCurves:
            {
                AnimationCurve curve0 = m_curveFrom->getCurve();
                AnimationCurve curve1 = m_curveTo->getCurve();

                curve0.setMinMaxMode(m_curveMinMaxMode);
                curve1.setMinMaxMode(m_curveMinMaxMode);

                if (curve0.empty())
                {
                    if (Math::Abs(m_parameter.getConst0()) <= 1.0f)
                    {
                        curve0.addKey(0.0f, m_parameter.getConst0());
                        curve0.addKey(1.0f, m_parameter.getConst0());
                    }
                    else
                    {
                        curve0.setScalar(Math::Abs(m_parameter.getConst0()));
                        curve0.addKey(0.0f, Math::Sign(m_parameter.getConst0()));
                        curve0.addKey(1.0f, Math::Sign(m_parameter.getConst0()));
                    }
                }
                if (curve1.empty())
                {
                    if (Math::Abs(m_parameter.getConst1()) <= 1.0f)
                    {
                        curve1.addKey(0.0f, m_parameter.getConst1());
                        curve1.addKey(1.0f, m_parameter.getConst1());
                    }
                    else
                    {
                        curve1.setScalar(Math::Abs(m_parameter.getConst1()));
                        curve1.addKey(0.0f, Math::Sign(m_parameter.getConst1()));
                        curve1.addKey(1.0f, Math::Sign(m_parameter.getConst1()));
                    }
                }

                parameter.setRandomBetweenCurves(curve0.normalized(), curve1.normalized());
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
    void PropertyDrawerParticleSystemParameterF32::setMode(ParticleSystemParameterF32SamplingMode _mode)
    {
        if (m_mode == _mode)
            return;

        m_mode = _mode;

        updateModeUI();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32::updateModeUI()
    {
        bool editBoxFromActive = false;
        bool editBoxToActive = false;
        bool curveFromActive = false;
        bool curveToActive = false;

        switch (m_mode)
        {
            case ParticleSystemParameterF32SamplingMode::Constant:
            {
                editBoxToActive = true;
                break;
            }
            case ParticleSystemParameterF32SamplingMode::Curve:
            {
                curveToActive = true;
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenConstants:
            {
                editBoxFromActive = true;
                editBoxToActive = true;
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenCurves:
            {
                curveFromActive = true;
                curveToActive = true;
                break;
            }
            default:
            {
                break;
            }
        }

        m_editBoxFrom->getEntityRaw()->setActiveSelf(editBoxFromActive);
        m_editBoxTo->getEntityRaw()->setActiveSelf(editBoxToActive);
        m_curveFrom->getEntityRaw()->setActiveSelf(curveFromActive);
        m_curveTo->getEntityRaw()->setActiveSelf(curveToActive);
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32::createModeDropdown()
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


        for (ParticleSystemParameterF32SamplingMode mode : ParticleSystemParameterF32SamplingMode::All())
        {
            if (mode == ParticleSystemParameterF32SamplingMode::None)
                continue;

            // Curves disabled
            if (m_curveMinMaxMode == AnimationCurveMinMaxMode::None)
            {
                if (mode == ParticleSystemParameterF32SamplingMode::Curve ||
                    mode == ParticleSystemParameterF32SamplingMode::RandomBetweenCurves)
                    continue;
            }

            m_modeDropdown->addOption(mode.toString(), false, false);
        }

        m_modeDropdown->updateCaption();
        m_modeDropdown->rebuildOptions();

        m_modeDropdown->eventValueChanged.subscribe(this, &PropertyDrawerParticleSystemParameterF32::notifyDropdownValueChanged);
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32::notifyDropdownValueChanged(Dropdown2D* _dropdown, S32 _index)
    {
        ParticleSystemParameterF32SamplingMode mode = ParticleSystemParameterF32SamplingMode::FromString(_dropdown->getValueString());
        setMode(mode);

        eventUIData();
    }


    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemParameterF32Positive
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterF32Positive, PropertyDrawerParticleSystemParameterF32);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerParticleSystemParameterF32Positive);

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterF32Positive::PropertyDrawerParticleSystemParameterF32Positive()
    {
        m_curveMinMaxMode = AnimationCurveMinMaxMode::NormalizedPositive;
    }

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterF32PositivePtr PropertyDrawerParticleSystemParameterF32Positive::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerParticleSystemParameterF32PositivePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerParticleSystemParameterF32Positive, object, init(_dataBlock));
        return object;
    }


    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemParameterF32Degrees
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterF32Degrees, PropertyDrawerParticleSystemParameterF32);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerParticleSystemParameterF32Degrees);

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterF32DegreesPtr PropertyDrawerParticleSystemParameterF32Degrees::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerParticleSystemParameterF32DegreesPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerParticleSystemParameterF32Degrees, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemParameterF32Degrees::setValue(ParticleSystemParameterF32 const& _value)
    {
        ParticleSystemParameterF32 value = _value;
        value.multiplyValues(180.0f / Math::c_pi);
        PropertyDrawerParticleSystemParameterF32::setValue(value);
    }

    //////////////////////////////////////////
    ParticleSystemParameterF32 PropertyDrawerParticleSystemParameterF32Degrees::getValue() const
    {
        ParticleSystemParameterF32 value = PropertyDrawerParticleSystemParameterF32::getValue();
        value.multiplyValues(Math::c_pi / 180.0f);
        return value;
    }


    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemParameterF32Constants
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterF32Constants, PropertyDrawerParticleSystemParameterF32);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerParticleSystemParameterF32Constants);

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterF32Constants::PropertyDrawerParticleSystemParameterF32Constants()
    {
        m_curveMinMaxMode = AnimationCurveMinMaxMode::None;
    }

    //////////////////////////////////////////
    PropertyDrawerParticleSystemParameterF32ConstantsPtr PropertyDrawerParticleSystemParameterF32Constants::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerParticleSystemParameterF32ConstantsPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerParticleSystemParameterF32Constants, object, init(_dataBlock));
        return object;
    }

} // namespace Maze
//////////////////////////////////////////
