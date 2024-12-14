#!/usr/bin/python3

import numpy, cv2
import os

try:
    os.mkdir("images")
except FileExistsError as e:
    os.rmdir("images")
    os.mkdir("images")

for i in range(0, 101 * 103):
    input_filepath = "output_" + str(i) + ".txt"
    output_filepath = "images/image_" + str(i) + ".png"
    with open(input_filepath) as fin:
        lines = fin.readlines()
        image_list = list(list(map(lambda c: 0 if c == '#' else 255, l.strip())) for l in lines)
        image_array = numpy.array(image_list)
        cv2.imwrite(output_filepath, image_array)

