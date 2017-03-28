import os
import datetime
import warnings
import math
import subprocess

from multiprocessing.pool import ThreadPool

class MandelbrotData:

	bin_path = './mandelbrot'

	def __init__(self, width, height, x_center, y_center, out_folder=None):

		self.width = width
		self.height = height
		self.x_center = x_center
		self.y_center = y_center

		if out_folder is None:
			out_folder = self.build_out_folder()

		self.out_folder = os.path.join(out_folder, 'data')

		if os.path.isdir(self.out_folder):
			warnings.warn("Folder %s already exists. Files will be overwritten.")
		else:
			os.makedirs(self.out_folder)

	def build_out_folder(self):
		return str(abs(hash((self.width, self.height, self.x_center, self.y_center))))[:5]

	def get_precision(self, zoom):
		return 64

	def get_max_iterations(self, zoom):
		return 80 + 35*int(zoom**.271)

	def get_file_name(self, zoom):
		return "%s.data" % int(math.log(zoom, 2))

	def get_command_for_zoom(self, zoom):

		precision = self.get_precision(zoom)
		max_iterations = self.get_max_iterations(zoom)
		file_name = self.get_file_name(zoom)

		command = [self.bin_path]
		command += [str(self.width), str(self.height)]
		command += ["%se0" % self.x_center, "%se0" % self.y_center]
		command += [str(zoom), str(max_iterations)]
		command += [str(precision), os.path.join(self.out_folder, file_name)]

		return command


	def generate_data(self, start_zoom_level, end_zoom_level, zoom_factor, n_workers=4):


		if n_workers is not None:
			tp = ThreadPool(n_workers)
			futures = []

		for zoom_level in range(start_zoom_level, end_zoom_level):
			zoom = zoom_factor ** zoom_level

			command = self.get_command_for_zoom(zoom)
			fn = lambda cmd: subprocess.check_output(cmd)
			if n_workers is not None:
				future = tp.apply_async(fn, [command])
				futures.append(future)
			else:
				fn(command)
				print "Done processing %s-th level"

		if n_workers is not None:
			tp.close()

		for i, future in enumerate(futures):
			future.get()
			print "Done processing %s-th level" % i










x_center = "0.013438870532012129028364919004019686867528573314565492885548699"
y_center = "0.655614218769465062251320027664617466691295975864786403994151735"
A = MandelbrotData(1280*2, 720*2, x_center, y_center, "1")
A.generate_data(0, 10, 2, n_workers=3)









