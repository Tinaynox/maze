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
#if (!defined(_MazeMetaPropertyDrawerParticleSystem3DMainModule_hpp_))
#define _MazeMetaPropertyDrawerParticleSystem3DMainModule_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DMainModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeParticleSystem3DModule.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MetaPropertyDrawerParticleSystem3DMainModule);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DMainModule
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API MetaPropertyDrawerParticleSystem3DMainModule
        : public MetaPropertyDrawerParticleSystem3DModule<ParticleSystem3DMainModule>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MetaPropertyDrawerParticleSystem3DMainModule, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MetaPropertyDrawerParticleSystem3DMainModule);

    public:

        //////////////////////////////////////////
        virtual ~MetaPropertyDrawerParticleSystem3DMainModule();

        //////////////////////////////////////////
        static MetaPropertyDrawerParticleSystem3DMainModulePtr Create(MetaProperty* _metaProperty);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        MetaPropertyDrawerParticleSystem3DMainModule();

        //////////////////////////////////////////
        virtual bool init(MetaProperty* _metaProperty) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

    protected:
        Vector<MetaPropertyDrawerPtr> m_propertyDrawers;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawerParticleSystem3DMainModule_hpp_
//////////////////////////////////////////
