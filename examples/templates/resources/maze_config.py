import fileinput
import sys

textures_extensions = ['.png', '.bmp', '.dxt', '.pvr', '.etc']
maze_texture_extension = '.mztexture'

sounds_extensions = ['.wav', '.ogg']
maze_sound_extension = '.mzsound'


def change_textures_extensions(full_path):
    print('Changing texture extensions in file: ' + full_path)
    file = fileinput.FileInput(full_path, inplace=True, backup='')
    for line in file:
        new_line = line
        for texture_ext in textures_extensions:
            new_line = new_line.replace(texture_ext, maze_texture_extension)
        sys.stdout.write(new_line)
    file.close()
