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
#if (!defined(_UnitPartRenderer_hpp_))
#define _UnitPartRenderer_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(UnitPartRenderer);
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(ShaderUniformVariant);


    //////////////////////////////////////////
    enum class UnitExtraMaterialType
    {
        None = -1,

        DamageHit,

        MAX
    };


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3(UnitPartRendererState,
        Default,
        DeathProgress,
        Death);


    //////////////////////////////////////////
    // Struct UnitExtraMaterialTimer
    //
    //////////////////////////////////////////
    struct UnitExtraMaterialTimer
    {
        //////////////////////////////////////////
        UnitExtraMaterialTimer(
            UnitExtraMaterialType _type,
            F32 _timer)
            : type(_type)
            , timer(_timer)
        {}

        UnitExtraMaterialType type;
        F32 timer;
    };


    //////////////////////////////////////////
    // Class UnitPartRenderer
    //
    //////////////////////////////////////////
    class UnitPartRenderer
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UnitPartRenderer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(UnitPartRenderer);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~UnitPartRenderer();


        //////////////////////////////////////////
        void setExtraMaterials(Vector<MaterialPtr> const& _extraMaterials) { m_extraMaterials = _extraMaterials; }

        //////////////////////////////////////////
        Vector<MaterialPtr> const& getExtraMaterials() const { return m_extraMaterials; }


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        void setState(UnitPartRendererState _state);

        //////////////////////////////////////////
        inline UnitPartRendererState getState() const { return m_state; }



        //////////////////////////////////////////
        inline void setDefaultMaterialsCount(U32 _defaultMaterialsCount) { m_defaultMaterialsCount = _defaultMaterialsCount; }

        //////////////////////////////////////////
        inline U32 getDefaultMaterialsCount() const { return m_defaultMaterialsCount; }


        //////////////////////////////////////////
        void setExtraMaterialActive(
            UnitExtraMaterialType _extraMaterialType,
            bool _active);

        //////////////////////////////////////////
        bool isExtraMaterialAvailable(UnitExtraMaterialType _extraMaterialType);

        //////////////////////////////////////////
        void increaseExtraMaterialTimer(
            UnitExtraMaterialType _extraMaterialType,
            F32 _time);

        //////////////////////////////////////////
        void disableAllExtraMaterials();



        //////////////////////////////////////////
        void setMaterials(
            MaterialPtr const& _defaultMaterial,
            MaterialPtr const& _deathMaterial);

        //////////////////////////////////////////
        MaterialPtr const& getDefaultMaterial() const { return m_defaultMaterial; }

        //////////////////////////////////////////
        void setDefaultMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        MaterialPtr const& getDeathMaterial() const { return m_deathMaterial; }

        //////////////////////////////////////////
        void setDeathMaterial(MaterialPtr const& _material);


        //////////////////////////////////////////
        virtual void prepare();

    protected:

        //////////////////////////////////////////
        UnitPartRenderer();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void updateRenderMaterials() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updateMaterial() MAZE_ABSTRACT;

    protected:
        Transform3DPtr m_transform;

        U32 m_defaultMaterialsCount;

        Vector<MaterialPtr> m_extraMaterials;    
        bool m_extraMaterialsStatus[(Size)UnitExtraMaterialType::MAX];
        List<UnitExtraMaterialTimer> m_extraMaterialTimers;

        UnitPartRendererState m_state;

        MaterialPtr m_defaultMaterial;
        MaterialPtr m_deathMaterial;
        ShaderUniformVariantPtr m_deathProgressUniform;

        F32 m_deathTimer;
        F32 m_deathTime;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _UnitPartRenderer_hpp_
//////////////////////////////////////////
