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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsEditorTools.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsActionManager.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionActionsGroup.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionEntityAdd.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionEntityRemove.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionCustom.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-core/ecs/MazeEntity.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline void SetColor(
        Vec4F const& _color)
    {
        GizmosHelper::SetColor(ColorF128(_color));
    }

    //////////////////////////////////////////
    inline void DrawLine(
        Vec3F const& _point0,
        Vec3F const& _point1,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawLine(
            _point0,
            _point1,
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawRay(
        Vec3F const& _point,
        Vec3F const& _delta,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawRay(
            _point,
            _delta,
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireQuad(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec2F const& _scale,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireQuad(
            _position,
            _forward,
            _up,
            _scale,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCircle(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCircle(
            _position,
            _direction,
            _radius,
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireHemicircle(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        F32 _radius,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireHemicircle(
            _position,
            _forward,
            _up,
            _radius,
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCube(
        Vec3F const& _position,
        Vec3F const& _scale,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCube(
            _position,
            _scale,
            _forward,
            _up,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireSphere(
        Vec3F const& _position,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireSphere(
            _position,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireHemisphere(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireHemisphere(
            _position,
            _direction,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCylinder(
        Vec3F const& _start,
        Vec3F const& _end,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCylinder(
            _start,
            _end,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCone(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _angle,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCone(
            _position,
            _direction,
            _angle,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireTruncatedCone(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _angle,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireTruncatedCone(
            _position,
            _direction,
            _radius,
            _angle,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireTorus(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _torusRadius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireTorus(
            _position,
            _direction,
            _radius,
            _torusRadius,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireArrow(
        Vec3F const& _position,
        Vec3F const& _direction,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireArrow(
            _position,
            _direction,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCapsule(
        Vec3F const& _start,
        Vec3F const& _end,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCapsule(
            _start,
            _end,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireAABB2D(
        AABB2D const& _aabb,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireAABB(
            _aabb,
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireAABB3D(
        AABB3D const& _aabb,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireAABB(
            _aabb,
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidAABB3D(
        AABB3D const& _aabb,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidAABB(
            _aabb,
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidTriangle(
        Vec3F const& _point0,
        Vec3F const& _point1,
        Vec3F const& _point2,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidTriangle(
            _point0,
            _point1,
            _point2,
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidQuad(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec2F const& _scale,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidQuad(
            _position,
            _forward,
            _up,
            _scale,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidCube(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec3F const& _scale,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidCube(
            _position,
            _forward,
            _up,
            _scale,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidSphere(
        Vec3F const& _position,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidSphere(
            _position,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidCone(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        F32 _radius,
        F32 _height,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidCone(
            _position,
            _forward,
            _up,
            _radius,
            _height,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidCylinder(
        Vec3F const& _position,
        Vec3F const& _forward,
        F32 _radius,
        F32 _height,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidCylinder(
            _position,
            _forward,
            _radius,
            _height,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidTorus(
        Vec3F const& _position,
        Vec3F const& _forward,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidTorus(
            _position,
            _forward,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer3D::GizmosMode(_gizmosMode),
            GizmosDrawer3D::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawLabel(
        Vec3F const& _position,
        MonoString* _text,
        Vec4F const& _color,
        F32 _fontSize = 16.0f,
        S32 _gizmosMode = 0)
    {
        Char* cstr = mono_string_to_utf8(_text);
        GizmosHelper::DrawLabel(
            _position,
            String(cstr),
            ColorF128(_color),
            _fontSize,
            GizmosDrawer3D::GizmosMode(_gizmosMode));
        mono_free(cstr);
    }

    //////////////////////////////////////////
    bool MAZE_PLUGIN_CSHARP_API IsEntitySelected(S32 _entityId)
    {
        return SelectionManager::GetInstancePtr()->isObjectSelected(EntityId(_entityId));
    }

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API MarkEditorChanged()
    {
        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->markChanged();
    }

    //////////////////////////////////////////
    // Undo group being recorded from C# (e.g. a brush stroke), its actions are applied immediately.
    // Nested Begin/End pairs extend the outermost group
    static EditorActionActionsGroupPtr s_recordingActionsGroup;
    static S32 s_recordingActionsGroupDepth = 0;

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API EditorUndoGroupBegin()
    {
        if (s_recordingActionsGroupDepth++ == 0)
            s_recordingActionsGroup = EditorActionActionsGroup::Create();
    }

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API EditorUndoGroupEnd()
    {
        MAZE_ERROR_RETURN_IF(s_recordingActionsGroupDepth == 0, "Undo group is not begun!");
        if (--s_recordingActionsGroupDepth > 0)
            return;

        EditorActionActionsGroupPtr group = s_recordingActionsGroup;
        s_recordingActionsGroup.reset();

        if (!group || group->getActionsCount() == 0)
            return;

        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->addAppliedAction(group);
    }

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API FlushEditorUndoGroup()
    {
        if (s_recordingActionsGroupDepth == 0)
            return;

        s_recordingActionsGroupDepth = 1;
        EditorUndoGroupEnd();
    }

    //////////////////////////////////////////
    inline void RecordAppliedEditorAction(EditorActionPtr const& _action)
    {
        if (s_recordingActionsGroup)
            s_recordingActionsGroup->addAppliedAction(_action);
        else
        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->addAppliedAction(_action);
    }

    //////////////////////////////////////////
    bool MAZE_PLUGIN_CSHARP_API EditorUndoRecordEntityAdded(Entity* _entity)
    {
        if (!_entity || !_entity->getEcsWorld())
            return false;

        EditorActionEntityAddPtr action = EditorActionEntityAdd::Create(_entity->getSharedPtr());
        if (!action)
            return false;

        // The entity is in the world already, so applying only marks the action as applied
        action->apply();
        RecordAppliedEditorAction(action);
        return true;
    }

    //////////////////////////////////////////
    bool MAZE_PLUGIN_CSHARP_API EditorUndoRemoveEntity(Entity* _entity)
    {
        if (!_entity || !_entity->getEcsWorld())
            return false;

        EditorActionEntityRemovePtr action = EditorActionEntityRemove::Create(_entity->getSharedPtr());
        if (!action)
            return false;

        action->apply();
        RecordAppliedEditorAction(action);
        return true;
    }

    //////////////////////////////////////////
    // C# undo actions (IEditorUndoAction) are kept by Maze.EditorTools.EditorUndoRegistry under native ids.
    // The class is looked up on every call, so the ids keep working (as no-ops) after assemblies reload
    static void InvokeEditorUndoRegistry(CString _methodName, S32 _id)
    {
        if (!MonoEngine::IsInitialized())
            return;

        MonoImage* image = MonoEngine::GetCoreAssemblyImage();
        if (!image)
            return;

        MonoClass* monoClass = mono_class_from_name(image, "Maze.EditorTools", "EditorUndoRegistry");
        MAZE_ERROR_RETURN_IF(!monoClass, "Maze.EditorTools.EditorUndoRegistry is not found!");

        MonoMethod* method = mono_class_get_method_from_name(monoClass, _methodName, 1);
        MAZE_ERROR_RETURN_IF(!method, "EditorUndoRegistry.%s is not found!", _methodName);

        void* params[] = { &_id };
        MonoHelper::InvokeMethod(nullptr, method, params);
    }

    //////////////////////////////////////////
    struct MonoUndoActionToken
    {
        S32 id = 0;

        // The action is recorded already applied
        bool skipApply = true;

        ~MonoUndoActionToken()
        {
            InvokeEditorUndoRegistry("Release", id);
        }
    };

    //////////////////////////////////////////
    static S32 s_lastMonoUndoActionId = 0;

    //////////////////////////////////////////
    S32 MAZE_PLUGIN_CSHARP_API EditorUndoGenerateActionId()
    {
        return ++s_lastMonoUndoActionId;
    }

    //////////////////////////////////////////
    bool MAZE_PLUGIN_CSHARP_API EditorUndoRecordMonoAction(S32 _id)
    {
        SharedPtr<MonoUndoActionToken> token = MakeShared<MonoUndoActionToken>();
        token->id = _id;

        EditorActionCustomPtr action = EditorActionCustom::Create(
            [token]()
            {
                if (token->skipApply)
                {
                    token->skipApply = false;
                    return;
                }

                InvokeEditorUndoRegistry("Redo", token->id);
            },
            [token]()
            {
                InvokeEditorUndoRegistry("Undo", token->id);
            });

        if (!action)
            return false;

        action->apply();
        RecordAppliedEditorAction(action);
        return true;
    }

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API EditorUndo()
    {
        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->undoAction();
    }

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API EditorRedo()
    {
        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->redoAction();
    }

    //////////////////////////////////////////
    U32 MAZE_PLUGIN_CSHARP_API EditorGetUndoHistoryVersion()
    {
        if (EditorToolsActionManager::GetInstancePtr())
            return EditorToolsActionManager::GetInstancePtr()->getHistoryVersion();

        return 0u;
    }


    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsEditorTools()
    {
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC_WITH_NAME(GizmosHelper::PushTransform, PushTransform);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC_WITH_NAME(GizmosHelper::PopTransform, PopTransform);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(SetColor);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawLine);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawRay);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireQuad);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCircle);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireHemicircle);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCube);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireSphere);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireHemisphere);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCylinder);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCone);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireTruncatedCone);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireTorus);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireArrow);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCapsule);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireAABB2D);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireAABB3D);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidAABB3D);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidTriangle);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidQuad);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidCube);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidSphere);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidCone);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidCylinder);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidTorus);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawLabel);

        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(IsEntitySelected);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(MarkEditorChanged);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorUndoGroupBegin);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorUndoGroupEnd);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorUndoRecordEntityAdded);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorUndoRemoveEntity);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorUndoGenerateActionId);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorUndoRecordMonoAction);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorUndo);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorRedo);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(EditorGetUndoHistoryVersion);
    }

} // namespace Maze
//////////////////////////////////////////
