using System;
using Maze;

namespace Maze
{
    public class Component
    {
        protected IntPtr nativeComponentPtr { get; set; }

        public int GetEntityId()
        {
            return InternalCalls.GetEntityId(nativeComponentPtr);
        }

        public T GetComponent<T>() where T : class
        {
            // TODO: Remove all string operations here.
            // We should have some table C# type => C++ componentId

            // Native component wrappers
            if (typeof(NativeComponent).IsAssignableFrom(typeof(T)))
            {
                int nativeComponentId = InternalCalls.GetComponentId($"Maze::{typeof(T).Name}");
                IntPtr componentPtr = InternalCalls.GetComponent(nativeComponentPtr, nativeComponentId);
                if (componentPtr == IntPtr.Zero)
                    return default(T);

                return (T)Activator.CreateInstance(typeof(T), componentPtr);
            }


            if (!typeof(MonoBehaviour).IsAssignableFrom(typeof(T)))
                return default(T);

            int componentId = InternalCalls.GetComponentId($"{typeof(T).Namespace}.{typeof(T).Name}");
            return (T)(object)InternalCalls.GetMonoBehaviourComponentObject(nativeComponentPtr, componentId);
        }
    }
}
