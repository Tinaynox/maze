using System;
using System.Collections.Generic;

namespace Maze.Core
{
    public class ByteBufferReadStream
    {
        ByteBuffer m_ByteBuffer;
        public int ByteBuffereSize => m_ByteBuffer?.Size ?? 0;
        public byte[] ByteBufferData => m_ByteBuffer?.Data;

        int m_Offset;
        public int Offset => m_Offset;
        
        public ByteBufferReadStream(
            ByteBuffer byteBuffer,
            int offset = 0)
        {
            Assign(byteBuffer, offset);
        }

        public void Assign(
            ByteBuffer byteBuffer,
            int offset = 0)
        {
            m_ByteBuffer = byteBuffer;
            m_Offset = offset;
        }

        public int ReadNoRewind(byte[] dst, int readSize, int dstOffset = 0)
        {
            readSize = Math.Min(readSize, ByteBuffereSize - m_Offset);
            Buffer.BlockCopy(m_ByteBuffer.Data, m_Offset, dst, dstOffset, readSize);
            return readSize;
        }

        public int ReadNoRewind(out byte[] buffer, int readSize, int dstOffset = 0)
        {
            buffer = new byte[readSize];
            return ReadNoRewind(buffer, readSize);
        }

        public ByteBufferReadStream ReadNoRewind(out byte value)
        {
            if (CanRead(sizeof(byte)))
                value = m_ByteBuffer.Data[m_Offset];
            else
                value = default;
            return this;
        }

        public int Read(byte[] dst, int readSize, int dstOffset = 0)
        {
            readSize = ReadNoRewind(dst, readSize, dstOffset);
            m_Offset += readSize;
            return readSize;
        }

        public ByteBufferReadStream Read(out byte[] arr, int readSize)
        {
            arr = new byte[readSize];
            Read(arr, readSize);
            return this;
        }

        public bool CanRead(int size)
        {
            return m_Offset + size <= ByteBuffereSize;
        }

        public bool SetOffset(int value)
        {
            if (value <= ByteBuffereSize)
            {
                m_Offset = value;
                return true;
            }
            else
            {
                m_Offset = ByteBuffereSize;
                return false;
            }
        }

        public int Rewind(int delta)
        {
            if (delta > 0)
                SetOffset(m_Offset + delta);
            else
            if (delta >= m_Offset)
                SetOffset(m_Offset - delta);
            else
                SetOffset(0);

            return m_Offset;
        }

        public bool IsEndOfBuffer()
        {
            return m_Offset >= ByteBuffereSize;
        }

        public ByteBufferReadStream Read(out byte value)
        {
            value = default;
            if (CanRead(sizeof(byte)))
            {
                value = m_ByteBuffer.Data[m_Offset];
                m_Offset += sizeof(byte);
            }
            return this;
        }

        public ByteBufferReadStream Read(out short value)
        {
            value = default;
            if (CanRead(sizeof(short)))
            {
                value = BitConverter.ToInt16(m_ByteBuffer.Data, m_Offset);
                m_Offset += sizeof(short);
            }
            return this;
        }

        public ByteBufferReadStream Read(out int value)
        {
            value = default;
            if (CanRead(sizeof(int)))
            {
                value = BitConverter.ToInt32(m_ByteBuffer.Data, m_Offset);
                m_Offset += sizeof(int);
            }
            return this;
        }

        public ByteBufferReadStream Read(out long value)
        {
            value = default;
            if (CanRead(sizeof(long)))
            {
                value = BitConverter.ToInt64(m_ByteBuffer.Data, m_Offset);
                m_Offset += sizeof(long);
            }
            return this;
        }

        public ByteBufferReadStream Read(out sbyte value)
        {
            value = default;
            if (CanRead(sizeof(sbyte)))
            {
                value = Convert.ToSByte(m_ByteBuffer.Data[m_Offset]);
                m_Offset += sizeof(sbyte);
            }
            return this;
        }

        public ByteBufferReadStream Read(out ushort value)
        {
            value = default;
            if (CanRead(sizeof(ushort)))
            {
                value = BitConverter.ToUInt16(m_ByteBuffer.Data, m_Offset);
                m_Offset += sizeof(ushort);
            }
            return this;
        }

        public ByteBufferReadStream Read(out uint value)
        {
            value = default;
            if (CanRead(sizeof(uint)))
            {
                value = BitConverter.ToUInt32(m_ByteBuffer.Data, m_Offset);
                m_Offset += sizeof(uint);
            }
            return this;
        }

        public ByteBufferReadStream Read(out ulong value)
        {
            value = default;
            if (CanRead(sizeof(ulong)))
            {
                value = BitConverter.ToUInt64(m_ByteBuffer.Data, m_Offset);
                m_Offset += sizeof(ulong);
            }
            return this;
        }

        public ByteBufferReadStream Read(out float value)
        {
            value = default;
            if (CanRead(sizeof(float)))
            {
                value = BitConverter.ToSingle(m_ByteBuffer.Data, m_Offset);
                m_Offset += sizeof(float);
            }
            return this;
        }

        public ByteBufferReadStream Read(out double value)
        {
            value = default;
            if (CanRead(sizeof(double)))
            {
                value = BitConverter.ToDouble(m_ByteBuffer.Data, m_Offset);
                m_Offset += sizeof(double);
            }
            return this;
        }

        public ByteBufferReadStream Read(out Vec4S8 value) { Read(out byte[] arr, 4); value = Vec4S8.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec4U8 value) { Read(out byte[] arr, 4); value = Vec4U8.FromBytes(arr); return this; }

        public ByteBufferReadStream Read(out Vec2S value) { Read(out byte[] arr, 8); value = Vec2S.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec3S value) { Read(out byte[] arr, 12); value = Vec3S.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec4S value) { Read(out byte[] arr, 16); value = Vec4S.FromBytes(arr); return this; }

        public ByteBufferReadStream Read(out Vec2U value) { Read(out byte[] arr, 8); value = Vec2U.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec3U value) { Read(out byte[] arr, 12); value = Vec3U.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec4U value) { Read(out byte[] arr, 16); value = Vec4U.FromBytes(arr); return this; }

        public ByteBufferReadStream Read(out Vec2F value) { Read(out byte[] arr, 8); value = Vec2F.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec3F value) { Read(out byte[] arr, 12); value = Vec3F.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec4F value) { Read(out byte[] arr, 16); value = Vec4F.FromBytes(arr); return this; }

        public ByteBufferReadStream Read(out Vec2B value) { Read(out byte[] arr, 2); value = Vec2B.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec3B value) { Read(out byte[] arr, 3); value = Vec3B.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Vec4B value) { Read(out byte[] arr, 4); value = Vec4B.FromBytes(arr); return this; }

        public ByteBufferReadStream Read(out Mat3F value) { Read(out byte[] arr, 36); value = Mat3F.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out Mat4F value) { Read(out byte[] arr, 64); value = Mat4F.FromBytes(arr); return this; }
        public ByteBufferReadStream Read(out TMat value) { Read(out byte[] arr, 48); value = TMat.FromBytes(arr); return this; }
    }
}
