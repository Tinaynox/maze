import argparse
import os
import shutil
import time

import folder_iterator
from utils import mkdir_p


class CopyResources:

    def __init__(self, args):
        self.input = args.src
        self.output = args.dst
        self.folder_iterator = folder_iterator.FolderIterator()

    def prepare(self):
        if not os.path.exists(self.output):
            print('Creating output directory...')
            mkdir_p(self.output)

    def run(self):

        self.prepare()

        print('Assembly Data...')
        print('Input: ' + self.input)
        print('Output: ' + self.output)

        start_time = time.time()

        def it_file_function(full_path, file_name):

            copy_from = '{0}/{1}'.format(self.input, full_path)
            copy_to = '{0}/{1}'.format(self.output, full_path)

            copy_required = not os.path.exists(copy_to)
            if (not copy_required) and (os.stat(copy_from).st_mtime != os.stat(copy_to).st_mtime):
                copy_required = True

            if copy_required:

                dir_name = os.path.dirname(copy_to)
                if not os.path.exists(dir_name):
                    mkdir_p(dir_name)

                print('Processing: ' + copy_from)
                shutil.copy2(copy_from, copy_to)

        def it_folder_function(full_path, folder_name):

            folder = '{0}/{1}'.format(self.output, full_path)
            if os.path.exists(folder):
                for file in os.listdir(folder):
                    src_file = '{0}/{1}/{2}'.format(self.input, full_path, file)
                    if not os.path.exists(src_file):
                        dst_file = '{0}/{1}/{2}'.format(self.output, full_path, file)
                        print('REMOVING: ' + dst_file + '...')
                        if os.path.isdir(dst_file):
                            shutil.rmtree(dst_file, ignore_errors=True)
                        else:
                            os.remove(dst_file)

            return True

        self.folder_iterator.run(self.input, it_file_function, it_folder_function)

        print('Time elapsed: {0} seconds'.format(time.time() - start_time))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Copy resources data tool")
    parser.add_argument('--src', type=str, help='Source folder')
    parser.add_argument('--dst', type=str, help='Output folder')
    args = parser.parse_args()
    main = CopyResources(args)
    main.run()
