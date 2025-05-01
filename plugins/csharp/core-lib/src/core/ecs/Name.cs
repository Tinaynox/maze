using System;

namespace Maze.Core
{
    public class Name : NativeComponent
    {
        public string Title
        {
            get => InternalCalls.NameGetName(NativeComponentPtr);
            set => InternalCalls.NameSetName(NativeComponentPtr, value);
        }

        public Name(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }
    }
}
