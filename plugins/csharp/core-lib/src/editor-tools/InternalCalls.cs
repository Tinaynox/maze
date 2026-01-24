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
        public extern static void DrawRay(
            Vec3F point,
            Vec3F delta,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireQuad(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            Vec2F scale,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireCircle(
            Vec3F position,
            Vec3F direction,
            float radius,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireHemicircle(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            float radius,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireCube(
            Vec3F position,
            Vec3F scale,
            Vec3F forward,
            Vec3F up,
            Vec4F color,
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

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireHemisphere(
            Vec3F position,
            Vec3F direction,
            float radius,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireCylinder(
            Vec3F start,
            Vec3F end,
            float radius,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireCone(
            Vec3F position,
            Vec3F direction,
            float angle,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireTruncatedCone(
            Vec3F position,
            Vec3F direction,
            float radius,
            float angle,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireTorus(
            Vec3F position,
            Vec3F direction,
            float radius,
            float torusRadius,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireArrow(
            Vec3F position,
            Vec3F direction,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireCapsule(
            Vec3F start,
            Vec3F end,
            float radius,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireAABB2D(
            AABB2D aabb,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawWireAABB3D(
            AABB3D aabb,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawSolidAABB3D(
            AABB3D aabb,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawSolidTriangle(
            Vec3F point0,
            Vec3F point1,
            Vec3F point2,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawSolidQuad(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            Vec2F scale,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawSolidCube(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            Vec3F scale,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawSolidSphere(
            Vec3F position,
            float radius,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawSolidCone(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            float radius,
            float height,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawSolidCylinder(
            Vec3F position,
            Vec3F forward,
            float radius,
            float height,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DrawSolidTorus(
            Vec3F position,
            Vec3F forward,
            float radius,
            Vec4F color,
            float duration,
            int gizmosMode,
            int renderMode);


        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool IsEntitySelected(int entityId);
        #endregion
    }
}
