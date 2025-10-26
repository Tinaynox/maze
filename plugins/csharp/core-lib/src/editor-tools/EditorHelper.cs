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
    }
}
