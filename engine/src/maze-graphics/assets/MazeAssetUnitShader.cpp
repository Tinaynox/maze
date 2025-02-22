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
#include "maze-graphics/assets/MazeAssetUnitShader.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitShader
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitShader, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitShader::AssetUnitShader()
    {

    }

    //////////////////////////////////////////
    AssetUnitShader::~AssetUnitShader()
    {
    }

    //////////////////////////////////////////
    AssetUnitShaderPtr AssetUnitShader::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitShaderPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitShader,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitShader::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    ShaderPtr const& AssetUnitShader::loadShader(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initShader();
            _syncLoad ? loadNow() : load();
        }

        return m_shader;
    }

    //////////////////////////////////////////
    bool AssetUnitShader::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initShader();
        if (!m_shader)
            return false;

        m_shader->loadFromAssetFile(assetFile);
        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitShader::unloadNowImpl()
    {
        if (m_shader)
        {
            
        }

        return true;
    }

    //////////////////////////////////////////
    ShaderPtr const& AssetUnitShader::initShader()
    {
        if (m_shader)
            return m_shader;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_shader;

        m_shader = Shader::Create();
        m_shader->setName(HashedString(m_data.getString(MAZE_HCS("name"), assetFile->getFileName().toUTF8())));

        if (ShaderSystem::GetCurrentInstancePtr())
        {
            ShaderLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitShaderWPtr)cast<AssetUnitShader>()](bool _syncLoad)
                {
                    if (AssetUnitShaderPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitShaderWPtr)cast<AssetUnitShader>()] (bool _syncLoad)
                {
                    if (AssetUnitShaderPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitShaderWPtr)cast<AssetUnitShader>()](bool _syncLoad)
                {
                    if (AssetUnitShaderPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitShaderWPtr)cast<AssetUnitShader>()](Set<String> const& _tags)
                {
                    if (AssetUnitShaderPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            ShaderSystem::GetCurrentInstancePtr()->addShaderToLibrary(
                m_shader,
                callbacks,
                info);
        }

        return m_shader;
    }


} // namespace Maze
//////////////////////////////////////////
