import binascii
import os
import pickle


def get_file_hash(full_path):
    try:
        mtime = os.path.getmtime(full_path)
        crc32 = binascii.crc32(full_path.encode())
        return mtime + crc32
    except OSError:
        print('[ERROR!]: getFileHash exception! ' + full_path)
        return 0


class Cache:

    def __init__(self, file_name=None):

        if file_name is None:
            return

        self.db_file_name = file_name

        setup_cache = not os.path.exists(file_name)

        if setup_cache:
            self.db = {}
        else:
            self.db = pickle.load(open(self.db_file_name, 'rb'))

    def get_folder_hash(self, full_path):
        mtime = 0
        for root, dirs, files in os.walk(full_path):
            for name in files:
                mtime = mtime + get_file_hash(os.path.join(root, name))
            for name in dirs:
                mtime = mtime + self.get_folder_hash(os.path.join(root, name))

        return mtime

    def is_up_to_date(self, full_path):
        return self.check_cache(full_path, get_file_hash(full_path))

    def is_folder_up_to_date(self, full_path):
        return self.check_cache(full_path, self.get_folder_hash(full_path))

    def check_cache(self, full_path, hash_value):
        if full_path not in self.db or self.db[full_path] != hash_value:
            self.db[full_path] = hash_value
            return False

        return True

    def save_cache(self):
        pickle.dump(self.db, open(self.db_file_name, 'wb'))
