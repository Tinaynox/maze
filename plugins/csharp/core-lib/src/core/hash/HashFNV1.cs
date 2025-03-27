using System;

namespace Maze.Core
{
    public static class HashFNV1
    {
        private const uint Offset32 = 0x811C9DC5;
        private const uint Prime32 = 0x01000193;


        public static uint CalculateFNV1(string text)
        {
            uint hash = Offset32;
            if (string.IsNullOrEmpty(text))
                return hash;

            foreach (char c in text)
            {
                hash *= Prime32;
                hash ^= (uint)c;
            }
            return hash;
        }

        public static uint CalculateFNV1(char[] text, int length)
        {
            uint hash = Offset32;
            if (text == null || length <= 0)
                return hash;

            for (int i = 0; i < length && i < text.Length; i++)
            {
                hash *= Prime32;
                hash ^= (uint)text[i];
            }
            return hash;
        }

        public static uint CalculateFNV1A(string text)
        {
            uint hash = Offset32;
            if (string.IsNullOrEmpty(text))
                return hash;

            foreach (char c in text)
            {
                hash ^= (uint)c;
                hash *= Prime32;
            }
            return hash;
        }

        public static uint CalculateFNV1A(char[] text, int length)
        {
            uint hash = Offset32;
            if (text == null || length <= 0)
                return hash;

            for (int i = 0; i < length && i < text.Length; i++)
            {
                hash ^= (uint)text[i];
                hash *= Prime32;
            }
            return hash;
        }

        public static uint CalculateFNV1(byte[] data)
        {
            uint hash = Offset32;
            if (data == null || data.Length == 0)
                return hash;

            foreach (byte b in data)
            {
                hash *= Prime32;
                hash ^= b;
            }
            return hash;
        }

        public static uint CalculateFNV1A(byte[] data)
        {
            uint hash = Offset32;
            if (data == null || data.Length == 0)
                return hash;

            foreach (byte b in data)
            {
                hash ^= b;
                hash *= Prime32;
            }
            return hash;
        }
    }
}
