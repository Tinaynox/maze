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
#include "maze-editor-tools/ecs/components/MazeColorGradientEdit2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-editor-tools/managers/MazeColorGradientPickerManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ColorGradientEdit2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ColorGradientEdit2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, gradientRenderer, ComponentPtr(), getGradientRendererComponent, setGradientRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorGradient, gradient, ColorGradient(), getGradient, setGradient));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ColorGradientEdit2D);

    //////////////////////////////////////////
    ColorGradientEdit2D::ColorGradientEdit2D()
        : m_selected(false)
    {
    }

    //////////////////////////////////////////
    ColorGradientEdit2D::~ColorGradientEdit2D()
    {
        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorPressIn.unsubscribe(this);
            m_UIElement2D->eventCursorReleaseOut.unsubscribe(this);
            m_UIElement2D->eventFocusChanged.unsubscribe(this);
            m_UIElement2D->eventPressedChanged.unsubscribe(this);
            m_UIElement2D->eventClick.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    ColorGradientEdit2DPtr ColorGradientEdit2D::Create()
    {
        ColorGradientEdit2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ColorGradientEdit2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ColorGradientEdit2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &ColorGradientEdit2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &ColorGradientEdit2D::notifyCursorReleaseOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &ColorGradientEdit2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &ColorGradientEdit2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &ColorGradientEdit2D::notifyClick);
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
            
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
    {
        setSelected(false);
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::notifyFocusChanged(bool _focused)
    {
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(true);
        }
        

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::notifyClick(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        ColorGradientPickerManager::GetInstancePtr()->openGradientPicker(
            [this](ColorGradient const& _gradient)
            {
                setGradient(_gradient);
            },
            m_gradient);

        eventClick(this);
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::setGradient(ColorGradient const& _gradient)
    {
        if (m_gradient == _gradient)
            return;

        m_gradient = _gradient;

        eventGradientChanged(this, m_gradient);

        processGradient();
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::subscribeGradientRenderer()
    {
        if (!m_gradientRenderer)
            return;

        m_gradientRenderer->setCustomRenderCallback(
            [&](SpriteRenderer2D* _spriteRenderer) -> MeshPtr
            {
                ColorGradient gradient = m_gradient;
                gradient.clamp01();
                gradient.addKey(0.0f, gradient.evaluate(0.0f));
                gradient.addKey(1.0f, gradient.evaluate(1.0f));

                Vec2F const& size = _spriteRenderer->getTransform()->getSize();
                Vec4F uv = Vec4F(0.0f, 0.0f, 1.0f, 1.0f);
                F32 canvasRendererAlpha = _spriteRenderer->getCanvasRenderer()->getAlpha();
                FastVector<Pair<F32, Vec4F>> gradientColors = gradient.toRawColors();
                

                F32 startTime = gradientColors.front().first;
                F32 deltaTime = gradientColors.back().first - gradientColors.front().first;

                MeshPtr mesh = Mesh::Create();

                SubMeshPtr subMesh = SubMesh::Create();
                subMesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

                Vector<Vec3F> positions;
                Vector<Vec3F> normals;
                Vector<Vec4F> colors;
                Vector<Vec2F> uvs;

                Vector<U16> indices;

                for (Size i = 0, in = gradientColors.size() - 1; i < in; ++i)
                {
                    F32 t0 = gradientColors[i].first;
                    F32 t1 = gradientColors[i + 1].first;

                    F32 p0 = (t0 - startTime) / deltaTime;
                    F32 p1 = (t1 - startTime) / deltaTime;

                    Vec4F color0 = gradientColors[i].second;
                    Vec4F color1 = gradientColors[i + 1].second;

                    color0.w *= canvasRendererAlpha;
                    color1.w *= canvasRendererAlpha;

                    F32 x0 = p0 * size.x;
                    F32 x1 = p1 * size.x;

                    positions.emplace_back(Vec3F(x1, size.y, 0.0f));    // Top right
                    positions.emplace_back(Vec3F(x1, 0.0f, 0.0f));      // Bottom right
                    positions.emplace_back(Vec3F(x0, 0.0f, 0.0f));      // Bottom left
                    positions.emplace_back(Vec3F(x0, size.y, 0.0f));    // Top left
                        
                    normals.emplace_back(Vec3F(+0.0f, +0.0f, +1.0f));   // Top right
                    normals.emplace_back(Vec3F(+0.0f, +0.0f, +1.0f));   // Bottom right
                    normals.emplace_back(Vec3F(+0.0f, +0.0f, +1.0f));   // Bottom left
                    normals.emplace_back(Vec3F(+0.0f, +0.0f, +1.0f));   // Top left                    

                    if (gradient.getMode() == ColorGradient::EvaluateMode::Fixed)
                    {
                        colors.emplace_back(color0);    // Top right
                        colors.emplace_back(color0);    // Bottom right
                        colors.emplace_back(color0);    // Bottom left
                        colors.emplace_back(color0);    // Top left
                    }
                    else
                    {
                        colors.emplace_back(color1);    // Top right
                        colors.emplace_back(color1);    // Bottom right
                        colors.emplace_back(color0);    // Bottom left
                        colors.emplace_back(color0);    // Top left
                    }
                    
                    uvs.emplace_back(Vec2F(uv.z, uv.w));    // Top right
                    uvs.emplace_back(Vec2F(uv.z, uv.y));    // Bottom right
                    uvs.emplace_back(Vec2F(uv.x, uv.y));    // Bottom left
                    uvs.emplace_back(Vec2F(uv.x, uv.w));    // Top left
                    
                    U16 quadStart = (U16)i * 4;
                    indices.emplace_back(0 + quadStart);
                    indices.emplace_back(1 + quadStart);
                    indices.emplace_back(3 + quadStart);
                    indices.emplace_back(1 + quadStart);
                    indices.emplace_back(2 + quadStart);
                    indices.emplace_back(3 + quadStart);
                }
                
                subMesh->setPositions(positions);
                subMesh->setNormals(normals);
                subMesh->setColors(colors);
                subMesh->setTexCoords(0, uvs);

                subMesh->setIndices(indices);

                mesh->addSubMesh(subMesh);

                return mesh;
            });
        m_gradientRenderer->setRenderMode(SpriteRenderMode::Custom);
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::unsubscribeGradientRenderer()
    {
        if (!m_gradientRenderer)
            return;

        m_gradientRenderer->setCustomRenderCallback(nullptr);
    }

    //////////////////////////////////////////
    void ColorGradientEdit2D::processGradient()
    {
        if (m_gradientRenderer)
        {
            m_gradientRenderer->updateMesh();
        }
        
    }
    

} // namespace Maze
//////////////////////////////////////////
