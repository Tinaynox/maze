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
#pragma once
#if (!defined(_MazePhysicsMaterial2D_hpp_))
#define _MazePhysicsMaterial2D_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/serialization/MazeXMLSerializable.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include <box2d/box2d.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PhysicsMaterial2D);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Class PhysicsMaterial2D
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API PhysicsMaterial2D
        : public SharedObject<PhysicsMaterial2D>
        , public IXMLElementSerializable
        , public IDataBlockSerializable
        , public MultiDelegateCallbackReceiver
        , public ISharedCopyable<PhysicsMaterial2D>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(PhysicsMaterial2D);

    public:

        //////////////////////////////////////////
        virtual ~PhysicsMaterial2D();

        //////////////////////////////////////////
        static PhysicsMaterial2DPtr Create(
            F32 _density = 0.75f,
            F32 _friction = 0.25f,
            F32 _restitution = 0.35f);

        //////////////////////////////////////////
        static PhysicsMaterial2DPtr Create(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        static PhysicsMaterial2DPtr Create(String const& _assetFileName);

        //////////////////////////////////////////
        static PhysicsMaterial2DPtr Create(PhysicsMaterial2DPtr const& _material);


        //////////////////////////////////////////
        virtual PhysicsMaterial2DPtr createCopy() MAZE_OVERRIDE
        {
            return PhysicsMaterial2D::Create(getSharedPtr());
        }


        //////////////////////////////////////////
        bool loadFromXMLDocument(tinyxml2::XMLDocument& _doc);

        //////////////////////////////////////////
        bool loadFromAssetFile(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        bool loadFromAssetFile(String const& _assetFileName);


        //////////////////////////////////////////
        inline F32 getDensity() const { return m_density; }

        //////////////////////////////////////////
        inline void setDensity(F32 _density) { m_density = _density; }


        //////////////////////////////////////////
        inline F32 getFriction() const { return m_friction; }

        //////////////////////////////////////////
        inline void setFriction(F32 _friction) { m_friction = _friction; }


        //////////////////////////////////////////
        inline F32 getRestitution() const { return m_restitution; }

        //////////////////////////////////////////
        inline void setRestitution(F32 _restitution) { m_restitution = _restitution; }


        //////////////////////////////////////////
        inline void setName(HashedString const& _name) { m_name = _name; }

        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }


    public:

        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual void loadFromXMLElement(tinyxml2::XMLElement const* _element) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual tinyxml2::XMLElement* toXMLElement(tinyxml2::XMLDocument& _doc) const MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        static void FromString(PhysicsMaterial2DPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(PhysicsMaterial2D const* _value, String& _data);

    protected:

        //////////////////////////////////////////
        PhysicsMaterial2D();

        //////////////////////////////////////////
        bool init(
            F32 _density = 0.75f,
            F32 _friction = 0.25f,
            F32 _restitution = 0.35f);

        //////////////////////////////////////////
        virtual bool init(PhysicsMaterial2DPtr const& _material);

    protected:
        HashedString m_name;
        F32 m_density;
        F32 m_friction;
        F32 m_restitution;
    };


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<PhysicsMaterial2DPtr>::value), void>::type
        ValueToString(PhysicsMaterial2DPtr const& _value, String& _data)
    {
        PhysicsMaterial2D::ToString(_value.get(), _data);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<PhysicsMaterial2DPtr>::value), void>::type
        ValueFromString(PhysicsMaterial2DPtr& _value, CString _data, Size _count)
    {
        PhysicsMaterial2D::FromString(_value, _data, _count);
    }


    //////////////////////////////////////////
    // Class MaterialAssetRef
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API PhysicsMaterial2DAssetRef
        : public IStringSerializable
        , public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        PhysicsMaterial2DAssetRef(PhysicsMaterial2DPtr const& _value = nullptr)
            : m_material(_value)
        {}

        //////////////////////////////////////////
        void setMaterial(PhysicsMaterial2DPtr const& _value) { m_material = _value; }

        //////////////////////////////////////////
        inline PhysicsMaterial2DPtr const& getMaterial() const { return m_material; }

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline bool operator==(PhysicsMaterial2DAssetRef const& _value) const { return m_material == _value.getMaterial(); }

        //////////////////////////////////////////
        inline bool operator!=(PhysicsMaterial2DAssetRef const& _value) const { return m_material != _value.getMaterial(); }

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    private:
        PhysicsMaterial2DPtr m_material;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysicsMaterial2D_hpp_
//////////////////////////////////////////
