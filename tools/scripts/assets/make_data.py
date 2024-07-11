import argparse
import errno
import os
import shutil
import time
import data_block

from PIL import Image

import atlas_packager
import cache
import folder_iterator
import maze_config
import resource_encryptor
import texture_compressor
import atlas_animator.atlas_animator
from utils import mkdir_p


class MakeData:

    TEXTURE_EXTENSION_POLICY_DEFAULT = 0
    TEXTURE_EXTENSION_POLICY_DO_NOT_CHANGE = 1

    def __init__(self, args):
        self.cache = None
        self.input = os.path.abspath(args.src)
        self.output = os.path.abspath(args.dst)
        self.first_party_tools = os.path.abspath(args.first_party_tools)
        self.third_party_tools = os.path.abspath(args.third_party_tools)
        self.temp = os.path.abspath(args.temp)

        self.texture_compression = args.tcompression
        self.texture_compression_ext_policy = args.tcompression_ext_policy

        self.mode = args.mode
        self.folder_iterator = folder_iterator.FolderIterator()
        self.need_to_change_textures_extensions = False #TODO: Rework with self.texture_compression_ext_policy
        self.root_folder = args.root
        self.cache_name = os.path.abspath('{0}/{1}'.format(self.temp, args.cache))
        if args.max_atlas_size:
            self.max_atlas_size = args.max_atlas_size
        else:
            self.max_atlas_size = 2048

        if args.tags and len(args.tags):
            self.build_tags = args.tags.split(',')
        else:
            self.build_tags = []

        self.mzdata_extensions = []
        for ext in args.mzdata_extensions:
            self.mzdata_extensions.append('.{0}'.format(ext))

        self.texture_compressor = None
        self.resource_encryptor = None

        self.tags_by_folder = {}


    def prepare(self):
        cache_dir = os.path.dirname(self.cache_name)
        if not os.path.exists(cache_dir):
            print('Creating cache directory...')
            mkdir_p(cache_dir)

        if not os.path.exists(self.output):
            print('Creating output directory...')
            mkdir_p(self.output)

        if self.cache_name:
            self.cache = cache.Cache(self.cache_name)
        else:
            self.cache = cache.Cache('temp/cache.pkl')

        self.texture_compressor = texture_compressor.TextureCompressor(
            self.texture_compression,
            self.mode,
            self.third_party_tools,
            self.temp)
        self.resource_encryptor = resource_encryptor.ResourceEncryptor()

    def run(self):

        self.prepare()

        print('Making data...')
        print('Input folder: ' + self.input)
        print('Output folder: ' + self.output)

        start_time = time.time()

        def it_file_function(full_path, file_name):

            if full_path.find('.mzmeta') != -1:
                return False

            if file_name.startswith('.'):
                return False

            name, ext = os.path.splitext(file_name)
            copy_from = '{0}/{1}'.format(self.input, full_path)
            copy_to = '{0}/{1}'.format(self.output, full_path)
            is_mzdata = ext in self.mzdata_extensions
            is_texture = ext in maze_config.textures_extensions
            is_sound = ext in maze_config.sounds_extensions

            info_file_path = '{0}/{1}.mzmeta'.format(self.input, full_path)
            is_up_to_date = self.cache.is_up_to_date('{0}/{1}'.format(self.input, full_path))

            dir_name = copy_to.replace(file_name, '')

            change_extension_enabled = True

            final_full_path_name = copy_to
            if is_texture and self.need_to_change_textures_extensions:
            
                parameters = data_block.DataBlock.load_text_file(info_file_path)
                if os.path.exists(info_file_path):
                    self.cache.is_up_to_date(info_file_path)

                change_extension_enabled = parameters.get_param_value('changeExtension', True)
                    
                if change_extension_enabled:
                    final_full_path_name = dir_name + name + maze_config.maze_texture_extension

            if is_up_to_date:
                if not os.path.exists(final_full_path_name):
                    is_up_to_date = False

            if is_up_to_date:
                if os.path.exists(info_file_path):
                    is_up_to_date = self.cache.is_up_to_date(info_file_path)

            tags = []
            for folder in self.tags_by_folder:
                if full_path.startswith(folder):
                    tags = self.tags_by_folder[folder]
                    break

            if not is_up_to_date:
                print('\tCOPYING FILE: ' + file_name)

                if not os.path.exists(dir_name):
                    os.makedirs(dir_name)

                parameters = data_block.DataBlock.load_text_file(info_file_path)
                compression_enabled = True

                if len(tags) > 0:
                    tags_value = data_block.DataBlock()
                    for tag in tags:
                        tags_value.add_param_string("tag", tag)
                    parameters.add_data_block("tags", tags_value)

                if os.path.exists(info_file_path):
                    self.cache.is_up_to_date(info_file_path)

                    if not parameters.get_param_value('compress', True):
                        compression_enabled = False

                    if is_sound:
                        if parameters.get_param_value('encrypted', False):
                            self.resource_encryptor.encrypt(copy_from, copy_to)
                        else:
                            shutil.copyfile(copy_from, copy_to)

                    else:
                        shutil.copyfile(copy_from, copy_to)
                else:
                    shutil.copyfile(copy_from, copy_to)

                if is_mzdata:
                    mzdata_binalizer = '{0}/{1}'.format(
                        self.first_party_tools,
                        'mzdata-converter/bin/maze-tool-mzdata-binalizer.exe')
                    system_command = '{0} {1} {2}'.format(
                        mzdata_binalizer,
                        copy_to,
                        copy_to)
                    print(system_command)
                    os.system(system_command)

                if is_texture:

                    if name.endswith('_Normal'):
                        compression_enabled = False

                    im_src = Image.open(copy_to)
                    parameters.add_param_s32('width', im_src.size[0])
                    parameters.add_param_s32('height', im_src.size[1])
                    im_src.close()

                    if compression_enabled:
                        copy_to = self.texture_compressor.compress_texture(copy_to)
                compressed_name, compressed_ext = os.path.splitext(copy_to)

                if is_texture and compression_enabled:
                    if self.texture_compression_ext_policy == MakeData.TEXTURE_EXTENSION_POLICY_DO_NOT_CHANGE:
                        new_full_name = copy_to.replace(compressed_ext, ext)
                        os.rename(copy_to, new_full_name)
                        copy_to = new_full_name

                if is_texture and self.need_to_change_textures_extensions and change_extension_enabled:
                    if os.path.exists(final_full_path_name):
                        os.remove(final_full_path_name)
                    os.rename(copy_to, final_full_path_name)

                if is_texture:
                    parameters.add_param_string('ext', compressed_ext.replace('.', ''))

                if len(parameters.params) > 0 or len(parameters.data_blocks) > 0:
                    parameters.save_text_file(final_full_path_name + '.mzmeta')


            return True

        def it_folder_function(full_path, folder_name):

            print('FOLDER: ' + folder_name)

            if folder_name.startswith('.'):
                print('\tSKIPPED')
                return False

            if full_path.endswith('.mzap'):

                self.cache.save_cache()

                info_file_path_folder = '{0}/{1}'.format(self.input, full_path)
                info_file_path = '{0}/mzap.mzmeta'.format(info_file_path_folder)

                is_up_to_date = True

                if is_up_to_date:
                    if os.path.exists(info_file_path):
                        is_up_to_date = self.cache.is_up_to_date(info_file_path)

                parameters = data_block.DataBlock.load_text_file(info_file_path)
                tags = parameters.get_data_block_as_string_list("tags")
                required_build_tags = parameters.get_data_block_as_string_list("requiredBuildTags")

                for required_build_tag in required_build_tags:
                    if required_build_tag not in self.build_tags:
                        print('\tSKIPPED(' + required_build_tag + ')')
                        return False

                self.tags_by_folder[full_path] = tags

                if not is_up_to_date:

                    output_info_file_folder = '{0}/{1}'.format(self.output, full_path)

                    try:
                        if not os.path.exists(output_info_file_folder):
                            os.makedirs(output_info_file_folder)
                    except OSError as e:
                        if e.errno != errno.EEXIST:
                            raise
                        pass

                    parameters.save_text_file('{0}.mzmeta'.format(output_info_file_folder))

            if full_path.endswith('.atlas'):
                print('NOT IMPLEMENTED!')
                return False

                input_folder = '{0}/{1}'.format(self.input, full_path)
                name, ext = os.path.splitext(folder_name)

                dir_name = full_path.replace(folder_name, '')
                output_folder = '{0}/{1}'.format(self.output, dir_name)

                is_up_to_date = self.cache.is_folder_up_to_date(os.path.join(self.input, full_path))

                if is_up_to_date and not os.path.exists(output_folder):
                    is_up_to_date = False

                info_file_path = '{0}/{1}'.format(self.input, full_path) + '.mzmeta'

                try:
                    if not os.path.exists(output_folder):
                        os.makedirs(output_folder)
                except OSError as e:
                    if e.errno != errno.EEXIST:
                        raise
                    pass

                if is_up_to_date:
                    if os.path.exists(info_file_path):
                        is_up_to_date = self.cache.is_up_to_date(info_file_path)

                if not is_up_to_date:

                    for i in os.listdir(output_folder):
                        if i.startswith(name) and\
                                (len(i) >= (len(name) + 2)) and\
                                (i[len(name)] == '_') and\
                                (i[len(name) + 1].isdigit()):
                            print('\tREMOVING OBSOLETE FILE: ' + output_folder + i)
                            os.remove(output_folder + i)

                    print('\tBAKING ATLAS: ' + folder_name)

                    parameters = data_block.DataBlock.load_text_file(info_file_path)

                    if os.path.exists(info_file_path):
                        self.cache.is_up_to_date(info_file_path)

                    atlas_info_file_path = input_folder + '.mzmeta'
                    extrude = 5
                    additional_params = ''

                    atlas_info_params = data_block.DataBlock.load_text_file(atlas_info_file_path)

                    if os.path.exists(atlas_info_file_path):
                        atlas_info_input_file = open(atlas_info_file_path)
                        content = atlas_info_input_file.read()
                        atlas_info_input_file.close()

                        for entry in content.split('\n'):
                            parameters.append(entry)
                            values = entry.split('=')
                            if len(values) == 2:
                                if values[0] == 'extrude':
                                    values[1] = values[1].replace('"', '')
                                    extrude = int(values[1])
                            if len(values) == 1:
                                if entry == 'disable_crop':
                                    additional_params = additional_params + ' --disable-crop'

                    compression_enabled = True
                    if 'compress=disabled' in parameters:
                        compression_enabled = False
                        
                    change_extension_enabled = True
                    if 'changeExtension=0' in parameters:
                        change_extension_enabled = False

                    if self.texture_compression:
                        additional_params = additional_params + ' --square'

                    atlas = atlas_packager.AtlasPackager(self.max_atlas_size)
                    atlas.pack(
                        input_folder,
                        output_folder,
                        self.third_party_tools,
                        name,
                        extrude,
                        additional_params)

                    print('\tBAKED.')

                    atlas_files_count = 0
                    for i in os.listdir(output_folder):
                        if i.startswith(name):
                            local_name, local_ext = os.path.splitext(i)

                            if local_ext in maze_config.textures_extensions:

                                local_parameters = list(parameters)
                                im_src = Image.open(output_folder + i)
                                local_parameters.append('width={0}'.format(im_src.size[0]))
                                local_parameters.append('height={0}'.format(im_src.size[1]))
                                im_src.close()

                                full_name = output_folder + i
                                if compression_enabled:
                                    full_name = self.texture_compressor.compress_texture(full_name)
                                compressed_name, compressed_ext = os.path.splitext(full_name)

                                if compression_enabled:
                                    if self.texture_compression_ext_policy == MakeData.TEXTURE_EXTENSION_POLICY_DO_NOT_CHANGE:
                                        new_full_name = full_name.replace(compressed_ext, local_ext)
                                        os.rename(full_name, new_full_name)
                                        full_name = new_full_name

                                if self.need_to_change_textures_extensions and change_extension_enabled:
                                    new_name = output_folder + local_name + maze_config.maze_texture_extension
                                    if os.path.exists(new_name):
                                        os.remove(new_name)
                                    if os.path.exists(full_name):
                                        os.rename(full_name, new_name)
                                        full_name = new_name
                                    else:
                                        print('\tERROR File is not exists - ' + full_name)

                                local_parameters.append('atlas={0}'.format(local_name + '.atlas'))
                                local_parameters.append('ext={0}'.format(compressed_ext.replace('.', '')))

                                output_info_file = open(full_name + '.mzmeta', 'wt')

                                for param in local_parameters:
                                    output_info_file.write(param + '\n')
                                output_info_file.close()

                            if local_ext == '.atlas':
                                atlas_files_count = atlas_files_count + 1

                                if self.need_to_change_textures_extensions:
                                    if os.path.isfile(output_folder + i):
                                        maze_config.change_textures_extensions(output_folder + i)

                    animator = atlas_animator.atlas_animator.AtlasAnimator()
                    if atlas_files_count == 1:
                        animator.run_with_atlas(
                            folder_name,
                            input_folder,
                            output_folder,
                            self.need_to_change_textures_extensions)
                    else:
                        animator.run_with_atlas(
                            name + '_{0}.atlas'.format(atlas_files_count),
                            input_folder,
                            output_folder,
                            self.need_to_change_textures_extensions)

                return False
            return True

        self.folder_iterator.run(self.input, it_file_function, it_folder_function)

        self.cache.save_cache()
        self.texture_compressor.shutdown()

        print('Time elapsed: {0} seconds'.format(time.time() - start_time))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Make data tool')
    parser.add_argument('--src', type=str, help='Source folder')
    parser.add_argument('--dst', type=str, help='Output folder')
    parser.add_argument('--first-party-tools', type=str, help='First party tools folder')
    parser.add_argument('--third-party-tools', type=str, help='Third party tools folder')
    parser.add_argument('--temp', type=str, help='Temp folder')
    parser.add_argument('--tcompression', type=str, help='Texture compression')
    parser.add_argument('--tcompression-ext-policy', type=int, help='Texture compression extension policy')
    parser.add_argument('--cache', type=str, help='Cache name')
    parser.add_argument('--root', type=str, help='Root folder')
    parser.add_argument('--mode', type=str, help='Production mode')
    parser.add_argument('--tags', type=str, help='Build tags')
    parser.add_argument('--max-atlas-size', type=str, help='Max atlas size')
    parser.add_argument('--mzdata-extensions', nargs='+', help='MZData asset extensions',
                        default='mzdata mzmaterial mzshader mzprefab mzfont mzfontmaterial')
    parsedArgs = parser.parse_args()
    try:
        make = MakeData(parsedArgs)
        make.run()
    except IOError as e:
        print('Error!')
        print('I/O error({0}): {1}'.format(e.errno, e))
        wait = input('Press Enter to continue......')
    except ValueError as e:
        print('Error!')
        print('ValueError({0))!.'.format(e))
        wait = input('Press Enter to continue......')
    except AssertionError as error:
        print('Error!')
        print(error)
        wait = input('Press Enter to continue......')
