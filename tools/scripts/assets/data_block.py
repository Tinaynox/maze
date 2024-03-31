from enum import Enum
import os


class DataBlockParamType(Enum):
    NONE = 0
    S32 = 1
    S64 = 2
    U32 = 3
    U64 = 4
    F32 = 5
    F64 = 6
    Bool = 7
    Vec4S8 = 8
    Vec4U8 = 9
    Vec2S32 = 10
    Vec3S32 = 11
    Vec4S32 = 12
    Vec2U32 = 13
    Vec3U32 = 14
    Vec4U32 = 15
    Vec2F32 = 16
    Vec3F32 = 17
    Vec4F32 = 18
    Vec2B = 19
    Vec3B = 20
    Vec4B = 21
    Mat3F32 = 22
    Mat4F32 = 23
    String = 24


class DataBlockParam:
    def __init__(self, param_name="", param_type=DataBlockParamType.NONE, param_value=None):
        assert isinstance(param_name, str), "param_name must be an str"
        self.name = param_name
        self.type = param_type
        self.value = param_value

    def set(self, param_name, param_type, param_value):
        assert isinstance(param_name, str), "param_name must be an str"
        self.name = param_name
        self.type = param_type
        self.value = param_value

    def set(self, param_type, param_value):
        self.type = param_type
        self.value = param_value


MAZE_DATA_BLOCK_COMMENT_PREFIX = "@\x0C"
MAZE_DATA_BLOCK_COMMENT_SUFFIX_CPP = "@\x01"
MAZE_DATA_BLOCK_COMMENT_SUFFIX_C = "@\x02"
MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP = "@\x011"
MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C = "@\x012"
MAZE_DATA_BLOCK_COMMENT_CPP = MAZE_DATA_BLOCK_COMMENT_PREFIX + MAZE_DATA_BLOCK_COMMENT_SUFFIX_CPP
MAZE_DATA_BLOCK_COMMENT_C = MAZE_DATA_BLOCK_COMMENT_PREFIX + MAZE_DATA_BLOCK_COMMENT_SUFFIX_C
MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP = MAZE_DATA_BLOCK_COMMENT_PREFIX + MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP
MAZE_DATA_BLOCK_COMMENT_ENDLINE_C = MAZE_DATA_BLOCK_COMMENT_PREFIX + MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C


def _is_identifier_char(ch):
    return (('a' <= ch <= 'z') or
            ('A' <= ch <= 'Z') or
            ('0' <= ch <= '9') or
            ch == '_' or
            ch == '-' or
            ch == '.' or
            ch == '~')


def _is_name_is_simple(name):
    result = True
    if name is None or name == "":
        result = False

    for ch in name:
        result = _is_identifier_char(ch)
        if not result:
            break
    return result


def _write_eof(file):
    file.write("\n")


def _write_indent(file, size):
    for i in size:
        file.write(' ')


def _write_complex_string(file, string):
    quote = "\""
    quote_len = 1

    has_line_break = False
    has_quote = False
    has_tick = False

    for ch in string:
        if ch == '\n' or ch == '\r':
            has_line_break = True
        elif ch == '\"':
            has_quote = True
        elif ch == '\'':
            has_tick = True
        else:
            continue

        if has_line_break and has_quote and has_tick:
            break

    if has_line_break and not has_quote:
        quote = "\"\"\"\n\"\"\""
        quote_len = 4
    elif has_line_break and not has_tick:
        quote = "'''\n'''"
        quote_len = 4
    elif has_line_break:
        quote = "\"\"\"\n\"\"\""
        quote_len = 4
    elif has_quote and not has_tick:
        quote = "'"

    file.write(quote[:quote_len])

    for i in range(len(string)):
        ch = string[i]
        if ch == '~':
            file.write("~~")
        elif ch == quote[0] and (quote_len == 1 or string[i + 1]):
            file.write("~\"" if ch == '\"' else "~\'")
        elif ch == '\r' and quote_len == 1:
            file.write("~r")
        elif ch == '\n' and quote_len == 1:
            file.write("~n")
        elif ch == '\t':
            file.write("~t")
        else:
            file.write(ch)

    file.write(quote[-quote_len:])


class DataBlock:
    def __init__(self, data_block_name=""):
        assert isinstance(data_block_name, str), "data_block_name must be an str"
        self.name = data_block_name
        self.params = []
        self.data_blocks = []

    # Params
    def add_param(self, param_name, param_type, param_value):
        self.params.append(DataBlockParam(param_name, param_type, param_value))

    def get_param(self, param_name):
        return next((x for x in self.params if x.name == param_name), None)

    def get_param_value(self, param_name, default_value):
        param = self.get_param(param_name)
        if param is not None:
            return param.value

        return default_value

    def set_param(self, param_index, param_type, param_value):
        if 0 <= param_index < len(self.params):
            param = self.params[param_index]
            param.set(param_type, param_value)

    # Data Blocks
    def add_data_block(self, data_block):
        assert isinstance(data_block, DataBlock), "value must be an DataBlock"
        self.data_blocks.append(data_block)
        return data_block

    def create_data_block(self, data_block_name):
        new_data_block = DataBlock(data_block_name)
        self.add_data_block(new_data_block)
        return new_data_block

    def get_data_block(self, data_block_name, default_value=None):
        if default_value is None:
            default_value = DataBlock()
        return next((x for x in self.data_blocks if x.name == data_block_name), default_value)

    # Serialization
    def save_text_file(self, file_path):
        with open(file_path, "w") as file:
            self._save_text_file(file, 0)

    def _save_text_file(self, file, level):
        skip_next_indent = False

        for param in self.params:
            name = param.name
            name_is_simple = _is_name_is_simple(name)

            # Indent
            if level > 0:
                if not skip_next_indent:
                    _write_indent(file, level * 2)
                else:
                    skip_next_indent = False

            if len(name) >= 3 and name.startswith(MAZE_DATA_BLOCK_COMMENT_PREFIX):
                if name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_C) or name.startswith(MAZE_DATA_BLOCK_COMMENT_C):
                    file.write("/*")
                    file.write(param.value)
                    file.write("*/")
                    _write_eof(file)
                elif name.startswith(MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP) or name.startswith(MAZE_DATA_BLOCK_COMMENT_CPP):
                    file.write("//")
                    file.write(param.value)
                    _write_eof(file)
                continue

            if _is_name_is_simple(name):
                file.write(name)
            else:
                _write_complex_string(file, name)

            file.write(":")
