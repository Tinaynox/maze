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
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
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
    Vec2DB const vec2dbValue = { boolValue, boolValue };
    Vec3DB const vec3dbValue = { boolValue, boolValue, boolValue };
    Vec4DB const vec4dbValue = { boolValue, boolValue, boolValue, boolValue };
    Vec2DS const vec2dsValue = { s32Value, s32Value };
    Vec3DS const vec3dsValue = { s32Value, s32Value, s32Value };
    Vec4DS const vec4dsValue = { s32Value, s32Value, s32Value, s32Value };
    Vec2DU const vec2duValue = { u32Value, u32Value };
    Vec3DU const vec3duValue = { u32Value, u32Value, u32Value };
    Vec4DU const vec4duValue = { u32Value, u32Value, u32Value, u32Value };
    Vec2DF const vec2dfValue = { f32Value, f32Value };
    Vec3DF const vec3dfValue = { f32Value, f32Value, f32Value };
    Vec4DF const vec4dfValue = { f32Value, f32Value, f32Value, f32Value };
    String const stringValue = "Single line string";
    String const stringValue2 = "Multi\n" "\tline\n" "\t\tstring";

    DataBlock testBuffer;
    testBuffer.addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_CPP), " Simple values");
    testBuffer.addBool(MAZE_HASHED_CSTRING("boolValue"), boolValue); testBuffer.addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " Bool");
    testBuffer.addS32(MAZE_HASHED_CSTRING("s32Value"), s32Value); testBuffer.addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " S32");
    testBuffer.addS64(MAZE_HASHED_CSTRING("s64Value"), s64Value); testBuffer.addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " S64");
    testBuffer.addU32(MAZE_HASHED_CSTRING("u32Value"), u32Value); testBuffer.addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " U32");
    testBuffer.addU64(MAZE_HASHED_CSTRING("u64Value"), u64Value); testBuffer.addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " U64");
    testBuffer.addF32(MAZE_HASHED_CSTRING("f32Value"), f32Value); testBuffer.addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " F32");
    testBuffer.addF64(MAZE_HASHED_CSTRING("f64Value"), f64Value); testBuffer.addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP), " F64");

    DataBlock* subBlock = testBuffer.addNewDataBlock(MAZE_HASHED_CSTRING("subBlock"));

    DataBlock* vectorsBlock = subBlock->addNewDataBlock(MAZE_HASHED_CSTRING("vectors"));
    vectorsBlock->addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_C), " Vectors ");
    vectorsBlock->addVec2DB(MAZE_HASHED_CSTRING("vec2dbValue"), vec2dbValue);
    vectorsBlock->addVec3DB(MAZE_HASHED_CSTRING("vec3dbValue"), vec3dbValue);
    vectorsBlock->addVec4DB(MAZE_HASHED_CSTRING("vec4dbValue"), vec4dbValue);
    vectorsBlock->addVec2DS(MAZE_HASHED_CSTRING("vec2dsValue"), vec2dsValue);
    vectorsBlock->addVec3DS(MAZE_HASHED_CSTRING("vec3dsValue"), vec3dsValue);
    vectorsBlock->addVec4DS(MAZE_HASHED_CSTRING("vec4dsValue"), vec4dsValue);
    vectorsBlock->addVec2DU(MAZE_HASHED_CSTRING("vec2duValue"), vec2duValue);
    vectorsBlock->addVec3DU(MAZE_HASHED_CSTRING("vec3duValue"), vec3duValue);
    vectorsBlock->addVec4DU(MAZE_HASHED_CSTRING("vec4duValue"), vec4duValue);
    vectorsBlock->addVec2DF(MAZE_HASHED_CSTRING("vec2dfValue"), vec2dfValue);
    vectorsBlock->addVec3DF(MAZE_HASHED_CSTRING("vec3dfValue"), vec3dfValue);
    vectorsBlock->addVec4DF(MAZE_HASHED_CSTRING("vec4dfValue"), vec4dfValue);

    DataBlock* stringsBlock = subBlock->addNewDataBlock(MAZE_HASHED_CSTRING("strings"));
    stringsBlock->addCString(MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_C), " Strings ");
    stringsBlock->addString(MAZE_HASHED_CSTRING("stringValue"), stringValue);
    stringsBlock->addString(MAZE_HASHED_CSTRING("stringValue2"), stringValue2);
    

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
            auto v00 = _dataBlock.getBool("boolValue"); MAZE_ASSERT(v00 == true);
            auto v01 = _dataBlock.getS32("s32Value"); MAZE_ASSERT(v01 == s32Value);
            auto v02 = _dataBlock.getS64("s64Value"); MAZE_ASSERT(v02 == s64Value);
            auto v03 = _dataBlock.getU32("u32Value"); MAZE_ASSERT(v03 == u32Value);
            auto v04 = _dataBlock.getU64("u64Value"); MAZE_ASSERT(v04 == u64Value);
            auto v05 = _dataBlock.getF32("f32Value"); MAZE_ASSERT(Math::IsNear(v05, f32Value, FLT_EPSILON));
            auto v06 = _dataBlock.getF64("f64Value"); MAZE_ASSERT(Math::IsNear(v06, f64Value, DBL_EPSILON));

            auto v07 = _dataBlock["subBlock"]["vectors"].getVec2DB("vec2dbValue"); MAZE_ASSERT(v07 == vec2dbValue);
            auto v08 = _dataBlock["subBlock"]["vectors"].getVec3DB("vec3dbValue"); MAZE_ASSERT(v08 == vec3dbValue);
            auto v09 = _dataBlock["subBlock"]["vectors"].getVec4DB("vec4dbValue"); MAZE_ASSERT(v09 == vec4dbValue);
            auto v10 = _dataBlock["subBlock"]["vectors"].getVec2DS("vec2dsValue"); MAZE_ASSERT(v10 == vec2dsValue);
            auto v11 = _dataBlock["subBlock"]["vectors"].getVec3DS("vec3dsValue"); MAZE_ASSERT(v11 == vec3dsValue);
            auto v12 = _dataBlock["subBlock"]["vectors"].getVec4DS("vec4dsValue"); MAZE_ASSERT(v12 == vec4dsValue);
            auto v13 = _dataBlock["subBlock"]["vectors"].getVec2DU("vec2duValue"); MAZE_ASSERT(v13 == vec2duValue);
            auto v14 = _dataBlock["subBlock"]["vectors"].getVec3DU("vec3duValue"); MAZE_ASSERT(v14 == vec3duValue);
            auto v15 = _dataBlock["subBlock"]["vectors"].getVec4DU("vec4duValue"); MAZE_ASSERT(v15 == vec4duValue);
            auto v16 = _dataBlock["subBlock"]["vectors"].getVec2DF("vec2dfValue"); MAZE_ASSERT(v16 == vec2dfValue);
            auto v17 = _dataBlock["subBlock"]["vectors"].getVec3DF("vec3dfValue"); MAZE_ASSERT(v17 == vec3dfValue);
            auto v18 = _dataBlock["subBlock"]["vectors"].getVec4DF("vec4dfValue"); MAZE_ASSERT(v18 == vec4dfValue);

            auto v19 = _dataBlock["subBlock"]["strings"].getString("stringValue"); MAZE_ASSERT(v19 == stringValue);
            auto v20 = _dataBlock["subBlock"]["strings"].getString("stringValue2"); MAZE_ASSERT(v20 == stringValue2);
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
