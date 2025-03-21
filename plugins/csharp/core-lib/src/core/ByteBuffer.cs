using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    public class ByteBuffer
    {
        byte[] m_Data;

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

        public void Append(byte[] data, int size)
        {
            int requiredSize = m_Size + size;

            if (requiredSize > m_Capacity)
                Reserve(Math.Max(requiredSize, m_Capacity + (m_Capacity >> 1) + 1));

            Buffer.BlockCopy(data, 0, m_Data, m_Size, size);
            m_Size += size;
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
            switch (value)
            {
                case int intValue:
                    Append(BitConverter.GetBytes(intValue));
                    break;
                case float floatValue:
                    Append(BitConverter.GetBytes(floatValue));
                    break;
                case double doubleValue:
                    Append(BitConverter.GetBytes(doubleValue));
                    break;
                case bool boolValue:
                    Append(BitConverter.GetBytes(boolValue));
                    break;
                case short shortValue:
                    Append(BitConverter.GetBytes(shortValue));
                    break;
                case long longValue:
                    Append(BitConverter.GetBytes(longValue));
                    break;
                case ushort ushortValue:
                    Append(BitConverter.GetBytes(ushortValue));
                    break;
                case uint uintValue:
                    Append(BitConverter.GetBytes(uintValue));
                    break;
                case ulong ulongValue:
                    Append(BitConverter.GetBytes(ulongValue));
                    break;
                case char charValue:
                    Append(BitConverter.GetBytes(charValue));
                    break;
                case Vec2F vecValue:
                    Append(vecValue.GetBytes());
                    break;
                case Vec3F vecValue:
                    Append(vecValue.GetBytes());
                    break;
                case Vec4F vecValue:
                    Append(vecValue.GetBytes());
                    break;
                case Vec2U vecValue:
                    Append(vecValue.GetBytes());
                    break;
                default:
                    throw new ArgumentException($"Type {typeof(T)} is not supported by BitConverter");
            }
        }

        public unsafe T ReadAs<T>(int startIndex) where T : struct
        {
            // #TODO: Optimize?
            switch (default(T))
            {
                case int intValue:
                    return (T)(object)BitConverter.ToInt32(m_Data, startIndex);
                case float floatValue:
                    return (T)(object)BitConverter.ToSingle(m_Data, startIndex);
                case double doubleValue:
                    return (T)(object)BitConverter.ToDouble(m_Data, startIndex);
                case bool boolValue:
                    return (T)(object)BitConverter.ToBoolean(m_Data, startIndex);
                case short shortValue:
                    return (T)(object)BitConverter.ToInt16(m_Data, startIndex);
                case long longValue:
                    return (T)(object)BitConverter.ToInt64(m_Data, startIndex);
                case ushort ushortValue:
                    return (T)(object)BitConverter.ToUInt16(m_Data, startIndex);
                case uint uintValue:
                    return (T)(object)BitConverter.ToUInt32(m_Data, startIndex);
                case ulong ulongValue:
                    return (T)(object)BitConverter.ToUInt64(m_Data, startIndex);
                case char charValue:
                    return (T)(object)BitConverter.ToChar(m_Data, startIndex);
                case Vec2F vecValue:
                    return (T)(object)Vec2F.FromBytes(m_Data, startIndex);
                case Vec3F vecValue:
                    return (T)(object)Vec3F.FromBytes(m_Data, startIndex);
                case Vec4F vecValue:
                    return (T)(object)Vec4F.FromBytes(m_Data, startIndex);
                case Vec2U vecValue:
                    return (T)(object)Vec2U.FromBytes(m_Data, startIndex);
                default:
                    throw new ArgumentException($"Type {typeof(T)} is not supported by BitConverter");
            }

            return default(T);
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

        public ByteBuffer Clone()
        {
            return new ByteBuffer(m_Data);
        }
    }
}
