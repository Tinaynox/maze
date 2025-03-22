using System;
using System.Collections.Generic;

namespace Maze.Core
{
    public class DataBlockShared
    {
        Dictionary<string, uint> m_Strings = new Dictionary<string, uint>();
        Dictionary<uint, string> m_StringsById = new Dictionary<uint, string>();
        uint m_StringsIndexCounter = 0u;

        public bool SetString(string name, uint stringId)
        {
            m_Strings[name] = stringId;
            m_StringsById[stringId] = name;
            return true;
        }

        public uint AddString(string name)
        {
            if (m_Strings.TryGetValue(name, out uint value))
                return value;

            uint stringId = ++m_StringsIndexCounter;
            m_Strings[name] = stringId;
            m_StringsById[stringId] = name;

            return stringId;
        }

        public uint GetStringId(string name)
        {
            if (m_Strings.TryGetValue(name, out uint value))
                return value;

            return 0u;
        }

        public string GetString(uint stringId)
        {
            if (m_StringsById.TryGetValue(stringId, out string value))
                return value;

            return null;
        }

        public void Clear()
        {
            m_Strings.Clear();
            m_StringsById.Clear();
            m_StringsIndexCounter = 0u;
        }
    }
}
