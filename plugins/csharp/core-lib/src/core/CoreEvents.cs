using System;

namespace Maze.Core
{
    public class PostUpdateEvent : NativeEvent {}

    public class MonoPreShutdownEvent : NativeEvent {}
    public class MonoReloadEvent : NativeEvent { }

}
