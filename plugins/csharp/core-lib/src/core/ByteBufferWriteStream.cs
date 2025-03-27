using System;
using System.Collections.Generic;

namespace Maze.Core
{
    public class ByteBufferWriteStream
    {
        ByteBuffer m_ByteBuffer;
        public int ByteBuffereSize => m_ByteBuffer?.Size ?? 0;
        public int ByteBuffereCapacity => m_ByteBuffer?.Capacity ?? 0;
        public byte[] ByteBufferData => m_ByteBuffer?.Data;

        int m_Offset;
        public int Offset => m_Offset;
        
        public ByteBufferWriteStream(
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

        public void ReserveToFit(int size)
        {
            int requiredSize = m_Offset + size;
            if (ByteBuffereCapacity < requiredSize)
            {
                m_ByteBuffer.Reserve(
                    Math.Max(requiredSize, ByteBuffereCapacity + (ByteBuffereCapacity >> 1) + 1));
            }
        }

        public ByteBufferWriteStream Write(byte[] arr, int size)
        {
            if (size <= 0)
                return this;

            ReserveToFit(size);
            m_ByteBuffer.Append(arr, size);
            return this;
        }

        public ByteBufferWriteStream Write(byte[] arr)
        {
            return Write(arr, arr.Length);
        }

        public ByteBufferWriteStream WriteASCII(string str)
        {
            byte[] asciiBytes = System.Text.Encoding.ASCII.GetBytes(str);
            return Write(asciiBytes);
        }

        public ByteBufferWriteStream Write(sbyte value) { ReserveToFit(sizeof(sbyte)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(short value) { ReserveToFit(sizeof(short)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(int value) { ReserveToFit(sizeof(int)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(long value) { ReserveToFit(sizeof(long)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(byte value) { ReserveToFit(sizeof(byte)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(ushort value) { ReserveToFit(sizeof(ushort)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(uint value) { ReserveToFit(sizeof(uint)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(ulong value) { ReserveToFit(sizeof(ulong)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(float value) { ReserveToFit(sizeof(float)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(double value) { ReserveToFit(sizeof(double)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(char value) { ReserveToFit(sizeof(char)); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec4S8 value) { ReserveToFit(4); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec4U8 value) { ReserveToFit(4); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec2S value) { ReserveToFit(8); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec3S value) { ReserveToFit(12); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec4S value) { ReserveToFit(16); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec2U value) { ReserveToFit(8); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec3U value) { ReserveToFit(12); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec4U value) { ReserveToFit(16); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec2F value) { ReserveToFit(8); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec3F value) { ReserveToFit(12); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec4F value) { ReserveToFit(16); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec2B value) { ReserveToFit(2); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec3B value) { ReserveToFit(3); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Vec4B value) { ReserveToFit(4); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Mat3F value) { ReserveToFit(36); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(Mat4F value) { ReserveToFit(64); m_ByteBuffer.Append(value); return this; }
        public ByteBufferWriteStream Write(TMat value) { ReserveToFit(48); m_ByteBuffer.Append(value); return this; }
    }
}
