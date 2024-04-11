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
#pragma once
#if (!defined(_ParticleEffectInfo_hpp_))
#define _ParticleEffectInfo_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(ParticleEffectInfo);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(HierarchyLinePool);
    MAZE_USING_SHARED_PTR(HierarchyLine);
    MAZE_USING_SHARED_PTR(ECSScene);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);


    //////////////////////////////////////////
    // Class ParticleEffectInfo
    //
    //////////////////////////////////////////
    class ParticleEffectInfo
        : public Component
        , public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ParticleEffectInfo, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ParticleEffectInfo);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ParticleEffectInfo();

        //////////////////////////////////////////
        static ParticleEffectInfoPtr Create();

        //////////////////////////////////////////
        virtual void update(F32 _dt);

        //////////////////////////////////////////
        inline Transform2DPtr const& getTransform() const { return m_transform; }


    protected:

        //////////////////////////////////////////
        ParticleEffectInfo();
      
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void updateActive();

        //////////////////////////////////////////
        void processDataToUI();


        //////////////////////////////////////////
        void notifySelectionChanged();

        //////////////////////////////////////////
        void notifyStopButtonClick(Button2D* _button, CursorInputEvent const& _cursor);

        //////////////////////////////////////////
        void notifyPlayButtonClick(Button2D* _button, CursorInputEvent const& _cursor);

        //////////////////////////////////////////
        void notifyRestartButtonClick(Button2D* _button, CursorInputEvent const& _cursor);

        //////////////////////////////////////////
        void notifyPauseButtonClick(Button2D* _button, CursorInputEvent const& _cursor);

    protected:
        Transform2DPtr m_transform;

        ClickButton2DPtr m_stopButton;
        ClickButton2DPtr m_playButton;
        ClickButton2DPtr m_restartButton;
        ClickButton2DPtr m_pauseButton;
        ToggleButton2DPtr m_recursiveButton;

        AbstractTextRenderer2DPtr m_particlesValueText;
        AbstractTextRenderer2DPtr m_timeValueText;
        AbstractTextRenderer2DPtr m_iteractionsValueText;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _ParticleEffectInfo_hpp_
//////////////////////////////////////////
