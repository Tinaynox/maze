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

        public Sprite Sprite
        {
            get { InternalCalls.SpriteRenderer2DGetSprite(NativeComponentPtr, out int resId); return new Sprite(resId); }
            set { InternalCalls.SpriteRenderer2DSetSprite(NativeComponentPtr, value.ResourceId); }
        }

        public ColorU32 Color
        {
            get { InternalCalls.SpriteRenderer2DGetColor(NativeComponentPtr, out uint res); return new ColorU32(res); }
            set { InternalCalls.SpriteRenderer2DSetColor(NativeComponentPtr, value.ToUInt()); }
        }

        public SpriteRenderer2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

    }
}
