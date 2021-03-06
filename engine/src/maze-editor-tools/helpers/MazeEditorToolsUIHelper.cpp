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
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
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
#include "maze-ui/managers/MazeUIManager.hpp"
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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EditorToolsUIHelper
    {

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ToggleButton2DPtr CreateDefaultToggleButton(
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene)
        {
            ToggleButton2DPtr button = UIHelper::CreateToggleButton(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                Vec2DF(18.0f, 18.0f),
                Vec2DF::c_zero,
                _parent,
                _ecsScene,
                Vec2DF(0.5f, 0.5f),
                Vec2DF(0.5f, 0.5f),
                { 200, 200, 200 },
                { 187, 187, 187 },
                { 161, 161, 161 },
                { 171, 171, 171 },
                { 151, 151, 151 });

            return button;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ToggleButton2DPtr CreateDefaultToggleButton(
            SpritePtr const& _sprite,
            ColorU32 const& _spriteColor,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _spriteSize)
        {
            ToggleButton2DPtr button = CreateDefaultToggleButton(_parent, _ecsScene);

            SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
                _sprite,
                _spriteSize,
                Vec2DF::c_zero,
                nullptr,
                button->getTransform(),
                _ecsScene);
            sprite->setColor(_spriteColor);

            return button;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ClickButton2DPtr CreateDefaultClickButton(
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene)
        {
            ClickButton2DPtr button = UIHelper::CreateDefaultClickButton(
                "",
                Vec2DF(18.0f, 18.0f),
                Vec2DF::c_zero,
                _parent,
                _ecsScene,
                Vec2DF(0.5f, 0.5f),
                Vec2DF(0.5f, 0.5f));
            button->setNormalColor({ 200, 200, 200 });
            button->setFocusedColor({ 187, 187, 187 });
            button->setSelectedColor({ 161, 161, 161 });
            button->setPressedColor({ 171, 171, 171 });

            return button;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ClickButton2DPtr CreateDefaultClickButton(
            SpritePtr const& _sprite,
            ColorU32 const& _spriteColor,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _spriteSize)
        {
            ClickButton2DPtr button = CreateDefaultClickButton(_parent, _ecsScene);

            SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
                _sprite,
                _spriteSize,
                Vec2DF::c_zero,
                nullptr,
                button->getTransform(),
                _ecsScene);
            sprite->setColor(_spriteColor);

            return button;
        }

    } // namespace EditorToolsUIHelper
    //////////////////////////////////////////
    

} // namespace Maze
//////////////////////////////////////////
