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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinShaderType);


    //////////////////////////////////////////
    // Class ShaderSystem
    //
    //////////////////////////////////////////
    ShaderSystem::ShaderSystem()
        : m_systemInited(false)
        , m_renderSystemRaw(nullptr)
    {
    }

    //////////////////////////////////////////
    ShaderSystem::~ShaderSystem()
    {
    }

    //////////////////////////////////////////
    bool ShaderSystem::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        return true;
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystem::addShaderToCache(ShaderPtr const& _shader)
    {
        static ShaderPtr const nullShader;

        if (!_shader)
            return nullShader;

        String const& name = _shader->getName();

        if (name.empty())
        {
            MAZE_WARNING("You cant add null name shader into the cache!");
            return nullShader;
        }

        StringKeyMap<ShaderPtr>::iterator it = m_shadersCache.find(name);
        if (m_shadersCache.end() != it)
            return nullShader;
        
        ShaderPtr* shader = m_shadersCache.insert(name, _shader);
        return *shader;
    }

    //////////////////////////////////////////
    bool ShaderSystem::removeShaderFromCache(ShaderPtr const& _shader)
    {
        String const& name = _shader->getName();
        StringKeyMap<ShaderPtr>::iterator it = m_shadersCache.find(name);
        if (m_shadersCache.end() == it)
            return false;

        m_shadersCache.erase(it);
        return true;
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystem::getShaderFromCache(HashedCString _shaderName)
    {
        static ShaderPtr nullShader;
        StringKeyMap<ShaderPtr>::iterator it = m_shadersCache.find(_shaderName);
        if (m_shadersCache.end() == it)
            return nullShader;

        return (*it).second;
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystem::getShader(HashedCString _shaderName)
    {
        static ShaderPtr nullShader;

        ShaderPtr const& result = getShaderFromCache(_shaderName);
        if (result)
            return result;

        AssetManager* assetManager = AssetManager::GetInstancePtr();
        AssetFilePtr const& assetFile = assetManager->getAssetFileByFileName(_shaderName);
        if (assetFile)
        {
            ShaderPtr shader = Shader::CreateFromFile(getRenderSystem(), assetFile);
            if (shader)
                return addShaderToCache(shader);
        }

        return nullShader;
    }

    //////////////////////////////////////////
    void ShaderSystem::findAssetShadersAndAddToCache()
    {
        AssetManager* assetManager = AssetManager::GetInstancePtr();

        Vector<AssetFilePtr> files = assetManager->getAssetFilesWithExtension("mzshader");

        addAssetFileShadersToCache(files);
    }

    //////////////////////////////////////////
    void ShaderSystem::addAssetFileShadersToCache(Vector<AssetFilePtr> const& _files)
    {
        for (AssetFilePtr const& file : _files)
        {
            if (file->getExtension() != "mzshader")
                continue;

            ShaderPtr shader = Shader::CreateFromFile(getRenderSystem(), file);
            if (shader)
            {
                addShaderToCache(shader);
            }

        }
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystem::ensureBuiltinShader(BuiltinShaderType _shaderType)
    {
        ShaderPtr const& shader = getBuiltinShader(_shaderType);
        if (shader)
            return shader;

        ShaderPtr const& createdShader = createBuiltinShader(_shaderType);
        if (createdShader)
        {
            addShaderToCache(createdShader);
        }
        return createdShader;
    }

    //////////////////////////////////////////
    void ShaderSystem::createBuiltinShaders()
    {
        for (BuiltinShaderType t = BuiltinShaderType(1); t < BuiltinShaderType::MAX; ++t)
            ensureBuiltinShader(t);
    }

    //////////////////////////////////////////
    void ShaderSystem::removeAssetFileShadersFromCache(Vector<AssetFilePtr> const& _files)
    {
        for (AssetFilePtr const& file : _files)
        {
            StringKeyMap<ShaderPtr>::iterator it = std::find_if(
                m_shadersCache.begin(),
                m_shadersCache.end(),
                [file](Pair<String, ShaderPtr> const& _a) -> bool
                {
                    if (_a.second->getShaderFile() == file)
                        return true;

                    return false;
                });

            if (it != m_shadersCache.end())
            {
                m_shadersCache.erase(it);
            }
        }
    }

    //////////////////////////////////////////
    void ShaderSystem::processSystemInited()
    {
        MAZE_ERROR_RETURN_IF(m_systemInited, "Double initialization detected!");

        m_systemInited = true;

        eventSystemInited();
    }

} // namespace Maze
//////////////////////////////////////////
