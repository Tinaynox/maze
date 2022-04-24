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
#if (!defined(_MazeShaderUniformVariantDrawer_hpp_))
#define _MazeShaderUniformVariantDrawer_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderUniformVariantDrawer);
    MAZE_USING_SHARED_PTR(Transform2D);


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawer
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API ShaderUniformVariantDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ShaderUniformVariantDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ShaderUniformVariantDrawer);

    public:

        //////////////////////////////////////////
        virtual ~ShaderUniformVariantDrawer();

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_ABSTRACT;

        //////////////////////////////////////////
        HashedString const& getShaderUniformName() const { return m_shaderUniformName; }

        //////////////////////////////////////////
        ShaderUniformType getShaderUniformType() const { return m_shaderUniformType; }


        //////////////////////////////////////////
        void linkMaterials(
            Set<MaterialPtr> const& _materials);

        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        ShaderUniformVariantDrawer();

        //////////////////////////////////////////
        virtual bool init(
            HashedCString _shaderUniformName,
            ShaderUniformType _shaderUniformType);

        //////////////////////////////////////////
        bool fetchPropertyValue(
            ShaderUniformVariant& _value,
            bool& _isMultiValue);

    protected:
        HashedString m_shaderUniformName;
        ShaderUniformType m_shaderUniformType;

        Set<MaterialPtr> m_materials;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniformVariantDrawer_hpp_
//////////////////////////////////////////
