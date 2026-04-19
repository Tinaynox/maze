using System;
using System.Collections;
using System.Collections.Generic;

namespace Maze.Core
{
    public class SizePolicy2D : NativeComponent
    {
        public Vec2F SizeDelta
        {
            get { InternalCalls.SizePolicy2DGetSizeDelta(NativeComponentPtr, out Vec2F size); return size; }
            set { InternalCalls.SizePolicy2DSetSizeDelta(NativeComponentPtr, ref value); }
        }
		
		public SizePolicy2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }
    }
}
