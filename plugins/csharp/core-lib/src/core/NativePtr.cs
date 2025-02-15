using System;

namespace Maze.Core
{
    public struct NativePtr
    {
        public static readonly NativePtr Zero = new NativePtr(IntPtr.Zero);

        private IntPtr m_Ptr;
        public IntPtr Ptr { get { return m_Ptr; } }

        public NativePtr(IntPtr ptr)
        {
            m_Ptr = ptr;
        }

        public override bool Equals(object obj)
        {
            return Ptr.Equals(obj);
        }

        public override int GetHashCode()
        {
            return Ptr.GetHashCode();
        }

        public static bool operator ==(NativePtr value0, NativePtr value1)
        {
            return value0.Ptr == value1.Ptr;
        }

        public static bool operator !=(NativePtr value0, NativePtr value1)
        {
            return value0.Ptr != value1.Ptr;
        }
    }
}
