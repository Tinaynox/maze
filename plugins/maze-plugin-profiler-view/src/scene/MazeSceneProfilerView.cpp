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
#include "MazeProfilerViewHeader.hpp"
#include "maze-plugin-profiler-view/scene/MazeSceneProfilerView.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneProfilerView
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneProfilerView, ECSRenderScene);

    //////////////////////////////////////////
    SceneProfilerView::SceneProfilerView()
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneProfilerView::~SceneProfilerView()
    {
        m_canvas.reset();
    }

    //////////////////////////////////////////
    SceneProfilerViewPtr SceneProfilerView::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneProfilerViewPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneProfilerView, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneProfilerView::init(RenderTargetPtr const& _renderTarget)
    {
        if (!ECSRenderScene::init(_renderTarget))
            return false;

        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneProfilerView::update(F32 _dt)
    {
        MAZE_PROFILER_SCOPED_LOCK(PROFILER);

        Size currentSampleIndex = Profiler::GetCurrentSampleIndex();
        Size prevSampleIndex = currentSampleIndex != 0 ? currentSampleIndex - 1u : Profiler::c_samplesCount - 1u;
        Vector<Profiler*> const& profilers = Profiler::GetAllProfilers();
        Size profilersCount = profilers.size();

        static Vec2DF const lbOffset(10.0f, 10.0f);
        static Vec2DF const statsSize(120.0f, 50.0f);

        Size prevDataSize = m_views.size();
        if (prevDataSize < profilersCount)
        {
            m_views.resize(profilersCount);

            for (Size i = prevDataSize, in = m_views.size(); i < in; ++i)
            {
                ProfilerViewData& viewData = m_views[i];
                viewData.background = SpriteHelper::CreateSprite(
                    ColorU32(7, 7, 7, 100),
                    statsSize,
                    lbOffset,
                    nullptr,
                    m_canvas->getTransform(),
                    this,
                    Vec2DF::c_zero,
                    Vec2DF::c_zero);

                viewData.graph = SpriteHelper::CreateSimpleLineRenderer(
                    Vec2DF(0.0f, 0.0f),
                    viewData.background->getTransform(),
                    this,
                    Vec2DF::c_zero,
                    Vec2DF::c_zero);
                viewData.graph->setColor(ColorF128::c_green);
                viewData.graph->resizePositions(Profiler::c_samplesCount - 1u);

                viewData.label0 = SpriteHelper::CreateSystemText(
                    "PROFILER",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Top,
                    statsSize,
                    Vec2DF(2.0f, -1.0f),
                    viewData.background->getTransform(),
                    this,
                    Vec2DF(0.0f, 1.0f),
                    Vec2DF(0.0f, 1.0f));
                viewData.label0->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefaultOutlined());

                viewData.label1 = SpriteHelper::CreateSystemText(
                    "",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Bottom,
                    statsSize,
                    Vec2DF(2.0f, 1.0f),
                    viewData.background->getTransform(),
                    this,
                    Vec2DF(0.0f, 0.0f),
                    Vec2DF(0.0f, 0.0f));
                viewData.label1->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefaultOutlined());
            }
        }

        static ColorU32 const bgrColorDefault(0, 0, 0, 100);
        static ColorU32 const bgrColorOverload0(135, 50, 0, 100);
        static ColorU32 const bgrColorOverload1(150, 0, 0, 100);

        Vec2DF viewportSize = m_canvas->getTransform()->getSize();

        U32 const statsPerRow = Math::Max(1u, U32((viewportSize.x - lbOffset.x) / (statsSize.x + lbOffset.x)));

        for (Size i = 0, in = profilersCount; i < in; ++i)
        {
            Profiler* profilerData = profilers[i];
            ProfilerViewData& viewData = m_views[i];

            Size column = i % statsPerRow;
            Size row = i / statsPerRow;

            Vec2DF pos = Vec2DF(
                lbOffset.x,
                lbOffset.y) +
                Vec2DF(
                    (F32)column * (statsSize.x + lbOffset.x),
                    (F32)row * (statsSize.y + lbOffset.y));
            viewData.background->getTransform()->setLocalPosition(pos);

            if (profilerData->getMaxDuration() <= 2)
                viewData.background->setColor(bgrColorDefault);
            else
            if (profilerData->getMaxDuration() <= 16)
                viewData.background->setColor(bgrColorOverload0);
            else
                viewData.background->setColor(bgrColorOverload1);

            Profiler* parent = profilerData->getParentProfiler();
            if (parent)
            {
                viewData.label0->setTextFormatted(
                    "%s\n >> %s",
                    profilerData->getShortName(),
                    parent->getShortName());
            }
            else
            {
                viewData.label0->setTextFormatted(
                    "%s\n",
                    profilerData->getShortName());
            }

            if (profilerData->getMaxDuration() == 0)
            {
                viewData.label1->setText("");

                viewData.graph->getEntityRaw()->setActiveSelf(false);
            }
            else
            {
                viewData.label1->setTextFormatted(
                    "cls: %u\navg:%.1f\nmin:%u max:%u",
                    profilerData->getSample(prevSampleIndex).callCount,
                    profilerData->getAverageDuration(),
                    profilerData->getMinDuration(),
                    profilerData->getMaxDuration());

                
                {
                    viewData.graph->getEntityRaw()->setActiveSelf(true);

                    if (profilerData->getMaxDuration() <= 2)
                        viewData.graph->setColor(ColorU32(0, 255, 0), false);
                    else
                    if (profilerData->getMaxDuration() <= 16)
                        viewData.graph->setColor(ColorU32(255, 165, 0), false);
                    else
                        viewData.graph->setColor(ColorU32(255, 0, 0), false);

                    for (Size j = 0, jn = Profiler::c_samplesCount - 1; j < jn; ++j)
                    {
                        ProfilerSample const& sample = profilerData->getSample(j + 1);

                        F32 val = Math::Min(1.0f, (F32)sample.durationMS / 100.0f);
                        viewData.graph->setPosition(
                            j,
                            Vec2DF(
                                ((F32)j / (F32)jn * (F32)statsSize.x),
                                val * (F32)statsSize.y));
                    }

                    viewData.graph->rebuildMesh();
                }
                
            }
        }
    }

    //////////////////////////////////////////
    void SceneProfilerView::create2D()
    {
        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearDepthFlag(false);
        m_canvas->setRenderTarget(m_renderTarget);
        m_canvas->setSortOrder(1500000);
        m_canvas->setViewport(0.0f, 0.0f, 1.0f, 1.0f);

        updateUI();
    }

    //////////////////////////////////////////
    void SceneProfilerView::updateUI()
    {
        if (m_state == ECSSceneState::Destroy)
            return;


    }

} // namespace Maze
//////////////////////////////////////////
