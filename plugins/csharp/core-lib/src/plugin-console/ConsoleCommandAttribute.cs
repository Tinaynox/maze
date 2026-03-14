
using Maze.Core;
using System;

namespace Maze.Console
{
    [AttributeUsage(AttributeTargets.Method)]
    public class ConsoleCommandAttribute : Attribute
    {
        string m_CommandName;
        int m_ArgsCount;
        string m_Description;

        public ConsoleCommandAttribute(
            string commandName,
            int argsCount = 0,
            string description = default)
        {
            m_CommandName = commandName;
            m_ArgsCount = argsCount;
            m_Description = description;
        }
    }

    public static class ConsoleHelper
    {
        public static bool ArgToBool(string arg)
        {
            bool value = false;
            if (!string.IsNullOrWhiteSpace(arg))
            {
                arg = arg.Trim().ToLowerInvariant();

                if (arg == "1" || arg == "true" || arg == "t" ||
                    arg == "yes" || arg == "y" || arg == "on")
                    value = true;
                else if (arg == "0" || arg == "false" || arg == "f" ||
                         arg == "no" || arg == "n" || arg == "off")
                    value = false;
                else
                    bool.TryParse(arg, out value);
            }

            return value;
        }

        public static bool ArgToBool(string[] args, int idx)
        {
            bool value = false;
            if (args != null && idx < args.Length)
                value = ArgToBool(args[idx]);
            return value;
        }

        public static string ArgToString(string[] args, int idx)
        {
            if (args != null && idx < args.Length)
                return args[idx];
            return string.Empty;
        }

        public static int ArgToInt(string[] args, int idx)
        {
            int value = 0;
            if (args != null && idx < args.Length)
                int.TryParse(args[idx], out value);
            return value;
        }
    }
}