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

    }
}
