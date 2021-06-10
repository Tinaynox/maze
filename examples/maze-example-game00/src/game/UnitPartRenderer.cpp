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
#include "UnitPartRenderer.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"
#include "maze-graphics/MazeShaderUniform.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(UnitPartRendererState);


    //////////////////////////////////////////
    // Class UnitPartRenderer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UnitPartRenderer, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<MaterialPtr>, extraMaterials, Vector<MaterialPtr>(), getExtraMaterials, setExtraMaterials),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U32, defaultMaterialsCount, 0, getDefaultMaterialsCount, setDefaultMaterialsCount),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(UnitPartRendererState, state, UnitPartRendererState::Default, getState, setState),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MaterialPtr, defaultMaterial, MaterialPtr(), getDefaultMaterial, setDefaultMaterial),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MaterialPtr, deathMaterial, MaterialPtr(), getDeathMaterial, setDeathMaterial));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(UnitPartRenderer);

    //////////////////////////////////////////
    UnitPartRenderer::UnitPartRenderer()
        : m_defaultMaterialsCount(0)
        , m_extraMaterialsStatus{ false }
        , m_state(UnitPartRendererState::Default)
        , m_deathTimer(0.0f)
        , m_deathTime(1.5f)
    {
    }

    //////////////////////////////////////////
    UnitPartRenderer::~UnitPartRenderer()
    {

    }
    
    //////////////////////////////////////////
    bool UnitPartRenderer::init()
    {

        return true;
    }    

    //////////////////////////////////////////
    void UnitPartRenderer::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
    }

    //////////////////////////////////////////
    void UnitPartRenderer::update(F32 _dt)
    {
        auto timerIt = m_extraMaterialTimers.begin();
        while (timerIt != m_extraMaterialTimers.end())
        {
            timerIt->timer -= _dt;
            if (timerIt->timer <= 0.0f)
            {
                setExtraMaterialActive(timerIt->type, false);
                timerIt = m_extraMaterialTimers.erase(timerIt);
            }
            else
            {
                ++timerIt;
            }
        }

        if (m_state == UnitPartRendererState::DeathProgress)
        {
            m_deathTimer += _dt;
            if (m_deathTimer >= m_deathTime)
            {
                setState(UnitPartRendererState::Death);
            }
            else
            {
                F32 deathProgress = Math::Clamp(m_deathTimer / m_deathTime, 0.0f, 1.0f);
                m_deathProgressUniform->set(pow(deathProgress, 0.35f));
            }
        }
    }

    //////////////////////////////////////////
    void UnitPartRenderer::setState(UnitPartRendererState _state)
    {
        if (m_state == _state)
            return;

        m_state = _state;

        updateMaterial();
    }

    //////////////////////////////////////////
    void UnitPartRenderer::prepare()
    {
        m_deathTimer = 0.0f;

        setState(UnitPartRendererState::Default);

        updateMaterial();
    }

    //////////////////////////////////////////
    void UnitPartRenderer::setExtraMaterialActive(
        UnitExtraMaterialType _extraMaterialType,
        bool _active)
    {
        if (m_extraMaterialsStatus[(Size)_extraMaterialType] == _active)
            return;

        m_extraMaterialsStatus[(Size)_extraMaterialType] = _active;

        updateRenderMaterials();
    }

    //////////////////////////////////////////
    bool UnitPartRenderer::isExtraMaterialAvailable(UnitExtraMaterialType _extraMaterialType)
    {
        if ((Size)_extraMaterialType < m_extraMaterials.size())
            return false;

        return true;
    }

    //////////////////////////////////////////
    void UnitPartRenderer::increaseExtraMaterialTimer(
        UnitExtraMaterialType _extraMaterialType,
        F32 _time)
    {
        if (_time <= 0.0f)
            return;

        for (UnitExtraMaterialTimer extraMaterialTimer : m_extraMaterialTimers)
        {
            if (extraMaterialTimer.type == _extraMaterialType)
            {
                extraMaterialTimer.timer += _time;
                return;
            }
        }

        setExtraMaterialActive(_extraMaterialType, true);
        m_extraMaterialTimers.emplace_back( UnitExtraMaterialTimer{ _extraMaterialType, _time } );
    }

    //////////////////////////////////////////
    void UnitPartRenderer::disableAllExtraMaterials()
    {
        m_extraMaterialTimers.clear();

        for (UnitExtraMaterialType type = UnitExtraMaterialType(1); type < UnitExtraMaterialType::MAX; type = UnitExtraMaterialType((Size)type + 1))
        {
            setExtraMaterialActive(type, false);
        }
    }

    //////////////////////////////////////////
    void UnitPartRenderer::setMaterials(
        MaterialPtr const& _defaultMaterial,
        MaterialPtr const& _deathMaterial)
    {
        setDefaultMaterial(_defaultMaterial);
        setDeathMaterial(_deathMaterial);
        updateMaterial();
    }

    //////////////////////////////////////////
    void UnitPartRenderer::setDefaultMaterial(MaterialPtr const& _material)
    {
        if (m_defaultMaterial == _material)
            return;

        m_defaultMaterial = _material;

        updateMaterial();
    }

    //////////////////////////////////////////
    void UnitPartRenderer::setDeathMaterial(MaterialPtr const& _material)
    {
        m_deathMaterial = _material->createCopy();

        m_deathProgressUniform = m_deathMaterial->getUniform("u_dissolveProgress");

        updateMaterial();
    }


} // namespace Maze
//////////////////////////////////////////
