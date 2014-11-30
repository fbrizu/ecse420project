#include <stdio.h>
#include <stdlib.h>

int m = 50; //Number of rows in the image
int n = 100; //Number of columns in the image
int b; //Size of the block
int step_size_initial = 16;

//Previous image RGB
int **img_prev_r;
int **img_prev_g;
int **img_prev_b;

//Current image RGB
int **img_curr_r;
int **img_curr_g;
int **img_curr_b;

void store_images();
void load_images();
void process_images();
int compute_sum_rgb();

/*
BLOCKS:
0 | 1 | 2
3 | 4 | 5
6 | 7 | 8
*/

int main(int argc, char *argv[]) {
	srand(100);
	if(argc != 2) {
		printf("Usage: ./pp <Block size>\n");
		exit(1);
	}

	b = atoi(argv[1]);

	store_images();

	//load_images();

	process_images();
}

void store_images() {
	int i, j;

	printf("Storing images in matrices...\n");

	//Allocates memory to the matrices
	img_prev_r = (int **)malloc(m * sizeof(int*));
	img_prev_g = (int **)malloc(m * sizeof(int*));
	img_prev_b = (int **)malloc(m * sizeof(int*));

	img_curr_r = (int **)malloc(m * sizeof(int*));
	img_curr_g = (int **)malloc(m * sizeof(int*));
	img_curr_b = (int **)malloc(m * sizeof(int*));

	for(i = 0; i < m; i++) {
		img_prev_r[i] = (int *)malloc(n * sizeof(int));
		img_prev_g[i] = (int *)malloc(n * sizeof(int));
		img_prev_b[i] = (int *)malloc(n * sizeof(int));

		img_curr_r[i] = (int *)malloc(n * sizeof(int));
		img_curr_g[i] = (int *)malloc(n * sizeof(int));
		img_curr_b[i] = (int *)malloc(n * sizeof(int));
	}

	//Initializes matrices to 0
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++) {
			img_prev_r[i][j] = 0;
			img_prev_g[i][j] = 0;
			img_prev_b[i][j] = 0;

			img_curr_r[i][j] = 0;
			img_curr_g[i][j] = 0;
			img_curr_b[i][j] = 0;
		}
	}

	int test = rand() % 50;
	//prev (33,63), curr (33,78)
	img_prev_r[test][test+30] = 100;
	img_prev_g[test][test+30] = 100;
	img_curr_r[test][test+45] = 100;
	img_curr_g[test][test+45] = 100;
	printf("TEST %d\n", test);
}

void load_images() {
	int i, j;

	printf("Loading images in matrices...\n");

	//Bitmap bitmap = new Bitmap(@"")

	//Allocates memory to the matrices
	img_prev_r = (int **)malloc(m * sizeof(int*));
	img_prev_g = (int **)malloc(m * sizeof(int*));
	img_prev_b = (int **)malloc(m * sizeof(int*));

	img_curr_r = (int **)malloc(m * sizeof(int*));
	img_curr_g = (int **)malloc(m * sizeof(int*));
	img_curr_b = (int **)malloc(m * sizeof(int*));

	for(i = 0; i < m; i++) {
		img_prev_r[i] = (int *)malloc(n * sizeof(int));
		img_prev_g[i] = (int *)malloc(n * sizeof(int));
		img_prev_b[i] = (int *)malloc(n * sizeof(int));

		img_curr_r[i] = (int *)malloc(n * sizeof(int));
		img_curr_g[i] = (int *)malloc(n * sizeof(int));
		img_curr_b[i] = (int *)malloc(n * sizeof(int));
	}

	//Initializes matrices to 0
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++) {
			img_prev_r[i][j] = 0;
			img_prev_g[i][j] = 0;
			img_prev_b[i][j] = 0;

			img_curr_r[i][j] = 0;
			img_curr_g[i][j] = 0;
			img_curr_b[i][j] = 0;
		}
	}

	int test = rand() % 50;
	//prev (33,63), curr (33,78)
	img_prev_r[test][test+30] = 100;
	img_prev_g[test][test+30] = 100;
	img_curr_r[test][test+45] = 100;
	img_curr_g[test][test+45] = 100;
	printf("TEST %d\n", test);
}

void process_images() {
	int i, j, k, l;

	printf("Computing the three step block matching algorithm...\n");

	//Property 1. Sum of RGB
	for(k = 0; k < m / b; k++) { // rows
		for(l = 0; l < n / b; l++) { // columns
			int step_size = step_size_initial;
			int curr_k, curr_l; //Temporary k and l variables to keep track of position

			curr_k = k * b;
			curr_l = l * b;

			printf("|");
			
			//If the previous image's block was empty
			if(compute_sum_rgb(curr_k, curr_l, 0) == -1) {
				printf("OoB");
				continue;
			}
			
			//If the previous image's block was empty
			if(compute_sum_rgb(curr_k, curr_l, 0) == 0) {
				printf("(0,0)");
				continue;
			}
			//If the block is the same (no movement detected)
			if(compute_sum_rgb(curr_k, curr_l, 0) - compute_sum_rgb(curr_k, curr_l, 1) == 0) {
				printf("(0,0)");
				continue;
			}

			int old_value = compute_sum_rgb(curr_k, curr_l, 0);
			while(step_size >= 1) {
				//Compare all 8 blocks around it
				int min_difference = -1;
				int block = 4;
				int new_value = compute_sum_rgb(curr_k, curr_l, 1);

				if(old_value - new_value == 0) {
					printf("(%d,%d)", curr_k - (k * b), curr_l - (l * b));
					break;
				}
				
				min_difference = (old_value - new_value)  * (old_value - new_value);

				for(i = 0; i < 3; i++) {
					for(j = 0; j < 3; j++) {
						new_value = compute_sum_rgb(curr_k + (i - 1) * step_size, curr_l + (j - 1) * step_size, 1);
						//printf("C%d %d %d\n", curr_k + (i - 1) * step_size, curr_l + (j - 1) * step_size, new_value);
						if(new_value == -1) {
							continue;
						}
						else {
							//A better match has been found
							if(min_difference == -1 || ((old_value - new_value) * (old_value - new_value) >= 0 && (old_value - new_value) * (old_value - new_value) < min_difference)) {
								min_difference = (old_value - new_value) * (old_value - new_value);
								block = i * 3 + j;
							}
						}
					}
				}
				if(block != 4) {
					//printf("BLOCK %d\n", block);
					curr_k += ((block / 3) - 1) * step_size;
					curr_l += ((block % 3) - 1) * step_size;
				}
				if(step_size == 1) {
					//If only black squares have been found
					//TODO: Maybe add a threshold in the future
					if(min_difference == old_value * old_value) {
						printf("(0,0)");
					}
					//Otherwise compute the vector
					else {
						printf("(%d,%d)", curr_k - (k * b), curr_l - (l * b));
					}
				}
				step_size /= 2;
			}
		}
		printf("|\n");
	}
}

//Input curr: 0 to calculate first image, 1 for second image
int compute_sum_rgb(int k, int l, int curr) {
	int i, j;
	int total_RGB = 0;

	//Block out of bound
	if(k < 0 || l < 0 || k + b > m || l + b > n) {
		return -1;
	}

	//Loops through the block and adds up the RGB values
	for(i = k; i < k + b; i++) {
		for(j = l; j < l + b; j++) {
			if(curr == 0) {
				total_RGB += img_prev_r[i][j];
				total_RGB += img_prev_g[i][j];
				total_RGB += img_prev_b[i][j];
			}
			else if(curr == 1) {
				total_RGB += img_curr_r[i][j];
				total_RGB += img_curr_g[i][j];
				total_RGB += img_curr_b[i][j];
			}
			else {
				printf("Error reading block due to the input matrix\n");
				printf("Exiting...\n");
				exit(1);
			}
		}
	}
	//printf("Block %d %d %d: %d\n", k, l, curr, total_RGB);
	return total_RGB;
}