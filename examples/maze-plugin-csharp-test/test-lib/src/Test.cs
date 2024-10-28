using System;
using System.Runtime.CompilerServices;

namespace Maze
{
    public struct TestVector3
    {
        public float X;
        public float Y;
        public float Z;

        public TestVector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }


    public class Test
    {
        public float FloatVar { get; set; }

        public Test()
        {
            Console.WriteLine("Test constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"C# says: {value}");
        }

        public void PrintInts(int value1, int value2)
        {
            Console.WriteLine($"C# says: {value1} and {value2}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }

        public void CallCppFunction()
        {
            CppFunction();
        }

        public void CallCppFunctionWithParams()
        {
            NativeLog($"{42} Some text", 42);

            TestVector3 pos0 = new TestVector3(5, 2, 1);
            NativeLogVector3(ref pos0);

            TestVector3 pos1 = new TestVector3(0, 1, 1);
            CrossVector3(ref pos0, ref pos1, out TestVector3 result);
            NativeLogVector3(ref result);
        }




        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void CppFunction();


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void NativeLog(string _text, int _arg0);

        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void NativeLogVector3(ref TestVector3 parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void CrossVector3(
            ref TestVector3 _arg0,
            ref TestVector3 _arg1,
            out TestVector3 _out);
    }
}
