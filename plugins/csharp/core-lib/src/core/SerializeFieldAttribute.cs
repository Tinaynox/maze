using System;

namespace Maze.Core
{
    [AttributeUsage(AttributeTargets.Field)]
    public class SerializeFieldAttribute : Attribute
    {
        public SerializeFieldAttribute()
        {
        }
    }
}
