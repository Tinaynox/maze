
using Maze.Core;

namespace Maze.Graphics
{
    public class RenderWindowWillCloseEvent : NativeEvent
    {
        public int renderWindowResourceId;
    }

    public class SpriteRenderer2DMaterialUpdatedEvent : NativeEvent { }
}
