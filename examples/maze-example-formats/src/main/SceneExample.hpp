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
#if (!defined(_SceneExample_hpp_))
#define _SceneExample_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneExample);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Rigidbody2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ExampleFPSCameraController);
    MAZE_USING_SHARED_PTR(Rotor3D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer3D);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_8(ExampleMeshRenderMode,
        Default,
        UV,
        NormalOS,
        NormalWS,
        TangentOS,
        BitangentOS,
        Diffuse,
        Color);


    //////////////////////////////////////////
    struct ExampleTextureData
    {
        MeshRendererPtr renderer;
        AssetFilePtr file;
        String textureName;
        U32 loadTime = 0u;
        MaterialPtr material;
        Texture2DPtr texture;
        SystemTextRenderer3DPtr labelRenderer;
    };


    //////////////////////////////////////////
    struct ExampleMeshData
    {
        MeshRendererPtr renderer;
        AssetFilePtr file;
        U32 loadTime = 0u;
        MaterialPtr material;
        MaterialPtr materialDebugNormalWS;
        MaterialPtr materialDebugTangentOS;
        MaterialPtr materialDebugBitangentOS;
        MaterialPtr materialDebugDiffuse;
        MaterialPtr materialDebugColor;
        Rotor3DPtr rotor;
    };


    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    class SceneExample
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneExample, ECSRenderScene);

    public:

        //////////////////////////////////////////
        static SceneExamplePtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneExample();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SceneExample();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void notifyRenderTargetResized(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void processCursorPress(Vec2DF const& _positionOS, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void processCursorDrag(Vec2DF const& _positionOS, CursorInputEvent const& _event);


        //////////////////////////////////////////
        void addTexturePreview(String const& _textureName);

        //////////////////////////////////////////
        void updateTextureLabel(ExampleTextureData const& _value);

        //////////////////////////////////////////
        void addTexturePreviewSpace();


        //////////////////////////////////////////
        void addMeshPreview(
            String const& _meshName,
            String const& _materialName,
            F32 _scale);

        //////////////////////////////////////////
        void addMeshPreviewSpace();

        //////////////////////////////////////////
        void updateHintText();


        //////////////////////////////////////////
        void setTextureMipmapsEnabled(bool _value);

        //////////////////////////////////////////
        void updateTextureMipmaps();


        //////////////////////////////////////////
        void setExampleMeshRenderMode(ExampleMeshRenderMode _mode);

        //////////////////////////////////////////
        void updateExampleMeshRenderMode();


        //////////////////////////////////////////
        void setRotorEnabled(bool _value);


        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);

    protected:
        CanvasPtr m_canvas;
        Camera3DPtr m_camera3D;

        SystemTextRenderer2DPtr m_hintText;
        
        ExampleFPSCameraControllerPtr m_fpsController;

        Vec2DF m_cursorPositionLastFrame = Vec2DF::c_zero;

        bool m_textureMipmapsEnabled = false;
        Vector<ExampleTextureData> m_textureData;
        F32 m_texturesOffset = 0.0f;

        Vector<ExampleMeshData> m_meshData;
        F32 m_meshesOffset = 0.0f;

        bool m_rotorEnabled = true;
        ExampleMeshRenderMode m_meshRenderMode = ExampleMeshRenderMode::Default;

    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneExample_hpp_
//////////////////////////////////////////
