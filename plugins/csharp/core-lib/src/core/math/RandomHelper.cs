using System;

namespace Maze.Core
{
    public static class RandomHelper
    {
        static Random s_Random = new Random();

        public static int Int()
        {
            return s_Random.Next();
        }

        // [a,b)
        public static int Int(int a, int b)
        {
            return s_Random.Next(a, b);
        }

        // [0,a)
        public static int Int(int a)
        {
            return s_Random.Next(0, a);
        }

        public static double Double()
        {
            return s_Random.NextDouble();
        }

        // [a,b]
        public static double Double(double a, double b)
        {
            return s_Random.NextDouble() * (b - a) + a;
        }

        // [0.0,a]
        public static double Double(double a)
        {
            return s_Random.NextDouble() * a;
        }

        public static float Float()
        {
            return (float)s_Random.NextDouble();
        }

        // [a,b]
        public static float Float(float a, float b)
        {
            return ((float)s_Random.NextDouble()) * (b - a) + a;
        }

        // [0.0f,a]
        public static float Float(float a)
        {
            return ((float)s_Random.NextDouble()) * a;
        }
    }
}
