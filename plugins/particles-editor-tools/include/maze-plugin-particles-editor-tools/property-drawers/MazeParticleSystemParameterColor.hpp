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
#if (!defined(_MazePropertyDrawerParticleSystemParameterColor_hpp_))
#define _MazePropertyDrawerParticleSystemParameterColor_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-editor-tools/property-drawers/MazeColorGradient.hpp"
#include "maze-editor-tools/ecs/components/MazeColorGradientEdit2D.hpp"
#include "maze-editor-tools/ecs/components/MazeColorHDREdit2D.hpp"
#include "maze-particles/MazeParticleSystemParameterColor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerParticleSystemParameterColor);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);
    MAZE_USING_SHARED_PTR(Dropdown2D);


    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemParameterColor
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API PropertyDrawerParticleSystemParameterColor
        : public GenericPropertyDrawer<ParticleSystemParameterColor>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterColor, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerParticleSystemParameterColor);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerParticleSystemParameterColor();

        //////////////////////////////////////////
        static PropertyDrawerParticleSystemParameterColorPtr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(ParticleSystemParameterColor const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ParticleSystemParameterColor getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;


        //////////////////////////////////////////
        EntityPtr const& getRootEntity() const { return m_rootEntity; }

    protected:

        //////////////////////////////////////////
        PropertyDrawerParticleSystemParameterColor();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        

        //////////////////////////////////////////
        void notifyColorChanged(ColorHDREdit2D* _edit, ColorF128 const& _color);

        //////////////////////////////////////////
        void notifyGradientChanged(ColorGradientEdit2D* _edit, ColorGradient const& _gradient);


        //////////////////////////////////////////
        void setMode(ParticleSystemParameterColorSamplingMode _mode);

        //////////////////////////////////////////
        void updateModeUI();


        //////////////////////////////////////////
        void createModeDropdown();


        //////////////////////////////////////////
        void notifyDropdownValueChanged(Dropdown2D* _dropdown, S32 _index);

    protected:
        EntityPtr m_rootEntity;
        HorizontalLayout2DPtr m_layout;
        ColorHDREdit2DPtr m_colorFrom;
        ColorHDREdit2DPtr m_colorTo;
        ColorGradientEdit2DPtr m_gradientFrom;
        ColorGradientEdit2DPtr m_gradientTo;

        Dropdown2DPtr m_modeDropdown;

        ParticleSystemParameterColor m_parameter;
        ParticleSystemParameterColorSamplingMode m_mode = ParticleSystemParameterColorSamplingMode::None;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerParticleSystemParameterColor_hpp_
//////////////////////////////////////////
