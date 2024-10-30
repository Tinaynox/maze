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
            // Native component wrappers
            if (typeof(NativeComponent).IsAssignableFrom(typeof(T)))
            {
                int componentId = InternalCalls.GetComponentId($"Maze::{typeof(T).Name}");
                IntPtr componentPtr = InternalCalls.GetComponent(nativeComponentPtr, componentId);
                if (componentPtr == IntPtr.Zero)
                    return default(T);

                return (T)Activator.CreateInstance(typeof(T), componentPtr);
            }

            T value = default(T);

            Debug.LogError("#TODO2!!!");

            return value;
        }
    }
}
