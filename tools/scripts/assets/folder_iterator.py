import os


class FolderIterator:
    # ctor
    def __init__(self):
        self.input = None

    def for_each_file(self, path, callback, recursive=True, file_extensions=None):

        def file_callback(full_path, file_name):
            name, ext = os.path.splitext(file_name)
            if file_extensions and (ext not in file_extensions):
                return

            callback(full_path, file_name)

        def folder_callback(full_path, folder_name):
            return recursive

        self.run(path, file_callback, folder_callback)

    def run(self, path, cb_file=None, cb_folder=None):

        if not os.path.exists(path):
            return False

        path = os.path.abspath(path)
        self.input = path

        for root, dirs, files, in os.walk(path):
            if cb_folder is not None:
                folder_path = os.path.relpath(root, path)
                folder_name = os.path.basename(root)
                iterate_over_files = cb_folder(folder_path, folder_name)
                if not iterate_over_files:
                    dirs[:] = []
                    continue

            if cb_file is not None:
                for file_name in files:
                    full_path = os.path.join(root, file_name)
                    try:
                        rel_path = os.path.relpath(full_path, path)
                    except ValueError as e:
                        print('Error!')
                        print('root=' + root)
                        print('fileName=' + file_name)
                        print('fullPath=' + full_path)
                        print('path=' + path)
                        raise e

                    cb_file(rel_path, file_name)

        return True
