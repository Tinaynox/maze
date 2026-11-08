using Maze.Core;
using Maze.Graphics;
using System;

namespace Maze.EditorTools
{
    public static class EditorHelper
    {
        public static bool IsEntitySelected(int eid)
        {
            return InternalCalls.IsEntitySelected(eid);
        }

        // Enables saving of the edited scene/prefab after changing it from code
        public static void MarkChanged()
        {
            InternalCalls.MarkEditorChanged();
        }

        // Everything recorded between Begin and End is undone/redone as one step of the editor history
        public static void BeginUndoGroup()
        {
            InternalCalls.EditorUndoGroupBegin();
        }

        public static void EndUndoGroup()
        {
            InternalCalls.EditorUndoGroupEnd();
        }

        // Call after the entity is created and parented
        public static bool RecordEntityAdded(Entity entity)
        {
            return entity.Valid && InternalCalls.EditorUndoRecordEntityAdded(entity.NativeEntityPtr);
        }

        public static bool RemoveEntityWithUndo(Entity entity)
        {
            return entity.Valid && InternalCalls.EditorUndoRemoveEntity(entity.NativeEntityPtr);
        }

        public static void RecordUndoAction(IEditorUndoAction action)
        {
            if (action == null)
                return;

            int id = InternalCalls.EditorUndoGenerateActionId();
            EditorUndoRegistry.Add(id, action);
            if (!InternalCalls.EditorUndoRecordMonoAction(id))
                EditorUndoRegistry.Remove(id);
        }

        public static void Undo()
        {
            InternalCalls.EditorUndo();
        }

        public static void Redo()
        {
            InternalCalls.EditorRedo();
        }

        public static uint GetUndoHistoryVersion()
        {
            return InternalCalls.EditorGetUndoHistoryVersion();
        }
    }
}
