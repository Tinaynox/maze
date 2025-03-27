using System;

namespace Maze.Core
{
    public static class MathHelper
    {
        public static float Lerp(float v0, float v1, float s) 
        {
            return v0 + (v1 - v0) * s;
        }

        public static Vec3F Lerp(Vec3F v0, Vec3F v1, float s)
        {
            return new Vec3F(
                Lerp(v0.X, v1.X, s),
                Lerp(v0.Y, v1.Y, s),
                Lerp(v0.Z, v1.Z, s));
        }

        public static float DegreesToRadians(float degrees)
        {
            return degrees * ((float)Math.PI / 180.0f);
        }

        public static float RadiansToDegrees(float radians)
        {
            return radians * (180.0f / (float)Math.PI);
        }

        public static Vec2F DegreesToRadians(Vec2F degrees)
        {
            return new Vec2F(
                DegreesToRadians(degrees.X),
                DegreesToRadians(degrees.Y));
        }

        public static Vec2F RadiansToDegrees(Vec2F radians)
        {
            return new Vec2F(
                RadiansToDegrees(radians.X),
                RadiansToDegrees(radians.Y));
        }

        public static Vec3F DegreesToRadians(Vec3F degrees)
        {
            return new Vec3F(
                DegreesToRadians(degrees.X),
                DegreesToRadians(degrees.Y),
                DegreesToRadians(degrees.Z));
        }

        public static Vec3F RadiansToDegrees(Vec3F radians)
        {
            return new Vec3F(
                RadiansToDegrees(radians.X),
                RadiansToDegrees(radians.Y),
                RadiansToDegrees(radians.Z));
        }

        public static float NormalizedAnglePI(float _angle)
        {
            while (_angle < -(float)Math.PI) _angle += (float)Math.PI * 2.0f;
            while (_angle > (float)Math.PI) _angle -= (float)Math.PI * 2.0f;
            return _angle;
        }

        public static float LerpAngle(float angleFrom, float angleTo, float ratio)
        {
            angleFrom = NormalizedAnglePI(angleFrom);
            angleTo = NormalizedAnglePI(angleTo);

            if (Math.Abs(angleTo - angleFrom) > Math.PI)
            {
                if (angleTo > angleFrom)
                    angleFrom += (float)Math.PI * 2.0f;
                else
                    angleTo += (float)Math.PI * 2.0f;
            }

            return Lerp(angleFrom, angleTo, ratio);
        }

        public static float RenormAngle(float angle, float pivotAngle)
        {
            float delta = pivotAngle - angle;
            return Math.Abs(delta) > Math.PI ? angle + (delta > 0.0f ? 1.0f : -1.0f) * (float)Math.PI * 2.0f : angle;
        }

        public static float AngleDifference(float angle0, float angle1)
        {
            float diff = (angle1 - angle0 + (float)Math.PI) % ((float)Math.PI * 2.0f) - (float)Math.PI;
            return diff < -(float)Math.PI ? diff + (float)(Math.PI * 2.0f) : diff;
        }
    }
}
