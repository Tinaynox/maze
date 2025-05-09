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
#if (!defined(_MazeShaderUniformVariantDrawerVec4U32_hpp_))
#define _MazeShaderUniformVariantDrawerVec4U32_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderUniformVariantDrawerVec4U32);
    MAZE_USING_SHARED_PTR(EditBox2D);


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawerVec4U32
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API ShaderUniformVariantDrawerVec4U32
        : public ShaderUniformVariantDrawer
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ShaderUniformVariantDrawerVec4U32, ShaderUniformVariantDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ShaderUniformVariantDrawerVec4U32);

    public:

        //////////////////////////////////////////
        virtual ~ShaderUniformVariantDrawerVec4U32();

        //////////////////////////////////////////
        static ShaderUniformVariantDrawerVec4U32Ptr Create(
            HashedCString _shaderUniformName,
            ShaderUniformType _shaderUniformType);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderUniformVariantDrawerVec4U32();

        //////////////////////////////////////////
        virtual bool init(
            HashedCString _shaderUniformName,
            ShaderUniformType _shaderUniformType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        EditBox2DPtr createF32Edit(
            CString _name,
            Transform2DPtr const& _parent);

        //////////////////////////////////////////
        void notifyTextInput(EditBox2D* _editBox);

        //////////////////////////////////////////
        void notifySelectedChanged(EditBox2D* _editBox, bool _selected);

    protected:
        EditBox2DPtr m_editBoxX;
        EditBox2DPtr m_editBoxY;
        EditBox2DPtr m_editBoxZ;
        EditBox2DPtr m_editBoxW;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniformVariantDrawerVec4U32_hpp_
//////////////////////////////////////////
