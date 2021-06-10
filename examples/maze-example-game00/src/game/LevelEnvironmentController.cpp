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
#include "LevelEnvironmentController.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/helpers/MazeColorHelper.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "scenes/SceneGame.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static F32 const c_backgroundZ = 5.0f;


    //////////////////////////////////////////
    // Class LevelEnvironmentController
    //
    //////////////////////////////////////////
    LevelEnvironmentController::LevelEnvironmentController()
        : m_sceneGame(nullptr)
        , m_hsvLight(ColorHelper::ConvertRGBToHSV(113, 161, 255))
    {
        m_hsvLight.x -= 50.0f;
        m_hsvLight.y = 0.5f;
    }

    //////////////////////////////////////////
    LevelEnvironmentController::~LevelEnvironmentController()
    {

    }

    //////////////////////////////////////////
    LevelEnvironmentControllerPtr LevelEnvironmentController::Create(SceneGame* _sceneGame)
    {
        LevelEnvironmentControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LevelEnvironmentController, object, init(_sceneGame));
        return object;
    }

    //////////////////////////////////////////
    bool LevelEnvironmentController::init(SceneGame* _sceneGame)
    {
        m_sceneGame = _sceneGame;
        
        EntityPtr lightEntity = m_sceneGame->createEntity("Light");
        m_mainLight3D = lightEntity->createComponent<Light3D>();
        m_mainLight3D->setColor(ColorHelper::ConvertHSVToRGB(m_hsvLight));
        m_mainLight3D->getTransform()->setLocalPosition(0.0f, 3.0f, -3.0f);
        m_mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);

        {
            EntityPtr skyEntity = m_sceneGame->createEntity("Sky");
            m_skyTransform = skyEntity->createComponent<Transform3D>();
            m_skyTransform->setLocalPosition(0.0f, m_sceneGame->getCamera3D()->getTransform()->getLocalPosition().y, c_backgroundZ);
            

            MaterialPtr const& material = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("GameSky01_00.mzmaterial");

            MeshRendererPtr meshRenderer = skyEntity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager()->getDefaultQuadMesh());
            meshRenderer->setMaterial(material);

            ShaderPtr shader = meshRenderer->getMaterials().front()->getFirstRenderPass()->getShader();

            m_backgroundHueUniform = material->ensureUniform("u_hue");
            m_backgroundHueUniform->set(m_hsvLight.x);
            m_sectorsCountUniform = material->ensureUniform("u_sectorsCount");

            skyEntity->setActiveSelf(true);

            updateSkyTransform();
            
        }
        

        return true;
    }

    //////////////////////////////////////////
    void LevelEnvironmentController::update(F32 _dt)
    {
        m_hsvLight.x += _dt * 4.0f;

        m_mainLight3D->setColor(ColorHelper::ConvertHSVToRGB(m_hsvLight));
        
        m_backgroundHueUniform->set(m_hsvLight.x);
    }

    //////////////////////////////////////////
    void LevelEnvironmentController::loadFromXMLElement(tinyxml2::XMLElement const* _element)
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
    void LevelEnvironmentController::updateSkyTransform()
    {
        F32 aspectRatio = m_sceneGame->getCamera3D()->getAspectRatio();
        F32 halfFOVY = m_sceneGame->getCamera3D()->getFOV() * 0.5f;
        F32 halfFOVX = atan(aspectRatio * tan(halfFOVY));
        F32 z = -m_sceneGame->getCamera3D()->getTransform()->getLocalPosition().z + m_skyTransform->getLocalPosition().z;
        F32 y = tanf(halfFOVY) * z;
        F32 x = tanf(halfFOVX) * z;

        m_skyTransform->setLocalPosition(0.0f, m_sceneGame->getCamera3D()->getTransform()->getLocalPosition().y, c_backgroundZ);
        m_skyTransform->setLocalScale(x * 2, y * 2);

        if (m_sectorsCountUniform)
            m_sectorsCountUniform->set((x * 2) / (y * 2));
    }


} // namespace Maze
//////////////////////////////////////////
