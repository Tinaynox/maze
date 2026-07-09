using System;

namespace Maze.Core
{
    // Marks a profiling scope visible in Optick using a dynamic name.
    // Convenient for one-off or rarely-called scopes, but re-hashes and
    // looks up the event description on every call. For scopes called
    // frequently (per-frame, per-entity), use ProfileMarker instead.
    //
    // Usage:
    //     using (new ProfileEvent("MyScope"))
    //     {
    //         ...
    //     }
    // Must be Disposed on the same thread it was created on.
    public struct ProfileEvent : IDisposable
    {
        public ProfileEvent(string name)
        {
            InternalCalls.MazeProfileEventBegin(name);
        }

        public void Dispose()
        {
            InternalCalls.MazeProfileEventEnd();
        }
    }

    // Marks a profiling scope visible in Optick using a description cached
    // once at marker-creation time, avoiding the per-call name hash/lookup
    // that ProfileEvent pays every time. Create one instance per call site
    // and reuse it, mirroring the native MAZE_PROFILE_EVENT scoped macro.
    //
    // Usage:
    //     static readonly ProfileMarker s_marker = new ProfileMarker("MyScope");
    //     ...
    //     using (s_marker.Begin())
    //     {
    //         ...
    //     }
    public sealed class ProfileMarker
    {
        private readonly NativePtr m_Description;

        public ProfileMarker(string name)
        {
            m_Description = InternalCalls.MazeProfileCreateEventDescription(name);
        }

        public Scope Begin()
        {
            InternalCalls.MazeProfileEventBeginDescription(m_Description);
            return new Scope();
        }

        // Must be Disposed on the same thread the marker was Begin()-ed on.
        public struct Scope : IDisposable
        {
            public void Dispose()
            {
                InternalCalls.MazeProfileEventEnd();
            }
        }
    }
}
