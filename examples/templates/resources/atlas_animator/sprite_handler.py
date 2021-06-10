import os

import folder_iterator


class SpriteHandler:

    def __init__(self):
        self.full_path = ''
        self.base_name = ''
        self.sprite_name = ''
        self.sprites_text = ''

    def run_with_sprite(self, base_name, full_path):

        print('RUN WITH SPRITE ' + base_name + ' ' + full_path)

        self.full_path = full_path
        self.base_name = base_name
        self.sprite_name = self.base_name.replace('.sprite', '')

        def it_file_function(full_path, file_name):

            if file_name == '--frames-flip.txt':
                self._parse_frames_flip(full_path)
                return True

            if file_name == '--animations-flip.txt':
                self._parse_animations_flip(full_path)
                return True

            if file_name.find('.png') != -1:
                statics_name = file_name.replace('.png', '')

                self.sprites_text = self.sprites_text + r'''
        <Statics frame="{0}"/>'''.format(statics_name)

                return True

        def it_folder_function(full_path, folder_name):

            if folder_name.find('.animation') != -1:
                animation_name = folder_name.replace('.animation', '')

                self.sprites_text = self.sprites_text + r'''
        <Animation animation="{0}/>"'''.format(animation_name)

                return False

            return True

        self.sprites_text = self.sprites_text + r'''
    <Sprite id="{0}">'''.format(self.sprite_name)

        iterator = folder_iterator.FolderIterator()
        iterator.run(self.full_path, it_file_function, it_folder_function)

        self.sprites_text = self.sprites_text + r'''
    </Sprite>'''

    def _parse_frames_flip(self, full_path):
        file_path = self.full_path + '/' + full_path

        if not os.path.exists(file_path):
            return

        input_file = open(file_path)
        content = input_file.read()
        input_file.close()

        for entry in content.split('\n'):
            frames_flip_data = entry.split('')
            statics_name = frames_flip_data[1].replace('.png', '')

            self.sprites_text = self.sprites_text + r'''
        <Statics frame="{0}"/>'''.format(statics_name)

    def _parse_animations_flip(self, full_path):

        file_path = self.full_path + '/' + full_path

        if not os.path.exists(file_path):
            return

        input_file = open(file_path)
        content = input_file.read()
        input_file.close()

        for entry in content.split('\n'):
            animations_flip_data = entry.split(' ')
            animation_name = animations_flip_data[1].replace('.png', '')

            self.sprites_text = self.sprites_text + r'''
        <Animation animation="{0}"/>'''.format(animation_name)

    def save_file(self, output_file_name, output_folder):

        self.sprites_text = r'''<?xml version="1.0" encoding="utf-8"?>
<Sprites>''' + self.sprites_text + r'''
</Sprites>'''

        output_full_file_name = '{0}/{1}'.format(output_folder, output_file_name)

        print('NOT IMPLEMENTED!')
