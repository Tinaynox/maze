using System;
using System.Collections.Generic;

namespace Maze.Core
{
    public enum DataBlockParamType : byte
    {
        None = 0,
        ParamS8,
        ParamS16,
        ParamS32,
        ParamS64,
        ParamU8,
        ParamU16,
        ParamU32,
        ParamU64,
        ParamF32,
        ParamF64,
        ParamBool,
        ParamVec4S8,
        ParamVec4U8,
        ParamVec2S32,
        ParamVec3S32,
        ParamVec4S32,
        ParamVec2U32,
        ParamVec3U32,
        ParamVec4U32,
        ParamVec2F32,
        ParamVec3F32,
        ParamVec4F32,
        ParamVec2B,
        ParamVec3B,
        ParamVec4B,
        ParamMat3F32,
        ParamMat4F32,
        ParamTMat,
        ParamString,

        MAX
    }

    public struct DataBlockParam
    {
        public uint NameId;
        public DataBlockParamType Type;
        public object Value;

        public static DataBlockParamType GetType<T>()
        {
            var t = typeof(T);
            if (t == typeof(sbyte)) return DataBlockParamType.ParamS8;
            if (t == typeof(short)) return DataBlockParamType.ParamS16;
            if (t == typeof(int)) return DataBlockParamType.ParamS32;
            if (t == typeof(long)) return DataBlockParamType.ParamS64;
            if (t == typeof(byte)) return DataBlockParamType.ParamU8;
            if (t == typeof(ushort)) return DataBlockParamType.ParamU16;
            if (t == typeof(uint)) return DataBlockParamType.ParamU32;
            if (t == typeof(ulong)) return DataBlockParamType.ParamU64;
            if (t == typeof(float)) return DataBlockParamType.ParamF32;
            if (t == typeof(double)) return DataBlockParamType.ParamF64;
            if (t == typeof(bool)) return DataBlockParamType.ParamBool;
            if (t == typeof(Vec4S8)) return DataBlockParamType.ParamVec4S8;
            if (t == typeof(Vec4U8)) return DataBlockParamType.ParamVec4U8;
            if (t == typeof(Vec2S)) return DataBlockParamType.ParamVec2S32;
            if (t == typeof(Vec3S)) return DataBlockParamType.ParamVec3S32;
            if (t == typeof(Vec4S)) return DataBlockParamType.ParamVec4S32;
            if (t == typeof(Vec2U)) return DataBlockParamType.ParamVec2U32;
            if (t == typeof(Vec3U)) return DataBlockParamType.ParamVec3U32;
            if (t == typeof(Vec4U)) return DataBlockParamType.ParamVec4U32;
            if (t == typeof(Vec2F)) return DataBlockParamType.ParamVec2F32;
            if (t == typeof(Vec3F)) return DataBlockParamType.ParamVec3F32;
            if (t == typeof(Vec4F)) return DataBlockParamType.ParamVec4F32;
            if (t == typeof(Vec2B)) return DataBlockParamType.ParamVec2B;
            if (t == typeof(Vec3B)) return DataBlockParamType.ParamVec3B;
            if (t == typeof(Vec4B)) return DataBlockParamType.ParamVec4B;
            if (t == typeof(Mat3F)) return DataBlockParamType.ParamMat3F32;
            if (t == typeof(Mat4F)) return DataBlockParamType.ParamMat4F32;
            if (t == typeof(TMat)) return DataBlockParamType.ParamTMat;
            if (t == typeof(string)) return DataBlockParamType.ParamString;
            throw new NotImplementedException();
        }

        public static byte[] GetBytes<T>(T value) where T : struct
        {
            var t = typeof(T);
            object o = value;

            if (t == typeof(sbyte)) return BitConverter.GetBytes((sbyte)o);
            if (t == typeof(short)) return BitConverter.GetBytes((short)o);
            if (t == typeof(int)) return BitConverter.GetBytes((int)o);
            if (t == typeof(long)) return BitConverter.GetBytes((long)o);
            if (t == typeof(byte)) return BitConverter.GetBytes((byte)o);
            if (t == typeof(ushort)) return BitConverter.GetBytes((ushort)o);
            if (t == typeof(uint)) return BitConverter.GetBytes((uint)o);
            if (t == typeof(ulong)) return BitConverter.GetBytes((ulong)o);
            if (t == typeof(float)) return BitConverter.GetBytes((float)o);
            if (t == typeof(double)) return BitConverter.GetBytes((double)o);
            if (t == typeof(bool)) return BitConverter.GetBytes((bool)o);
            if (t == typeof(Vec4S8)) return ((Vec4S8)o).GetBytes();
            if (t == typeof(Vec4U8)) return ((Vec4U8)o).GetBytes();
            if (t == typeof(Vec2S)) return ((Vec2S)o).GetBytes();
            if (t == typeof(Vec3S)) return ((Vec3S)o).GetBytes();
            if (t == typeof(Vec4S)) return ((Vec4S)o).GetBytes();
            if (t == typeof(Vec2U)) return ((Vec2U)o).GetBytes();
            if (t == typeof(Vec3U)) return ((Vec3U)o).GetBytes();
            if (t == typeof(Vec4U)) return ((Vec4U)o).GetBytes();
            if (t == typeof(Vec2F)) return ((Vec2F)o).GetBytes();
            if (t == typeof(Vec3F)) return ((Vec3F)o).GetBytes();
            if (t == typeof(Vec4F)) return ((Vec4F)o).GetBytes();
            if (t == typeof(Vec2B)) return ((Vec2B)o).GetBytes();
            if (t == typeof(Vec3B)) return ((Vec3B)o).GetBytes();
            if (t == typeof(Vec4B)) return ((Vec4B)o).GetBytes();
            if (t == typeof(Mat3F)) return ((Mat3F)o).GetBytes();
            if (t == typeof(Mat4F)) return ((Mat4F)o).GetBytes();
            if (t == typeof(TMat)) return ((TMat)o).GetBytes();

            throw new NotImplementedException($"value={value}");
        }
    }
}
