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
#if (!defined(_MazeTexture2DsInspector_hpp_))
#define _MazeTexture2DsInspector_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/inspectors/MazeInspector.hpp"
#include "maze-editor-tools/inspectors/asset-materials/MazeMaterialsRenderPassDrawer.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawer.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Texture2DsInspector);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(ComponentEditor);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Shader);
    

    //////////////////////////////////////////
    // Class EditorToolsTexture2DChangedEvent
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorToolsTexture2DChangedEvent
        : public Event
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EditorToolsTexture2DChangedEvent);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EditorToolsTexture2DChangedEvent);

    public:
        //////////////////////////////////////////
        EditorToolsTexture2DChangedEvent(String const& _textureName = String())
            : m_textureName(_textureName)
        {}

        //////////////////////////////////////////
        String const& getTextureName() const { return m_textureName; }

    protected:
        String m_textureName;
    };


    //////////////////////////////////////////
    // Class Texture2DsInspector
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API Texture2DsInspector
        : public Inspector
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture2DsInspector, Inspector);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Texture2DsInspector);

    public:

        //////////////////////////////////////////
        virtual ~Texture2DsInspector();

        //////////////////////////////////////////
        static Texture2DsInspectorPtr Create(Transform2DPtr const& _parent);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void setTextures(Set<Texture2DPtr> const& _textures);

        //////////////////////////////////////////
        virtual bool setAssetFiles(Set<AssetFilePtr> const& _assetFiles) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setObjects(Set<ObjectPtr> const& _objects) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        Texture2DsInspector();

        //////////////////////////////////////////
        virtual bool init(Transform2DPtr const& _parent) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void buildTexturesPropertiesList();

        //////////////////////////////////////////
        Set<Texture2DPtr> const& getTextures();

        //////////////////////////////////////////
        void updatePropertyValues();

    protected:
        Set<Texture2DPtr> m_textures;

        bool m_texturesPropertiesListDirty;

        Texture2DPtr m_textureCopy;
        F32 m_textureChangedTimer = 0.0f;

        Vector<MetaPropertyDrawerPtr> m_propertyDrawers;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2DsInspector_hpp_
//////////////////////////////////////////
