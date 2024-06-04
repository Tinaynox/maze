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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyControllerOBSOLETE.hpp"
#include "maze-editor-tools/ecs/components/MazeInspectorController.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EditorToolsUIHelper
    {

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ToggleButton2DPtr CreateDefaultToggleButton(
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene)
        {
            ToggleButton2DPtr button = UIHelper::CreateToggleButton(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                Vec2F(18.0f, 18.0f),
                Vec2F::c_zero,
                _parent,
                _ecsScene,
                Vec2F(0.5f, 0.5f),
                Vec2F(0.5f, 0.5f),
                ColorU32{ 200, 200, 200 },
                ColorU32{ 187, 187, 187 },
                ColorU32{ 161, 161, 161 },
                ColorU32{ 171, 171, 171 },
                ColorU32{ 151, 151, 151 });

            return button;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ToggleButton2DPtr CreateDefaultToggleButton(
            SpritePtr const& _sprite,
            ColorU32 const& _spriteColor,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _spriteSize)
        {
            ToggleButton2DPtr button = CreateDefaultToggleButton(_parent, _ecsScene);

            SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
                _sprite,
                _spriteSize,
                Vec2F::c_zero,
                nullptr,
                button->getTransform(),
                _ecsScene);
            sprite->setColor(_spriteColor);

            return button;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ClickButton2DPtr CreateDefaultClickButton(
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene)
        {
            ClickButton2DPtr button = UIHelper::CreateDefaultClickButton(
                "",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                Vec2F(18.0f, 18.0f),
                Vec2F::c_zero,
                _parent,
                _ecsScene,
                Vec2F(0.5f, 0.5f),
                Vec2F(0.5f, 0.5f));
            button->setNormalColor(ColorU32{ 200, 200, 200 });
            button->setFocusedColor(ColorU32{ 187, 187, 187 });
            button->setSelectedColor(ColorU32{ 161, 161, 161 });
            button->setPressedColor(ColorU32{ 171, 171, 171 });

            return button;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ClickButton2DPtr CreateDefaultClickButton(
            SpritePtr const& _sprite,
            ColorU32 const& _spriteColor,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _spriteSize)
        {
            ClickButton2DPtr button = CreateDefaultClickButton(_parent, _ecsScene);

            SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
                _sprite,
                _spriteSize,
                Vec2F::c_zero,
                nullptr,
                button->getTransform(),
                _ecsScene);
            sprite->setColor(_spriteColor);

            return button;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API AbstractTextRenderer2DPtr CreateText(
            CString _text,
            FontMaterialPtr const& _fontMaterial,
            U32 _fontSize,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            FontMaterialPtr const& fontMaterial = _fontMaterial ? _fontMaterial : EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial();
            return UIHelper::CreateTextOrSystemText(
                _text,
                fontMaterial,
                _fontSize,
                _horizontalAlignment,
                _verticalAlignment,
                _size,
                _position,
                _parent,
                _ecsScene,
                _anchor,
                _pivot);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API Dropdown2DPtr CreateDefaultDropdown(
            FontMaterialPtr const& _fontMaterial,
            U32 _fontSize,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            FontMaterialPtr const& fontMaterial = _fontMaterial ? _fontMaterial : EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial();
            return UIHelper::CreateDefaultDropdown(
                fontMaterial,
                _fontSize,
                _size,
                _position,
                _parent,
                _ecsScene,
                _anchor,
                _pivot);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ColorEdit2DPtr CreateDefaultColorEdit(
            ColorU32 _color,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr colorEditEntity = _ecsScene->createEntity();
            colorEditEntity->ensureComponent<Name>("ColorEdit");

            ColorEdit2DPtr colorEdit = colorEditEntity->createComponent<ColorEdit2D>();
            colorEdit->setColor(_color);

            ScissorMask2DPtr scissorMask = colorEditEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = colorEdit->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr colorSpriteRenderer = SpriteHelper::CreateSprite(
                _color,
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            colorSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);
            colorEdit->setColorRenderer(colorSpriteRenderer);
            // SpriteRenderer2D* colorSpriteRendererRaw = colorSpriteRenderer.get();

            SpriteRenderer2DPtr alphaBackSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_black,
                { _size.x, 3.0f },
                Vec2F(0.0f, 1.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene,
                Vec2F(0.5f, 0.0f),
                Vec2F(0.5f, 0.0f));
            SizePolicy2DPtr alphaBackSizePolicy = alphaBackSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>();
            alphaBackSizePolicy->setFlag(SizePolicy2D::Height, false);
            alphaBackSizePolicy->setSizeDelta(-2.0f, 0.0f);

            SpriteRenderer2DPtr alphaSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                { (_size.x - 2.0f) * 0.5f, 3.0f },
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                alphaBackSpriteRenderer->getTransform(),
                _ecsScene,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            SizePolicy2DPtr alphaSizePolicy = alphaSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>();
            alphaSizePolicy->setFlag(SizePolicy2D::Width, false);
            colorEdit->setAlphaRenderer(alphaSpriteRenderer);


            SpriteRenderer2DPtr borderSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default),
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            SpriteRenderer2D* borderSpriteRendererRaw = borderSpriteRenderer.get();
            borderSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);



            auto updateColorEditState =
                [](ColorEdit2D* _colorEdit, SpriteRenderer2D* _borderSpriteRenderer)
            {
                if (_colorEdit->getSelected())
                {
                    _borderSpriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Selected));
                }
                else
                {
                    if (_colorEdit->getFocused())
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Focused));
                    }
                    else
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default));
                    }
                }
            };

            colorEdit->eventFocusChanged.subscribe(
                [=](ColorEdit2D* _colorEdit, bool _value)
            {
                updateColorEditState(_colorEdit, borderSpriteRendererRaw);
            });

            colorEdit->eventSelectedChanged.subscribe(
                [=](ColorEdit2D* _colorEdit, bool _value)
            {
                updateColorEditState(_colorEdit, borderSpriteRendererRaw);
            });

            return colorEdit;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ColorHDREdit2DPtr CreateDefaultColorHDREdit(
            ColorF128 _color,
            FontMaterialPtr const& _fontMaterial,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot,
            bool _hdrLabel,
            bool _alphaBand)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr colorEditEntity = _ecsScene->createEntity();
            colorEditEntity->ensureComponent<Name>("ColorHDREdit");

            ColorHDREdit2DPtr colorEdit = colorEditEntity->createComponent<ColorHDREdit2D>();
            colorEdit->setColor(_color);

            ScissorMask2DPtr scissorMask = colorEditEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = colorEdit->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr colorSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::FromVec4F32((Vec4F)_color),
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorHDR),
                transform,
                _ecsScene);
            colorSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);
            colorEdit->setColorRenderer(colorSpriteRenderer);

            AbstractTextRenderer2D* itemTextRendererRaw = nullptr;
            if (_hdrLabel)
            {
                AbstractTextRenderer2DPtr itemTextRenderer = UIHelper::CreateTextOrSystemText(
                    "HDR",
                    _fontMaterial,
                    10,
                    HorizontalAlignment2D::Center,
                    VerticalAlignment2D::Middle,
                    colorSpriteRenderer->getTransform()->getSize(),
                    Vec2F(0.0f, 0.0f),
                    colorSpriteRenderer->getTransform(),
                    _ecsScene,
                    Vec2F(0.5f, 0.5f),
                    Vec2F(0.5f, 0.5f));
                itemTextRendererRaw = itemTextRenderer.get();
            }

            if (_alphaBand)
            {
                SpriteRenderer2DPtr alphaBackSpriteRenderer = SpriteHelper::CreateSprite(
                    ColorU32::c_black,
                    { _size.x, 3.0f },
                    Vec2F(0.0f, 1.0f),
                    renderSystem->getMaterialManager()->getColorTextureMaterial(),
                    transform,
                    _ecsScene,
                    Vec2F(0.5f, 0.0f),
                    Vec2F(0.5f, 0.0f));
                SizePolicy2DPtr alphaBackSizePolicy = alphaBackSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>();
                alphaBackSizePolicy->setFlag(SizePolicy2D::Height, false);
                alphaBackSizePolicy->setSizeDelta(-2.0f, 0.0f);

                SpriteRenderer2DPtr alphaSpriteRenderer = SpriteHelper::CreateSprite(
                    ColorU32::c_white,
                    { (_size.x - 2.0f) * 0.5f, 3.0f },
                    Vec2F(0.0f, 0.0f),
                    renderSystem->getMaterialManager()->getColorTextureMaterial(),
                    alphaBackSpriteRenderer->getTransform(),
                    _ecsScene,
                    Vec2F(0.0f, 0.0f),
                    Vec2F(0.0f, 0.0f));
                SizePolicy2DPtr alphaSizePolicy = alphaSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>();
                alphaSizePolicy->setFlag(SizePolicy2D::Width, false);
                colorEdit->setAlphaRenderer(alphaSpriteRenderer);
            }


            SpriteRenderer2DPtr borderSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default),
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            SpriteRenderer2D* borderSpriteRendererRaw = borderSpriteRenderer.get();
            borderSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);



            auto updateColorEditState =
                [](ColorHDREdit2D* _colorEdit, SpriteRenderer2D* _borderSpriteRenderer)
            {
                if (_colorEdit->getSelected())
                {
                    _borderSpriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Selected));
                }
                else
                {
                    if (_colorEdit->getFocused())
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Focused));
                    }
                    else
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default));
                    }
                }
            };

            auto updateHDRColor =
                [](ColorHDREdit2D* _colorEdit, AbstractTextRenderer2D* _hdrTextRenderer)
            {
                Vec3F const luminosityCoeff(0.2125f, 0.7154f, 0.0721f);
                F32 l = Math::Clamp(luminosityCoeff.dotProduct((Vec3F)_colorEdit->getColor()), 0.0f, 1.0f);
                F32 invL = 1.0f - l;
                _hdrTextRenderer->setColor(
                    ColorU32::FromVec3F32({ invL, invL, invL }));
            };


            colorEdit->eventFocusChanged.subscribe(
                [=](ColorHDREdit2D* _colorEdit, bool _value)
            {
                updateColorEditState(_colorEdit, borderSpriteRendererRaw);
            });

            colorEdit->eventSelectedChanged.subscribe(
                [=](ColorHDREdit2D* _colorEdit, bool _value)
            {
                updateColorEditState(_colorEdit, borderSpriteRendererRaw);
            });

            if (_hdrLabel)
            {
                colorEdit->eventColorChanged.subscribe(
                    [=](ColorHDREdit2D* _colorEdit, ColorF128 const& _color)
                {
                    updateHDRColor(_colorEdit, itemTextRendererRaw);
                });

                updateHDRColor(colorEdit.get(), itemTextRendererRaw);
            }


            return colorEdit;
        }


        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ColorGradientEdit2DPtr CreateDefaultColorGradientEdit(
            ColorGradient const& _gradient,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr colorEditEntity = _ecsScene->createEntity();
            colorEditEntity->ensureComponent<Name>("ColorGradientEdit");

            ColorGradientEdit2DPtr colorEdit = colorEditEntity->createComponent<ColorGradientEdit2D>();
            colorEdit->setGradient(_gradient);

            ScissorMask2DPtr scissorMask = colorEditEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = colorEdit->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr transparentBackSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TransparentChess),
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            transparentBackSpriteRenderer->getMaterial()->setUniform(
                MAZE_HCS("u_baseMapST"),
                Vec4F(
                    transparentBackSpriteRenderer->getTransform()->getWidth() / 8.0f,
                    transparentBackSpriteRenderer->getTransform()->getHeight() / 8.0f,
                    0.0f,
                    0.0f));
            transparentBackSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);

            SpriteRenderer2DPtr gradientSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorMaterial(),
                transform,
                _ecsScene);
            gradientSpriteRenderer->setRenderMode(SpriteRenderMode::Custom);
            gradientSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);
            colorEdit->setGradientRenderer(gradientSpriteRenderer);


            SpriteRenderer2DPtr borderSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default),
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            SpriteRenderer2D* borderSpriteRendererRaw = borderSpriteRenderer.get();
            borderSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            auto updateColorEditState =
                [](ColorGradientEdit2D* _colorEdit, SpriteRenderer2D* _borderSpriteRenderer)
            {
                if (_colorEdit->getSelected())
                {
                    _borderSpriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Selected));
                }
                else
                {
                    if (_colorEdit->getFocused())
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Focused));
                    }
                    else
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default));
                    }
                }
            };

            colorEdit->eventFocusChanged.subscribe(
                [=](ColorGradientEdit2D* _colorEdit, bool _value)
            {
                updateColorEditState(_colorEdit, borderSpriteRendererRaw);
            });

            colorEdit->eventSelectedChanged.subscribe(
                [=](ColorGradientEdit2D* _colorEdit, bool _value)
            {
                updateColorEditState(_colorEdit, borderSpriteRendererRaw);
            });

            return colorEdit;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API AnimationCurveEdit2DPtr CreateDefaultAnimationCurveEdit(
            AnimationCurve const& _curve,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr curveEditEntity = _ecsScene->createEntity();
            curveEditEntity->ensureComponent<Name>("CurveEdit");

            AnimationCurveEdit2DPtr curveEdit = curveEditEntity->createComponent<AnimationCurveEdit2D>();
            curveEdit->setCurve(_curve);

            ScissorMask2DPtr scissorMask = curveEditEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = curveEdit->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr curveSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            curveSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);
            curveEdit->setCurveRenderer(curveSpriteRenderer);


            SpriteRenderer2DPtr borderSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default),
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            SpriteRenderer2D* borderSpriteRendererRaw = borderSpriteRenderer.get();
            borderSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            auto updateCurveEditState =
                [](AnimationCurveEdit2D* _curveEdit, SpriteRenderer2D* _borderSpriteRenderer)
            {
                if (_curveEdit->getSelected())
                {
                    _borderSpriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Selected));
                }
                else
                {
                    if (_curveEdit->getFocused())
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Focused));
                    }
                    else
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default));
                    }
                }
            };

            curveEdit->eventFocusChanged.subscribe(
                [=](AnimationCurveEdit2D* _colorEdit, bool _value)
            {
                updateCurveEditState(_colorEdit, borderSpriteRendererRaw);
            });

            curveEdit->eventSelectedChanged.subscribe(
                [=](AnimationCurveEdit2D* _colorEdit, bool _value)
            {
                updateCurveEditState(_colorEdit, borderSpriteRendererRaw);
            });

            return curveEdit;
        }

    } // namespace EditorToolsUIHelper
    //////////////////////////////////////////
    

} // namespace Maze
//////////////////////////////////////////
