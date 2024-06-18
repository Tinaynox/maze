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
#if (!defined(_LevelBloomController_hpp_))
#define _LevelBloomController_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(LevelBloomController);
    MAZE_USING_SHARED_PTR(SceneExample);
    MAZE_USING_SHARED_PTR(SpaceObject);
    MAZE_USING_SHARED_PTR(Projectile);
    MAZE_USING_SHARED_PTR(RenderBuffer);


    //////////////////////////////////////////
    // Class LevelBloomController
    //
    //////////////////////////////////////////
    class LevelBloomController
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~LevelBloomController();

        //////////////////////////////////////////
        static LevelBloomControllerPtr Create(RenderBufferPtr const& _renderBuffer);


        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        void loadFromXMLElement(tinyxml2::XMLElement const* _element);

        //////////////////////////////////////////
        RenderBufferPtr const& getBloomRenderBuffer() const { return m_bloomRenderBuffer[m_currentBloomRenderBufferIndex]; }

        //////////////////////////////////////////
        Vector<RenderBufferPtr> const& getBloomMipMapRenderBuffer() const { return m_bloomMipMapRenderBuffers; }

    protected:

        //////////////////////////////////////////
        LevelBloomController();
        
        //////////////////////////////////////////
        bool init(RenderBufferPtr const& _renderBuffer);

        //////////////////////////////////////////
        void notifyPostGameplayUpdate(F32 _dt);

        //////////////////////////////////////////
        void notifyRenderBufferSizeChanged(RenderBufferPtr const& _renderBuffer);

        //////////////////////////////////////////
        void notifyRenderBufferEndDraw(RenderBuffer* _renderBuffer);

        //////////////////////////////////////////
        void updateBuffers();

    protected:
        RenderBufferPtr m_renderBuffer;

        Size m_currentBloomRenderBufferIndex;
        RenderBufferPtr m_bloomRenderBuffer[2];
        Vector<RenderBufferPtr> m_bloomMipMapRenderBuffers;

        MaterialPtr m_bloomMaterial;
        ShaderUniformVariantPtr m_bloomBaseMapUniform;
        ShaderUniformVariantPtr m_bloomBaseMapTexelSizeUniform;
        ShaderUniformVariantPtr m_bloomPrevBloomMapUniform;
        
        Size m_blurIterations;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _LevelBloomController_hpp_
//////////////////////////////////////////
