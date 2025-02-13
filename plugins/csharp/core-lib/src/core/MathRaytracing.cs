using System;

namespace Maze.Core
{
    public class MathRaytracing
    {
        private MathRaytracing() {}

        public static bool RaycastPlane(
            Vec3F rayPoint,
            Vec3F rayDirection,
            Vec3F planePoint,
            Vec3F planeNormal,
            out float outDist)
        {
            float denom = planeNormal.Dot(rayDirection);
            if ((float)Math.Abs(denom) > 0.0001f)
            {
                float t = (planePoint - rayPoint).Dot(planeNormal) / denom;
                if (t >= 0.0f)
                {
                    Vec3F p = rayPoint + t * rayDirection;
                    outDist = (p - rayPoint).Length();
                    return true;
                }
            }

            outDist = 0.0f;
            return false;
        }

    }
}
