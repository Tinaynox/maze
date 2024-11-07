using System;
using Maze;

namespace Maze.Core
{
    public class Component
    {
        protected NativePtr NativeComponentPtr { get; set; }

        public int GetEntityId()
        {
            return InternalCalls.ComponentGetEntityId(NativeComponentPtr);
        }

        public Entity GetEntity()
        {
            return new Entity(InternalCalls.ComponentGetEntity(NativeComponentPtr));
        }

        public T GetComponent<T>() where T : class
        {
            return GetComponentByType(typeof(T)) as T;
        }

        public object GetComponentByType(Type _componentType)
        {
            return EcsUtils.GetComponentByType(NativeComponentPtr, _componentType);
        }

        public static bool operator !(Component _instance)
        {
            return _instance.NativeComponentPtr == NativePtr.Zero;
        }

        public static bool operator true(Component _instance)
        {
            return _instance.NativeComponentPtr != NativePtr.Zero;
        }

        public static bool operator false(Component _instance)
        {
            return _instance.NativeComponentPtr == NativePtr.Zero;
        }
    }
}
