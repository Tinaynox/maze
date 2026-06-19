using Maze.Core;
using System.Collections.Generic;

namespace Maze.Graphics
{
    public class ColorGradient
    {
        public struct KeyframeRGB
        {
            public float Time;
            public Vec3F Value;

            public KeyframeRGB(float time, Vec3F value)
            {
                Time = time;
                Value = value;
            }

            public void ToDataBlock(DataBlock dataBlock)
            {
                dataBlock.SetF32("time", Time);
                dataBlock.SetVec3F("value", Value);
            }

            public void LoadFromDataBlock(DataBlock dataBlock)
            {
                Time = dataBlock.GetF32("time");
                Value = dataBlock.GetVec3F("value");
            }
        }

        public struct KeyframeAlpha
        {
            public float Time;
            public float Value;

            public KeyframeAlpha(float time, float value)
            {
                Time = time;
                Value = value;
            }

            public void ToDataBlock(DataBlock dataBlock)
            {
                dataBlock.SetF32("time", Time);
                dataBlock.SetF32("value", Value);
            }

            public void LoadFromDataBlock(DataBlock dataBlock)
            {
                Time = dataBlock.GetF32("time");
                Value = dataBlock.GetF32("value");
            }
        }

        public enum EvaluateMode
        {
            Fixed,
            Linear,
        }

        List<KeyframeRGB> m_KeysRGB = new List<KeyframeRGB>();
        public IReadOnlyList<KeyframeRGB> KeysRGB => m_KeysRGB;

        List<KeyframeAlpha> m_KeysAlpha = new List<KeyframeAlpha>();
        public IReadOnlyList<KeyframeAlpha> KeysAlpha => m_KeysAlpha;

        EvaluateMode m_Mode = EvaluateMode.Linear;
        public EvaluateMode Mode { get => m_Mode; set => m_Mode = value; }

        public ColorGradient()
        {
        }

        public ColorGradient(float time0, ColorF128 color0)
        {
            AddKey(time0, color0);
        }

        public ColorGradient(ColorF128 color0)
        {
            AddKey(0.0f, color0);
        }

        public ColorGradient(float time0, ColorF128 color0, float time1, ColorF128 color1)
        {
            AddKey(time0, color0);
            AddKey(time1, color1);
        }

        public bool IsEmpty => IsEmptyRGB && IsEmptyAlpha;
        public bool IsEmptyRGB => m_KeysRGB.Count == 0;
        public bool IsEmptyAlpha => m_KeysAlpha.Count == 0;

        public void AddKeyRGB(KeyframeRGB keyframe)
        {
            m_KeysRGB.Add(keyframe);
            SortKeysRGB();
        }

        public void AddKeyRGB(float time, Vec3F value)
        {
            AddKeyRGB(new KeyframeRGB(time, value));
        }

        public void SetKeyRGB(int index, Vec3F value)
        {
            KeyframeRGB keyframe = m_KeysRGB[index];
            keyframe.Value = value;
            m_KeysRGB[index] = keyframe;
        }

        public void SetKeyRGBTime(int index, float time)
        {
            KeyframeRGB keyframe = m_KeysRGB[index];
            keyframe.Time = time;
            m_KeysRGB[index] = keyframe;
            SortKeysRGB();
        }

        public void RemoveKeyRGB(int index)
        {
            m_KeysRGB.RemoveAt(index);
        }

        public void ClearKeysRGB()
        {
            m_KeysRGB.Clear();
        }

        public void AddKeyAlpha(KeyframeAlpha keyframe)
        {
            m_KeysAlpha.Add(keyframe);
            SortKeysAlpha();
        }

        public void AddKeyAlpha(float time, float value)
        {
            AddKeyAlpha(new KeyframeAlpha(time, value));
        }

        public void SetKeyAlpha(int index, float value)
        {
            KeyframeAlpha keyframe = m_KeysAlpha[index];
            keyframe.Value = value;
            m_KeysAlpha[index] = keyframe;
        }

        public void SetKeyAlphaTime(int index, float time)
        {
            KeyframeAlpha keyframe = m_KeysAlpha[index];
            keyframe.Time = time;
            m_KeysAlpha[index] = keyframe;
            SortKeysAlpha();
        }

        public void RemoveKeyAlpha(int index)
        {
            m_KeysAlpha.RemoveAt(index);
        }

        public void ClearKeysAlpha()
        {
            m_KeysAlpha.Clear();
        }

        public void AddKey(float time, ColorF128 value)
        {
            AddKeyRGB(time, new Vec3F(value.R, value.G, value.B));
            AddKeyAlpha(time, value.A);
        }

        public void Clear()
        {
            ClearKeysRGB();
            ClearKeysAlpha();
        }

        public void MultiplyAlpha(float power)
        {
            for (int i = 0; i < m_KeysAlpha.Count; ++i)
            {
                KeyframeAlpha keyframe = m_KeysAlpha[i];
                keyframe.Value *= power;
                m_KeysAlpha[i] = keyframe;
            }
        }

        void SortKeysRGB()
        {
            m_KeysRGB.Sort((a, b) => a.Time.CompareTo(b.Time));
        }

        void SortKeysAlpha()
        {
            m_KeysAlpha.Sort((a, b) => a.Time.CompareTo(b.Time));
        }

        public float GetStartTime()
        {
            bool isRGBEmpty = IsEmptyRGB;
            bool isAlphaEmpty = IsEmptyAlpha;

            if (!isRGBEmpty && !isAlphaEmpty)
                return System.Math.Min(m_KeysRGB[0].Time, m_KeysAlpha[0].Time);
            else if (!isRGBEmpty)
                return m_KeysRGB[0].Time;
            else if (!isAlphaEmpty)
                return m_KeysAlpha[0].Time;

            return 0.0f;
        }

        public float GetEndTime()
        {
            bool isRGBEmpty = IsEmptyRGB;
            bool isAlphaEmpty = IsEmptyAlpha;

            if (!isRGBEmpty && !isAlphaEmpty)
                return System.Math.Max(m_KeysRGB[m_KeysRGB.Count - 1].Time, m_KeysAlpha[m_KeysAlpha.Count - 1].Time);
            else if (!isRGBEmpty)
                return m_KeysRGB[m_KeysRGB.Count - 1].Time;
            else if (!isAlphaEmpty)
                return m_KeysAlpha[m_KeysAlpha.Count - 1].Time;

            return 1.0f;
        }

        public float GetTime()
        {
            return GetEndTime() - GetStartTime();
        }

        public void Clamp01()
        {
            m_KeysRGB.RemoveAll(keyframe => keyframe.Time < 0.0f || keyframe.Time > 1.0f);
            m_KeysAlpha.RemoveAll(keyframe => keyframe.Time < 0.0f || keyframe.Time > 1.0f);
        }

        public Vec3F EvaluateRGB(float time)
        {
            if (m_KeysRGB.Count == 0)
                return Vec3F.Zero;

            if (time < m_KeysRGB[0].Time)
                return m_KeysRGB[0].Value;

            if (time >= m_KeysRGB[m_KeysRGB.Count - 1].Time)
                return m_KeysRGB[m_KeysRGB.Count - 1].Value;

            for (int i = m_KeysRGB.Count - 2; i >= 0; --i)
            {
                KeyframeRGB keyframe0 = m_KeysRGB[i];
                if (time >= keyframe0.Time)
                {
                    KeyframeRGB keyframe1 = m_KeysRGB[i + 1];
                    return EvaluateRGBBetween(keyframe0, keyframe1, time);
                }
            }

            return m_KeysRGB[0].Value;
        }

        public float EvaluateAlpha(float time)
        {
            if (m_KeysAlpha.Count == 0)
                return 0.0f;

            if (time < m_KeysAlpha[0].Time)
                return m_KeysAlpha[0].Value;

            if (time >= m_KeysAlpha[m_KeysAlpha.Count - 1].Time)
                return m_KeysAlpha[m_KeysAlpha.Count - 1].Value;

            for (int i = m_KeysAlpha.Count - 2; i >= 0; --i)
            {
                KeyframeAlpha keyframe0 = m_KeysAlpha[i];
                if (time >= keyframe0.Time)
                {
                    KeyframeAlpha keyframe1 = m_KeysAlpha[i + 1];
                    return EvaluateAlphaBetween(keyframe0, keyframe1, time);
                }
            }

            return m_KeysAlpha[0].Value;
        }

        public ColorF128 Evaluate(float time)
        {
            Vec3F rgb = EvaluateRGB(time);
            float a = EvaluateAlpha(time);
            return new ColorF128(rgb.X, rgb.Y, rgb.Z, a);
        }

        Vec3F EvaluateRGBBetween(KeyframeRGB keyframe0, KeyframeRGB keyframe1, float time)
        {
            if (m_Mode == EvaluateMode.Fixed)
                return keyframe1.Value;

            float dt = keyframe1.Time - keyframe0.Time;
            float t = (time - keyframe0.Time) / dt;
            return keyframe0.Value + (keyframe1.Value - keyframe0.Value) * t;
        }

        float EvaluateAlphaBetween(KeyframeAlpha keyframe0, KeyframeAlpha keyframe1, float time)
        {
            if (m_Mode == EvaluateMode.Fixed)
                return keyframe1.Value;

            float dt = keyframe1.Time - keyframe0.Time;
            float t = (time - keyframe0.Time) / dt;
            return keyframe0.Value + (keyframe1.Value - keyframe0.Value) * t;
        }

        public DataBlock ToDataBlock()
        {
            DataBlock dataBlock = new DataBlock();

            DataBlock keyframesRGBBlock = dataBlock.EnsureDataBlock("keyframesRGB");
            foreach (KeyframeRGB keyframe in m_KeysRGB)
                keyframe.ToDataBlock(keyframesRGBBlock.AddNewDataBlock("item"));

            DataBlock keyframesAlphaBlock = dataBlock.EnsureDataBlock("keyframesAlpha");
            foreach (KeyframeAlpha keyframe in m_KeysAlpha)
                keyframe.ToDataBlock(keyframesAlphaBlock.AddNewDataBlock("item"));

            dataBlock.SetS32("mode", (int)m_Mode);

            return dataBlock;
        }

        public void LoadFromDataBlock(DataBlock dataBlock)
        {
            m_KeysRGB.Clear();
            DataBlock keyframesRGBBlock = dataBlock.GetDataBlock("keyframesRGB");
            if (keyframesRGBBlock != null)
            {
                foreach (DataBlock itemBlock in keyframesRGBBlock)
                {
                    KeyframeRGB keyframe = new KeyframeRGB();
                    keyframe.LoadFromDataBlock(itemBlock);
                    m_KeysRGB.Add(keyframe);
                }

                SortKeysRGB();
            }

            m_KeysAlpha.Clear();
            DataBlock keyframesAlphaBlock = dataBlock.GetDataBlock("keyframesAlpha");
            if (keyframesAlphaBlock != null)
            {
                foreach (DataBlock itemBlock in keyframesAlphaBlock)
                {
                    KeyframeAlpha keyframe = new KeyframeAlpha();
                    keyframe.LoadFromDataBlock(itemBlock);
                    m_KeysAlpha.Add(keyframe);
                }

                SortKeysAlpha();
            }

            m_Mode = (EvaluateMode)dataBlock.GetS32("mode", (int)EvaluateMode.Linear);
        }
    }
}
