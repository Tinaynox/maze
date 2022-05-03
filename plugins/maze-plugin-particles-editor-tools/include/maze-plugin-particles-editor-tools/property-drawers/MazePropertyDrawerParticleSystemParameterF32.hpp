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
#if (!defined(_MazePropertyDrawerParticleSystemParameterF32_hpp_))
#define _MazePropertyDrawerParticleSystemParameterF32_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawerAnimationCurve.hpp"
#include "maze-particles/MazeParticleSystemParameterF32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerParticleSystemParameterF32);
    MAZE_USING_SHARED_PTR(PropertyDrawerParticleSystemParameterF32Positive);
    MAZE_USING_SHARED_PTR(PropertyDrawerParticleSystemParameterF32Degrees);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);
    MAZE_USING_SHARED_PTR(SystemTextDropdown2D);


    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemParameterF32
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API PropertyDrawerParticleSystemParameterF32
        : public GenericPropertyDrawer<ParticleSystemParameterF32>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterF32, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerParticleSystemParameterF32);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerParticleSystemParameterF32();

        //////////////////////////////////////////
        static PropertyDrawerParticleSystemParameterF32Ptr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(ParticleSystemParameterF32 const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ParticleSystemParameterF32 getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(String const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual String getString() MAZE_OVERRIDE;


        //////////////////////////////////////////
        EntityPtr const& getRootEntity() const { return m_rootEntity; }

    protected:

        //////////////////////////////////////////
        PropertyDrawerParticleSystemParameterF32();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        

        //////////////////////////////////////////
        void notifyTextInput(SystemTextEditBox2D* _editBox);

        //////////////////////////////////////////
        void notifySelectedChanged(SystemTextEditBox2D* _editBox, bool _selected);

        //////////////////////////////////////////
        void notifyCurveChanged(AnimationCurveEdit2D* _edit, AnimationCurve const& _curve);


        //////////////////////////////////////////
        void setMode(ParticleSystemParameterF32SamplingMode _mode);

        //////////////////////////////////////////
        void updateModeUI();


        //////////////////////////////////////////
        void createModeDropdown();


        //////////////////////////////////////////
        void notifyDropdownValueChanged(SystemTextDropdown2D* _dropdown, S32 _index);

    protected:
        EntityPtr m_rootEntity;
        HorizontalLayout2DPtr m_layout;
        SystemTextEditBox2DPtr m_editBoxFrom;
        SystemTextEditBox2DPtr m_editBoxTo;
        AnimationCurveEdit2DPtr m_curveFrom;
        AnimationCurveEdit2DPtr m_curveTo;

        SystemTextDropdown2DPtr m_modeDropdown;

        ParticleSystemParameterF32 m_parameter;
        ParticleSystemParameterF32SamplingMode m_mode = ParticleSystemParameterF32SamplingMode::None;

        AnimationCurveMinMaxMode m_curveMinMaxMode = AnimationCurveMinMaxMode::Normalized;
    };


    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemParameterF32Positive
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API PropertyDrawerParticleSystemParameterF32Positive
        : public PropertyDrawerParticleSystemParameterF32
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterF32Positive, PropertyDrawerParticleSystemParameterF32);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerParticleSystemParameterF32Positive);

    public:

        //////////////////////////////////////////
        static PropertyDrawerParticleSystemParameterF32PositivePtr Create(String const& _label);

    protected:

        //////////////////////////////////////////
        PropertyDrawerParticleSystemParameterF32Positive();
    };


    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemParameterF32Degrees
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API PropertyDrawerParticleSystemParameterF32Degrees
        : public PropertyDrawerParticleSystemParameterF32
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemParameterF32Degrees, PropertyDrawerParticleSystemParameterF32);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerParticleSystemParameterF32Degrees);
         
    public:

        //////////////////////////////////////////
        static PropertyDrawerParticleSystemParameterF32DegreesPtr Create(String const& _label);

    
        //////////////////////////////////////////
        virtual void setValue(ParticleSystemParameterF32 const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ParticleSystemParameterF32 getValue() const MAZE_OVERRIDE;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerParticleSystemParameterF32_hpp_
//////////////////////////////////////////
