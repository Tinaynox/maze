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
    }
}
