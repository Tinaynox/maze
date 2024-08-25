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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionMaterialSetUniform.hpp"
#include "maze-graphics/MazeMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionMaterialSetUniform
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorActionMaterialSetUniform, EditorAction);
 
    //////////////////////////////////////////
    EditorActionMaterialSetUniform::EditorActionMaterialSetUniform()
    {

    }
     
    //////////////////////////////////////////
    EditorActionMaterialSetUniform::~EditorActionMaterialSetUniform()
    {

    }

    //////////////////////////////////////////
    EditorActionMaterialSetUniformPtr EditorActionMaterialSetUniform::Create(
        MaterialPtr const& _material,
        HashedString const& _shaderUniformName,
        DataBlock const& _newValue)
    {
        EditorActionMaterialSetUniformPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionMaterialSetUniform, object, init(_material, _shaderUniformName, _newValue));
        return object;
    }

    //////////////////////////////////////////
    EditorActionMaterialSetUniformPtr EditorActionMaterialSetUniform::Create(
        Vector<MaterialPtr> const& _materials,
        HashedString const& _shaderUniformName,
        DataBlock const& _newValue)
    {
        EditorActionMaterialSetUniformPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionMaterialSetUniform, object, init(_materials, _shaderUniformName, _newValue));
        return object;
    }

    //////////////////////////////////////////
    bool EditorActionMaterialSetUniform::init(
        MaterialPtr const& _material,
        HashedString const& _shaderUniformName,
        DataBlock const& _newValue)
    {
        m_materials.push_back(_material);
        m_shaderUniformName = _shaderUniformName;
        m_prevValues.resize(m_materials.size());
        m_materials[0]->getUniform(m_shaderUniformName)->toDataBlock(m_prevValues[0]);
        m_newValue = _newValue;

        return true;
    }

    //////////////////////////////////////////
    bool EditorActionMaterialSetUniform::init(
        Vector<MaterialPtr> const& _materials,
        HashedString const& _shaderUniformName,
        DataBlock const& _newValue)
    {
        m_materials = _materials;
        m_shaderUniformName = _shaderUniformName;

        m_prevValues.resize(m_materials.size());
        for (Size i = 0, in = m_materials.size(); i != in; ++i)
            m_materials[i]->getUniform(m_shaderUniformName)->toDataBlock(m_prevValues[i]);
        m_newValue = _newValue;

        return true;
    }

    //////////////////////////////////////////
    void EditorActionMaterialSetUniform::merge(DataBlock const& _newValue)
    {
        m_newValue = _newValue;

        if (m_applied)
        {
            resetTimestamp();
            applyImpl();
        }
    }

    //////////////////////////////////////////
    void EditorActionMaterialSetUniform::applyImpl()
    {
        for (Size i = 0, in = m_materials.size(); i != in; ++i)
            m_materials[i]->getUniform(m_shaderUniformName)->loadFromDataBlock(m_newValue);
    }

    //////////////////////////////////////////
    void EditorActionMaterialSetUniform::revertImpl()
    {
        for (Size i = 0, in = m_materials.size(); i != in; ++i)
            m_materials[i]->getUniform(m_shaderUniformName)->loadFromDataBlock(m_prevValues[i]);
    }


} // namespace Maze
//////////////////////////////////////////
