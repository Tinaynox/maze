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
#if (!defined(_MazeMetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule_hpp_))
#define _MazeMetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerF32.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerBool.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DMainModule.hpp"
#include "maze-particles/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DModule.hpp"
#include "maze-particles/property-drawers/MazePropertyDrawerParticleSystemParameterF32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule
        : public MetaPropertyDrawerParticleSystem3DModule<ParticleSystem3DMainModule::RotationOverLifetimeModule>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule);

    public:

        //////////////////////////////////////////
        virtual ~MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule();

        //////////////////////////////////////////
        static MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModulePtr Create(MetaProperty* _metaProperty);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        MetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule();

        //////////////////////////////////////////
        virtual bool init(MetaProperty* _metaProperty) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

    protected:
        PropertyDrawerBoolPtr m_enabledDrawer;
        PropertyDrawerParticleSystemParameterF32DegreesPtr m_parameterDrawer;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawerParticleSystem3DRotationOverLifetimeModule_hpp_
//////////////////////////////////////////
