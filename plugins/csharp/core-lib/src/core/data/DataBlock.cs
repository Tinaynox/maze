using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Maze.Core
{
    public enum DataBlockFlags : uint
    {
        TopmostBlock = 1
    }

    public class DataBlock
        : IEnumerable<DataBlock>
    {
        DataBlockShared m_Shared;
        public DataBlockShared Shared => m_Shared;

        List<DataBlockParam> m_Params;
        List<DataBlock> m_DataBlocks;

        uint m_NameId;
        public uint NameId => m_NameId;
        public string Name => GetSharedString(NameId);

        uint m_Flags;
        public uint Flags => m_Flags;

        public int ParamsCount => m_Params?.Count ?? 0;
        public int DataBlocksCount => m_DataBlocks?.Count ?? 0;

        public DataBlock()
        {
            m_Shared = new DataBlockShared();
            m_Flags |= (uint)DataBlockFlags.TopmostBlock;
        }

        public DataBlock(DataBlockShared shared, string name)
        {
            m_Shared = shared;
            m_NameId = AddSharedString(name);
        }

        public bool IsTopmost() => (m_Flags & (uint)DataBlockFlags.TopmostBlock) != 0u;
        public bool IsEmpty() => ParamsCount == 0 && DataBlocksCount == 0;

        public void ClearData()
        {
            m_Params?.Clear();
            m_DataBlocks?.Clear();
        }

        public void Clear()
        {
            if (!IsTopmost())
            {
                Debug.LogError("Clear is cannot be called for the not topmost block");
                return;
            }

            m_NameId = 0u;

            ClearData();
            m_Shared.Clear();
        }

        #region IEnumerable
        public IEnumerator<DataBlock> GetEnumerator()
        {
            return m_DataBlocks?.GetEnumerator() ?? Enumerable.Empty<DataBlock>().GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
        #endregion


        #region Shared
        public uint AddSharedString(string name)
        {
            return m_Shared.AddString(name);
        }

        public uint GetSharedStringId(string name)
        {
            return m_Shared.GetStringId(name);
        }

        public string GetSharedString(uint nameId)
        {
            return m_Shared.GetString(nameId);
        }
        #endregion

        #region Param
        List<DataBlockParam> EnsureParams()
        {
            if (m_Params == null)
                m_Params = new List<DataBlockParam>();

            return m_Params;
        }

        List<DataBlock> EnsureDataBlocks()
        {
            if (m_DataBlocks == null)
                m_DataBlocks = new List<DataBlock>();

            return m_DataBlocks;
        }

        public void ResizeParams(int paramsCount)
        {
            EnsureParams().Resize(paramsCount);
        }

        public void InsertParamAt(int atIndex, uint nameId, DataBlockParamType type, object value)
        {
            EnsureParams().Insert(atIndex, new DataBlockParam { NameId = nameId, Type = type, Value = value });
        }

        public void SetParamAt(int atIndex, uint nameId, DataBlockParamType type, object value)
        {
            EnsureParams()[atIndex] = new DataBlockParam { NameId = nameId, Type = type, Value = value };
        }

        public int AddParamByNameId<T>(uint nameId, T value) where T : struct
        {
            int at = ParamsCount;
            InsertParamAt(at, nameId, DataBlockParam.GetType<T>(), value);
            return at;
        }

        public int AddParamByName<T>(string name, T value) where T : struct
        {
            uint nameId = AddSharedString(name);
            return AddParamByNameId<T>(nameId, value);
        }

        public void SetParam<T>(int paramIndex, uint nameId, T value) where T : struct
        {
            SetParamAt(paramIndex, nameId, DataBlockParam.GetType<T>(), value);
        }

        public void SetParam<T>(int paramIndex, T value) where T : struct
        {
            var param = GetParam(paramIndex);
            SetParamAt(paramIndex, param.NameId, DataBlockParam.GetType<T>(), value);
        }

        public void SetParam<T>(string name, T value) where T : struct
        {
            uint nameId = AddSharedString(name);
            int paramIndex = FindParamIndex(nameId);
            if (paramIndex >= 0)
                SetParamAt(paramIndex, nameId, DataBlockParam.GetType<T>(), value);
            else
                AddParamByNameId(nameId, value);
        }

        public DataBlockParam GetParam(int paramIndex)
        {
            if (paramIndex < 0 || paramIndex >= ParamsCount)
                throw new ArgumentOutOfRangeException();

            return m_Params[paramIndex];
        }

        public T GetParamValue<T>(int paramIndex, T defValue = default(T)) where T : struct
        {
            DataBlockParam param = GetParam(paramIndex);
            DataBlockParamType type = DataBlockParam.GetType<T>();

            if (param.Type != type)
            {
                Debug.LogError($"Param type mismatch. Inner type: '{param.Type}'. Requested type: '{type}'");
                return defValue;
            }

            return (T)param.Value;
        }

        public int FindParamIndex(uint nameId)
        {
            if (ParamsCount == 0 || nameId == 0)
                return -1;

            for (int i = 0, j = ParamsCount; i != j; ++i)
                if (m_Params[i].NameId == nameId)
                    return i;

            return -1;
        }

        public int FindParamIndex(string name)
        {
            uint nameId = GetSharedStringId(name);
            if (nameId == 0)
                return -1;

            return FindParamIndex(nameId);
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

        public int AddParamByNameId(uint nameId, string value)
        {
            int at = ParamsCount;
            uint valueNameId = AddSharedString(value);
            InsertParamAt(at, nameId, DataBlockParamType.ParamString, valueNameId);
            return at;
        }

        public int AddParamByName(string name, string value)
        {
            uint nameId = AddSharedString(name);
            return AddParamByNameId(nameId, value);
        }

        public void SetParam(int paramIndex, string value)
        {
            var param = GetParam(paramIndex);
            SetParamAt(paramIndex, param.NameId, DataBlockParamType.ParamString, AddSharedString(value));
        }

        public void SetParam(string name, string value)
        {
            uint nameId = AddSharedString(name);
            int paramIndex = FindParamIndex(nameId);
            if (paramIndex >= 0)
                SetParamAt(paramIndex, nameId, DataBlockParamType.ParamString, AddSharedString(value));
            else
                AddParamByNameId(nameId, value);
        }

        string GetParamValue(int paramIndex, string defValue)
        {
            DataBlockParam param = GetParam(paramIndex);

            if (param.Type != DataBlockParamType.ParamString)
            {
                Debug.LogError($"Param type mismatch. Inner type: '{param.Type}'. Requested type: '{DataBlockParamType.ParamString}'");
                return defValue;
            }

            return GetSharedString((uint)param.Value) ?? defValue;
        }

        string GetParamValueByNameId(uint nameId, string defValue = default)
        {
            int paramIndex = FindParamIndex(nameId);
            return paramIndex < 0 ? defValue : GetParamValue(paramIndex, defValue);
        }

        string GetParamValueByName(string name, string defValue = default)
        {
            uint nameId = GetSharedStringId(name);
            return nameId == 0 ? defValue : GetParamValueByNameId(nameId, defValue);
        }
        #endregion


        #region Param S8
        public void AddS8(string name, sbyte value) { AddParamByName(name, value); }
        public void SetS8(int paramIndex, sbyte value) { SetParam(paramIndex, value); }
        public void SetS8(string name, sbyte value) { SetParam(name, value); }
        public sbyte GetS8(string name, sbyte defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param S16
        public void AddS16(string name, short value) { AddParamByName(name, value); }
        public void SetS16(int paramIndex, short value) { SetParam(paramIndex, value); }
        public void SetS16(string name, short value) { SetParam(name, value); }
        public short GetS16(string name, short defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param S32
        public void AddS32(string name, int value) { AddParamByName(name, value); }
        public void SetS32(int paramIndex, int value) { SetParam(paramIndex, value); }
        public void SetS32(string name, int value) { SetParam(name, value); }
        public int GetS32(string name, int defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param S64
        public void AddS64(string name, long value) { AddParamByName(name, value); }
        public void SetS64(int paramIndex, long value) { SetParam(paramIndex, value); }
        public void SetS64(string name, long value) { SetParam(name, value); }
        public long GetS64(string name, long defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param U8
        public void AddU8(string name, byte value) { AddParamByName(name, value); }
        public void SetU8(int paramIndex, byte value) { SetParam(paramIndex, value); }
        public void SetU8(string name, byte value) { SetParam(name, value); }
        public byte GetU8(string name, byte defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param U16
        public void AddU16(string name, ushort value) { AddParamByName(name, value); }
        public void SetU16(int paramIndex, ushort value) { SetParam(paramIndex, value); }
        public void SetU16(string name, ushort value) { SetParam(name, value); }
        public ushort GetU16(string name, ushort defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param U32
        public void AddU32(string name, uint value) { AddParamByName(name, value); }
        public void SetU32(int paramIndex, uint value) { SetParam(paramIndex, value); }
        public void SetU32(string name, uint value) { SetParam(name, value); }
        public uint GetU32(string name, uint defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param U64
        public void AddU64(string name, ulong value) { AddParamByName(name, value); }
        public void SetU64(int paramIndex, ulong value) { SetParam(paramIndex, value); }
        public void SetU64(string name, ulong value) { SetParam(name, value); }
        public ulong GetU64(string name, ulong defValue = 0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param F32
        public void AddF32(string name, float value) { AddParamByName(name, value); }
        public void SetF32(int paramIndex, float value) { SetParam(paramIndex, value); }
        public void SetF32(string name, float value) { SetParam(name, value); }
        public float GetF32(string name, float defValue = 0.0f) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param F64
        public void AddF64(string name, double value) { AddParamByName(name, value); }
        public void SetF64(int paramIndex, double value) { SetParam(paramIndex, value); }
        public void SetF64(string name, double value) { SetParam(name, value); }
        public double GetF64(string name, double defValue = 0.0) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Bool
        public void AddBool(string name, bool value) { AddParamByName(name, value); }
        public void SetBool(int paramIndex, bool value) { SetParam(paramIndex, value); }
        public void SetBool(string name, bool value) { SetParam(name, value); }
        public bool GetBool(string name, bool defValue = false) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec4S8
        public void AddVec4S8(string name, Vec4S8 value) { AddParamByName(name, value); }
        public void SetVec4S8(int paramIndex, Vec4S8 value) { SetParam(paramIndex, value); }
        public void SetVec4S8(string name, Vec4S8 value) { SetParam(name, value); }
        public Vec4S8 GetVec4S8(string name, Vec4S8 defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec4U8
        public void AddVec4U8(string name, Vec4U8 value) { AddParamByName(name, value); }
        public void SetVec4U8(int paramIndex, Vec4U8 value) { SetParam(paramIndex, value); }
        public void SetVec4U8(string name, Vec4U8 value) { SetParam(name, value); }
        public Vec4U8 GetVec4U8(string name, Vec4U8 defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec2S
        public void AddVec2S(string name, Vec2S value) { AddParamByName(name, value); }
        public void SetVec2S(int paramIndex, Vec2S value) { SetParam(paramIndex, value); }
        public void SetVec2S(string name, Vec2S value) { SetParam(name, value); }
        public Vec2S GetVec2S(string name, Vec2S defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec3S
        public void AddVec3S(string name, Vec3S value) { AddParamByName(name, value); }
        public void SetVec3S(int paramIndex, Vec3S value) { SetParam(paramIndex, value); }
        public void SetVec3S(string name, Vec3S value) { SetParam(name, value); }
        public Vec3S GetVec3S(string name, Vec3S defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec4S
        public void AddVec4S(string name, Vec4S value) { AddParamByName(name, value); }
        public void SetVec4S(int paramIndex, Vec4S value) { SetParam(paramIndex, value); }
        public void SetVec4S(string name, Vec4S value) { SetParam(name, value); }
        public Vec4S GetVec4S(string name, Vec4S defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec2U
        public void AddVec2U(string name, Vec2U value) { AddParamByName(name, value); }
        public void SetVec2U(int paramIndex, Vec2U value) { SetParam(paramIndex, value); }
        public void SetVec2U(string name, Vec2U value) { SetParam(name, value); }
        public Vec2U GetVec2U(string name, Vec2U defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec3U
        public void AddVec3U(string name, Vec3U value) { AddParamByName(name, value); }
        public void SetVec3U(int paramIndex, Vec3U value) { SetParam(paramIndex, value); }
        public void SetVec3U(string name, Vec3U value) { SetParam(name, value); }
        public Vec3U GetVec3U(string name, Vec3U defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec4U
        public void AddVec4U(string name, Vec4U value) { AddParamByName(name, value); }
        public void SetVec4U(int paramIndex, Vec4U value) { SetParam(paramIndex, value); }
        public void SetVec4U(string name, Vec4U value) { SetParam(name, value); }
        public Vec4U GetVec4U(string name, Vec4U defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec2F
        public void AddVec2F(string name, Vec2F value) { AddParamByName(name, value); }
        public void SetVec2F(int paramIndex, Vec2F value) { SetParam(paramIndex, value); }
        public void SetVec2F(string name, Vec2F value) { SetParam(name, value); }
        public Vec2F GetVec2F(string name, Vec2F defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec3F
        public void AddVec3F(string name, Vec3F value) { AddParamByName(name, value); }
        public void SetVec3F(int paramIndex, Vec3F value) { SetParam(paramIndex, value); }
        public void SetVec3F(string name, Vec3F value) { SetParam(name, value); }
        public Vec3F GetVec3F(string name, Vec3F defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec4F
        public void AddVec4F(string name, Vec4F value) { AddParamByName(name, value); }
        public void SetVec4F(int paramIndex, Vec4F value) { SetParam(paramIndex, value); }
        public void SetVec4F(string name, Vec4F value) { SetParam(name, value); }
        public Vec4F GetVec4F(string name, Vec4F defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec2B
        public void AddVec2B(string name, Vec2B value) { AddParamByName(name, value); }
        public void SetVec2B(int paramIndex, Vec2B value) { SetParam(paramIndex, value); }
        public void SetVec2B(string name, Vec2B value) { SetParam(name, value); }
        public Vec2B GetVec2B(string name, Vec2B defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec3B
        public void AddVec3B(string name, Vec3B value) { AddParamByName(name, value); }
        public void SetVec3B(int paramIndex, Vec3B value) { SetParam(paramIndex, value); }
        public void SetVec3B(string name, Vec3B value) { SetParam(name, value); }
        public Vec3B GetVec3B(string name, Vec3B defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Vec4B
        public void AddVec4B(string name, Vec4B value) { AddParamByName(name, value); }
        public void SetVec4B(int paramIndex, Vec4B value) { SetParam(paramIndex, value); }
        public void SetVec4B(string name, Vec4B value) { SetParam(name, value); }
        public Vec4B GetVec4B(string name, Vec4B defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Mat3F
        public void AddMat3F(string name, Mat3F value) { AddParamByName(name, value); }
        public void SetMat3F(int paramIndex, Mat3F value) { SetParam(paramIndex, value); }
        public void SetMat3F(string name, Mat3F value) { SetParam(name, value); }
        public Mat3F GetMat3F(string name, Mat3F defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param Mat4F
        public void AddMat4F(string name, Mat4F value) { AddParamByName(name, value); }
        public void SetMat4F(int paramIndex, Mat4F value) { SetParam(paramIndex, value); }
        public void SetMat4F(string name, Mat4F value) { SetParam(name, value); }
        public Mat4F GetMat4F(string name, Mat4F defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param TMat
        public void AddTMat(string name, TMat value) { AddParamByName(name, value); }
        public void SetTMat(int paramIndex, TMat value) { SetParam(paramIndex, value); }
        public void SetTMat(string name, TMat value) { SetParam(name, value); }
        public TMat GetTMat(string name, TMat defValue = default) { return GetParamValueByName(name, defValue); }
        #endregion
        #region Param String
        public void AddString(string name, string value) { AddParamByName(name, value); }
        public void AddString(string name, byte[] value) { AddString(name, Encoding.ASCII.GetString(value)); }
        public void SetString(int paramIndex, string value) { SetParam(paramIndex, value); }
        public void SetString(string name, string value) { SetParam(name, value); }
        public string GetString(string name, string defValue = default) { return GetParamValueByName(name, defValue); }
        public string GetString(int paramIndex, string defValue = default) { return GetParamValue(paramIndex, defValue); }
        #endregion


        #region DataBlock
        public DataBlock EnsureDataBlock(string name)
        {
            DataBlock blk = GetDataBlock(name);
            if (blk != null)
                return blk;

            return AddNewDataBlock(name);
        }

        public DataBlock GetDataBlock(string name)
        {
            uint nameId = GetSharedStringId(name);
            if (nameId == 0)
                return null;
            return GetDataBlockByNameId(nameId);
        }

        public DataBlock GetDataBlockByNameId(uint nameId, int startAfter = 0)
        {
            int blockIndex = FindDataBlockIndex(nameId, startAfter);
            return GetDataBlock(blockIndex);
        }

        public int FindDataBlockIndex(uint nameId, int startAfter = 0)
        {
            int dataBlocksCount = DataBlocksCount;
            if (dataBlocksCount == 0)
                return -1;

            for (int i = startAfter; i < dataBlocksCount; ++i)
                if (m_DataBlocks[i].NameId == nameId)
                    return i;
            return -1;
        }

        public DataBlock GetDataBlock(int blockIndex)
        {
            if (blockIndex >= 0 && blockIndex < DataBlocksCount)
                return m_DataBlocks[blockIndex];
            return null;
        }

        public DataBlock AddNewDataBlock(string name)
        {
            DataBlock newBlock = new DataBlock(m_Shared, name);
            EnsureDataBlocks().Add(newBlock);
            return newBlock;
        }

        public DataBlock this[string name] { get => EnsureDataBlock(name); }
        #endregion
    }

}
