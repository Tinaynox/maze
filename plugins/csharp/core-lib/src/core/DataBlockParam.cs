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
            switch (default(T))
            {
                case sbyte _: return DataBlockParamType.ParamS8;
                case short _: return DataBlockParamType.ParamS16;
                case int _: return DataBlockParamType.ParamS32;
                case long _: return DataBlockParamType.ParamS64;
                case byte _: return DataBlockParamType.ParamU8;
                case ushort _: return DataBlockParamType.ParamU16;
                case uint _: return DataBlockParamType.ParamU32;
                case ulong _: return DataBlockParamType.ParamU64;
                case float _: return DataBlockParamType.ParamF32;
                case double _: return DataBlockParamType.ParamF64;
                case bool _: return DataBlockParamType.ParamBool;
                case Vec4S8 _: return DataBlockParamType.ParamVec4S8;
                case Vec4U8 _: return DataBlockParamType.ParamVec4U8;
                case Vec2S _: return DataBlockParamType.ParamVec2S32;
                case Vec3S _: return DataBlockParamType.ParamVec3S32;
                case Vec4S _: return DataBlockParamType.ParamVec4S32;
                case Vec2U _: return DataBlockParamType.ParamVec2U32;
                case Vec3U _: return DataBlockParamType.ParamVec3U32;
                case Vec4U _: return DataBlockParamType.ParamVec4U32;
                case Vec2F _: return DataBlockParamType.ParamVec2F32;
                case Vec3F _: return DataBlockParamType.ParamVec3F32;
                case Vec4F _: return DataBlockParamType.ParamVec4F32;
                case Vec2B _: return DataBlockParamType.ParamVec2B;
                case Vec3B _: return DataBlockParamType.ParamVec3B;
                case Vec4B _: return DataBlockParamType.ParamVec4B;
                case Mat3F _: return DataBlockParamType.ParamMat3F32;
                case Mat4F _: return DataBlockParamType.ParamMat4F32;
                case TMat _: return DataBlockParamType.ParamTMat;
                case string _: return DataBlockParamType.ParamString;
                default: throw new NotImplementedException();
            }
        }

        public static byte[] GetBytes<T>(T value) where T : struct
        {
            switch (value)
            {
                case sbyte castValue: return BitConverter.GetBytes(castValue);
                case short castValue: return BitConverter.GetBytes(castValue);
                case int castValue: return BitConverter.GetBytes(castValue);
                case long castValue: return BitConverter.GetBytes(castValue);
                case byte castValue: return BitConverter.GetBytes(castValue);
                case ushort castValue: return BitConverter.GetBytes(castValue);
                case uint castValue: return BitConverter.GetBytes(castValue);
                case ulong castValue: return BitConverter.GetBytes(castValue);
                case float castValue: return BitConverter.GetBytes(castValue);
                case double castValue: return BitConverter.GetBytes(castValue);
                case bool castValue: return BitConverter.GetBytes(castValue);
                case Vec4S8 castValue: return castValue.GetBytes();
                case Vec4U8 castValue: return castValue.GetBytes();
                case Vec2S castValue: return castValue.GetBytes();
                case Vec3S castValue: return castValue.GetBytes();
                case Vec4S castValue: return castValue.GetBytes();
                case Vec2U castValue: return castValue.GetBytes();
                case Vec3U castValue: return castValue.GetBytes();
                case Vec4U castValue: return castValue.GetBytes();
                case Vec2F castValue: return castValue.GetBytes();
                case Vec3F castValue: return castValue.GetBytes();
                case Vec4F castValue: return castValue.GetBytes();
                case Vec2B castValue: return castValue.GetBytes();
                case Vec3B castValue: return castValue.GetBytes();
                case Vec4B castValue: return castValue.GetBytes();
                case Mat3F castValue: return castValue.GetBytes();
                case Mat4F castValue: return castValue.GetBytes();
                case TMat castValue: return castValue.GetBytes();
                default: throw new NotImplementedException($"value={value}");
            }
        }
    }
}
