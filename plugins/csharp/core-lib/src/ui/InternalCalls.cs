using Maze.Core;
using System;
using System.Runtime.CompilerServices;

namespace Maze.UI
{
    public static class InternalCalls
    {
        #region UIElement2D
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UIElement2DSetFlags(NativePtr nativeComponentPtr, int flags);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UIElement2DGetFlags(NativePtr nativeComponentPtr, out int outFlags);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UIElement2DSetFocused(NativePtr nativeComponentPtr, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UIElement2DGetFocused(NativePtr nativeComponentPtr, out bool outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UIElement2DSetPressed(NativePtr nativeComponentPtr, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UIElement2DGetPressed(NativePtr nativeComponentPtr, out bool outValue);
        #endregion

    }
}
