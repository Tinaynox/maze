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
#if (!defined(_MazeShaderUniformVariantDrawerVec4DF_hpp_))
#define _MazeShaderUniformVariantDrawerVec4DF_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderUniformVariantDrawerVec4DF);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawerVec4DF
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API ShaderUniformVariantDrawerVec4DF
        : public ShaderUniformVariantDrawer
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ShaderUniformVariantDrawerVec4DF, ShaderUniformVariantDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ShaderUniformVariantDrawerVec4DF);

    public:

        //////////////////////////////////////////
        virtual ~ShaderUniformVariantDrawerVec4DF();

        //////////////////////////////////////////
        static ShaderUniformVariantDrawerVec4DFPtr Create(
            HashedCString _shaderUniformName,
            ShaderUniformType _shaderUniformType);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderUniformVariantDrawerVec4DF();

        //////////////////////////////////////////
        virtual bool init(
            HashedCString _shaderUniformName,
            ShaderUniformType _shaderUniformType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        SystemTextEditBox2DPtr createF32Edit(
            CString _name,
            Transform2DPtr const& _parent);

        //////////////////////////////////////////
        void notifyTextInput(SystemTextEditBox2D* _editBox);

        //////////////////////////////////////////
        void notifySelectedChanged(SystemTextEditBox2D* _editBox, bool _selected);

    protected:
        SystemTextEditBox2DPtr m_editBoxX;
        SystemTextEditBox2DPtr m_editBoxY;
        SystemTextEditBox2DPtr m_editBoxZ;
        SystemTextEditBox2DPtr m_editBoxW;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniformVariantDrawerVec4DF_hpp_
//////////////////////////////////////////
