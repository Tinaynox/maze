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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawer.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ShaderUniformVariantDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ShaderUniformVariantDrawer);

    //////////////////////////////////////////
    ShaderUniformVariantDrawer::ShaderUniformVariantDrawer()
        : m_shaderUniformType(ShaderUniformType::None)
    {
        
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawer::~ShaderUniformVariantDrawer()
    {
    }

    //////////////////////////////////////////
    bool ShaderUniformVariantDrawer::init(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        m_shaderUniformName = _shaderUniformName;
        m_shaderUniformType = _shaderUniformType;

        return true;
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawer::linkMaterials(
        Set<MaterialPtr> const& _materials)
    {
        m_materials = _materials;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariantDrawer::fetchPropertyValue(
        ShaderUniformVariant& _value,
        bool& _isMultiValue)
    {
        _isMultiValue = false;

        if (!m_materials.empty())
        {
            _value = *(*m_materials.begin())->getUniform(m_shaderUniformName).get();
            
            for (Set<MaterialPtr>::const_iterator   it = ++m_materials.begin(),
                                                    end = m_materials.end();
                                                    it != end;
                                                    ++it)
            {
                ShaderUniformVariant value = *(*it)->getUniform(m_shaderUniformName).get();
                
                if (value != _value)
                {
                    _isMultiValue = true;
                    return true;
                }
            }
        }

        return false;
    }

} // namespace Maze
//////////////////////////////////////////
