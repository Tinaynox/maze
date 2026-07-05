import os
import platform
import subprocess


class AtlasPackager:

    def __init__(self, max_atlas_size):
        tools = {
            "Darwin": 'Cheetah-Texture-Packer/bin/osx/Cheetah-Texture-Packer',
            "Linux": 'Cheetah-Texture-Packer/bin/linux/Cheetah-Texture-Packer',
            "Windows": 'Cheetah-Texture-Packer\\bin\\win\\Cheetah-Texture-Packer'
        }

        self.tool = tools[platform.system()]
        self.max_atlas_size = max_atlas_size

    def pack(self, source_folder, output_folder, tools_folder, name, extrude = 0, additional_params =''):
        input_images = source_folder
        output_image = output_folder + '/' + name + '.png'

        if not tools_folder.endswith('/') and not tools_folder.endswith('\\'):
            tools_folder = tools_folder + '/'

        system_command = [
            tools_folder + self.tool,
            input_images,
            '-o', output_image,
            '-s', str(self.max_atlas_size),
            '--extrude--size', str(extrude),
            '--border-size', '1'] + additional_params.split()
        print(subprocess.list2cmdline(system_command))
        subprocess.call(system_command)
