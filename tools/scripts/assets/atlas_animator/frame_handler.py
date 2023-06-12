class FrameHandler:

    def __init__(self):
        self.frames_text = ''

    def add_frame(self, frame_name, image_name, flip_x = False, flip_y = False):

        if flip_x:
            flip_x_text = ' flipX="1"'
        else:
            flip_x_text = ''

        if flip_y:
            flip_y_text = ' flipY="1"'
        else:
            flip_y_text = ''

        self.frames_text = self.frames_text + r'''
    <Frame id="{0}" image="{1}"{2}{3}/>'''.format(frame_name, image_name, flip_x_text, flip_y_text)

    def save_file(self, output_file_name, output_folder):

        self.frames_text = r'''<?xml version="1.0" encoding="utf-8"?>
<Frames>''' + self.frames_text + r'''
</Frames>'''

        output_full_file_name = '{0}/{1}'.format(output_folder, output_file_name)

        print('NOT IMPLEMENTED!')