using Maze.Core;
using Maze.Graphics;
using System.Runtime.CompilerServices;

namespace Maze.EditorTools
{
    public class InternalCalls
    {
        #region GizmosHelper
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void SetColor(
            Vec4F color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawLine(
            Vec3F point0,
            Vec3F point1,
            float duration,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireSphere(
            Vec3F position,
            float radius,
            Vec4F color,
            float duration,
            int renderMode);
        #endregion
    }
}
