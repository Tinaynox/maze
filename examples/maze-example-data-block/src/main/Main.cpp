//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "maze-core/MazeTypes.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/system/MazeThread.hpp"
#include "maze-core/system/std/MazeThread_std.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazePlatformHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/serialization/MazeDataBlockSerializationUtils.hpp"


//////////////////////////////////////////
using namespace Maze;




//////////////////////////////////////////
S32 main(S32 _argc, S8 const* _argv[])
{
    Bool const boolValue = true;
    S32 const s32Value = S32_MAX;
    S64 const s64Value = S64_MAX;
    U32 const u32Value = U32_MAX;
    U64 const u64Value = U64_MAX;
    F32 const f32Value = F32_VERY_BIG_NUMBER;
    F64 const f64Value = F64_VERY_BIG_NUMBER;
    Vec2B const vec2bValue = { boolValue, boolValue };
    Vec3B const vec3bValue = { boolValue, boolValue, boolValue };
    Vec4B const vec4bValue = { boolValue, boolValue, boolValue, boolValue };
    Vec2S32 const vec2sValue = { s32Value, s32Value };
    Vec3S32 const vec3sValue = { s32Value, s32Value, s32Value };
    Vec4S32 const vec4sValue = { s32Value, s32Value, s32Value, s32Value };
    Vec2U32 const vec2uValue = { u32Value, u32Value };
    Vec3U32 const vec3uValue = { u32Value, u32Value, u32Value };
    Vec4U32 const vec4uValue = { u32Value, u32Value, u32Value, u32Value };
    Vec2F32 const vec2fValue = { f32Value, f32Value };
    Vec3F32 const vec3fValue = { f32Value, f32Value, f32Value };
    Vec4F32 const vec4fValue = { f32Value, f32Value, f32Value, f32Value };
    String const stringValue = "Single line string";
    String const stringValue2 = "Multi\n" "\tline\n" "\t\tstring";

    DataBlock testBuffer;
    testBuffer.addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_CPP), " Simple values");
    testBuffer.addBool(MAZE_HS("boolValue"), boolValue); testBuffer.addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " Bool");
    testBuffer.addS32(MAZE_HS("s32Value"), s32Value); testBuffer.addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " S32");
    testBuffer.addS64(MAZE_HS("s64Value"), s64Value); testBuffer.addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " S64");
    testBuffer.addU32(MAZE_HS("u32Value"), u32Value); testBuffer.addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " U32");
    testBuffer.addU64(MAZE_HS("u64Value"), u64Value); testBuffer.addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " U64");
    testBuffer.addF32(MAZE_HS("f32Value"), f32Value); testBuffer.addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " F32");
    testBuffer.addF64(MAZE_HS("f64Value"), f64Value); testBuffer.addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " F64");

    DataBlock* subBlock = testBuffer.addNewDataBlock(MAZE_HS("subBlock"));

    DataBlock* vectorsBlock = subBlock->addNewDataBlock(MAZE_HS("vectors"));
    vectorsBlock->addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_C), " Vectors ");
    vectorsBlock->addVec2B(MAZE_HS("vec2bValue"), vec2bValue);
    vectorsBlock->addVec3B(MAZE_HS("vec3bValue"), vec3bValue);
    vectorsBlock->addVec4B(MAZE_HS("vec4bValue"), vec4bValue);
    vectorsBlock->addVec2S32(MAZE_HS("vec2sValue"), vec2sValue);
    vectorsBlock->addVec3S32(MAZE_HS("vec3sValue"), vec3sValue);
    vectorsBlock->addVec4S32(MAZE_HS("vec4sValue"), vec4sValue);
    vectorsBlock->addVec2U32(MAZE_HS("vec2uValue"), vec2uValue);
    vectorsBlock->addVec3U32(MAZE_HS("vec3uValue"), vec3uValue);
    vectorsBlock->addVec4U32(MAZE_HS("vec4uValue"), vec4uValue);
    vectorsBlock->addVec2F32(MAZE_HS("vec2fValue"), vec2fValue);
    vectorsBlock->addVec3F32(MAZE_HS("vec3fValue"), vec3fValue);
    vectorsBlock->addVec4F32(MAZE_HS("vec4fValue"), vec4fValue);

    DataBlock* stringsBlock = subBlock->addNewDataBlock(MAZE_HS("strings"));
    stringsBlock->addCString(MAZE_HS(MAZE_DATA_BLOCK_COMMENT_C), " Strings ");
    stringsBlock->addString(MAZE_HS("stringValue"), stringValue);
    stringsBlock->addString(MAZE_HS("stringValue2"), stringValue2);
    

    // Write test
    Path const binaryFileName = "TestBinary.mzdata";
    Path const textFileName = "TestText.mzdata";
    Path const textCompactFileName = "TestTextCompact.mzdata";

    testBuffer.saveBinaryFile(binaryFileName);
    testBuffer.saveTextFile(textFileName);
    testBuffer.saveTextFile(textCompactFileName, U32(DataBlockTextFlags::Compact));

    auto validateBuffer =
        [&](DataBlock& _dataBlock)
        {
            auto v00 = _dataBlock.getBool(MAZE_HS("boolValue")); MAZE_ASSERT(v00 == true);
            auto v01 = _dataBlock.getS32(MAZE_HS("s32Value")); MAZE_ASSERT(v01 == s32Value);
            auto v02 = _dataBlock.getS64(MAZE_HS("s64Value")); MAZE_ASSERT(v02 == s64Value);
            auto v03 = _dataBlock.getU32(MAZE_HS("u32Value")); MAZE_ASSERT(v03 == u32Value);
            auto v04 = _dataBlock.getU64(MAZE_HS("u64Value")); MAZE_ASSERT(v04 == u64Value);
            auto v05 = _dataBlock.getF32(MAZE_HS("f32Value")); MAZE_ASSERT(Math::IsNear(v05, f32Value, FLT_EPSILON));
            auto v06 = _dataBlock.getF64(MAZE_HS("f64Value")); MAZE_ASSERT(Math::IsNear(v06, f64Value, DBL_EPSILON));

            auto v07 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec2B(MAZE_HS("vec2bValue")); MAZE_ASSERT(v07 == vec2bValue);
            auto v08 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec3B(MAZE_HS("vec3bValue")); MAZE_ASSERT(v08 == vec3bValue);
            auto v09 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec4B(MAZE_HS("vec4bValue")); MAZE_ASSERT(v09 == vec4bValue);
            auto v10 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec2S32(MAZE_HS("vec2sValue")); MAZE_ASSERT(v10 == vec2sValue);
            auto v11 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec3S32(MAZE_HS("vec3sValue")); MAZE_ASSERT(v11 == vec3sValue);
            auto v12 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec4S32(MAZE_HS("vec4sValue")); MAZE_ASSERT(v12 == vec4sValue);
            auto v13 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec2U32(MAZE_HS("vec2uValue")); MAZE_ASSERT(v13 == vec2uValue);
            auto v14 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec3U32(MAZE_HS("vec3uValue")); MAZE_ASSERT(v14 == vec3uValue);
            auto v15 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec4U32(MAZE_HS("vec4uValue")); MAZE_ASSERT(v15 == vec4uValue);
            auto v16 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec2F32(MAZE_HS("vec2fValue")); MAZE_ASSERT(v16 == vec2fValue);
            auto v17 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec3F32(MAZE_HS("vec3fValue")); MAZE_ASSERT(v17 == vec3fValue);
            auto v18 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("vectors")].getVec4F32(MAZE_HS("vec4fValue")); MAZE_ASSERT(v18 == vec4fValue);

            auto v19 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("strings")].getString(MAZE_HS("stringValue")); MAZE_ASSERT(v19 == stringValue);
            auto v20 = _dataBlock[MAZE_HS("subBlock")][MAZE_HS("strings")].getString(MAZE_HS("stringValue2")); MAZE_ASSERT(v20 == stringValue2);
        };

    // Read binary test
    {
        DataBlock readTestBuffer;
        MAZE_ASSERT(readTestBuffer.loadBinaryFile(binaryFileName));
        validateBuffer(readTestBuffer);
    }

    // Read text test
    {
        DataBlock readTestBuffer;
        MAZE_ASSERT(readTestBuffer.loadTextFile(textFileName));
        validateBuffer(readTestBuffer);
    }

    // Read text compact test
    {
        DataBlock readTestBuffer;
        MAZE_ASSERT(readTestBuffer.loadTextFile(textCompactFileName));
        validateBuffer(readTestBuffer);
    }

    return 0;
}
