using System;

namespace Maze.Core
{
    [AttributeUsage(AttributeTargets.Method)]
    public class EnableInEditorAttribute : Attribute
    {
        public EnableInEditorAttribute()
        {
        }
    }
}
