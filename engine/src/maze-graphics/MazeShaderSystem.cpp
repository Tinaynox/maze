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
    ShaderLibraryData* ShaderSystem::addShaderToLibrary(ShaderPtr const& _shader)
    {
        if (!_shader)
            return nullptr;

        String const& name = _shader->getName();

        if (name.empty())
        {
            MAZE_WARNING("You cant add null name shader into the cache!");
            return nullptr;
        }

        StringKeyMap<ShaderLibraryData>::iterator it = m_shadersLibrary.find(name);
        if (m_shadersLibrary.end() != it)
        {
            MAZE_WARNING("Shader duplicate detected: %s!", name.c_str());
            return nullptr;
        }
        
        return m_shadersLibrary.insert(
            name,
            { _shader, nullptr });
    }

    //////////////////////////////////////////
    bool ShaderSystem::removeShaderFromLibrary(ShaderPtr const& _shader)
    {
        String const& name = _shader->getName();
        StringKeyMap<ShaderLibraryData>::iterator it = m_shadersLibrary.find(name);
        if (m_shadersLibrary.end() == it)
            return false;

        m_shadersLibrary.erase(it);
        return true;
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystem::getShaderFromLibrary(HashedCString _shaderName)
    {
        static ShaderPtr nullShader;
        StringKeyMap<ShaderLibraryData>::iterator it = m_shadersLibrary.find(_shaderName);
        if (m_shadersLibrary.end() == it)
            return nullShader;

        return (*it).second.shader;
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystem::getShader(HashedCString _shaderName)
    {
        static ShaderPtr nullShader;

        ShaderPtr const& result = getShaderFromLibrary(_shaderName);
        if (result)
            return result;

        AssetManager* assetManager = AssetManager::GetInstancePtr();
        AssetFilePtr const& assetFile = assetManager->getAssetFileByFileName(_shaderName);
        if (assetFile)
        {
            ShaderPtr shader = Shader::CreateFromFile(getRenderSystem(), assetFile);
            if (shader)
            {
                ShaderLibraryData* libraryData = addShaderToLibrary(shader);
                if (libraryData)
                {
                    libraryData->assetFile = assetFile;
                    return libraryData->shader;
                }
            }
        }

        return nullShader;
    }

    //////////////////////////////////////////
    void ShaderSystem::findAssetShadersAndAddToLibrary()
    {
        MAZE_PROFILE_EVENT("ShaderSystem::findAssetShadersAndAddToLibrary");

        AssetManager* assetManager = AssetManager::GetInstancePtr();

        Vector<AssetFilePtr> files = assetManager->getAssetFilesWithExtension("mzshader");

        addAssetFileShadersToLibrary(files);
    }

    //////////////////////////////////////////
    void ShaderSystem::addAssetFileShadersToLibrary(Vector<AssetFilePtr> const& _files)
    {
        for (AssetFilePtr const& file : _files)
        {
            if (file->getExtension() != "mzshader")
                continue;

            if (getShaderFromLibrary(file->getFileName()))
                continue;

            ShaderPtr shader = Shader::CreateFromFile(getRenderSystem(), file);
            if (shader)
            {
                ShaderLibraryData* libraryData = addShaderToLibrary(shader);
                if (libraryData)
                {
                    libraryData->assetFile = file;
                }
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
            addShaderToLibrary(createdShader);
        }
        return createdShader;
    }

    //////////////////////////////////////////
    void ShaderSystem::createBuiltinShaders()
    {
        MAZE_PROFILE_EVENT("ShaderSystem::createBuiltinShaders");

        for (BuiltinShaderType t = BuiltinShaderType(1); t < BuiltinShaderType::MAX; ++t)
            ensureBuiltinShader(t);
    }

    //////////////////////////////////////////
    void ShaderSystem::removeAssetFileShadersFromLibrary(Vector<AssetFilePtr> const& _files)
    {
        for (AssetFilePtr const& file : _files)
        {
            StringKeyMap<ShaderLibraryData>::iterator it = std::find_if(
                m_shadersLibrary.begin(),
                m_shadersLibrary.end(),
                [file](Pair<String, ShaderLibraryData> const& _a) -> bool
                {
                    if (_a.second.shader->getShaderFile() == file)
                        return true;

                    return false;
                });

            if (it != m_shadersLibrary.end())
            {
                m_shadersLibrary.erase(it);
            }
        }
    }

    //////////////////////////////////////////
    void ShaderSystem::processSystemInited()
    {
        MAZE_ERROR_RETURN_IF(m_systemInited, "ShaderSystem: Double initialization detected!");

        m_systemInited = true;

        eventSystemInited();
    }

    //////////////////////////////////////////
    void ShaderSystem::reloadShaders()
    {
        for (auto const& shaderData : m_shadersLibrary)
        {
            shaderData.second.shader->reload();
        }
    }

} // namespace Maze
//////////////////////////////////////////
