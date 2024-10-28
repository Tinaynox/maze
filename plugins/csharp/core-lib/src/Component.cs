using System;
using Maze;

namespace Maze
{
    public class Component
    {
        public int eid { get; private set; } = Ecs.InvalidEntityId;

        public Component()
        {
            Debug.Log("Component constructor!");
        }

        public T GetComponent<T>()
        {
            T value = default(T);

            return value;
        }
    }
}
