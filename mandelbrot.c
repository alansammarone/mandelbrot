#include <time.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

void mandelbrot_region(mpf_t start_x, mpf_t start_y, int increments_x, int increments_y, mpf_t increment_size, long max_iterations, char fname[]) {

 	int n_pixels = increments_x * increments_y;
 	long* iteration_count;
 	iteration_count = malloc(sizeof(long) * n_pixels);

 	long int k = 0;
 	mpf_t c_real;
 	mpf_init_set(c_real, start_x);

 	mpf_t c_imag;
 	mpf_init_set(c_imag, start_y);

 	mpf_t z_real;
 	mpf_t z_imag;
 	mpf_t new_z_real;
 	mpf_t new_z_imag;

 	mpf_t z_imag_sqrd;

 	mpf_init(z_real);
 	mpf_init(z_imag);
 	mpf_init(new_z_real);
 	mpf_init(new_z_imag);

 	mpf_init(z_imag_sqrd);

 	int escaped;

 	// iterate through pixels
	for (int j = 0; j < increments_y; j++) {
 		for (int i = 0; i < increments_x; i++) {

 			mpf_set(z_real, c_real);
			mpf_set(z_imag, c_imag);
			escaped = 0;


 			for (long n=0; n<max_iterations; n++){

 				// for each pixel c, iterate the mandelbrot sequence
 				// z -> z**2 + c

 				mpf_mul(new_z_real, z_real, z_real);
 				mpf_mul(z_imag_sqrd, z_imag, z_imag);

 				mpf_sub(new_z_real, new_z_real, z_imag_sqrd);
 				mpf_add(new_z_real, new_z_real, c_real);

 				mpf_mul(z_imag, z_real, z_imag);
 				mpf_mul_ui(z_imag, z_imag, 2);
 				mpf_add(z_imag, z_imag, c_imag);

 				mpf_set(z_real, new_z_real);

 				if (
 					mpf_cmp_d(z_real, 2.) >=0 ||
 					mpf_cmp_d(z_imag, 2.) >= 0||
 					mpf_cmp_d(z_real, -2.) <=0 ||
 					mpf_cmp_d(z_imag, -2.) <= 0
				) {
 					escaped = 1;
 					iteration_count[k] = n;
 					break;
 				}
 			}

 			if (escaped == 0) {
 				iteration_count[k] = max_iterations + 1;
 			}

 			// moves to the pixel to the right
 			mpf_add(c_real, c_real, increment_size);
 			k++;

 		};


 		// since we're iterating from top to bottom, y is decremented
 		mpf_sub(c_imag, c_imag, increment_size);
 		mpf_set(c_real, start_x);

 	};

 	FILE *f = fopen(fname, "wb");
 	fprintf(f, "%ld ", max_iterations);
 	fprintf(f, "%d ", increments_x);
 	fprintf(f, "%d ", increments_y);

 	for (int n=0; n<n_pixels; n++){
 		fprintf(f, "%ld ", iteration_count[n]);
 	}

	fclose(f);

	mpf_clear(c_real);
 	mpf_clear(c_imag);
 	mpf_clear(z_real);
 	mpf_clear(z_imag);
 	mpf_clear(new_z_real);
 	mpf_clear(new_z_imag);
 	mpf_clear(z_imag_sqrd);
 	free(iteration_count);
 }




int main(int argc, char **argv){

	int precision = atof(argv[7]);
	mpf_set_default_prec(precision);

	mpf_t zoom;
	mpf_t scale;

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	double x_center_input = atof(argv[3]);
	double y_center_input = atof(argv[4]);

	mpf_t ratio;
	mpf_init(ratio);
	mpf_set_ui(ratio, width);
	mpf_div_ui(ratio, ratio, height);


	mpf_t x_center;
	mpf_t y_center;
	mpf_init_set_str(x_center, argv[3], 10);
	mpf_init_set_str(y_center, argv[4], 10);

	mpf_init(zoom);

	mpf_set_ui(zoom, atol(argv[5]));

	long max_iterations = atof(argv[6]);

	mpf_init(scale);
	mpf_set_d(scale, 1.);
	mpf_div(scale, scale, zoom);
	printf("Width: %d\nHeight: %d\nReal: %.10f\nImag: %.10f\nZoom: ", width, height, x_center_input, y_center_input);

	mpf_out_str(stdout, 10, precision, zoom);
	printf("\nScale: ");
	mpf_out_str(stdout, 10, precision, scale);
	printf("\n");

	mpf_t x_start;
	mpf_init(x_start);

	mpf_t base_start_x;
	mpf_init(base_start_x);
	mpf_set_si(base_start_x, -2);

	mpf_div(x_start, base_start_x, zoom);
	mpf_add(x_start, x_start, x_center);

	mpf_t y_start;
	mpf_t base_y_start;
	mpf_init(base_y_start);
	mpf_set_si(base_y_start, 2);

	mpf_init(y_start);

	mpf_div(y_start, base_y_start, ratio);
	mpf_div(y_start, y_start, zoom);
	mpf_add(y_start, y_start, y_center);

	mpf_t increment_size;
	mpf_init(increment_size);
	mpf_div_ui(increment_size, scale, width);
	mpf_mul_ui(increment_size, increment_size, 4);
	printf("\nStart x: ");
	mpf_out_str(stdout, 10, precision, x_start);
	printf("\nStart y: ");
	mpf_out_str(stdout, 10, precision, y_start);
	printf("\nBlock size: ");
	mpf_out_str(stdout, 10, precision, increment_size);
	printf("\n");

	mandelbrot_region(x_start, y_start, width, height, increment_size, max_iterations, argv[8]);

	mpf_clear(scale);
	mpf_clear(x_start);
	mpf_clear(y_start);
	mpf_clear(increment_size);
	mpf_clear(x_center);
	mpf_clear(y_center);
}