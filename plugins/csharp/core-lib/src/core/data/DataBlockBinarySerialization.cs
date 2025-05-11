using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Maze.Core
{
    public enum DataBlockBinaryFlags : uint
    {
        CheckSumProtection = 0x1
    }


    public static class DataBlockBinarySerialization
    {
        static uint DataBlockBinaryHeaderMagic = HashFNV1.CalculateFNV1("MZDATA");

        static bool ReadDataBlockBinary(ByteBufferReadStream readStream, DataBlock dataBlock)
        {
            readStream.Read(out ushort paramsCount);
            readStream.Read(out uint complexParamsUsedSize);

            uint dataBlockParamSize = 3 + 1 + sizeof(uint);
            uint paramUsedSize = paramsCount * dataBlockParamSize;
            uint paramsAndComplexParamsSize = paramUsedSize + complexParamsUsedSize;

            if (!readStream.CanRead((int)paramsAndComplexParamsSize))
                return false;

            int complexParamsOffset = readStream.Offset + (int)paramUsedSize;
            int finalOffset = readStream.Offset + (int)paramsAndComplexParamsSize;

            

            dataBlock.ResizeParams(paramsCount);
            for (int i = 0; i < paramsCount; ++i)
            {
                readStream.Read(out byte[] paramNameIdArr, 3);
                readStream.Read(out byte paramType);
                readStream.Read(out uint paramValue);

                byte[] paddedBytes = new byte[4];
                Array.Copy(paramNameIdArr, 0, paddedBytes, 0, 3);
                uint paramNameId = BitConverter.ToUInt32(paddedBytes, 0);

                switch ((DataBlockParamType)paramType)
                {
                    case DataBlockParamType.ParamS8: dataBlock.SetParam(i, paramNameId, (sbyte)paramValue); break;
                    case DataBlockParamType.ParamS16: dataBlock.SetParam(i, paramNameId, (short)paramValue); break;
                    case DataBlockParamType.ParamS32: dataBlock.SetParam(i, paramNameId, (int)paramValue); break;
                    case DataBlockParamType.ParamS64:
                        dataBlock.SetParam(i, paramNameId,
                            BitConverter.ToInt64(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamU8: dataBlock.SetParam(i, paramNameId, (byte)paramValue); break;
                    case DataBlockParamType.ParamU16: dataBlock.SetParam(i, paramNameId, (ushort)paramValue); break;
                    case DataBlockParamType.ParamU32: dataBlock.SetParam(i, paramNameId, paramValue); break;
                    case DataBlockParamType.ParamU64:
                        dataBlock.SetParam(i, paramNameId,
                            BitConverter.ToUInt64(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamF32: dataBlock.SetParam(i, paramNameId,
                        BitConverter.ToSingle(DataBlockParam.GetBytes(paramValue), 0)); break;
                    case DataBlockParamType.ParamF64:
                        dataBlock.SetParam(i, paramNameId,
                            BitConverter.ToDouble(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamBool: dataBlock.SetParam(i, paramNameId, paramValue != 0); break;
                    case DataBlockParamType.ParamVec4S8:
                        dataBlock.SetParam(i, paramNameId,
                            Vec4S8.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec4U8:
                        dataBlock.SetParam(i, paramNameId,
                            Vec4U8.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec2S32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec2S.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec3S32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec3S.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec4S32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec4S.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec2U32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec2U.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec3U32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec3U.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec4U32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec4U.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec2F32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec2F.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec3F32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec3F.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec4F32:
                        dataBlock.SetParam(i, paramNameId,
                            Vec4F.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamVec2B:
                        dataBlock.SetParam(i, paramNameId,
                            Vec2B.FromBytes(DataBlockParam.GetBytes(paramValue)));
                        break;
                    case DataBlockParamType.ParamVec3B:
                        dataBlock.SetParam(i, paramNameId,
                            Vec3B.FromBytes(DataBlockParam.GetBytes(paramValue)));
                        break;
                    case DataBlockParamType.ParamVec4B:
                        dataBlock.SetParam(i, paramNameId,
                            Vec4B.FromBytes(DataBlockParam.GetBytes(paramValue)));
                        break;
                    case DataBlockParamType.ParamMat3F32:
                        dataBlock.SetParam(i, paramNameId,
                            Mat3F.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamMat4F32:
                        dataBlock.SetParam(i, paramNameId,
                            Mat4F.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamTMat:
                        dataBlock.SetParam(i, paramNameId,
                            TMat.FromBytes(readStream.ByteBufferData, (int)(complexParamsOffset + paramValue)));
                        break;
                    case DataBlockParamType.ParamString: dataBlock.SetParamAt(i, paramNameId, (DataBlockParamType)paramType, paramValue); break;
                    default: throw new NotImplementedException($"Unsupported DataBlockParamType: {(DataBlockParamType)paramType}!");
                }
            }

            readStream.SetOffset(finalOffset);

            readStream.Read(out ushort dataBlocksCount);

            for (int i = 0; i < dataBlocksCount; ++i)
            {
                readStream.Read(out uint nameId);
                string blockName = dataBlock.Shared.GetString(nameId);

                DataBlock subBlock = dataBlock.AddNewDataBlock(blockName);
                if (!ReadDataBlockBinary(readStream, subBlock))
                    return false;
            }

            return true;
        }

        public static bool LoadBinary(out DataBlock dataBlock, ByteBuffer buffer)
        {
            dataBlock = new DataBlock();

            ByteBufferReadStream readStream = new ByteBufferReadStream(buffer);
            readStream.Read(out uint headerMagic);
            if (headerMagic != DataBlockBinaryHeaderMagic)
                return false;

            readStream.Read(out uint flags);
            readStream.Read(out uint stringsCount);
            readStream.Read(out uint stringsIndexCounter);

            dataBlock.Shared.StringsIndexCounter = stringsIndexCounter;

            for (uint i = 0; i < stringsCount; ++i)
            {
                readStream.Read(out ushort stringSize);
                readStream.Read(out byte[] stringBytes, stringSize);
                readStream.Read(out uint stringId);

                string str = Encoding.ASCII.GetString(stringBytes);

                dataBlock.Shared.SetString(str, stringId);
            }

            if (!ReadDataBlockBinary(readStream, dataBlock))
                return false;

            if ((flags & (uint)DataBlockBinaryFlags.CheckSumProtection) != 0)
            {
                readStream.Read(out uint loadedCheckSumHash);
                // #TODO: CRC32
            }

            return true;
        }

        public static bool LoadBinaryFile(out DataBlock dataBlock, string path)
        {
            ByteBuffer buffer = ByteBuffer.LoadFile(path);
            return LoadBinary(out dataBlock, buffer);
        }

        static void AlignTo(byte[] value, byte[] paddedBytes)
        {
            Array.Copy(value, 0, paddedBytes, 0, value.Length);
        }

        static void WriteComplexParam(
            uint complexParamId,
            byte[] value,
            byte[] paddedBytes,
            ByteBufferWriteStream complexParamsBufferStream)
        {
            Array.Copy(BitConverter.GetBytes(complexParamsBufferStream.ByteBuffereSize), 0, paddedBytes, 0, 4);
            complexParamsBufferStream.Write(value);
        }

        public static void WriteDataBlockBinary(ByteBufferWriteStream stream, DataBlock dataBlock)
        {
            if (dataBlock.ParamsCount > 0)
            {
                ByteBuffer paramsBuffer = new ByteBuffer();
                ByteBufferWriteStream paramsBufferStream = new ByteBufferWriteStream(paramsBuffer);
                paramsBufferStream.ReserveToFit(8 * dataBlock.ParamsCount);

                ByteBuffer complexParamsBuffer = new ByteBuffer();
                ByteBufferWriteStream complexParamsBufferStream = new ByteBufferWriteStream(complexParamsBuffer);


                // Write params
                uint complexParamsCount = 0;
                for (int i = 0; i < dataBlock.ParamsCount; ++i)
                {
                    DataBlockParam param = dataBlock.GetParam(i);

                    byte[] nameIdPaddedBytes = new byte[3];
                    Array.Copy(BitConverter.GetBytes(param.NameId), 0, nameIdPaddedBytes, 0, 3);
                    paramsBufferStream.Write(nameIdPaddedBytes);

                    paramsBufferStream.Write((byte)param.Type);


                    byte[] paddedBytes = new byte[4];
                    switch (param.Type)
                    {
                        case DataBlockParamType.ParamS8: paddedBytes[0] = (byte)(sbyte)param.Value; break;
                        case DataBlockParamType.ParamS16: AlignTo(BitConverter.GetBytes((short)param.Value), paddedBytes); break;
                        case DataBlockParamType.ParamS32: AlignTo(BitConverter.GetBytes((int)param.Value), paddedBytes); break;
                        case DataBlockParamType.ParamS64:
                            WriteComplexParam(complexParamsCount++, BitConverter.GetBytes((long)param.Value), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamU8: paddedBytes[0] = (byte)param.Value; break;
                        case DataBlockParamType.ParamU16: AlignTo(BitConverter.GetBytes((ushort)param.Value), paddedBytes); break;
                        case DataBlockParamType.ParamU32: AlignTo(BitConverter.GetBytes((uint)param.Value), paddedBytes); break;
                        case DataBlockParamType.ParamU64:
                            WriteComplexParam(complexParamsCount++, BitConverter.GetBytes((ulong)param.Value), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamF32: AlignTo(BitConverter.GetBytes((float)param.Value), paddedBytes); break;
                        case DataBlockParamType.ParamF64:
                            WriteComplexParam(complexParamsCount++, BitConverter.GetBytes((double)param.Value), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamBool: AlignTo(BitConverter.GetBytes((bool)param.Value), paddedBytes); break;
                        case DataBlockParamType.ParamVec4S8: AlignTo(((Vec4S8)param.Value).GetBytes(), paddedBytes); break;
                        case DataBlockParamType.ParamVec4U8: AlignTo(((Vec4U8)param.Value).GetBytes(), paddedBytes); break;
                        case DataBlockParamType.ParamVec2S32:
                            WriteComplexParam(complexParamsCount++, ((Vec2S)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec3S32:
                            WriteComplexParam(complexParamsCount++, ((Vec3S)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec4S32:
                            WriteComplexParam(complexParamsCount++, ((Vec4S)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec2U32:
                            WriteComplexParam(complexParamsCount++, ((Vec2U)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec3U32:
                            WriteComplexParam(complexParamsCount++, ((Vec3U)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec4U32:
                            WriteComplexParam(complexParamsCount++, ((Vec4U)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec2F32:
                            WriteComplexParam(complexParamsCount++, ((Vec2F)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec3F32:
                            WriteComplexParam(complexParamsCount++, ((Vec3F)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec4F32:
                            WriteComplexParam(complexParamsCount++, ((Vec4F)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamVec2B: AlignTo(((Vec2B)param.Value).GetBytes(), paddedBytes); break;
                        case DataBlockParamType.ParamVec3B: AlignTo(((Vec3B)param.Value).GetBytes(), paddedBytes); break;
                        case DataBlockParamType.ParamVec4B: AlignTo(((Vec4B)param.Value).GetBytes(), paddedBytes); break;
                        case DataBlockParamType.ParamMat3F32:
                            WriteComplexParam(complexParamsCount++, ((Mat3F)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamMat4F32:
                            WriteComplexParam(complexParamsCount++, ((Mat4F)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamTMat:
                            WriteComplexParam(complexParamsCount++, ((TMat)param.Value).GetBytes(), paddedBytes, complexParamsBufferStream); break;
                        case DataBlockParamType.ParamString: AlignTo(BitConverter.GetBytes((uint)param.Value), paddedBytes); break;
                        default: throw new NotImplementedException($"param.Type={param.Type}");
                    }

                    paramsBufferStream.Write(paddedBytes);
                }

                stream.Write((ushort)dataBlock.ParamsCount);
                stream.Write(complexParamsBuffer.Size);


                stream.Write(paramsBuffer.Data, paramsBuffer.Size);

                if (complexParamsBuffer.Size > 0)
                {
                    stream.Write(complexParamsBuffer.Data, complexParamsBuffer.Size);
                }
            }
            else
            {
                stream.Write((ushort)0);
                stream.Write(0);
            }

            // Write data blocks
            stream.Write((ushort)dataBlock.DataBlocksCount);
            foreach (DataBlock subBlock in dataBlock)
            {
                stream.Write(subBlock.NameId);
                WriteDataBlockBinary(stream, subBlock);
            }
        }

        public static bool SaveBinary(DataBlock dataBlock, out ByteBuffer buffer, uint flags = 0)
        {
            var stringsMap = dataBlock.Shared.Strings;

            buffer = new ByteBuffer();
            ByteBufferWriteStream writeStream = new ByteBufferWriteStream(buffer);
            writeStream.ReserveToFit(stringsMap.Count * 128);

            writeStream.Write(DataBlockBinaryHeaderMagic);
            writeStream.Write(flags);

            writeStream.Write((uint)stringsMap.Count);
            writeStream.Write(dataBlock.Shared.StringsIndexCounter);

            foreach (var stringData in stringsMap)
            {
                byte[] asciiBytes = Encoding.ASCII.GetBytes(stringData.Key);
                writeStream.Write((ushort)asciiBytes.Length);
                
                writeStream.Write(asciiBytes);
                writeStream.Write((uint)stringData.Value);
            }

            WriteDataBlockBinary(writeStream, dataBlock);

            if ((flags & (uint)DataBlockBinaryFlags.CheckSumProtection) != 0)
            {
                // #TODO: CRC32
                uint loadedCheckSumHash = 0;
                writeStream.Write(loadedCheckSumHash);
            }

            return true;
        }

        public static bool SaveBinaryFile(ByteBuffer buffer, string path)
        {
            buffer.CapacityToSize();
            File.WriteAllBytes(path, buffer.Data);
            return true;
        }

        public static bool SaveBinaryFile(DataBlock dataBlock, string path)
        {
            if (!SaveBinary(dataBlock, out ByteBuffer buffer))
                return false;

            return SaveBinaryFile(buffer, path);
        }
    }
}
