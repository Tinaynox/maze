using Maze.Core;
using Maze.Graphics;
using System.Runtime.CompilerServices;

namespace Maze.EditorTools
{
    public static class InternalCalls
    {
        #region GizmosHelper
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void PushTransform(
            TMat tm);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void PopTransform();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SetColor(
            Vec4F color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawLine(
            Vec3F point0,
            Vec3F point1,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireSphere(
            Vec3F position,
            float radius,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);
        #endregion
    }
}
