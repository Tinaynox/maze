import os
import shutil
import data_block

import folder_iterator

from PIL import Image

import maze_config
from utils import mkdir_p


class ResourcePackageScaler:

    def __init__(self):
        self.src_parameters = {}
        self.rescale_enabled = False
        self.src_path = ''
        self.dst_path = ''
        self.scale = 1.0
        self.iterator = folder_iterator.FolderIterator()

    def rescale(self, src_path, dst_path, scale):
        if not isinstance(scale, float):
            raise ValueError("Scale should have float type!")

        dst_folder_dir = os.path.dirname(dst_path)
        dst_folder_fullname = os.path.basename(dst_path)
        dst_folder_name, dst_folder_ext = os.path.splitext(dst_folder_fullname)

        self.src_path = src_path
        self.scale = float(scale)

        self.dst_path = dst_folder_dir + '/' + dst_folder_name + '-x' + str(self.scale) + dst_folder_ext

        src_info_file_path = '{0}.mzmeta'.format(src_path)
        self.src_parameters = data_block.DataBlock.load_text_file(src_info_file_path)

        self.rescale_enabled = True
        if self.src_parameters.get_param_value('rescaleDisabled', False):
            self.rescale_enabled = False

        def it_file_function(full_path, file_name):
            name, ext = os.path.splitext(file_name)
            copy_from = '{0}/{1}'.format(self.src_path, full_path)
            copy_to = '{0}/{1}'.format(self.dst_path, full_path)

            copy_required = not os.path.exists(copy_to)

            copy_from_stat = os.stat(copy_from)
            copy_from_time = copy_from_stat.st_mtime

            info_file_path = '{0}.mzmeta'.format(copy_from)
            parameters = data_block.DataBlock.load_text_file(info_file_path)

            if not copy_required:

                copy_to_stat = os.stat(copy_to)

                if copy_from_time != copy_to_stat.st_mtime:
                    copy_required = True
                    print(file_name + ': Time is different')
                else:
                    if (scale == 1.0) and (copy_from_stat.st_size != copy_to_stat.st_size):
                        copy_required = True
                        print(file_name + ': Size is different')

            if copy_required:
                dir_name = os.path.dirname(copy_to)
                if not os.path.exists(dir_name):
                    mkdir_p(dir_name)

                copy_from = os.path.normpath(os.path.join(os.getcwd(), copy_from))
                copy_to = os.path.normpath(os.path.join(os.getcwd(), copy_to))
                print('Processing: ' + copy_to)

                is_texture = ext in maze_config.textures_extensions
                if is_texture:
                    rescale_enabled = self.rescale_enabled

                    if parameters.get_param_value('rescaleDisabled', False):
                        rescale_enabled = False

                    if os.path.exists(info_file_path):
                        shutil.copy2(info_file_path, "{0}.mzmeta".format(copy_to))

                    if (scale == 1.0) or (not rescale_enabled):
                        shutil.copy2(copy_from, copy_to)
                    else:
                        im_src = Image.open(copy_from)
                        size = [max(1, int(im_src.size[0] * self.scale)), max(1, int(im_src.size[1] * self.scale))]
                        im_src.thumbnail(size, Image.ANTIALIAS)
                        im_src.save(copy_to)
                        im_src.close()
                        os.utime(copy_to, (copy_from_time, copy_from_time))

                        new_meta_file_path = "{0}.mzmeta".format(copy_to)
                        new_meta_data_block = data_block.DataBlock.load_text_file(new_meta_file_path)
                        new_meta_data_block.add_param_f32("rescale", self.scale)
                        new_meta_data_block.save_text_file(new_meta_file_path)
                else:
                    if ext == ".mzmeta":
                        _, origin_ext = os.path.splitext(name)
                        if origin_ext in maze_config.textures_extensions:
                            # We will process texture mzmeta data manually
                            return

                    shutil.copy2(copy_from, copy_to)

        def it_folder_function(full_path, folder_name):
            folder = '{0}/{1}'.format(self.dst_path, full_path)
            if os.path.exists(folder):
                for file in os.listdir(folder):
                    src_file = '{0}/{1}/{2}'.format(self.src_path, full_path, file)
                    if not os.path.exists(src_file):
                        dst_file = '{0}/{1}/{2}'.format(self.dst_path, full_path, file)
                        print('REMOVING: ' + dst_file + '...')
                        if os.path.isdir(dst_file):
                            shutil.rmtree(dst_file, ignore_errors=True)
                        else:
                            os.remove(dst_file)

            return True

        self.iterator.run(self.src_path, it_file_function, it_folder_function)
