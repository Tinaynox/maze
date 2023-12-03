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


//////////////////////////////////////////
using namespace Maze;


//////////////////////////////////////////
S32 SecondThreadEntry()
{
    String text = "Second Thread!";
    Debug::log << "Hello from " << text << endl;

    return 1;
}


//////////////////////////////////////////
void test()
{
    DataBlock* fileTest = DataBlock::LoadBinaryFile("data.mzdata");
    if (fileTest)
    {
        S32 ab = fileTest->getS32("some");
        Mat4DF ab2 = fileTest->getMat4DF("some2");
        Vec4DF ab3 = fileTest->getVec4DF("some3");
        String const& ab4 = fileTest->getString("some4");
        if (fileTest->getDataBlock("block1"))
        {
            S32 hhh = fileTest->getDataBlock("block1")->getS32("block1_param");
            int b = 0;
        }
        int a = 0;
    }


    DataBlock test;
    test.addS32("some", 42);
    test.addMat4DF("some2", Mat4DF::c_identity);
    test.addVec4DF("some3", Vec4DF(1.0f, 2.0f, 3.0f, 4.0f));
    //test.addCString("some4", "Hello world!");
    test.setString("some4", "Hello world!");

    test.removeParam("some3");

    DataBlock* subBlock1 = test.addDataBlock("block1");
    subBlock1->addS32("block1_param", 422);
    // test.removeBlock("block1");

    DataBlock* subBlock11 = subBlock1->addDataBlock("block11");
    subBlock11->addMat3DF("HEHEH", Mat3DF::c_identity);

    S32 b = test.getS32("some");
    Mat4DF b2 = test.getMat4DF("some2");
    Vec4DF b3 = test.getVec4DF("some3");
    CString b4 = test.getCString("some4");

    DataBlock* subBlock2 = test.addDataBlock("block2");


    S32 block1_param = subBlock1->getS32("block1_param");

    /*
    ByteBuffer buffer;
    
    buffer.append((U8)1);
    buffer.append((U8)2);
    buffer.append((U8)3);
    buffer.append((U8)4);
    buffer.append((U8)5);
    buffer.append((U8)6);
    buffer.append((U8)7);

    buffer.resize(5);
    buffer.setByte(0, 0);
    buffer.setByte(1, 1);
    buffer.setByte(2, 2);
    buffer.setByte(3, 3);
    buffer.setByte(4, 4);
    buffer.insert(2, 2);

    ByteBuffer buffer2;
    buffer2 = std::move(buffer);
    */

    DataBlock* dataBlock = test.duplicate();
    S32 ab = dataBlock->getS32("some");
    Mat4DF ab2 = dataBlock->getMat4DF("some2");
    Vec4DF ab3 = dataBlock->getVec4DF("some3");
    CString ab4 = dataBlock->getCString("some4");
    S32 hhh = dataBlock->getDataBlock("block1")->getS32("block1_param");
    //MAZE_DELETE(dataBlock);

    ByteBufferPtr byteBuffer = dataBlock->saveBinary();

    DataBlock yyyBuffer;
    yyyBuffer.loadBinary(*byteBuffer.get());
    S32 qab = yyyBuffer.getS32("some");
    Mat4DF qab2 = yyyBuffer.getMat4DF("some2");
    Vec4DF qab3 = yyyBuffer.getVec4DF("some3");
    CString qab4 = yyyBuffer.getCString("some4");
    S32 qhhh = yyyBuffer.getDataBlock("block1")->getS32("block1_param");

    /*
    for (Size i = 0; i < 2000; ++i)
        yyyBuffer.addMat4DF(("Mat" + StringHelper::ToString(i)).c_str(), Mat4DF::c_identity);
    */
    yyyBuffer.setBool("Bool", true);
    yyyBuffer.setS32("S32", S32_MAX);
    yyyBuffer.setS64("S64", S64_MAX);
    yyyBuffer.setU32("U32", U32_MAX);
    yyyBuffer.setU64("U64", U64_MAX);
    yyyBuffer.setF32("F32", F32_MAX);
    yyyBuffer.setF64("F64", F64_MAX);
    yyyBuffer.setVec2DS("Vec2DS", Vec2DS(8));
    yyyBuffer.setVec3DS("Vec3DS", Vec3DS(9));
    yyyBuffer.setVec4DS("Vec4DS", Vec4DS(10));
    yyyBuffer.setVec2DU("Vec2DU", Vec2DU(11));
    yyyBuffer.setVec3DU("Vec3DU", Vec3DU(12));
    yyyBuffer.setVec4DU("Vec4DU", Vec4DU(13));
    yyyBuffer.setVec2DF("Vec2DF", Vec2DF(14.0f));
    yyyBuffer.setVec3DF("Vec3DF", Vec3DF(15.0f));
    yyyBuffer.setVec4DF("Vec4DF", Vec4DF(16.0f));
    yyyBuffer.setVec2DB("Vec2DB", Vec2DB(true));
    yyyBuffer.setVec3DB("Vec3DB", Vec3DB(false));
    yyyBuffer.setVec4DB("Vec4DB", Vec4DB(true));
    yyyBuffer.setMat3DF("Mat3DF", Mat3DF::c_identity);
    yyyBuffer.setMat4DF("Mat4DF", Mat4DF::c_identity);
    
    yyyBuffer.saveBinaryFile("data.mzdata");
    yyyBuffer.saveTextFile("text.mzdata");

    int a = 0;
}

//////////////////////////////////////////
S32 main(S32 _argc, S8 const* _argv[])
{
    test();
    return 0;

    LogService::GetInstancePtr()->splitAndLog(PlatformHelper::ConstructApplicationInfo());
    LogService::GetInstancePtr()->splitAndLog(PlatformHelper::ConstructEngineInfo());
    LogService::GetInstancePtr()->splitAndLog(SystemHelper::ConstructSystemInfo());
    Debug::log << endl;

    Thread thread(SecondThreadEntry);
    thread.run();
    thread.wait();

    LogService::GetInstancePtr()->logFormatted("qwerty %d", 42);
    LogService::GetInstancePtr()->setLogFile("log.log");
    Debug::log << "123" << endl;

    Vec2DS a(42, 228);
    a = Vec2DS::c_zero;
    a += 55;
    a.y = 0;
    auto aa = a.squaredLength();
    Debug::log << aa << endl;


    Vec2DF b(42, 228);
    b = Vec2DF::c_zero;
    b += 33.5f;
    b.x = 0;
    auto bb = b.squaredLength();
    Debug::log << bb << endl;



    Vec3DF c(a);


    Mat3DF mm(Mat3DF::c_identity);
    Mat4DF mmm(Mat4DF::c_identity);

    Debug::log << a << endl;
    Debug::log << b << endl;
    Debug::log << c << endl;
    Debug::log << mm << endl;
    Debug::log << mmm << endl;

    AABB2D aabb(a, b);
    Debug::log << aabb << endl;

    Rect2DU rect = aabb.toRect<U32>();
    Debug::log << rect << endl;

    Debug::log << "Hello, world!" << endl;


    Char text[256];
    StringHelper::FormatString(text, 256, "Check %d! %s", 123, "456");
    Debug::log << text << endl;

    Debug::log << StringHelper::FormattedStringSize("Check %d! %s", 123, "456") << endl;

    MAZE_ERROR("Hello, error! [2]");

    std::cin.get();

    return 0;
}
