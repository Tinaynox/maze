using System;
using System.Runtime.InteropServices;

namespace Maze.Core
{
    public enum InputEventKeyboardType : byte
    {
        None = 0,
        KeyDown,
        KeyUp
    }

    [Flags]
    public enum InputEventKeyboardModifiers : uint
    {
        None                = 0u,

        CapsLockOn          = 1u << 1,
        NumLockOn           = 1u << 2,
        ScrollLockOn        = 1u << 3,
        ShiftDown           = 1u << 4,
        ControlDown         = 1u << 5,
        AltDown             = 1u << 6,
        CommandDown         = 1u << 7,
        FnDown              = 1u << 8,
        IsKeyPad            = 1u << 9,
        IsLeft              = 1u << 10,
        IsRight             = 1u << 11,
        IsKeyRepeat         = 1u << 12,
    }

    public class InputKeyboardEvent : NativeEvent
    {
        public InputEventKeyboardType type;

        public long scanCode;
        public long virtualCode;
        public KeyCode keyCode;

        public uint modifiers;
        public long data;

        public bool IsCapsLockOn() { return (modifiers & (uint)InputEventKeyboardModifiers.CapsLockOn) != 0; }
        public bool IsNumLockOn() { return (modifiers & (uint)InputEventKeyboardModifiers.NumLockOn) != 0; }
        public bool IsScrollLockOn() { return (modifiers & (uint)InputEventKeyboardModifiers.ScrollLockOn) != 0; }
        public bool IsShiftDown() { return (modifiers & (uint)InputEventKeyboardModifiers.ShiftDown) != 0; }
        public bool IsControlDown() { return (modifiers & (uint)InputEventKeyboardModifiers.ControlDown) != 0; }
        public bool IsAltDown() { return (modifiers & (uint)InputEventKeyboardModifiers.AltDown) != 0; }
        public bool IsCommandDown() { return (modifiers & (uint)InputEventKeyboardModifiers.CommandDown) != 0; }
        public bool IsFnDown() { return (modifiers & (uint)InputEventKeyboardModifiers.FnDown) != 0; }
        public bool IsKeyPad() { return (modifiers & (uint)InputEventKeyboardModifiers.IsKeyPad) != 0; }
        public bool IsLeft() { return (modifiers & (uint)InputEventKeyboardModifiers.IsLeft) != 0; }
        public bool IsRight() { return (modifiers & (uint)InputEventKeyboardModifiers.IsRight) != 0; }
        public bool IsKeyRepeat() { return (modifiers & (uint)InputEventKeyboardModifiers.IsKeyRepeat) != 0; }
    }

    public class PostUpdateEvent : NativeEvent {}

    public class MonoPreShutdownEvent : NativeEvent {}
    public class MonoReloadEvent : NativeEvent { }

}
