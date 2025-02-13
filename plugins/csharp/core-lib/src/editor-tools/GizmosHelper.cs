using Maze.Core;
using Maze.Graphics;
using System;

namespace Maze.EditorTools
{
    public class GizmosHelper
    {
        private GizmosHelper() {}

        public static void SetColor(
            ColorF128 color)
        {
            InternalCalls.SetColor(color.ToVec4F());
        }

        public static void DrawLine(
            Vec3F point0,
            Vec3F point1,
            float duration = 0.0f,
            int renderMode = 0)
        {
            InternalCalls.DrawLine(point0, point1, duration, renderMode);
        }

        public static void DrawLine(
            Vec3F point0,
            Vec3F point1,
            ColorF128 color,
            float duration = 0.0f,
            int renderMode = 0)
        {
            SetColor(color);
            InternalCalls.DrawLine(point0, point1, duration, renderMode);
        }

        public static void DrawWireSphere(
            Vec3F position,
            float radius,
            ColorF128 color,
            float duration = 0.0f,
            int renderMode = 0) 
        {
            InternalCalls.DrawWireSphere(position, radius, color.ToVec4F(), duration, renderMode);
        }

    }
}
