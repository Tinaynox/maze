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
#include "LevelBloomController.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/helpers/MazeColorHelper.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "main/SceneExample.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class LevelBloomController
    //
    //////////////////////////////////////////
    LevelBloomController::LevelBloomController()
        : m_sceneGame(nullptr)
        , m_currentBloomRenderBufferIndex(0)
        , m_blurIterations(5)
    {

    }

    //////////////////////////////////////////
    LevelBloomController::~LevelBloomController()
    {
        m_sceneGame->getRenderBuffer()->eventRenderBufferSizeChanged.unsubscribe(this);
        m_sceneGame->getRenderBuffer()->eventRenderBufferEndDraw.unsubscribe(this);
    }

    //////////////////////////////////////////
    LevelBloomControllerPtr LevelBloomController::Create(SceneExample* _sceneGame)
    {
        LevelBloomControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LevelBloomController, object, init(_sceneGame));
        return object;
    }

    //////////////////////////////////////////
    bool LevelBloomController::init(SceneExample* _sceneGame)
    {
        m_sceneGame = _sceneGame;
        
        RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();
        
        _sceneGame->getRenderBuffer()->eventRenderBufferSizeChanged.subscribe(this, &LevelBloomController::notifyRenderBufferSizeChanged);
        _sceneGame->getRenderBuffer()->eventRenderBufferEndDraw.subscribe(this, &LevelBloomController::notifyRenderBufferEndDraw);

        m_bloomMaterial = renderSystem->getMaterialManager()->getMaterial("Bloom00.mzmaterial");
        m_bloomBaseMapUniform = m_bloomMaterial->ensureUniform(MAZE_HCS("u_baseMap"), ShaderUniformType::UniformTexture2D);
        m_bloomBaseMapTexelSizeUniform = m_bloomMaterial->ensureUniform(MAZE_HCS("u_baseMapTexelSize"), ShaderUniformType::UniformVec2F32);
        m_bloomPrevBloomMapUniform = m_bloomMaterial->ensureUniform(MAZE_HCS("u_prevBloomMap"), ShaderUniformType::UniformTexture2D);

        updateBuffers();

        return true;
    }

    //////////////////////////////////////////
    void LevelBloomController::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void LevelBloomController::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        tinyxml2::XMLNode const* node = _element->FirstChild();
        while (node)
        {
            tinyxml2::XMLElement const* childElement = node->ToElement();
            if (!childElement)
            {
                node = node->NextSibling();
                continue;
            }


            node = node->NextSibling();
        }
    }


    //////////////////////////////////////////
    void LevelBloomController::notifyRenderBufferSizeChanged(RenderBufferPtr const& _renderBuffer)
    {
        updateBuffers();
    }

    //////////////////////////////////////////
    void LevelBloomController::notifyRenderBufferEndDraw(RenderBuffer* _renderBuffer)
    {
        Size prevRenderBufferIndex = m_currentBloomRenderBufferIndex == 0 ? 1 : 0;
        RenderBufferPtr currentRenderBuffer = m_bloomRenderBuffer[m_currentBloomRenderBufferIndex];
        RenderBufferPtr prevRenderBuffer = m_bloomRenderBuffer[prevRenderBufferIndex];

        // First downscale
        {
            Texture2D* baseMap = m_sceneGame->getRenderBuffer()->getColorTexture()->castRaw<Texture2D>();
            m_bloomBaseMapUniform->set(baseMap);
            m_bloomBaseMapTexelSizeUniform->set(1.0f / (Vec2F32)baseMap->getSize());
            currentRenderBuffer->blit(m_bloomMaterial, 0);
        }

        // Downscale
        RenderBufferPtr srcBuffer = currentRenderBuffer;
        for (Size i = 0; i < m_blurIterations - 1; ++i)
        {
            RenderBufferPtr bloomMipMapRenderBuffer = m_bloomMipMapRenderBuffers[i];

            Texture2D* baseMap = srcBuffer->getColorTexture()->castRaw<Texture2D>();
            m_bloomBaseMapUniform->set(baseMap);
            m_bloomBaseMapTexelSizeUniform->set(1.0f / (Vec2F32)baseMap->getSize());
            bloomMipMapRenderBuffer->blit(m_bloomMaterial, 1);

            srcBuffer = bloomMipMapRenderBuffer;
        }

        // Upscale
        for (Size i = 1; i < m_blurIterations - 1; ++i)
        {
            Size j = m_blurIterations - 2 - i;
            RenderBufferPtr bloomMipMapRenderBuffer = m_bloomMipMapRenderBuffers[j];

            Texture2D* baseMap = srcBuffer->getColorTexture()->castRaw<Texture2D>();
            m_bloomBaseMapUniform->set(baseMap);
            m_bloomBaseMapTexelSizeUniform->set(1.0f / (Vec2F32)baseMap->getSize());
            bloomMipMapRenderBuffer->blit(m_bloomMaterial, 2);

            srcBuffer = bloomMipMapRenderBuffer;
        }

        // Final upscale
        {
            m_bloomPrevBloomMapUniform->set(prevRenderBuffer->getColorTexture()->cast<Texture2D>());

            Texture2D* baseMap = srcBuffer->getColorTexture()->castRaw<Texture2D>();
            m_bloomBaseMapUniform->set(baseMap);
            m_bloomBaseMapTexelSizeUniform->set(1.0f / (Vec2F32)baseMap->getSize());
            currentRenderBuffer->blit(m_bloomMaterial, 3);
        }

        m_currentBloomRenderBufferIndex = prevRenderBufferIndex;
    }

    //////////////////////////////////////////
    void LevelBloomController::updateBuffers()
    {
        Vec2U32 bloomRenderBufferSize(
            m_sceneGame->getRenderBuffer()->getWidth() >> 1,
            m_sceneGame->getRenderBuffer()->getHeight() >> 1);

        for (Size i = 0; i < 2; ++i)
        {
            m_bloomRenderBuffer[i] = RenderBuffer::Create(
                {
                    bloomRenderBufferSize,
                    PixelFormat::RGBA_U8
                });
            m_bloomRenderBuffer[i]->getColorTexture()->castRaw<Texture2D>()->setMinFilter(TextureFilter::Linear);
            m_bloomRenderBuffer[i]->getColorTexture()->castRaw<Texture2D>()->setMagFilter(TextureFilter::Linear);
        }

        m_bloomMipMapRenderBuffers.clear();
        for (Size i = 0; i < m_blurIterations - 1; ++i)
        {
            RenderBufferPtr renderBuffer = RenderBuffer::Create(
            {
                { 
                    bloomRenderBufferSize.x >> (i + 1),
                    bloomRenderBufferSize.y >> (i + 1)
                },
                PixelFormat::RGBA_U8
            });
            renderBuffer->getColorTexture()->castRaw<Texture2D>()->setMinFilter(TextureFilter::Linear);
            renderBuffer->getColorTexture()->castRaw<Texture2D>()->setMagFilter(TextureFilter::Linear);
            m_bloomMipMapRenderBuffers.push_back(renderBuffer);
        }
    }


} // namespace Maze
//////////////////////////////////////////
