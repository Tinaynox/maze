using System;
using System.Collections.Generic;

namespace Maze.Core
{
    public enum DataBlockFlags : uint
    {
        TopmostBlock = 1
    }

    public enum DataBlockParamType : uint
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
        public uint Value;

        public static DataBlockParamType GetType<T>() where T : struct
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
                case Vec2F _: return DataBlockParamType.ParamVec2F32;
                case Vec3F _: return DataBlockParamType.ParamVec3F32;
                case Vec4F _: return DataBlockParamType.ParamVec4F32;
                case Vec2U _: return DataBlockParamType.ParamVec2U32;
                case Mat3F _: return DataBlockParamType.ParamMat3F32;
                case Mat4F _: return DataBlockParamType.ParamMat4F32;
                case TMat _: return DataBlockParamType.ParamTMat;
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
                case Vec2F castValue: return castValue.GetBytes();
                case Vec3F castValue: return castValue.GetBytes();
                case Vec4F castValue: return castValue.GetBytes();
                case Vec2U castValue: return castValue.GetBytes();
                default: throw new NotImplementedException($"value={value}");
            }
        }
    }

    public class DataBlock
    {
        DataBlockShared m_Shared;
        List<DataBlockParam> m_Params;
        ByteBuffer m_ComplexParamsBuffer;

        uint m_NameId;
        public uint NameId => m_NameId;

        uint m_Flags;
        public uint Flags => m_Flags;

        ushort m_DataBlockCount;
        public ushort DataBlockCount => m_DataBlockCount;

        public int ParamsCount => m_Params?.Count ?? 0;



        public DataBlock()
        {
            m_Shared = new DataBlockShared();
            m_Flags |= (uint)DataBlockFlags.TopmostBlock;
        }


        #region Shared
        uint AddSharedString(string name)
        {
            return m_Shared.AddString(name);
        }

        uint GetSharedStringId(string name)
        {
            return m_Shared.GetStringId(name);
        }
        #endregion


        #region Param
        List<DataBlockParam> EnsureParams()
        {
            if (m_Params == null)
                m_Params = new List<DataBlockParam>();

            return m_Params;
        }

        ByteBuffer EnsureComplexParamsBuffer()
        {
            if (m_ComplexParamsBuffer == null)
                m_ComplexParamsBuffer = new ByteBuffer();

            return m_ComplexParamsBuffer;
        }

        public void InsertSimpleParamAt<T>(int atIndex, uint nameId, DataBlockParamType type, uint value) where T : struct
        {
            EnsureParams().Insert(atIndex, new DataBlockParam { NameId = nameId, Type = type, Value = value });
        }

        public void InsertParamAt<T>(int atIndex, uint nameId, T value) where T : struct
        {
            switch (value)
            {
                case sbyte _:
                case short _:
                case int _:
                case byte _:
                case ushort _:
                case uint _:
                case float _:
                case bool _:

                    uint valueUint;
                    byte[] bytes = DataBlockParam.GetBytes(value);
                    if (bytes.Length < 4)
                    {
                        byte[] paddedBytes = new byte[4];
                        Array.Copy(bytes, 0, paddedBytes, 0, bytes.Length);
                        valueUint = BitConverter.ToUInt32(paddedBytes, 0);
                    }
                    else
                    {
                        valueUint = BitConverter.ToUInt32(bytes, 0);
                    }

                    InsertSimpleParamAt<T>(atIndex, nameId, DataBlockParam.GetType<T>(), valueUint);
                    break;
                default: throw new NotImplementedException($"value={value}");
            }
        }

        public int AddParamByNameId<T>(uint nameId, T value) where T : struct
        {
            int at = ParamsCount;
            InsertParamAt<T>(at, nameId, value);
            return at;
        }

        public int AddParamByName<T>(string name, T value) where T : struct
        {
            uint nameId = AddSharedString(name);
            return AddParamByNameId<T>(nameId, value);
        }


        public void SetParam<T>(int paramIndex, T value) where T : struct
        {

        }

        public DataBlockParam GetParam(int paramIndex)
        {
            if (paramIndex < 0 || paramIndex >= ParamsCount)
                throw new ArgumentOutOfRangeException();

            return m_Params[paramIndex];
        }

        T GetParamValue<T>(int paramIndex, T defValue = default(T)) where T : struct
        {
            DataBlockParam param = GetParam(paramIndex);
            DataBlockParamType type = DataBlockParam.GetType<T>();

            if (param.Type != type)
            {
                Debug.LogError($"Param type mismatch. Inner type: '{param.Type}'. Requested type: '{type}'");
                return defValue;
            }

            switch (type)
            {
                case DataBlockParamType.ParamS8: return (T)(object)(sbyte)BitConverter.ToInt32(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamS16: return (T)(object)BitConverter.ToInt16(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamS32: return (T)(object)BitConverter.ToInt32(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamS64: return (T)(object)BitConverter.ToInt64(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamU8: return (T)(object)(byte)BitConverter.ToUInt32(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamU16: return (T)(object)BitConverter.ToUInt16(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamU32: return (T)(object)BitConverter.ToUInt32(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamU64: return (T)(object)BitConverter.ToUInt64(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamF32: return (T)(object)BitConverter.ToSingle(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamF64: return (T)(object)BitConverter.ToDouble(DataBlockParam.GetBytes(param.Value), 0);
                case DataBlockParamType.ParamBool: return (T)(object)BitConverter.ToBoolean(DataBlockParam.GetBytes(param.Value), 0);
                default: throw new NotImplementedException($"param.Type={param.Type}");
            }
        }

        int FindParamIndex(uint nameId)
        {
            if (ParamsCount == 0 || nameId == 0)
                return -1;

            for (int i = 0, j = ParamsCount; i != j; ++i)
                if (m_Params[i].NameId == nameId)
                    return i;

            return -1;
        }

        T GetParamValueByNameId<T>(uint nameId, T defValue = default(T)) where T : struct
        {
            int paramIndex = FindParamIndex(nameId);
            return paramIndex < 0 ? defValue : GetParamValue(paramIndex, defValue);
        }

        T GetParamValueByName<T>(string name, T defValue = default(T)) where T : struct
        {
            uint nameId = GetSharedStringId(name);
            return nameId == 0 ? defValue : GetParamValueByNameId(nameId, defValue);
        }
        #endregion

        #region Param S8
        public void AddS8(string name, sbyte value) { AddParamByName(name, value); }
        public void SetS8(int paramIndex, sbyte value) { SetParam(paramIndex, value); }
        public sbyte GetS8(string name, sbyte defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion

        #region Param S16
        public void AddS16(string name, short value) { AddParamByName(name, value); }
        public void SetS16(int paramIndex, short value) { SetParam(paramIndex, value); }
        public short GetS16(string name, short defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion

        #region Param S32
        public void AddS32(string name, int value) { AddParamByName(name, value); }
        public void SetS32(int paramIndex, int value) { SetParam(paramIndex, value); }
        public int GetS32(string name, int defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion

        #region Param S64
        public void AddS64(string name, long value) { AddParamByName(name, value); }
        public void SetS64(int paramIndex, long value) { SetParam(paramIndex, value); }
        public long GetS64(string name, long defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion

        #region Param Bool
        public void AddBool(string name, bool value) { AddParamByName(name, value); }
        public void SetBool(int paramIndex, bool value) { SetParam(paramIndex, value); }
        public bool GetBool(string name, bool defValue = false) { return GetParamValueByName(name, defValue); }
        #endregion
    }
}
