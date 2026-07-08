import os
import platform
import shutil
import subprocess
import texture_utils

from utils import mkdir_p


class TextureCompressionError(Exception):
    pass


class TextureCompressor:

    COMPRESSOR_NONE = 0
    COMPRESSOR_PVRTEXTOOL = 1
    COMPRESSOR_COMPRESSONATOR = 2
    COMPRESSOR_CRUNCH = 3
    COMPRESSOR_ASTCEVALUATIONCODEC = 4

    EXTENSION_COMPRESSION = 0
    EXTENSION_DO_NOT_CHANGE = 1

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
                        'format': 'dds -dxt1',
                        'format_alpha': 'dds -dxt5',
                        # crunch/crnlib has no BC4/BC5 encoder - normal maps are
                        # left uncompressed on this platform (see compress_texture).
                        'format_normal': None,
                        'args': ''
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'DXT1',
                        'format_alpha': 'DXT5',
                        # BC5 (two-channel, X/Y only - Z is reconstructed in the shader)
                        'format_normal': 'BC5',
                        'args': ''
                    },
                "Windows":
                    {
                        'tool': 'TheCompressonator\\TheCompressonator.exe',
                        'index': TextureCompressor.COMPRESSOR_COMPRESSONATOR,
                        'format': 'DXT1',
                        'format_alpha': 'DXT5',
                        # BC5 (two-channel, X/Y only - Z is reconstructed in the shader).
                        # Verify this fourCC token against the installed Compressonator
                        # CLI version - it must produce a DDS with 'ATI2' fourCC so the
                        # engine's DDS loader (ID_ATI2) recognizes it.
                        'format_normal': 'ATI2',
                        'args': ''
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_format_alpha = tools[system]['format_alpha']
            self.tool_format_normal = tools[system]['format_normal']
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
                        'format_alpha': 'PVRTC1_4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'PVRTC1_4',
                        'format_alpha': 'PVRTC1_4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Windows":
                    {
                        'tool': 'PVRTexTool\\CLI\\Windows_x86_32\\PVRTexToolCLI.exe',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'PVRTC1_4',
                        'format_alpha': 'PVRTC1_4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_format_alpha = tools[system]['format_alpha']
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
                        'format_alpha': 'ASTC_4x4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_4x4',
                        'format_alpha': 'ASTC_4x4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Windows":
                    {
                        'tool': 'PVRTexTool\\CLI\\Windows_x86_32\\PVRTexToolCLI.exe',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_4x4',
                        'format_alpha': 'ASTC_4x4',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_format_alpha = tools[system]['format_alpha']
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
                        'format_alpha': 'ASTC_6x6',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_6x6',
                        'format_alpha': 'ASTC_6x6',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Windows":
                    {
                        'tool': 'PVRTexTool\\CLI\\Windows_x86_32\\PVRTexToolCLI.exe',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ASTC_6x6',
                        'format_alpha': 'ASTC_6x6',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_format_alpha = tools[system]['format_alpha']
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
                        'format_alpha': 'ETC2_RGBA',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Linux":
                    {
                        'tool': 'PVRTexTool/CLI/Linux_x86_32/PVRTexToolCLI',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ETC2_RGBA',
                        'format_alpha': 'ETC2_RGBA',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    },
                "Windows":
                    {
                        'tool': 'PVRTexTool\\CLI\\Windows_x86_32\\PVRTexToolCLI.exe',
                        'index': TextureCompressor.COMPRESSOR_PVRTEXTOOL,
                        'format': 'ETC2_RGBA',
                        'format_alpha': 'ETC2_RGBA',
                        'args': '-m -pot + -square + -q {0}'.format(quality_mode)
                    }
            }

            system = platform.system()
            self.tool = self.tools_folder + tools[system]['tool']
            self.tool_index = tools[system]['index']
            self.tool_format = tools[system]['format']
            self.tool_format_alpha = tools[system]['format_alpha']
            self.tool_args = tools[system]['args']
            self.tool_file_extension = 'etc'

    def _run_compressor_tool(self, system_command, expected_output_path, source_texture_path):
        print(subprocess.list2cmdline(system_command))
        return_code = subprocess.call(system_command)
        print(' ')

        if return_code != 0:
            print('\tCOMPRESSION ERROR: tool exited with code {0} (it may have crashed) while compressing "{1}"'.format(
                return_code, source_texture_path))
            raise TextureCompressionError(
                'Texture compression tool crashed or failed (exit code {0}) on "{1}". Command: {2}'.format(
                    return_code, source_texture_path, subprocess.list2cmdline(system_command)))

        if not os.path.exists(expected_output_path):
            print('\tCOMPRESSION ERROR: tool exited with code 0 but did not produce output file "{0}" for "{1}"'.format(
                expected_output_path, source_texture_path))
            raise TextureCompressionError(
                'Texture compression tool did not produce expected output "{0}" for "{1}". Command: {2}'.format(
                    expected_output_path, source_texture_path, subprocess.list2cmdline(system_command)))

    def compress_texture(self, texture_full_path, is_normal_map=False):

        if self.tool_index == TextureCompressor.COMPRESSOR_NONE:
            return texture_full_path

        if not texture_utils.is_pot_texture(texture_full_path):
            return texture_full_path

        if is_normal_map:
            # Normal maps get their own codec (BC5/RGTC2 where available) instead of
            # DXT1/DXT5, since color-endpoint compression distorts stored directions.
            tool_format = getattr(self, 'tool_format_normal', None)
            if not tool_format:
                return texture_full_path
        else:
            has_alpha = texture_utils.has_transparent_pixel(texture_full_path)
            tool_format = self.tool_format_alpha if has_alpha else self.tool_format

        name, ext = os.path.splitext(os.path.basename(texture_full_path))
        final_ext = self.tool_file_extension

        if self.tool_index == TextureCompressor.COMPRESSOR_COMPRESSONATOR:
            new_full_path = (os.path.dirname(texture_full_path) +
                             '/' +
                             name +
                             '.' +
                             final_ext).replace('\\', '/')

            print('\t{0} COMPRESSION...'.format(tool_format))
            system_command = [
                self.tool,
                '-convert', '-overwrite',
                texture_full_path,
                new_full_path,
                '+fourCC', tool_format]
            self._run_compressor_tool(system_command, new_full_path, texture_full_path)
            os.remove(texture_full_path)
            print('\tCOMPRESSED.')
            return new_full_path

        if self.tool_index == TextureCompressor.COMPRESSOR_PVRTEXTOOL:
            prev_full_path = (os.path.dirname(texture_full_path) + '/' + name + '.pvr').replace('\\', '/')
            new_full_path = (os.path.dirname(texture_full_path) +
                             '/' +
                             name +
                             '.' +
                             final_ext).replace('\\', '/')

            print('\t{0} COMPRESSION...'.format(tool_format))
            system_command = [
                self.tool,
                '-i', texture_full_path,
                '-o', prev_full_path,
                '-f', tool_format] + self.tool_args.split()
            self._run_compressor_tool(system_command, prev_full_path, texture_full_path)
            os.remove(texture_full_path)
            os.rename(prev_full_path, new_full_path)
            print('\tCOMPRESSED.')
            return new_full_path

        if self.tool_index == TextureCompressor.COMPRESSOR_CRUNCH:
            new_full_path = (os.path.dirname(texture_full_path) +
                             '/' +
                             name +
                             '.' +
                             final_ext).replace('\\', '/')

            print('\t{0} COMPRESSION...'.format(tool_format))
            system_command = [
                self.tool,
                '-file', texture_full_path,
                '-out', new_full_path,
                '-fileformat'] + tool_format.split() + self.tool_args.split()
            self._run_compressor_tool(system_command, new_full_path, texture_full_path)
            os.remove(texture_full_path)
            print('\tCOMPRESSED.')
            return new_full_path

        if self.tool_index == TextureCompressor.COMPRESSOR_ASTCEVALUATIONCODEC:
            astc_full_path = (os.path.dirname(texture_full_path) + '/' + name + '.astc').replace('\\', '/')
            new_full_path = (os.path.dirname(texture_full_path) +
                             '/' +
                             name +
                             '.' +
                             final_ext).replace('\\', '/')

            print('\t{0} COMPRESSION...'.format(tool_format))
            system_command = [
                self.tool,
                '-c', texture_full_path,
                astc_full_path,
                '4x4',
                '-f', tool_format] + self.tool_args.split()
            self._run_compressor_tool(system_command, astc_full_path, texture_full_path)
            os.remove(texture_full_path)
            os.rename(astc_full_path, new_full_path)
            print('\tCOMPRESSED.')
            return new_full_path

        return texture_full_path

    def shutdown(self):
        if os.path.exists(self.temp_folder):
            shutil.rmtree(self.temp_folder)
