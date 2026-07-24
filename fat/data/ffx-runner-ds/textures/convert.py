import os

from PIL import Image

for f in os.listdir("."):
    if not f.lower().endswith(".png"): continue

    img = Image.open(f).convert("RGBA")

    if img.size[0] > 128:
        img = img.resize((128, 128))
    else:
        img = img.resize((img.size[0]/2, img.size[0]/2))

    img = img.convert("P", dither=None, colors=15)
    img.save(f)
