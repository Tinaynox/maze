import os
import platform


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
        atlas_size = '-s {0}'.format(self.max_atlas_size)

        if not tools_folder.endswith('/') and not tools_folder.endswith('\\'):
            tools_folder = self.tools_folder + '/'

        os.system('{0} {1} -o {2} {3} --extrude--size {4} --border-size 1 {5}'.format(
            tools_folder + self.tool,
            input_images,
            output_image,
            atlas_size,
            extrude,
            additional_params))
