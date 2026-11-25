//////////////////////////////////////////
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
#if (!defined(_MazeOutlineController_hpp_))
#define _MazeOutlineController_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngineHeader.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderBuffer.hpp"
#include "maze-graphics/MazeGlobalShaderUniform.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(OutlineController);
    MAZE_USING_SHARED_PTR(Camera3D);


    //////////////////////////////////////////
    // Class OutlineController
    //
    //////////////////////////////////////////
    class MAZE_ENGINE_API OutlineController
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~OutlineController();

        //////////////////////////////////////////
        static OutlineControllerPtr Create(
            EcsScene* _scene,
            Transform2DPtr const& _compositeParent,
            Vec2U32 const& _size,
            HashedCString _compositeMaterialName);


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        void setSourceCamera(Camera3DPtr const& _camera);

        //////////////////////////////////////////
        void setSceneDepthBuffer(RenderBufferPtr const& _renderBuffer);

        //////////////////////////////////////////
        void setSize(Vec2U32 const& _size);

        //////////////////////////////////////////
        void setOrder(S32 _order);


        //////////////////////////////////////////
        inline RenderBufferPtr const& getMaskRenderBuffer() const { return m_maskRenderBuffer; }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getCompositeSprite() const { return m_compositeSprite; }

    protected:

        //////////////////////////////////////////
        OutlineController();

        //////////////////////////////////////////
        bool init(
            EcsScene* _scene,
            Transform2DPtr const& _compositeParent,
            Vec2U32 const& _size,
            HashedCString _compositeMaterialName);

        //////////////////////////////////////////
        void ensureMaskCamera();

        //////////////////////////////////////////
        bool isMaskEnabled() const;

        //////////////////////////////////////////
        void updateSceneDepthUniforms();

        //////////////////////////////////////////
        void updateCompositeUniforms();

        //////////////////////////////////////////
        void notifyMaskRenderBufferEndDraw(RenderBuffer* _renderBuffer);

    protected:
        EcsScene* m_scene = nullptr;

        RenderBufferPtr m_maskRenderBuffer;
        RenderBufferPtr m_sceneDepthBuffer;

        Camera3DPtr m_sourceCamera;
        Camera3DPtr m_maskCamera;

        SpriteRenderer2DPtr m_compositeSprite;

        bool m_maskHasContent = false;

        GlobalShaderUniformPtr m_sceneDepthMapUniform;
        GlobalShaderUniformPtr m_sceneDepthParamsUniform;
        GlobalShaderUniformPtr m_screenSpaceUniform;
        GlobalShaderUniformPtr m_maskEnabledUniform;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeOutlineController_hpp_
//////////////////////////////////////////
