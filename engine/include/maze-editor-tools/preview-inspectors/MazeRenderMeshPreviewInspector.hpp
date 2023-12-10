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
#if (!defined(_MazeRenderMeshPreviewInspector_hpp_))
#define _MazeRenderMeshPreviewInspector_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/preview-inspectors/MazePreviewInspector.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawer.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMeshPreviewInspector);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(ComponentEditor);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Shader);
    

    //////////////////////////////////////////
    // Class RenderMeshPreviewInspector
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API RenderMeshPreviewInspector
        : public PreviewInspector
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderMeshPreviewInspector, PreviewInspector);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(RenderMeshPreviewInspector);

    public:

        //////////////////////////////////////////
        virtual ~RenderMeshPreviewInspector();

        //////////////////////////////////////////
        static RenderMeshPreviewInspectorPtr Create(
            Transform2DPtr const& _parent2D,
            SceneDebugPreviewPtr const& _scene);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool getCameraActive() const MAZE_OVERRIDE { return true; }

        //////////////////////////////////////////
        virtual bool getCanvasActive() const MAZE_OVERRIDE { return false; }

        //////////////////////////////////////////
        virtual void processCursorPress(Vec2F const& _positionOS, CursorInputEvent const& _event) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processCursorDrag(Vec2F const& _positionOS, CursorInputEvent const& _event) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processCursorWheel(CursorWheelInputEvent const& _event) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void setRenderMeshes(Set<RenderMeshPtr> const& _renderMeshes);

        //////////////////////////////////////////
        virtual bool setAssetFiles(Set<AssetFilePtr> const& _assetFiles) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setObjects(Set<ObjectPtr> const& _objects) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderMeshPreviewInspector();

        //////////////////////////////////////////
        virtual bool init(
            Transform2DPtr const& _parent2D,
            SceneDebugPreviewPtr const& _scene) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void buildRenderMeshes();

        //////////////////////////////////////////
        void updateCameraPosition();

    protected:
        Set<RenderMeshPtr> m_renderMeshes;

        bool m_renderMeshesDirty = false;
        Vec2F m_cursorPositionLastFrame = Vec2F::c_zero;

        F32 m_cameraDistance = 7.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderMeshPreviewInspector_hpp_
//////////////////////////////////////////
