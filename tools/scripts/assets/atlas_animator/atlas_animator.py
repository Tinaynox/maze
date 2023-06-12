import os

import folder_iterator
import maze_config
from utils import is_number


class AtlasAnimator:

    def __init__(self):
        self.full_path = ''
        self.base_name = ''
        self.atlas_name = ''
        self.output_folder = ''
        self.animations = 0
        self.animation = ''
        self.sprites = []
        self.sprites_duration = []
        self.animations_text = ''
        self.flipped_animations_text = ''
        self.default_frame_duration = 0.2
        self.need_to_change_textures_extensions = True

    def run_with_atlas(self, base_name, full_path, output_folder, need_to_change_textures_extensions):

        self.full_path = full_path
        self.base_name = base_name
        self.output_folder = output_folder
        self.atlas_name = self.base_name.replace('.atlas', '')
        self.animations = 0
        self.animation = ''
        self.sprites = []
        self.sprites_duration = []
        self.animations_text = ''
        self.flipped_animations_text = ''
        self.default_frame_duration = 0.2
        self.need_to_change_textures_extensions = need_to_change_textures_extensions

        def it_file_function(full_path, file_name):

            if file_name == '--animations-flip.txt':
                self._parse_animations_flip(full_path)

            n = full_path.find('.animation')
            if n != -1:

                if self.animation == '':
                    return

                if file_name.find(self.animation) > -1:

                    if self.need_to_change_textures_extensions:
                        name, ext = os.path.splitext(file_name)
                        file_name = file_name.replace(ext, maze_config.maze_texture_extension)

                    self.sprites.append(file_name)

                if file_name == '--animation--info.txt':
                    self._parse_frames_info(full_path)

            return True

        def it_folder_function(full_path, folder_name):

            n = folder_name.find('.animation')
            if n != -1:
                self._save_animation()
                self.animation = folder_name.replace('.animation', '')

            return True

        self._header_animation_text()

        iterator = folder_iterator.FolderIterator()
        iterator.run(self.full_path, it_file_function, it_folder_function)

        self._save_animation()

        self.animations_text = self.animations_text + self.flipped_animations_text

        self._footer_animation_text()

        if self.animations > 0:

            output_folder = self.output_folder

            output_file_name = '{0}.animations'.format(self.atlas_name)
            output_full_file_name = '{0}/{1}'.format(output_folder, output_file_name)
            output_file = open(output_full_file_name, 'wt')
            output_file.write(self.animations_text)
            output_file.close()

            return True
        return False

    def _save_animation(self):

        if self.animation == '':
            return

        self.animations_text = self.animations_text + r'''
    <Animation id="{0}">'''.format(self.animation)

        frame_number = 0

        self.sprites.sort()

        for frame in self.sprites:

            frame_duration = 1

            if len(self.sprites_duration) > frame_number:
                frame_duration = self.sprites_duration[frame_number]

            self.animations_text = self.animations_text + r'''
        <Phase image="{0}" time="{1}" />'''.format(frame, float(frame_duration))

            frame_number = frame_number + 1

        self.animations_text = self.animations_text + r'''
    </Animation>'''

        self.animations = self.animations + 1
        self.sprites = []
        self.sprites_duration = []
        self.animation = ''

    def _parse_frames_info(self, full_path):

        file_path = self.full_path + '/' + full_path

        if not os.path.exists(file_path):
            return

        self.sprites_duration = []

        input_file = open(file_path)
        content = input_file.read()
        input_file.close()

        for entry in content.split('\n'):
            if is_number(entry):
                self.sprites_duration.append(entry)

    def _parse_animations_flip(self, full_path):

        file_path = self.full_path + '/' + full_path

        if not os.path.exists(file_path):
            return

        input_file = open(file_path)
        content = input_file.read()
        input_file.close()

        for entry in content.split('\n'):
            animations_flip_data = entry.split(' ')

            flip_x = (animations_flip_data[2].find('x') != -1)
            flip_y = (animations_flip_data[2].find('y') != -1)

            if flip_x:
                flip_x_text = ' flipX="1"'
            else:
                flip_x_text = ''

            if flip_y:
                flip_y_text = ' flipY="1"'
            else:
                flip_y_text = ''

            self.flipped_animations_text = self.flipped_animations_text + r'''
    <Animation id="{0}" animation="{1}"{2}{3}>'''.format(
                animations_flip_data[1].replace('.png', ''),
                animations_flip_data[0],
                flip_x_text,
                flip_y_text)

    def _header_animation_text(self):
        self.animations_text = r'''<?xml version="1.0" encoding="utf-8"?>
<Animations>'''

    def _footer_animation_text(self):
        self.animations_text = self.animations_text + r'''
</Animations>'''
