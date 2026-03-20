using System;

namespace Maze.Core
{
    [AttributeUsage(AttributeTargets.Method)]
    public class EntitySystemAttribute : Attribute
    {
        private string[] Tags;
        private string[] After;
        private string[] Before;


        public EntitySystemAttribute(
            string[] tags = default(string[]),
            string[] after = default(string[]),
            string[] before = default(string[]))
        {
            Tags = tags;
            After = after;
            Before = before;
        }
    }
}
