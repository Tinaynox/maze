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
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-graphics/assets/MazeAssetUnitShader.hpp"


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


        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                MAZE_HCS("shader"),
                [](AssetFilePtr const& _file, DataBlock const& _data)
                {
                    return AssetUnitShader::Create(_file, _data);
                });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
                {
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitShader>::UID())
                        _assetUnit->castRaw<AssetUnitShader>()->initShader();
                });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
                {
                    if (_extension == MAZE_HCS("mzshader"))
                    {
                        if (!_assetFile->getAssetUnit<AssetUnitShader>())
                            _assetFile->addAssetUnit(AssetUnitShader::Create(_assetFile));
                    }
                });
        }

        return true;
    }

    //////////////////////////////////////////
    ShaderSystemPtr const& ShaderSystem::GetCurrentInstancePtr()
    {
        static ShaderSystemPtr nullPointer;

        RenderSystem* renderSystem = RenderSystem::GetCurrentInstancePtr();
        if (!renderSystem)
            return nullPointer;

        return renderSystem->getShaderSystem();
    }

    //////////////////////////////////////////
    ShaderLibraryData* ShaderSystem::addShaderToLibrary(
        ShaderPtr const& _shader,
        ShaderLibraryDataCallbacks const& _callbacks)
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
            { _shader, _callbacks });
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
    void ShaderSystem::addGlobalFeature(CString _name, CString _value)
    {
        MAZE_ERROR_RETURN_IF(!_name, "Name is null!");

        m_globalFeatures[_name] = _value;
        m_globalFeaturesStringDirty = true;
    }

    //////////////////////////////////////////
    void ShaderSystem::removeGlobalFeature(CString _name)
    {
        auto it = m_globalFeatures.find(_name);
        if (it != m_globalFeatures.end())
        {
            m_globalFeatures.erase(it);
            m_globalFeaturesStringDirty = true;
        }
    }

    //////////////////////////////////////////
    String const& ShaderSystem::ensureGlobalFeaturesString()
    {
        updateGlobalShaderFeaturesString();
        return m_globalFeaturesString;
    }

    //////////////////////////////////////////
    ShaderLibraryData const* ShaderSystem::getShaderLibraryData(HashedCString _shaderName)
    {
        StringKeyMap<ShaderLibraryData>::const_iterator it = m_shadersLibrary.find(_shaderName);
        if (it != m_shadersLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystem::getShaderFromLibrary(HashedCString _shaderName)
    {
        static ShaderPtr nullShader;
        ShaderLibraryData const* shaderLibraryData = getShaderLibraryData(_shaderName);
        if (!shaderLibraryData)
            return nullShader;

        return shaderLibraryData->shader;
    }

    //////////////////////////////////////////
    ShaderPtr const& ShaderSystem::getOrLoadShader(
        HashedCString _shaderName,
        bool _syncLoad)
    {
        static ShaderPtr nullShader;

        ShaderLibraryData const* shaderLibraryData = getShaderLibraryData(_shaderName);
        if (shaderLibraryData)
        {
            if (shaderLibraryData->callbacks.requestLoad)
                shaderLibraryData->callbacks.requestLoad(_syncLoad);

            return shaderLibraryData->shader;
        }

        AssetManager* assetManager = AssetManager::GetInstancePtr();
        AssetFilePtr const& assetFile = assetManager->getAssetFileByFileName(_shaderName);
        if (assetFile)
        {
            ShaderPtr shader = Shader::CreateFromFile(getRenderSystem(), assetFile);
            if (shader)
            {
                ShaderLibraryData* data = addShaderToLibrary(shader);
                if (data)
                {
                    data->callbacks.requestReload =
                        [
                            assetFileWeak = (AssetFileWPtr)assetFile,
                            shaderWeak = (ShaderWPtr)shader
                        ](bool _immediate)
                        {
                            AssetFilePtr assetFile = assetFileWeak.lock();
                            ShaderPtr shader = shaderWeak.lock();
                            if (assetFile && shader)
                                shader->loadFromAssetFile(assetFile);
                        };
                    data->callbacks.hasAnyOfTags =
                        [assetFileWeak = (AssetFileWPtr)assetFile](Set<String> const& _tags)
                        {
                            if (AssetFilePtr assetFile = assetFileWeak.lock())
                                return assetFile->hasAnyOfTags(_tags);

                            return false;
                        };

                    return data->shader;
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

            getOrLoadShader(file->getFileName());
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

    //////////////////////////////////////////
    String ShaderSystem::buildGlobalShaderFeatures()
    {
        String result;

        for (auto const& globalFeatureData : m_globalFeatures)
            result += (String)"#define " + globalFeatureData.first + " " + globalFeatureData.second + '\n';

        return result;
    }

    //////////////////////////////////////////
    void ShaderSystem::updateGlobalShaderFeaturesString()
    {
        if (!m_globalFeaturesStringDirty)
            return;

        m_globalFeaturesString = buildGlobalShaderFeatures();
        m_globalFeaturesStringDirty = false;
    }

} // namespace Maze
//////////////////////////////////////////
