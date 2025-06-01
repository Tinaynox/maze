using Maze.Core;

namespace Maze.Graphics
{
    public class SpriteRenderer2D : NativeComponent
    {

        public Material Material
        {
            get { InternalCalls.SpriteRenderer2DGetMaterial(NativeComponentPtr, out int resId); return new Material(resId); }
            set { InternalCalls.SpriteRenderer2DSetMaterial(NativeComponentPtr, value.ResourceId); }
        }

        public SpriteRenderer2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

    }
}
