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
#include <tinyxml2.h>


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
        m_settingsFileFullPath = FileHelper::GetDocumentsDirectory() + "/" + _projectName;

#if !(MAZE_PRODUCTION)
        m_settingsFileFullPath += "/.dev";
#endif
#if (MAZE_DEBUG)
        m_settingsFileFullPath += "/.debug";
#endif

        m_settingsFileFullPath += "/settings.xml";
        return true;
    }

    //////////////////////////////////////////
    Pair<MetaClass* const, SettingsPtr>* SettingsManager::getSettings(String const& _settingsClassName)
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
        MAZE_PROFILE_EVENT("SettingsManager::loadSettings");

        if (m_settingsFileFullPath.empty())
            return false;
        
        Debug::Log("Loading settings file %s'...", m_settingsFileFullPath.toUTF8().c_str());

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError loadError = XMLHelper::LoadXMLFile(m_settingsFileFullPath, doc);
        if (tinyxml2::XML_SUCCESS != loadError)
        {
            if (tinyxml2::XML_ERROR_FILE_NOT_FOUND == loadError)
                return false;

            MAZE_ERROR("File '%s' loading error - XMLError: %d!", m_settingsFileFullPath.toUTF8().c_str(), (S32)loadError);
            return false;
        }

        tinyxml2::XMLNode* rootNode = doc.FirstChild();
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
            auto* settings = getSettings(settingsName);
            
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
                    m_unregisteredSettings[settingsName].emplace_back(
                        Pair<String, String>(key, value));
                }

                settingsElementNode = settingsElementNode->NextSibling();
            }
  
            settingsNode = settingsNode->NextSibling();
        }

        Debug::Log("Settings file '%s' loaded", m_settingsFileFullPath.toUTF8().c_str());

        return true;
    }

    //////////////////////////////////////////
    bool SettingsManager::saveSettings()
    {
        MAZE_PROFILE_EVENT("SettingsManager::saveSettings");

        if (m_settingsFileFullPath.empty())
            return false;

        String settingsFileDirectoryFullPath = FileHelper::GetDirectoryInPath(m_settingsFileFullPath);
        FileHelper::CreateDirectoryRecursive(settingsFileDirectoryFullPath);

        tinyxml2::XMLDocument doc;

        tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
        doc.LinkEndChild(decl);

        tinyxml2::XMLNode* root = doc.NewElement("Settings");
        doc.InsertEndChild(root);

        for (auto const& settings : m_settings)
        {
            MetaClass* settingsMetaClass = settings.first;
            MetaInstance settingsMetainstance = settings.second->getMetaInstance();

            tinyxml2::XMLElement* settingsMetaClassElement = doc.NewElement(settingsMetaClass->getName());

            for (S32 i = 0; i < settingsMetaClass->getPropertiesCount(); ++i)
            {
                Maze::MetaProperty* metaProperty = settingsMetaClass->getProperty(i);

                Maze::CString propertyName = metaProperty->getName();
                String propertyValue = metaProperty->toString(settingsMetainstance);

                tinyxml2::XMLElement* settingsElement = doc.NewElement("Property");
                settingsElement->SetAttribute("key", propertyName);
                settingsElement->SetAttribute("value", propertyValue.c_str());
                settingsMetaClassElement->InsertEndChild(settingsElement);
            }            

            root->InsertEndChild(settingsMetaClassElement);
        }

        for (auto const& unregisteredSettings : m_unregisteredSettings)
        {
            tinyxml2::XMLElement* settingsMetaClassElement = doc.NewElement(unregisteredSettings.first.c_str());
            
            for (auto const& property : unregisteredSettings.second)
            {
                tinyxml2::XMLElement* settingsElement = doc.NewElement("Property");
                settingsElement->SetAttribute("key", property.first.c_str());
                settingsElement->SetAttribute("value", property.second.c_str());
                settingsMetaClassElement->InsertEndChild(settingsElement);
            }
        }

        tinyxml2::XMLError loadError = XMLHelper::SaveXMLFile(m_settingsFileFullPath, doc);
        if (tinyxml2::XML_SUCCESS != loadError)
        {
            MAZE_ERROR("Saving settings file '%s' error - %d!", m_settingsFileFullPath.toUTF8().c_str(), loadError);
            return false;
        }

        Debug::Log("Settings file '%s' saved.", m_settingsFileFullPath.toUTF8().c_str());

        return true;
    }

    //////////////////////////////////////////
    void SettingsManager::indentifyUnregisteredSetting(Settings* _settings)
    {
        CString settingsName = _settings->getClassName();
        auto it = m_unregisteredSettings.find(settingsName);
        if (it == m_unregisteredSettings.end())
            return;

        for (auto& property : it->second)
        {
            MetaClass* const settingsMetaClass = _settings->getMetaClass();
            MetaInstance settingsMetaInstance = _settings->getMetaInstance();
            MetaProperty* metaProperty = settingsMetaClass->getProperty(property.first.c_str());
            if (metaProperty)
                metaProperty->setString(settingsMetaInstance, property.second);
        }

        m_unregisteredSettings.erase(it);
    }

} // namespace Maze
//////////////////////////////////////////
