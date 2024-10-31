using System;

namespace Maze
{
    [AttributeUsage(AttributeTargets.Method)]
    public class EntitySystemAttribute : Attribute
    {
        private string[] Tags;
        private string[] After;
        private string[] Before;


        public EntitySystemAttribute(
            string[] tags = default,
            string[] after = default,
            string[] before = default)
        {
            Tags = tags;
            After = after;
            Before = before;
        }
    }
}
