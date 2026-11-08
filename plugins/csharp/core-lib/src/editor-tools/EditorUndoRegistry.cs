using System.Collections.Generic;

namespace Maze.EditorTools
{
    // Custom step of the editor undo history, recorded via EditorHelper.RecordUndoAction after it was done
    public interface IEditorUndoAction
    {
        void Undo();
        void Redo();
    }

    // C# side of the undo actions, the native history refers to them by id and calls the methods below
    internal static class EditorUndoRegistry
    {
        static Dictionary<int, IEditorUndoAction> s_Actions = new Dictionary<int, IEditorUndoAction>();

        internal static void Add(int id, IEditorUndoAction action)
        {
            s_Actions[id] = action;
        }

        internal static void Remove(int id)
        {
            s_Actions.Remove(id);
        }

        static void Undo(int id)
        {
            if (s_Actions.TryGetValue(id, out IEditorUndoAction action))
                action.Undo();
        }

        static void Redo(int id)
        {
            if (s_Actions.TryGetValue(id, out IEditorUndoAction action))
                action.Redo();
        }

        // The native action left the history
        static void Release(int id) => Remove(id);
    }
}
