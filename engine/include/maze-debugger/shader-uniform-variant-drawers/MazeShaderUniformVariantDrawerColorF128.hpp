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
#if (!defined(_MazeShaderUniformVariantDrawerColorF128_hpp_))
#define _MazeShaderUniformVariantDrawerColorF128_hpp_


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
    MAZE_USING_SHARED_PTR(ShaderUniformVariantDrawerColorF128);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(ColorEdit2D);
    MAZE_USING_SHARED_PTR(ColorHDREdit2D);


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawerColorF128
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API ShaderUniformVariantDrawerColorF128
        : public ShaderUniformVariantDrawer
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ShaderUniformVariantDrawerColorF128, ShaderUniformVariantDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ShaderUniformVariantDrawerColorF128);

    public:

        //////////////////////////////////////////
        virtual ~ShaderUniformVariantDrawerColorF128();

        //////////////////////////////////////////
        static ShaderUniformVariantDrawerColorF128Ptr Create(
            String const& _shaderUniformName,
            ShaderUniformType _shaderUniformType);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderUniformVariantDrawerColorF128();

        //////////////////////////////////////////
        virtual bool init(
            String const& _shaderUniformName,
            ShaderUniformType _shaderUniformType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyColorChanged(ColorHDREdit2D* _colorEdit, ColorF128 const& _color);

    protected:
        ColorHDREdit2DPtr m_colorEdit;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniformVariantDrawerColorF128_hpp_
//////////////////////////////////////////
