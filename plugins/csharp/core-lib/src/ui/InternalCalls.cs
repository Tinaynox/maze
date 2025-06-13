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

        #region UITweenTransitionAlpha
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionAlphaSetHidden(NativePtr nativeComponentPtr, bool value, bool resetProgress = false);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionAlphaGetHidden(NativePtr nativeComponentPtr, out bool outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionAlphaAddDelayTimer(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionAlphaSetDelayTimer(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionAlphaSetShowTime(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionAlphaGetShowTime(NativePtr nativeComponentPtr, out float outValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionAlphaSetHideKoef(NativePtr nativeComponentPtr, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionAlphaGetHideKoef(NativePtr nativeComponentPtr, out float outValue);
        #endregion

        #region UITweenTransitionScale
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionScaleSetHidden(NativePtr nativeComponentPtr, bool value, bool resetProgress = false);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionScaleGetHidden(NativePtr nativeComponentPtr, out bool outValue);
        #endregion

        #region UITweenTransitionTranslation
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionTranslationSetHidden(NativePtr nativeComponentPtr, bool value, bool resetProgress = false);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void UITweenTransitionTranslationGetHidden(NativePtr nativeComponentPtr, out bool outValue);
        #endregion
    }
}
