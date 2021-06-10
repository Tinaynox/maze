import argparse
import os
import shutil
import time

import folder_iterator
import resource_package_scaler

from utils import mkdir_p


class AssemblyResources:

    # ctor
    def __init__(self, args):
        self.input = os.path.abspath(args.src)
        self.output = os.path.abspath(args.dst)
        self.tscales = args.tscales
        self.folder_iterator = folder_iterator.FolderIterator()
        self.resource_package_scaler = resource_package_scaler.ResourcePackageScaler()

        print('Assembly Resources')
        print('Input folder: ' + self.input)
        print('Output folder: ' + self.output)

    def prepare(self):
        if not os.path.exists(self.output):
            print('Creating output directory...')
            mkdir_p(self.output)

    # run
    def run(self):
        self.prepare()

        def it_file_function(full_path, file_name):
            copy_from = '{0}/{1}'.format(self.input, full_path)
            copy_to = '{0}/{1}'.format(self.output, full_path)

            copy_required = not os.path.exists(copy_to)

            if (not copy_required) and (os.stat(copy_from).st_mtime - os.stat(copy_to).st_mtime > 1):
                copy_required = True

            if copy_required:

                dir_name = os.path.dirname(copy_to)

                if not os.path.exists(dir_name):
                    mkdir_p(dir_name)

                    print('Processing: ' + copy_from)
                    shutil.copy2(copy_from, copy_to)

        def it_folder_function(full_path, folder_name):
            name, ext = os.path.splitext(folder_name)

            if ext == '.mzap':
                parameters = {}

                info_file_path_folder = '{0}/{1}'.format(self.input, full_path)
                info_file_path = '{0}.meta'.format(info_file_path_folder)
                if os.path.exists(info_file_path):
                    info_input_file = open(info_file_path)
                    content = info_input_file.read()
                    info_input_file.close()

                    for entry in content.split('\n'):
                        s = entry.split('=')
                        if len(s) == 2:
                            parameters[s[0]] = s[1]

                for tscale in self.tscales:

                    self.resource_package_scaler.rescale(
                        info_file_path_folder,
                        '{0}/{1}'.format(self.output, folder_name), tscale)

                return False

            return True

        start_time = time.time()

        self.folder_iterator.run(self.input, it_file_function, it_folder_function)

        print('Time elapsed: {0} seconds'.format(time.time() - start_time))


# Main
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Assembly Resources data tool")
    parser.add_argument('--src', type=str, help='Source folder')
    parser.add_argument('--dst', type=str, help='Output folder')
    parser.add_argument('--tscales', nargs='+', help='Textures scales')
    parsedArgs = parser.parse_args()
    resourceAssembler = AssemblyResources(parsedArgs)
    resourceAssembler.run()
