using System;

namespace Maze.Editor
{
    [AttributeUsage(AttributeTargets.Class)]
    public class EditorMenuBarAttribute : Attribute
    {
        string m_MenuName;
        public string MenuName => m_MenuName;

        string m_OptionPath;
        public string OptionPath => m_OptionPath;

        string m_Option;
        public string Option => m_Option;

        public EditorMenuBarAttribute(
            string menuName = default,
            string optionPath = default,
            string option = default)
        {
            m_MenuName = menuName;
            m_OptionPath = optionPath;
            m_Option = option;
        }
    }
}
