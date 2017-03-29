import os
import PIL
import math
import PIL
from PIL import Image


class MandelbrotImage:


	def __init__(self, folder):
		self.folder = folder
		self.data_folder =  os.path.join(folder, 'data')
		self.image_folder = os.path.join(folder, 'image')

		if not os.path.isdir(self.image_folder):
			os.makedirs(self.image_folder)

	def list_data_files(self):

		fnames = [fname for fname in os.listdir(self.data_folder)]
		fnames = [fname for fname in fnames if fname.endswith('.data')]
		fnames.sort(key=lambda x: int(x.split(".")[0]))
		return fnames

	def data_file_to_data(self, filepath):

		with open(os.path.join(self.data_folder, filepath)) as file:
			data = file.read()

		data = data.split(" ")
		width, height, max_iterations, precision = data[:4]
		data = data[4:]
		return int(width), int(height), int(max_iterations), int(precision), data


	def data_to_pixel_data(self, data, coloring_scheme):

		pixel_data = []

		for i in xrange(0, len(data), 3):

			escape_time = data[i]
			z_real = data[i+1]
			z_imag = data[i+2]
			color = coloring_scheme(escape_time, z_real, z_imag, max_iter)
			pixel_data.append(color)

		return pixel_data

	def pixel_data_to_image(self, filename, pixel_data, width, height):
		image = Image.new('RGB', (width, height))
		image.putdata(pixel_data)
		image.save(os.path.join(self.image_folder, filename))

def coloring(escape_time, z_real, z_imag, max_iterations):

	escape_time = int(escape_time)
	z_real = float(z_real)
	z_imag = float(z_imag)
	max_iterations = int(max_iterations)
	if escape_time == max_iterations + 1:
		return (255, 255, 255)
	else:
		q = escape_time - math.log(math.log((z_real ** 2 + z_imag ** 2))/(2*math.log(2)))
		return (int(q*255./max_iterations), 0, 0)

f = "1"
A = MandelbrotImage("1")
for idx, file in enumerate(A.list_data_files()):

	width, height, max_iter, precision, data = A.data_file_to_data(file)
	pixel_data = A.data_to_pixel_data(data, coloring)
	A.pixel_data_to_image("%s.png" % idx, pixel_data, width, height)
	print "Done with file %s" % file









