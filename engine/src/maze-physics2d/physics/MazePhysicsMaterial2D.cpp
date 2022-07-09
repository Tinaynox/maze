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
#include "MazePhysics2DHeader.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-physics2d/managers/MazePhysicsMaterial2DManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PhysicsMaterial2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(PhysicsMaterial2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, density, 0.75f, getDensity, setDensity),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, friction, 0.25f, getFriction, setFriction),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, restitution, 0.35f, getRestitution, setRestitution));

    //////////////////////////////////////////
    PhysicsMaterial2D::PhysicsMaterial2D()
        : m_density(0.75f)
        , m_friction(0.25f)
        , m_restitution(0.35f)
    {
        
    }

    //////////////////////////////////////////
    PhysicsMaterial2D::~PhysicsMaterial2D()
    {

    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr PhysicsMaterial2D::Create(
        F32 _density,
        F32 _friction,
        F32 _restitution)
    {
        PhysicsMaterial2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PhysicsMaterial2D, object, init(_density, _friction, _restitution));
        return object;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr PhysicsMaterial2D::Create(AssetFilePtr const& _assetFile)
    {
        PhysicsMaterial2DPtr material = PhysicsMaterial2D::Create();

        if (material)
            material->loadFromAssetFile(_assetFile);

        return material;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr PhysicsMaterial2D::Create(String const& _assetFileName)
    {
        PhysicsMaterial2DPtr material = PhysicsMaterial2D::Create();

        if (material)
            material->loadFromAssetFile(_assetFileName);

        return material;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr PhysicsMaterial2D::Create(PhysicsMaterial2DPtr const& _material)
    {
        return _material->createCopy();
    }

    //////////////////////////////////////////
    bool PhysicsMaterial2D::init(
        F32 _density,
        F32 _friction,
        F32 _restitution)
    {
        m_density = _density;
        m_friction = _friction;
        m_restitution = _restitution;

        return true;
    }

    //////////////////////////////////////////
    bool PhysicsMaterial2D::init(PhysicsMaterial2DPtr const& _material)
    {
        if (!MetaClassHelper::CopyProperties(
            getMetaInstance(),
            _material->getMetaInstance()))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PhysicsMaterial2D::loadFromXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        tinyxml2::XMLNode* rootNode = _doc.FirstChild();

        MAZE_ERROR_RETURN_IF(!rootNode, "File loading error - empty root node!");

        rootNode = rootNode->NextSibling();
        MAZE_ERROR_RETURN_IF(!rootNode, "File loading error - empty root node children!");

        tinyxml2::XMLElement const* rootElement = rootNode->ToElement();
        MAZE_ERROR_RETURN_IF(!rootElement, "File loading error - root node is not XML element!");

        loadFromXMLElement(rootElement);
    }

    //////////////////////////////////////////
    void PhysicsMaterial2D::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        tinyxml2::XMLDocument doc;
        MAZE_LOG("Loading PhysicsMaterial2D: %s...", _assetFile->getFileName().toUTF8().c_str());
        _assetFile->readToXMLDocument(doc);
        loadFromXMLDocument(doc);
    }

    //////////////////////////////////////////
    void PhysicsMaterial2D::loadFromAssetFile(String const& _assetFileName)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        loadFromAssetFile(assetFile);
    }

    //////////////////////////////////////////
    void PhysicsMaterial2D::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        DeserializeMetaInstanceFromXMLElement(getMetaClass(), getMetaInstance(), _element);
    }

    //////////////////////////////////////////
    tinyxml2::XMLElement* PhysicsMaterial2D::toXMLElement(tinyxml2::XMLDocument& _doc) const
    {
        tinyxml2::XMLElement* element = SerializeMetaInstanceToXMLElement(getMetaClass(), getMetaInstance(), _doc);

        return element;
    }

    //////////////////////////////////////////
    void PhysicsMaterial2D::FromString(PhysicsMaterial2DPtr& _value, CString _data, Size _count)
    {
        if (!_data || strcmp(_data, "") == 0)
        {
            _value.reset();
            return;
        }

        if (_count == 0)
            _count = strlen(_data);

        if (StringHelper::IsStartsWith(_data, "ptr:"))
        {
            String data = String(_data + 4, _data + _count);
            StringHelper::StringToObjectPtr(_value, data);
        }
        else
        {
            _value = PhysicsMaterial2DManager::GetInstancePtr()->getMaterial(MAZE_HASHED_CSTRING(_data));
        }
    }

    //////////////////////////////////////////
    void PhysicsMaterial2D::ToString(PhysicsMaterial2D const* _value, String& _data)
    {
        if (!_value)
        {
            _data.clear();
            return;
        }

        String const& materialName = PhysicsMaterial2DManager::GetInstancePtr()->getMaterialName(_value);
        if (!materialName.empty())
        {
            _data = materialName;
        }
        else
        {
            StringHelper::FormatString(_data, "ptr:%p", _value);
        }
    }

} // namespace Maze
//////////////////////////////////////////
