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
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
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
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/ecs/components/MazeInspectorController.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorToolsStyles
    //
    //////////////////////////////////////////
    F32 const EditorToolsStyles::c_titleHeightDefault = 20.0f;
    F32 const topBarHeight = 0.025f;
    ColorU32 const EditorToolsStyles::c_titleBackgroundColorDefault = ColorU32(203, 203, 203, 255);
    U32 const EditorToolsStyles::c_titleFontSizeDefault = 12;
    F32 const EditorToolsStyles::c_titleLabelShiftDefault = 5.0f;
    ColorU32 const EditorToolsStyles::c_bodyBackgroundColorDefault = ColorU32(194, 194, 194, 255);
    ColorU32 const EditorToolsStyles::c_bodySubBackgroundColorDefault = ColorU32(164, 164, 164, 255);
    U32 const EditorToolsStyles::c_inspectorPropertyFontSizeDefault = 12;
    ColorU32 const EditorToolsStyles::c_inspectorPropertyColorDefault = ColorU32(5, 7, 5, 255);
    Rect2F const EditorToolsStyles::c_hierarchyViewportDefault(0.0f, 0.33f, 0.25f, 0.67f - topBarHeight);
    Rect2F const EditorToolsStyles::c_inspectorViewportDefault(0.75f, 0.33f, 0.25f, 0.67f - topBarHeight);
    Rect2F const EditorToolsStyles::c_assetsViewportDefault(0.0f, 0.0f, 0.5f, 0.33f);
    Rect2F const EditorToolsStyles::c_sceneViewportDefault(0.5f, 0.0f, 0.5f, 0.33f);
    Rect2F const EditorToolsStyles::c_topBarViewportDefault(0.0f, 0.975f, 1.0f, topBarHeight);

    ColorU32 const EditorToolsStyles::c_listObjectTextColorDefault = ColorU32(0, 0, 0, 255);
    ColorU32 const EditorToolsStyles::c_listObjectTextColorSelected = ColorU32(255, 255, 255, 255);
    ColorU32 const EditorToolsStyles::c_listObjectTextColorInactive = ColorU32(130, 130, 130, 255);
    ColorU32 const EditorToolsStyles::c_listObjectBackgroundColorDefault = ColorU32(0, 0, 0, 0);
    ColorU32 const EditorToolsStyles::c_listObjectBackgroundColorFocused = ColorU32(178, 178, 178, 255);
    ColorU32 const EditorToolsStyles::c_listObjectBackgroundColorSelected = ColorU32(78, 134, 206, 255);

    EditorToolsStyles* EditorToolsStyles::s_instance = nullptr;


    //////////////////////////////////////////
    EditorToolsStyles::EditorToolsStyles()
        : m_titleHeight(c_titleHeightDefault)
        , m_titleBackgroundColor(c_titleBackgroundColorDefault)
        , m_titleFontSize(c_titleFontSizeDefault)
        , m_titleLabelShift(c_titleLabelShiftDefault)
        , m_bodyBackgroundColor(c_bodyBackgroundColorDefault)
        , m_bodySubBackgroundColor(c_bodySubBackgroundColorDefault)
        , m_inspectorPropertyFontSize(c_inspectorPropertyFontSizeDefault)
        , m_inspectorPropertyColor(c_inspectorPropertyColorDefault)
        , m_hierarchyViewport(c_hierarchyViewportDefault)
        , m_inspectorViewport(c_inspectorViewportDefault)
        , m_assetsViewport(c_assetsViewportDefault)
        , m_sceneViewport(c_sceneViewportDefault)
        , m_topBarViewport(c_topBarViewportDefault)
        , m_listObjectTextColorDefault(c_listObjectTextColorDefault)
        , m_listObjectTextColorSelected(c_listObjectTextColorSelected)
        , m_listObjectTextColorInactive(c_listObjectTextColorInactive)
        , m_listObjectBackgroundColorDefault(c_listObjectBackgroundColorDefault)
        , m_listObjectBackgroundColorFocused(c_listObjectBackgroundColorFocused)
        , m_listObjectBackgroundColorSelected(c_listObjectBackgroundColorSelected)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorToolsStyles::~EditorToolsStyles()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EditorToolsStyles::Initialize(EditorToolsStylesPtr& _editorToolsStyles)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorToolsStyles, _editorToolsStyles, init());
    }

    //////////////////////////////////////////
    bool EditorToolsStyles::init()
    {
        return true;
    }

    //////////////////////////////////////////
    bool EditorToolsStyles::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        m_defaultFontMaterialName = _dataBlock.getString(MAZE_HCS("defaultFontMaterial"));
        m_defaultFontMaterial = FontMaterialManager::GetInstancePtr()->getOrLoadFontMaterial(m_defaultFontMaterialName);

        m_defaultBoldFontMaterialName = _dataBlock.getString(MAZE_HCS("defaultBoldFontMaterial"));
        m_defaultBoldFontMaterial = FontMaterialManager::GetInstancePtr()->getOrLoadFontMaterial(m_defaultBoldFontMaterialName);

        return true;
    }


    //////////////////////////////////////////
    void EditorToolsStyles::loadStyles(Path const& _fileName)
    {           
        AssetFilePtr const& stylesAssetFile = AssetManager::GetInstancePtr()->getAssetFile(_fileName);
        if (stylesAssetFile)
        {
            DataBlock dataBlock;
            ByteBufferPtr byteBuffer = stylesAssetFile->readAsByteBuffer();
            dataBlock.loadFromByteBuffer(*byteBuffer.get());
            loadFromDataBlock(dataBlock);
        }
    }
        

} // namespace Maze
//////////////////////////////////////////
