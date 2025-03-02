using System;

namespace Maze.Editor
{
    [AttributeUsage(AttributeTargets.Class)]
    public class EditorAttribute : Attribute
    {
        public EditorAttribute()
        {
        }
    }
}
