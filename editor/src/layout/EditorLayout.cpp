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
#include "layout/EditorLayout.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
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
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyController.hpp"
#include "maze-editor-tools/ecs/components/MazeInspectorController.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "Editor.hpp"
#include "managers/EditorManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorLayout
    //
    //////////////////////////////////////////
    F32 const EditorLayout::c_menuBarHeight = 17.0f;
    F32 const topBarHeight = 0.025f;
    Rect2F const EditorLayout::c_sceneViewport(0.25f, 0.33f, 0.5f, 0.645f);
    Rect2F const EditorLayout::c_hierarchyViewport(0.0f, 0.33f, 0.25f, 0.67f - topBarHeight);
    Rect2F const EditorLayout::c_inspectorViewport(0.75f, 0.33f, 0.25f, 0.67f - topBarHeight);
    Rect2F const EditorLayout::c_assetsViewport(0.0f, 0.0f, 0.5f, 0.33f);
    Rect2F const EditorLayout::c_previewViewport(0.5f, 0.0f, 0.5f, 0.33f);
    Rect2F const EditorLayout::c_topBarViewport(0.0f, 0.975f, 1.0f, topBarHeight);
    F32 const EditorLayout::c_workspaceTopBarHeight = 20.0f;
        
    //////////////////////////////////////////
    Rect2F EditorLayout::CalculateWorkViewport(Rect2F const& _viewport)
    {
        Vec2F32 fullRenderTargetSize = (Vec2F32)Editor::GetInstancePtr()->getMainRenderWindow()->getRenderTargetSize();
        if (fullRenderTargetSize == Vec2F32::c_zero)
            return Rect2F::c_zero;

        Vec2F32 workRenderTargetSize = Vec2F32(fullRenderTargetSize.x, fullRenderTargetSize.y - EditorLayout::c_menuBarHeight);

        Rect2F targetViewportZone = _viewport;
        targetViewportZone.position *= workRenderTargetSize / fullRenderTargetSize;
        targetViewportZone.size *= workRenderTargetSize / fullRenderTargetSize;

        return targetViewportZone;
    }

    //////////////////////////////////////////
    Vec2F32 EditorLayout::ConvertRenderWindowCoordsToWorkspaceViewport(Vec2F32 const& _coord)
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
        Rect2F viewport = CalculateWorkViewport(c_sceneViewport);

        Vec2F32 workspaceStart = viewport.position * (Vec2F32)renderWindow->getRenderTargetSize();

        return _coord - workspaceStart;
    }

} // namespace Maze
//////////////////////////////////////////
