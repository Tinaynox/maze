using System;
using System.IO;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    public class ByteBuffer
    {
        byte[] m_Data;
        public byte[] Data => m_Data;

        int m_Size = 0;
        public int Size => m_Size;

        int m_Capacity = 0;
        public int Capacity => m_Capacity;

        public ByteBuffer(int size = 0)
        {
            Resize(size);
        }

        public ByteBuffer(byte[] data)
        {
            SetData(data);
        }

        public ByteBuffer(byte[] data, int size)
        {
            SetData(data, size);
        }

        public ByteBuffer(int size, byte fillByte)
        {
            Resize(size);
            Fill(fillByte);
        }

        public ByteBuffer(ByteBuffer byteBuffer)
        {
            SetData(byteBuffer.m_Data, byteBuffer.m_Size);
        }

        public void Clear()
        {
            m_Data = null;
            m_Size = 0;
            m_Capacity = 0;
        }

        bool Reallocate(int capacity)
        {
            if (m_Capacity == capacity)
                return false;

            if (capacity == 0)
            {
                Clear();
                return false;
            }

            m_Capacity = capacity;
            m_Size = Math.Min(m_Size, m_Capacity);

            if (m_Capacity > 0)
                m_Data = new byte[m_Capacity];
            else
                m_Data = null;

            return true;
        }

        public void Reserve(int capacity)
        {
            if (capacity <= m_Capacity)
                return;

            byte[] prevData = m_Data;

            if (!Reallocate(capacity))
                return;

            if (prevData != null)
                Buffer.BlockCopy(prevData, 0, m_Data, 0, m_Size);
        }

        public void Resize(int size)
        {
            if (size > m_Capacity)
                Reserve(size);

            m_Size = size;
        }

        public void CapacityToSize()
        {
            if (m_Size == m_Capacity)
                return;

            byte[] prevData = m_Data;

            if (!Reallocate(m_Size))
                return;

            if (prevData != null)
                Buffer.BlockCopy(prevData, 0, m_Data, 0, m_Size);
        }

        public void Assign(byte[] data)
        {
            m_Data = data;
            m_Capacity = m_Data?.Length ?? 0;
            m_Size = m_Capacity;
        }

        public void SetData(byte[] data)
        {
            Resize(data.Length);
            Buffer.BlockCopy(data, 0, m_Data, 0, data.Length);
        }

        public void SetData(byte[] data, int size)
        {
            Resize(size);
            Buffer.BlockCopy(data, 0, m_Data, 0, size);
        }

        public void Fill(byte fillByte)
        {
            for (int i = 0; i < m_Size; i++)
                m_Data[i] = fillByte;
        }

        public int Insert(int at, int size)
        {
            int requiredSize = m_Size + size;

            if (requiredSize <= m_Capacity)
            {
                Buffer.BlockCopy(m_Data, at, m_Data, at + size, m_Size - at);
            }
            else
            {
                byte[] prevData = m_Data;

                int newCapacity = Math.Max(requiredSize, m_Capacity + (m_Capacity >> 1) + 1);

                if (!Reallocate(newCapacity))
                    return -1;

                if (prevData != null)
                {
                    Buffer.BlockCopy(prevData, 0, m_Data, 0, at);
                    Buffer.BlockCopy(prevData, at, m_Data, at + size, m_Size - at);
                }
            }

            m_Size += size;
            return at;
        }

        public int InsertAt(int at, int size, byte[] data)
        {
            int shift = Insert(at, size);
            Buffer.BlockCopy(data, 0, m_Data, at, size);
            return shift;
        }

        public void Erase(int at, int size)
        {
            Buffer.BlockCopy(m_Data, at + size, m_Data, at, m_Size - (at + size));
            m_Size -= size;
        }

        public void Append(byte data)
        {
            int requiredSize = m_Size + 1;

            if (requiredSize > m_Capacity)
                Reserve(Math.Max(requiredSize, m_Capacity + (m_Capacity >> 1) + 1));

            m_Data[m_Size] = data;
            m_Size += 1;
        }

        public void Append(sbyte data)
        {
            Append((byte)data);
        }

        public void Append(byte[] data, int offs, int size)
        {
            int requiredSize = m_Size + size;

            if (requiredSize > m_Capacity)
                Reserve(Math.Max(requiredSize, m_Capacity + (m_Capacity >> 1) + 1));

            Buffer.BlockCopy(data, offs, m_Data, m_Size, size);
            m_Size += size;
        }

        public void Append(byte[] data, int size)
        {
            Append(data, 0, size);
        }

        public void Append(byte[] data)
        {
            Append(data, data.Length);
        }

        public void Append(ByteBuffer other)
        {
            if (other == null)
                return;

            int size = Size;
            Resize(size + other.Size);
            Buffer.BlockCopy(other.m_Data, 0, m_Data, size, other.Size);
        }

        public void Append<T>(T value) where T : struct
        {
            var t = typeof(T);

            if (t == typeof(sbyte)) { Append((sbyte)(object)value); return; }
            if (t == typeof(short)) { Append(BitConverter.GetBytes((short)(object)value)); return; }
            if (t == typeof(int)) { Append(BitConverter.GetBytes((int)(object)value)); return; }
            if (t == typeof(long)) { Append(BitConverter.GetBytes((long)(object)value)); return; }
            if (t == typeof(byte)) { Append((byte)(object)value); return; }
            if (t == typeof(ushort)) { Append(BitConverter.GetBytes((ushort)(object)value)); return; }
            if (t == typeof(uint)) { Append(BitConverter.GetBytes((uint)(object)value)); return; }
            if (t == typeof(ulong)) { Append(BitConverter.GetBytes((ulong)(object)value)); return; }
            if (t == typeof(float)) { Append(BitConverter.GetBytes((float)(object)value)); return; }
            if (t == typeof(double)) { Append(BitConverter.GetBytes((double)(object)value)); return; }
            if (t == typeof(bool)) { Append(BitConverter.GetBytes((bool)(object)value)); return; }
            if (t == typeof(char)) { Append(BitConverter.GetBytes((char)(object)value)); return; }

            if (t == typeof(Vec4S8)) { Append(((Vec4S8)(object)value).GetBytes()); return; }
            if (t == typeof(Vec4U8)) { Append(((Vec4U8)(object)value).GetBytes()); return; }
            if (t == typeof(Vec2S)) { Append(((Vec2S)(object)value).GetBytes()); return; }
            if (t == typeof(Vec3S)) { Append(((Vec3S)(object)value).GetBytes()); return; }
            if (t == typeof(Vec4S)) { Append(((Vec4S)(object)value).GetBytes()); return; }
            if (t == typeof(Vec2U)) { Append(((Vec2U)(object)value).GetBytes()); return; }
            if (t == typeof(Vec3U)) { Append(((Vec3U)(object)value).GetBytes()); return; }
            if (t == typeof(Vec4U)) { Append(((Vec4U)(object)value).GetBytes()); return; }
            if (t == typeof(Vec2F)) { Append(((Vec2F)(object)value).GetBytes()); return; }
            if (t == typeof(Vec3F)) { Append(((Vec3F)(object)value).GetBytes()); return; }
            if (t == typeof(Vec4F)) { Append(((Vec4F)(object)value).GetBytes()); return; }
            if (t == typeof(Vec2B)) { Append(((Vec2B)(object)value).GetBytes()); return; }
            if (t == typeof(Vec3B)) { Append(((Vec3B)(object)value).GetBytes()); return; }
            if (t == typeof(Vec4B)) { Append(((Vec4B)(object)value).GetBytes()); return; }
            if (t == typeof(Mat3F)) { Append(((Mat3F)(object)value).GetBytes()); return; }
            if (t == typeof(Mat4F)) { Append(((Mat4F)(object)value).GetBytes()); return; }
            if (t == typeof(TMat)) { Append(((TMat)(object)value).GetBytes()); return; }

            throw new ArgumentException($"Type {typeof(T)} is not supported by BitConverter");
        }

        public unsafe T ReadAs<T>(int startIndex) where T : struct
        {
            var t = typeof(T);

            if (t == typeof(int)) return (T)(object)BitConverter.ToInt32(m_Data, startIndex);
            if (t == typeof(float)) return (T)(object)BitConverter.ToSingle(m_Data, startIndex);
            if (t == typeof(double)) return (T)(object)BitConverter.ToDouble(m_Data, startIndex);
            if (t == typeof(bool)) return (T)(object)BitConverter.ToBoolean(m_Data, startIndex);
            if (t == typeof(short)) return (T)(object)BitConverter.ToInt16(m_Data, startIndex);
            if (t == typeof(long)) return (T)(object)BitConverter.ToInt64(m_Data, startIndex);
            if (t == typeof(ushort)) return (T)(object)BitConverter.ToUInt16(m_Data, startIndex);
            if (t == typeof(uint)) return (T)(object)BitConverter.ToUInt32(m_Data, startIndex);
            if (t == typeof(ulong)) return (T)(object)BitConverter.ToUInt64(m_Data, startIndex);
            if (t == typeof(char)) return (T)(object)BitConverter.ToChar(m_Data, startIndex);

            if (t == typeof(Vec2F)) return (T)(object)Vec2F.FromBytes(m_Data, startIndex);
            if (t == typeof(Vec3F)) return (T)(object)Vec3F.FromBytes(m_Data, startIndex);
            if (t == typeof(Vec4F)) return (T)(object)Vec4F.FromBytes(m_Data, startIndex);
            if (t == typeof(Vec2U)) return (T)(object)Vec2U.FromBytes(m_Data, startIndex);

            throw new ArgumentException($"Type {t} is not supported by BitConverter");
        }

        public bool IsEqual(ByteBuffer other)
        {
            if (other == null)
                return false;

            if (m_Size != other.m_Size)
                return false;

            for (int i = 0; i < m_Size; i++)
            {
                if (m_Data[i] != other.m_Data[i])
                    return false;
            }
            return true;
        }

        public static ByteBuffer LoadFile(string path)
        {
            byte[] bytes = File.ReadAllBytes(path);
            ByteBuffer buffer = new ByteBuffer();
            buffer.Assign(bytes);
            return buffer;
        }

        public static unsafe ByteBuffer LoadBytes(byte* bytes, uint size)
        {
            ByteBuffer byteBuffer = new ByteBuffer();
            byteBuffer.Resize((int)size);

            for (int i = 0; i < byteBuffer.Size; i++)
                byteBuffer.Data[i] = bytes[i];

            return byteBuffer;
        }

        public ByteBuffer Clone()
        {
            return new ByteBuffer(m_Data);
        }
    }
}
