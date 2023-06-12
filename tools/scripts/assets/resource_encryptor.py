class ResourceEncryptor:
    def encrypt(self, src_path, dst_path):
        src_data = bytearray(open(src_path, 'rb').read())
        key_array = bytearray([0xa5, 0xeb, 0x60, 0xd3, 0x4e, 0x72, 0x04, 0x64])
        for i in range(len(src_data)):
            src_data[i] ^= key_array[i % 8]

        open(dst_path, 'wb').write(src_data)
