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
