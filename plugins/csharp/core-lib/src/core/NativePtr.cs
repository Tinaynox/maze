using System;

namespace Maze.Core
{
    public struct NativePtr
    {
        public static readonly NativePtr Zero = new NativePtr(IntPtr.Zero);

        private IntPtr m_Ptr;
        public IntPtr Ptr { get { return m_Ptr; } }

        public NativePtr(IntPtr _ptr)
        {
            m_Ptr = _ptr;
        }

        public override bool Equals(object obj)
        {
            return Ptr.Equals(obj);
        }

        public override int GetHashCode()
        {
            return Ptr.GetHashCode();
        }

        public static bool operator ==(NativePtr value1, NativePtr value2)
        {
            return value1.Ptr == value2.Ptr;
        }

        public static bool operator !=(NativePtr value1, NativePtr value2)
        {
            return value1.Ptr != value2.Ptr;
        }
    }
}
