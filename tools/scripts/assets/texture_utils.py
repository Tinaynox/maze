from PIL import Image


def has_transparent_pixel(image_path):
    image = Image.open(image_path)
    image = image.convert("RGBA")

    for pixel in image.getdata():
        if pixel[3] < 255:
            image.close()
            return True
    image.close()
    return False


def is_power_of_two(n):
    return n > 0 and (n & (n - 1)) == 0


def is_pot_texture(image_path):
    image = Image.open(image_path)
    result = is_power_of_two(image.size[0]) and is_power_of_two(image.size[1])
    image.close()
    return result
