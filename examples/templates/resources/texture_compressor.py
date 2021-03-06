import os
import platform
import shutil

from utils import mkdir_p


class TextureCompressor:

    COMPRESSOR_NONE = 0
    COMPRESSOR_PVRTEXTOOL = 1
    COMPRESSOR_COMPRESSONATOR = 2
    COMPRESSOR_CRUNCH = 3
    COMPRESSOR_ASTCEVALUATIONCODEC = 4

    def __init__(self, texture_compression, mode, tools_folder, temp_folder):
        self.texture_compression = texture_compression
        self.mode = mode

        if self.mode == 'Debug':
            self.is_debug = True
        else:
            self.is_debug = False

        self.temp_folder = os.path.abspath('{0}/.compressor-temp'.format(temp_folder))

        if not os.path.exists(self.temp_folder):
            mkdir_p(self.temp_folder)

        self.tools_folder = tools_folder
        if not self.tools_folder.endswith('/') and not self.tools_folder.endswith('\\'):
            self.tools_folder = self.tools_folder + '/'

        if self.texture_compression is None:
            self.tool_index = TextureCompressor.COMPRESSOR_NONE

        if self.texture_compression == 'dxt':
            tools = {
                "Darwin":
                    {
                        'tool': 'crunch/osx/crunch',
                        'index': TextureCompressor.COMPRESSOR_CRUNCH,
                        'format': 'dds -dxt5',
                        'args': ''
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'DXT5',
                        'args': ''
                    },
                "Windows":
                    {
                        'tool': 'TheCompressonator\\TheCompressonator.exe',
                        'index': TextureCompressor.COMPRESSOR_COMPRESSONATOR,
                        'format': 'DXT5',
                        'args': ''
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_args = tools[system]['args']
            self.tool_file_extension = 'dds'

        if self.texture_compression == 'pvr':
            quality_mode = ('pvrtcfastest' if self.is_debug else 'pvrtcbest')
            tools = {
                "Darwin":
                    {
                        'tool': 'PVRTexTool/CLI/OSX_x86/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'PVRTC1_4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'PVRTC1_4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Windows":
                    {
                        'tool': 'PVRTexTool\\CLI\\Windows_x86_32\\PVRTexToolCLI.exe',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'PVRTC1_4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_args = tools[system]['args']
            self.tool_file_extension = 'pvr'

        if self.texture_compression == 'astc4x4':
            quality_mode = ('pvrtcfastest' if self.is_debug else 'pvrtcbest')
            tools = {
                "Darwin":
                    {
                        'tool': 'PVRTexTool/CLI/OSX_x86/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_4x4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_4x4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Windows":
                    {
                        'tool': 'PVRTexTool\\CLI\\Windows_x86_32\\PVRTexToolCLI.exe',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_4x4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_args = tools[system]['args']
            self.tool_file_extension = 'astc'

        if self.texture_compression == 'astc6x6':
            quality_mode = ('pvrtcfastest' if self.is_debug else 'pvrtcbest')
            tools = {
                "Darwin":
                    {
                        'tool': 'PVRTexTool/CLI/OSX_x86/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_6x6',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_6x6',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Windows":
                    {
                        'tool': 'PVRTexTool\\CLI\\Windows_x86_32\\PVRTexToolCLI.exe',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_6x6',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_args = tools[system]['args']
            self.tool_file_extension = 'astc'

        if self.texture_compression == 'etc':
            quality_mode = ('etcfast' if self.is_debug else 'etcslow')
            tools = {
                "Darwin":
                    {
                        'tool': 'PVRTexTool/CLI/OSX_x86/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ETC2_RGBA',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ETC2_RGBA',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Windows":
                    {
                        'tool': 'PVRTexTool\\CLI\\Windows_x86_32\\PVRTexToolCLI.exe',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ETC2_RGBA',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_args = tools[system]['args']
            self.tool_file_extension = 'etc'

    def compress_texture(self, texture_full_path):

        if self.tool_index == TextureCompressor.COMPRESSOR_NONE:
            return texture_full_path

        if self.tool_index == TextureCompressor.COMPRESSOR_COMPRESSONATOR:
            name, ext = os.path.splitext(os.path.basename(texture_full_path))
            new_full_path = (os.path.dirname(texture_full_path) +
                             '/' +
                             name +
                             '.' +
                             self.tool_file_extension).replace('\\', '/')

            print('\t{0} COMPRESSION...'.format(self.tool_format))
            system_command = '{0} -convert -overwrite {1} {2} +fourCC {3}'.format(
                self.tool,
                texture_full_path,
                new_full_path,
                self.tool_format)
            print(system_command)
            os.system(system_command)
            os.remove(texture_full_path)
            print(' ')
            print('\tCOMPRESSED.')

            return new_full_path

        if self.tool_index == TextureCompressor.COMPRESSOR_PVRTEXTOOL:
            name, ext = os.path.splitext(os.path.basename(texture_full_path))
            prev_full_path = (os.path.dirname(texture_full_path) + '/' + name + '.pvr').replace('\\', '/')
            new_full_path = (os.path.dirname(texture_full_path) +
                             '/' +
                             name +
                             '.' +
                             self.tool_file_extension).replace('\\', '/')

            print('\t{0} COMPRESSION...'.format(self.tool_format))
            system_command = '{0} -i {1} -o {2} -f {3} {4}'.format(
                self.tool,
                texture_full_path,
                prev_full_path,
                self.tool_format,
                self.tool_args)
            print(system_command)
            os.system(system_command)
            os.remove(texture_full_path)
            os.rename(prev_full_path, new_full_path)
            print(' ')
            print('\tCOMPRESSED.')

            return new_full_path

        if self.tool_index == TextureCompressor.COMPRESSOR_CRUNCH:

            name, ext = os.path.splitext(os.path.basename(texture_full_path))

            new_full_path = (os.path.dirname(texture_full_path) +
                             '/' +
                             name +
                             '.' +
                             self.tool_file_extension).replace('\\', '/')

            print('\t{0} COMPRESSION...'.format(self.tool_format))
            system_command = '{0} -file {1} -out {2} -fileformat {3} {4}'.format(
                self.tool,
                texture_full_path,
                new_full_path,
                self.tool_format,
                self.tool_args)
            print(system_command)
            os.system(system_command)
            os.remove(texture_full_path)
            print(' ')
            print('\tCOMPRESSED.')

            return new_full_path

        if self.tool_index == TextureCompressor.COMPRESSOR_ASTCEVALUATIONCODEC:
            name, ext = os.path.splitext(os.path.basename(texture_full_path))
            astc_full_path = (os.path.dirname(texture_full_path) + '/' + name + '.astc').replace('\\', '/')
            new_full_path = (os.path.dirname(texture_full_path) +
                             '/' +
                             name +
                             '.' +
                             self.tool_file_extension).replace('\\', '/')

            print('\t{0} COMPRESSION...'.format(self.tool_format))
            system_command = '{0} -c {1} {2} 4x4 -f {3} {4}'.format(
                self.tool,
                texture_full_path,
                astc_full_path,
                self.tool_format,
                self.tool_args)
            print(system_command)
            os.system(system_command)
            os.remove(texture_full_path)
            os.rename(astc_full_path, new_full_path)
            print(' ')
            print('\tCOMPRESSED.')

            return new_full_path

        return texture_full_path

    def shutdown(self):
        if os.path.exists(self.temp_folder):
            shutil.rmtree(self.temp_folder)
