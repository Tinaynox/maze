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
#include "MazeCoreHeader.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeXMLHelper.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"
#include "maze-core/helpers/MazeByteBufferHelper.hpp"
#include <tinyxml2.h>

#if (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)
#   include "maze-core/helpers/emscripten/MazeFileHelperEmscripten.hpp"
#endif


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class SettingsManager
    //
    //////////////////////////////////////////
    SettingsManager* SettingsManager::s_instance = nullptr;

    //////////////////////////////////////////
    SettingsManager::SettingsManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SettingsManager::~SettingsManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void SettingsManager::Initialize(SettingsManagerPtr& _updateManager, String const& _projectName)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SettingsManager, _updateManager, init(_projectName));
    }

    //////////////////////////////////////////
    bool SettingsManager::init(String const& _projectName)
    {
        m_projectName = _projectName;
#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        m_settingsFileFullPath = FileHelper::GetDocumentsDirectory();
#else
        m_settingsFileFullPath = FileHelper::GetDocumentsDirectory() + "/" + _projectName;
#endif

#if !(MAZE_PRODUCTION)
        m_settingsFileFullPath += "/.dev";
#endif
#if (MAZE_DEBUG)
        m_settingsFileFullPath += "/.debug";
#endif

        m_settingsFileFullPath += "/settings.mzdata";
        return true;
    }

    //////////////////////////////////////////
    Pair<MetaClass* const, SettingsPtr>* SettingsManager::getSettings(String const& _settingsClassName)
    {
        return getSettings(HashedCString(_settingsClassName.c_str()));
    }

    //////////////////////////////////////////
    Pair<MetaClass* const, SettingsPtr>* SettingsManager::getSettings(HashedCString const& _settingsClassName)
    {
        for (auto& settings : m_settings)
        {
            MetaClass* settingsMetaClass = settings.first;

            if (_settingsClassName == settingsMetaClass->getName())
            {
                return &settings;
            }
        }

        return nullptr;
    }

    //////////////////////////////////////////
    bool SettingsManager::loadSettings()
    {
        processBackCompatibility();

        return loadSettings(m_settingsFileFullPath);
    }

    //////////////////////////////////////////
    bool SettingsManager::loadSettings(Path const& _path)
    {
        MAZE_PROFILE_EVENT("SettingsManager::loadSettings");

        if (_path.empty())
            return false;
        
        Debug::Log("Loading settings file %s'...", _path.toUTF8().c_str());

        ByteBuffer fileBuffer;
        if (!ByteBufferHelper::LoadBinaryFile(fileBuffer, _path))
            return false;
        Char header[5];
        fileBuffer.read(0, header, sizeof(header));

        if (strstr(header, "xml") != nullptr)
        {
            Debug::LogWarning("Obsolete settings file format - %s", _path.toUTF8().c_str());

            tinyxml2::XMLDocument doc;
            tinyxml2::XMLError loadError = doc.Parse((CString)fileBuffer.getDataPointer(), fileBuffer.getSize());

            MAZE_ERROR_RETURN_VALUE_IF(
                tinyxml2::XML_SUCCESS != loadError,
                false,
                "File '%s' loading error - XMLError: %d!", _path.toUTF8().c_str(), (S32)loadError);

            return loadSettingsFromXMLDocument(doc);
        }
        else
        {
            DataBlock dataBlock;
            dataBlock.loadFromByteBuffer(fileBuffer);
            return loadSettingsFromDataBlock(dataBlock);
        }
    }

    //////////////////////////////////////////
    bool SettingsManager::loadSettingsFromXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        tinyxml2::XMLNode* rootNode = _doc.FirstChild();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node!", m_settingsFileFullPath.toUTF8().c_str());
            return false;
        }

        rootNode = rootNode->NextSibling();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node children!", m_settingsFileFullPath.toUTF8().c_str());
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

            CString const settingsName = settingsMetaClassElement->Name();
            auto* settings = getSettings(MAZE_HASHED_CSTRING(settingsName));

            tinyxml2::XMLNode* settingsElementNode = settingsNode->FirstChild();
            while (settingsElementNode)
            {
                tinyxml2::XMLElement* settingsElement = settingsElementNode->ToElement();

                if (!settingsElement)
                {
                    settingsElementNode = settingsElementNode->NextSibling();
                    continue;
                }

                String key = settingsElement->Attribute("key");
                String value = settingsElement->Attribute("value");

                if (settings)
                {
                    MetaClass* const settingsMetaClass = settings->first;
                    MetaInstance settingsMetaInstance = settings->second->getMetaInstance();
                    MetaProperty* metaProperty = settingsMetaClass->getProperty(key.c_str());
                    if (metaProperty)
                        metaProperty->setString(settingsMetaInstance, value);
                }
                else
                {
                    //m_unregisteredSettings[settingsName].emplace_back(
                    //    Pair<String, String>(key, value));
                }

                settingsElementNode = settingsElementNode->NextSibling();
            }

            settingsNode = settingsNode->NextSibling();
        }

        Debug::Log("Settings file '%s' loaded", m_settingsFileFullPath.toUTF8().c_str());

        return true;
    }

    //////////////////////////////////////////
    bool SettingsManager::loadSettingsFromDataBlock(DataBlock const& _dataBlock)
    {
        for (DataBlock const* settingsMetaClassBlock : _dataBlock)
        {
            HashedCString const& settingsName = settingsMetaClassBlock->getName();
            auto* settings = getSettings(settingsName);

            if (settings)
            {
                //DataBlockHelper::DeserializeMetaInstanceFromDataBlock(settings->second->getMetaClass(), settings->second->getMetaInstance(), *settingsMetaClassBlock);
                settings->second->loadFromDataBlock(*settingsMetaClassBlock);
            }
            else
            {
                m_unregisteredSettings[settingsName] = *settingsMetaClassBlock;
            }
        }

        return true;
    }

    //////////////////////////////////////////
    bool SettingsManager::saveSettings()
    {
        MAZE_PROFILE_EVENT("SettingsManager::saveSettings");

        return saveSettingsAsDataBlock();
    }

    //////////////////////////////////////////
    bool SettingsManager::saveSettingsAsDataBlock()
    {
        if (m_settingsFileFullPath.empty())
            return false;

        String settingsFileDirectoryFullPath = FileHelper::GetDirectoryInPath(m_settingsFileFullPath);
        FileHelper::CreateDirectoryRecursive(settingsFileDirectoryFullPath);

        DataBlock dataBlock;

        for (auto const& settings : m_settings)
        {
            MetaClass const* settingsMetaClass = settings.first;
            //ConstMetaInstance settingsMetainstance = settings.second->getMetaInstance();

            DataBlock* settingsMetaClassBlock = dataBlock.addNewDataBlock(settingsMetaClass->getName());
            settings.second->toDataBlock(*settingsMetaClassBlock);
            //DataBlockHelper::SerializeMetaInstanceToDataBlock(settingsMetaClass, settingsMetainstance, *settingsMetaClassBlock);
        }

        for (auto const& unregisteredSettings : m_unregisteredSettings)
        {
            dataBlock.addDataBlock(HashedCString(unregisteredSettings.first.c_str()))->copyFrom(&unregisteredSettings.second);
        }

        if (!dataBlock.saveTextFile(m_settingsFileFullPath))
        {
            MAZE_ERROR("Saving settings file '%s' failed!", m_settingsFileFullPath.toUTF8().c_str());
            return false;
        }

        Debug::Log("Settings file '%s' saved.", m_settingsFileFullPath.toUTF8().c_str());

#if (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)
        Maze::FileHelper::TrySyncEmscriptenLocalStorage();
#endif

        return true;
    }

    //////////////////////////////////////////
    void SettingsManager::indentifyUnregisteredSetting(Settings* _settings)
    {
        HashedCString settingsName = HashedCString(_settings->getClassName());
        auto it = m_unregisteredSettings.find(settingsName);
        if (it == m_unregisteredSettings.end())
            return;

        MetaClass* const settingsMetaClass = _settings->getMetaClass();
        _settings->loadFromDataBlock(it->second);
        //MetaInstance settingsMetaInstance = _settings->getMetaInstance();
        //DataBlockHelper::DeserializeMetaInstanceFromDataBlock(settingsMetaClass, settingsMetaInstance, it->second);
      
        m_unregisteredSettings.erase(it);
    }

    //////////////////////////////////////////
    void SettingsManager::processBackCompatibility()
    {
#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        Path settingsFileFullPath = FileHelper::GetDocumentsDirectory();
#else
        Path settingsFileFullPath = FileHelper::GetDocumentsDirectory() + "/" + m_projectName;
#endif

#if !(MAZE_PRODUCTION)
        settingsFileFullPath += "/.dev";
#endif
#if (MAZE_DEBUG)
        settingsFileFullPath += "/.debug";
#endif

        settingsFileFullPath += "/settings.xml";

        if (FileHelper::IsFileExists(settingsFileFullPath))
        {
            // XML to DataBlock
            loadSettings(settingsFileFullPath);
            saveSettingsAsDataBlock();
            FileHelper::DeleteRegularFile(settingsFileFullPath);
        }
    }

} // namespace Maze
//////////////////////////////////////////
