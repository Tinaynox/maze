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
#include "maze-core/ecs/MazeEcsScene.hpp"
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
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "ExampleHelper.hpp"
#include "BaseSceneExample.hpp"


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
    MAZE_USING_SHARED_PTR(LevelBloomController);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);


    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    class SceneExample
        : public BaseSceneExample
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneExample, BaseSceneExample);

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
        void addMeshPreview(
            String const& _meshName,
            String const& _materialName,
            String const& _text,
            F32 _scale);

        //////////////////////////////////////////
        void addMeshPreviewSpace();

        //////////////////////////////////////////
        virtual void updateHintText() MAZE_OVERRIDE;




        //////////////////////////////////////////
        void setMeshMovementEnabled(bool _value);


        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);


        //////////////////////////////////////////
        virtual void updateRenderTargetViewport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Rect2F getMainViewport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        Rect2F getRedCameraViewport();

        //////////////////////////////////////////
        Vec2U32 getRedRenderBufferSize();

        //////////////////////////////////////////
        Rect2F getGreenCameraViewport();

        //////////////////////////////////////////
        Vec2U32 getGreenRenderBufferSize();

        //////////////////////////////////////////
        Vec2U32 getBlueRenderBufferSize();

        //////////////////////////////////////////
        virtual void updateRenderBuffersSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void createMeshForCamera(
            Camera3DPtr const& _camera,
            String const& _material);

        //////////////////////////////////////////
        EntityPtr createTV(
            Vec3F const& _pos,
            Vec4F const& _rotation,
            Vec3F const& _scale,
            Texture2DPtr const& _texture,
            String const& _materialName);

    protected:
        RenderBufferPtr m_renderBufferRed;
        RenderBufferPtr m_renderBufferRedTV;
        Camera3DPtr m_cameraRed;
        CanvasPtr m_canvasRed;
        SpriteRenderer2DPtr m_renderColorSpriteRed;

        RenderBufferPtr m_renderBufferGreen;
        RenderBufferPtr m_renderBufferGreenTV;
        Camera3DPtr m_cameraGreen;
        CanvasPtr m_canvasGreen;
        SpriteRenderer2DPtr m_renderColorSpriteGreen;

        RenderBufferPtr m_renderBufferBlue;
        RenderBufferPtr m_renderBufferBlueTV;
        Camera3DPtr m_cameraBlue;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneExample_hpp_
//////////////////////////////////////////
