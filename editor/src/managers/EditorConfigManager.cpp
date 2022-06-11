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
#include "EditorConfigManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
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
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "Editor.hpp"
#include "settings/MazeEditorSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorConfigManager
    //
    //////////////////////////////////////////
    EditorConfigManager* EditorConfigManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorConfigManager::EditorConfigManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorConfigManager::~EditorConfigManager()
    {
        s_instance = nullptr;

        
    }

    //////////////////////////////////////////
    void EditorConfigManager::Initialize(EditorConfigManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorConfigManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorConfigManager::init()
    {
       
        return true;
    }

    //////////////////////////////////////////
    bool EditorConfigManager::loadConfig()
    {
        m_configs.clear();

        String editorConfigFullPath = FileHelper::GetWorkingDirectory() + "/config.xml";

        Debug::Log("Loading editor config %s'...", editorConfigFullPath.c_str());

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError loadError = doc.LoadFile(editorConfigFullPath.c_str());
        if (tinyxml2::XML_SUCCESS != loadError)
        {
            if (tinyxml2::XML_ERROR_FILE_NOT_FOUND == loadError)
                return false;

            MAZE_ERROR("File '%s' loading error - XMLError: %d!", editorConfigFullPath.c_str(), (S32)loadError);
            return false;
        }

        tinyxml2::XMLNode* rootNode = doc.FirstChild();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node!", editorConfigFullPath.c_str());
            return false;
        }

        rootNode = rootNode->NextSibling();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node children!", editorConfigFullPath.c_str());
            return false;
        }

        tinyxml2::XMLNode* settingsNode = rootNode->FirstChild();
        while (settingsNode)
        {
            tinyxml2::XMLElement* settingsMetaClassElement = settingsNode->ToElement();

            if (!settingsMetaClassElement)
            {
                settingsNode = settingsNode->NextSibling();
                continue;
            }

            String key = settingsMetaClassElement->Attribute("key");
            String value = settingsMetaClassElement->Attribute("value");

            m_configs[key] = value;

            settingsNode = settingsNode->NextSibling();
        }

        Debug::Log("Editor config '%s' loaded", editorConfigFullPath.c_str());

        applyConfigs();

        return true;
    }

    //////////////////////////////////////////
    void EditorConfigManager::applyConfigs()
    {
        EditorSettings* editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();

        Vector<String> extensions;
        String const& additionalPrefabExtensions = getConfig("additionalPrefabExtensions");
        StringHelper::SplitWords(additionalPrefabExtensions, extensions, ',');
        for (String const& extension : extensions)
            AssetEditorToolsManager::GetInstancePtr()->addPrefabExtension(extension);

        String const& editorMode = getConfig("editorMode");
        if (!editorMode.empty())
            editorSettings->setEditorMode(EditorMode::FromString(editorMode));

        String const& assetsFullPath = getConfig("assetsFullPath");
        if (!assetsFullPath.empty())
            editorSettings->setAssetsFullPath(FileHelper::NormalizedFilePath(assetsFullPath));
    }

    //////////////////////////////////////////
    String const& EditorConfigManager::getConfig(String const& _key)
    {
        static String const nullValue;

        Map<String, String>::iterator it = m_configs.find(_key);
        if (it == m_configs.end())
            return nullValue;

        return it->second;
    }

} // namespace Maze
//////////////////////////////////////////
