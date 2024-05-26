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
#if (!defined(_MazePropertyDrawerParticleSystemBurst_hpp_))
#define _MazePropertyDrawerParticleSystemBurst_hpp_


//////////////////////////////////////////
#include "maze-plugin-particles-editor-tools/MazeParticlesEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-editor-tools/property-drawers/MazeColorGradient.hpp"
#include "maze-editor-tools/property-drawers/MazeColorGradient.hpp"
#include "maze-editor-tools/ecs/components/MazeColorGradientEdit2D.hpp"
#include "maze-editor-tools/ecs/components/MazeColorHDREdit2D.hpp"
#include "maze-particles/MazeParticleSystemBurst.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerParticleSystemBurst);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);
    

    //////////////////////////////////////////
    // Class PropertyDrawerParticleSystemBurst
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API PropertyDrawerParticleSystemBurst
        : public GenericPropertyDrawer<ParticleSystemBurst>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemBurst, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerParticleSystemBurst);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerParticleSystemBurst();

        //////////////////////////////////////////
        static PropertyDrawerParticleSystemBurstPtr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(ParticleSystemBurst const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ParticleSystemBurst getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;


        //////////////////////////////////////////
        EntityPtr const& getRootEntity() const { return m_rootEntity; }

        //////////////////////////////////////////
        void resetSelection();

    protected:

        //////////////////////////////////////////
        PropertyDrawerParticleSystemBurst();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        EditBox2DPtr createEdit(
            CString _name,
            Transform2DPtr const& _parent);

        //////////////////////////////////////////
        void notifyTextInput(EditBox2D* _edit);

        //////////////////////////////////////////
        void notifySelectedChanged(EditBox2D* _edit, bool _selected);

    protected:
        EntityPtr m_rootEntity;
        HorizontalLayout2DPtr m_layout;
        
        EditBox2DPtr m_editBoxTime;
        EditBox2DPtr m_editBoxMinCount;
        EditBox2DPtr m_editBoxMaxCount;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerParticleSystemBurst_hpp_
//////////////////////////////////////////
