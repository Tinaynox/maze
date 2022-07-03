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
#if (!defined(_MazeMetaPropertyDrawerParticleSystem3DEmissionModule_hpp_))
#define _MazeMetaPropertyDrawerParticleSystem3DEmissionModule_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeVector.hpp"
#include "maze-editor-tools/property-drawers/MazeF32.hpp"
#include "maze-editor-tools/property-drawers/MazeBool.hpp"
#include "maze-editor-tools/property-drawers/MazeVector.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DMainModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DModule.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemParameterF32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MetaPropertyDrawerParticleSystem3DEmissionModule);
    MAZE_USING_SHARED_PTR(PropertyDrawerParticleSystemBurst);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DEmissionModule
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API MetaPropertyDrawerParticleSystem3DEmissionModule
        : public MetaPropertyDrawerParticleSystem3DModule<ParticleSystem3DMainModule::EmissionModule>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MetaPropertyDrawerParticleSystem3DEmissionModule, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MetaPropertyDrawerParticleSystem3DEmissionModule);

    public:

        //////////////////////////////////////////
        virtual ~MetaPropertyDrawerParticleSystem3DEmissionModule();

        //////////////////////////////////////////
        static MetaPropertyDrawerParticleSystem3DEmissionModulePtr Create(MetaProperty* _metaProperty);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processMetaInstancesChanged() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        MetaPropertyDrawerParticleSystem3DEmissionModule();

        //////////////////////////////////////////
        virtual bool init(MetaProperty* _metaProperty) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

    protected:
        PropertyDrawerBoolPtr m_enabledDrawer;
        PropertyDrawerParticleSystemParameterF32Ptr m_emissionPerSecondDrawer;
        PropertyDrawerVectorPtr m_burstsDrawer;

        bool m_processingDataToUI = false;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawerParticleSystem3DEmissionModule_hpp_
//////////////////////////////////////////
