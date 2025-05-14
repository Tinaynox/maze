using System;
using System.Collections.Generic;
using System.Linq;

namespace Maze.Core
{
    public static class StringHelper
    {
        public static int ParseInteger<T>(string str, int start, int end, out T value)
        where T : struct, IConvertible
        {
            // Ensure T is an integer type
            Type type = typeof(T);
            if (type != typeof(sbyte) && type != typeof(byte) &&
                type != typeof(short) && type != typeof(ushort) &&
                type != typeof(int) && type != typeof(uint) &&
                type != typeof(long) && type != typeof(ulong))
            {
                throw new ArgumentException("T must be an integer type.");
            }

            value = default(T);
            if (string.IsNullOrEmpty(str) || start < 0 || end > str.Length || start >= end)
            {
                return -1;
            }

            int index = start;
            bool isNegative = false;

            // Handle sign
            if (str[index] == '-')
            {
                isNegative = true;
                index++;
            }
            else if (str[index] == '+')
            {
                index++;
            }

            // Check for invalid input or no digits
            if (index >= end || !char.IsDigit(str[index]))
            {
                return -1;
            }

            long result = 0; // Use long for intermediate calculations to handle overflow

            // Handle hexadecimal (e.g., "0x1A")
            if (str[index] == '0' && !isNegative && (end - index) >= 2 && char.ToLower(str[index + 1]) == 'x')
            {
                index += 2; // Skip "0x"
                while (index < end)
                {
                    char ch = char.ToLower(str[index]);
                    int digit;

                    if (char.IsDigit(ch))
                        digit = ch - '0';
                    else if (ch >= 'a' && ch <= 'f')
                        digit = ch - 'a' + 10;
                    else
                        break; // Invalid hex character

                    result = (result << 4) | (uint)digit;
                    index++;
                }

                // Apply result to output value with type checking
                try
                {
                    value = (T)Convert.ChangeType(isNegative ? -result : result, typeof(T));
                }
                catch (OverflowException)
                {
                    return -1; // Overflow occurred
                }

                return index;
            }

            // Handle decimal
            while (index < end && char.IsDigit(str[index]))
            {
                result = result * 10 + (str[index] - '0');
                index++;
            }

            // Apply sign and convert to target type
            try
            {
                value = (T)Convert.ChangeType(isNegative ? -result : result, typeof(T));
            }
            catch (OverflowException)
            {
                return -1; // Overflow occurred
            }

            return index;
        }


        public static int ParseInteger<T>(List<byte> str, int start, int end, out T value)
        where T : struct, IConvertible
        {
            // Ensure T is an integer type
            Type type = typeof(T);
            if (type != typeof(sbyte) && type != typeof(byte) &&
                type != typeof(short) && type != typeof(ushort) &&
                type != typeof(int) && type != typeof(uint) &&
                type != typeof(long) && type != typeof(ulong))
            {
                throw new ArgumentException("T must be an integer type.");
            }

            value = default(T);
            if (str.Count == 0 || start < 0 || end > str.Count || start >= end)
            {
                return -1;
            }

            int index = start;
            bool isNegative = false;

            // Handle sign
            if (str[index] == '-')
            {
                isNegative = true;
                index++;
            }
            else if (str[index] == '+')
            {
                index++;
            }

            // Check for invalid input or no digits
            if (index >= end || !char.IsDigit((char)str[index]))
            {
                return -1;
            }

            long result = 0; // Use long for intermediate calculations to handle overflow

            // Handle hexadecimal (e.g., "0x1A")
            if (str[index] == '0' && !isNegative && (end - index) >= 2 && char.ToLower((char)str[index + 1]) == 'x')
            {
                index += 2; // Skip "0x"
                while (index < end)
                {
                    char ch = char.ToLower((char)str[index]);
                    int digit;

                    if (char.IsDigit(ch))
                        digit = ch - '0';
                    else if (ch >= 'a' && ch <= 'f')
                        digit = ch - 'a' + 10;
                    else
                        break; // Invalid hex character

                    result = (result << 4) | (uint)digit;
                    index++;
                }

                // Apply result to output value with type checking
                try
                {
                    value = (T)Convert.ChangeType(isNegative ? -result : result, typeof(T));
                }
                catch (OverflowException)
                {
                    return -1; // Overflow occurred
                }

                return index;
            }

            // Handle decimal
            while (index < end && char.IsDigit((char)str[index]))
            {
                result = result * 10 + (str[index] - '0');
                index++;
            }

            // Apply sign and convert to target type
            try
            {
                value = (T)Convert.ChangeType(isNegative ? -result : result, typeof(T));
            }
            catch (OverflowException)
            {
                return -1; // Overflow occurred
            }

            return index;
        }


        public static int ParseFloat<T>(string str, int start, int end, out T value)
        where T : struct
        {
            // Ensure T is a floating-point type
            Type type = typeof(T);
            if (type != typeof(float) && type != typeof(double) && type != typeof(decimal))
            {
                throw new ArgumentException("T must be a floating-point type (float, double, or decimal).");
            }

            value = default(T);
            if (string.IsNullOrEmpty(str) || start < 0 || end > str.Length || start >= end)
            {
                return -1;
            }

            int index = start;
            bool isNegative = false;

            // Handle sign
            if (str[index] == '-')
            {
                isNegative = true;
                index++;
            }
            else if (str[index] == '+')
            {
                index++;
            }

            // Check for valid start (digit or decimal point)
            if (index >= end || (!char.IsDigit(str[index]) && str[index] != '.'))
            {
                return -1;
            }

            // Parse integer part
            decimal integerPart = 0;
            while (index < end && char.IsDigit(str[index]))
            {
                integerPart = integerPart * 10 + (str[index] - '0');
                index++;
            }

            // Parse fractional part
            decimal fractionalPart = 0;
            decimal divisor = 1;
            if (index < end && str[index] == '.')
            {
                index++;
                while (index < end && char.IsDigit(str[index]))
                {
                    fractionalPart = fractionalPart * 10 + (str[index] - '0');
                    divisor *= 10;
                    index++;
                }
            }

            // Calculate base number
            decimal result = integerPart + (fractionalPart / divisor);

            // Handle exponent (scientific notation, e.g., "1.23e-4")
            int exponent = 0;
            if (index < end && char.ToLower(str[index]) == 'e')
            {
                index++;
                bool exponentNegative = false;
                if (index < end && str[index] == '-')
                {
                    exponentNegative = true;
                    index++;
                }
                else if (index < end && str[index] == '+')
                {
                    index++;
                }

                if (index >= end || !char.IsDigit(str[index]))
                {
                    return -1; // Invalid exponent
                }

                while (index < end && char.IsDigit(str[index]))
                {
                    exponent = exponent * 10 + (str[index] - '0');
                    index++;
                }

                if (exponentNegative)
                {
                    exponent = -exponent;
                }
            }

            // Apply exponent
            if (exponent != 0)
            {
                result *= (decimal)Math.Pow(10, exponent);
            }

            // Apply sign
            if (isNegative)
            {
                result = -result;
            }

            // Convert to target type
            try
            {
                if (type == typeof(float))
                {
                    value = (T)(object)(float)result;
                }
                else if (type == typeof(double))
                {
                    value = (T)(object)(double)result;
                }
                else // decimal
                {
                    value = (T)(object)result;
                }
            }
            catch (OverflowException)
            {
                return -1; // Overflow occurred
            }

            return index;
        }

        public static int ParseFloat<T>(List<byte> str, int start, int end, out T value)
        where T : struct
        {
            // Ensure T is a floating-point type
            Type type = typeof(T);
            if (type != typeof(float) && type != typeof(double) && type != typeof(decimal))
            {
                throw new ArgumentException("T must be a floating-point type (float, double, or decimal).");
            }

            value = default(T);
            if (str.Count == 0 || start < 0 || end > str.Count || start >= end)
            {
                return -1;
            }

            int index = start;
            bool isNegative = false;

            // Handle sign
            if (str[index] == '-')
            {
                isNegative = true;
                index++;
            }
            else if (str[index] == '+')
            {
                index++;
            }

            // Check for valid start (digit or decimal point)
            if (index >= end || (!char.IsDigit((char)str[index]) && str[index] != '.'))
            {
                return -1;
            }

            // Parse integer part
            decimal integerPart = 0;
            while (index < end && char.IsDigit((char)str[index]))
            {
                integerPart = integerPart * 10 + (str[index] - '0');
                index++;
            }

            // Parse fractional part
            decimal fractionalPart = 0;
            decimal divisor = 1;
            if (index < end && str[index] == '.')
            {
                index++;
                while (index < end && char.IsDigit((char)str[index]))
                {
                    fractionalPart = fractionalPart * 10 + (str[index] - '0');
                    divisor *= 10;
                    index++;
                }
            }

            // Calculate base number
            decimal result = integerPart + (fractionalPart / divisor);

            // Handle exponent (scientific notation, e.g., "1.23e-4")
            int exponent = 0;
            if (index < end && char.ToLower((char)str[index]) == 'e')
            {
                index++;
                bool exponentNegative = false;
                if (index < end && str[index] == '-')
                {
                    exponentNegative = true;
                    index++;
                }
                else if (index < end && str[index] == '+')
                {
                    index++;
                }

                if (index >= end || !char.IsDigit((char)str[index]))
                {
                    return -1; // Invalid exponent
                }

                while (index < end && char.IsDigit((char)str[index]))
                {
                    exponent = exponent * 10 + (str[index] - '0');
                    index++;
                }

                if (exponentNegative)
                {
                    exponent = -exponent;
                }
            }

            // Apply exponent
            if (exponent != 0)
            {
                result *= (decimal)Math.Pow(10, exponent);
            }

            // Apply sign
            if (isNegative)
            {
                result = -result;
            }

            // Convert to target type
            try
            {
                if (type == typeof(float))
                {
                    value = (T)(object)(float)result;
                }
                else if (type == typeof(double))
                {
                    value = (T)(object)(double)result;
                }
                else // decimal
                {
                    value = (T)(object)result;
                }
            }
            catch (OverflowException)
            {
                return -1; // Overflow occurred
            }

            return index;
        }

        public static int ParseBoolPretty(string str, int start, int end, out bool value)
        {
            value = false; // Default value
            if (string.IsNullOrEmpty(str) || start < 0 || end > str.Length || start >= end)
            {
                return -1; // Return original string on invalid input
            }

            int size = end - start;
            string substring = str.Substring(start, size);

            if (size >= 4 && substring.StartsWith("true", StringComparison.OrdinalIgnoreCase))
            {
                value = true;
                start += 4;
            }
            else if (size >= 1 && substring.StartsWith("1", StringComparison.OrdinalIgnoreCase))
            {
                value = true;
                start += 1;
            }
            else if (size >= 5 && substring.StartsWith("false", StringComparison.OrdinalIgnoreCase))
            {
                value = false;
                start += 5;
            }
            else if (size >= 1 && substring.StartsWith("0", StringComparison.OrdinalIgnoreCase))
            {
                value = false;
                start += 1;
            }
            else
            {
                return -1; // No valid boolean found, return original string
            }

            return start;
        }

        public static int ParseBoolPretty(List<byte> str, int start, int end, out bool value)
        {
            value = false; // Default value
            if (str == null || str.Count == 0 || start < 0 || end > str.Count || start >= end)
            {
                return -1; // Invalid input
            }

            int size = end - start;

            // Helper function to check if a byte sequence matches a string (case-insensitive)
            bool Matches(int index, string target)
            {
                if (index + target.Length > end)
                    return false;

                for (int i = 0; i < target.Length; i++)
                {
                    byte b = str[index + i];
                    char c = (char)b;
                    char t = target[i];
                    if (char.ToLower(c) != char.ToLower(t))
                        return false;
                }
                return true;
            }

            if (size >= 4 && Matches(start, "true"))
            {
                value = true;
                start += 4;
            }
            else if (size >= 1 && Matches(start, "1"))
            {
                value = true;
                start += 1;
            }
            else if (size >= 5 && Matches(start, "false"))
            {
                value = false;
                start += 5;
            }
            else if (size >= 1 && Matches(start, "0"))
            {
                value = false;
                start += 1;
            }
            else
            {
                return -1; // No valid boolean found
            }

            return start;
        }

        public static int SkipChar(string str, int start, int end, char ch)
        {
            if (str.Length == 0 || start < 0)
                return -1;

            for (; start < end; ++start)
                if (str[start] != ch)
                    break;

            return start;
        }

        public static int SkipChar(List<byte> str, int start, int end, byte ch)
        {
            if (str.Count == 0 || start < 0)
                return -1;

            for (; start < end; ++start)
                if (str[start] != ch)
                    break;

            return start;
        }

        public static int ExpectSkipChar(string str, int start, int end, char ch, int count = 1)
        {
            if (count == 0 || start < 0 || str.Length == 0)
                return -1;

            for (; start < end && count > 0; ++start)
                if (str[start] == ch)
                    --count;
                else
                    return -1;

            return count == 0 ? start : -1;
        }

        public static int ExpectSkipChar(List<byte> str, int start, int end, byte ch, int count = 1)
        {
            if (count == 0 || start < 0 || str.Count == 0)
                return -1;

            for (; start < end && count > 0; ++start)
                if (str[start] == ch)
                    --count;
                else
                    return -1;

            return count == 0 ? start : -1;
        }

        public static string BoolToStringPretty(bool value) => value ? "true" : "false";
    }
}
