using Maze.Core;
using Maze.Graphics;
using System;

namespace Maze.EditorTools
{
    public enum GizmosMode
    {
        Debug = 0,
        CustomGeometry = 1,
    }

    public enum MeshRenderMode
    {
        Opaque = 0,
        Transparent = 1,
        TransparentTop = 2,
    }


    public static class GizmosHelper
    {
        public static void PushTransform(
            TMat tm)
        {
            InternalCalls.PushTransform(tm);
        }

        public static void PopTransform()
        {
            InternalCalls.PopTransform();
        }

        public static void SetColor(
            ColorF128 color)
        {
            InternalCalls.SetColor(color.ToVec4F());
        }

        public static void DrawLine(
            Vec3F point0,
            Vec3F point1,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawLine(point0, point1, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawLine(
            Vec3F point0,
            Vec3F point1,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            SetColor(color);
            InternalCalls.DrawLine(point0, point1, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawRay(
            Vec3F point,
            Vec3F delta,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawRay(point, delta, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireQuad(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            Vec2F scale,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireQuad(
                position, forward, up, scale, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireCircle(
            Vec3F position,
            Vec3F direction,
            float radius,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireCircle(position, direction, radius, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireHemicircle(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            float radius,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireHemicircle(
                position, forward, up, radius, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireCube(
            Vec3F position,
            Vec3F scale,
            Vec3F forward,
            Vec3F up,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireCube(
                position, scale, forward, up, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireSphere(
            Vec3F position,
            float radius,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque) 
        {
            InternalCalls.DrawWireSphere(position, radius, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireHemisphere(
            Vec3F position,
            Vec3F direction,
            float radius,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireHemisphere(position, direction, radius, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireCylinder(
            Vec3F start,
            Vec3F end,
            float radius,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireCylinder(start, end, radius, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireCone(
            Vec3F position,
            Vec3F direction,
            float angle,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireCone(position, direction, angle, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireTruncatedCone(
            Vec3F position,
            Vec3F direction,
            float radius,
            float angle,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireTruncatedCone(position, direction, radius, angle, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireTorus(
            Vec3F position,
            Vec3F direction,
            float radius,
            float torusRadius,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireTorus(position, direction, radius, torusRadius, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireArrow(
            Vec3F position,
            Vec3F direction,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireArrow(position, direction, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireCapsule(
            Vec3F start,
            Vec3F end,
            float radius,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireCapsule(start, end, radius, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireAABB2D(
            AABB2D aabb,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireAABB2D(aabb, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawWireAABB3D(
            AABB3D aabb,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawWireAABB3D(aabb, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawSolidAABB3D(
            AABB3D aabb,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawSolidAABB3D(aabb, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawSolidTriangle(
            Vec3F point0,
            Vec3F point1,
            Vec3F point2,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawSolidTriangle(point0, point1, point2, duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawSolidQuad(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            Vec2F scale,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawSolidQuad(position, forward, up, scale, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawSolidCube(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            Vec3F scale,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawSolidCube(position, forward, up, scale, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawSolidSphere(
            Vec3F position,
            float radius,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawSolidSphere(position, radius, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawSolidCone(
            Vec3F position,
            Vec3F forward,
            Vec3F up,
            float radius,
            float height,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawSolidCone(position, forward, up, radius, height, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawSolidCylinder(
            Vec3F position,
            Vec3F forward,
            float radius,
            float height,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawSolidCylinder(position, forward, radius, height, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }

        public static void DrawSolidTorus(
            Vec3F position,
            Vec3F forward,
            float radius,
            ColorF128 color,
            float duration = 0.0f,
            GizmosMode gizmosMode = GizmosMode.Debug,
            MeshRenderMode renderMode = MeshRenderMode.Opaque)
        {
            InternalCalls.DrawSolidTorus(position, forward, radius, color.ToVec4F(), duration, (int)gizmosMode, (int)renderMode);
        }
    }
}
