using System;

namespace Maze.Core
{
    [AttributeUsage(AttributeTargets.Property)]
    public class SerializePropertyAttribute : Attribute
    {
        public SerializePropertyAttribute()
        {
        }
    }
}
