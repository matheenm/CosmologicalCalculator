#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX_ROWS_GREEN 871419 
#define GREEN_NAME "data.csv" 
#define OUT_NAME_DIST "data_x_y_z.csv"

const double Omega_m = 0.3;
const double Omega_L = 0.7;
const double H0 = 70;


// Function to calculate the comoving distance
double comoving_distance(double z, double Omega_m, double Omega_L, double H0) {
    double integral = 0.0;
    double dz = 0.00001;
    double z_prime;

    for (z_prime = 0.0; z_prime <= z; z_prime += dz) {
        double E = sqrt(Omega_m * pow(1 + z_prime, 3) + Omega_L);
        integral += dz / E;
    }

    return integral * (2.99792458e5 / H0); // Convert to Mpc
}

// Function to calculate the Cartesian coordinates
void cartesian_coordinates(double ra, double dec, double comoving, double *x, double *y, double *z) {
    double ra_rad = ra * M_PI / 180.0;
    double dec_rad = dec * M_PI / 180.0;

    *x = cos(ra_rad) * cos(dec_rad)*comoving;
    *y = sin(ra_rad) * cos(dec_rad)*comoving;
    *z = sin(dec_rad)*comoving;
}



typedef struct{
    double ra, dec, z,distance;
    int id;
} Point;


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return -1;
    }

    const char *INPUT_NAME = argv[1];
    const char *OUTPUT_NAME = argv[2];
    
    // Count lines in file (excluding header)
    FILE *file = fopen(INPUT_NAME, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    int MAX_ROWS = 0;
    char line[256];
    fgets(line, sizeof(line), file); // Skip header
    while (fgets(line, sizeof(line), file)) MAX_ROWS++;
    fclose(file);
    
    printf("read files %s\n", INPUT_NAME);
    Point *points = (Point *)malloc(MAX_ROWS * sizeof(Point));
    if (!points) {
        perror("Memory allocation failed");
        return -1;
    }

    // Read data points
    file = fopen(INPUT_NAME, "r");
    if (!file) {
        perror("Error opening file");
        free(points);
        return -1;
    }
    
    fgets(line, sizeof(line), file); // Skip header
    int num_rows = 0;
    double ra,dec,z;
    while (fgets(line, sizeof(line), file) && num_rows < MAX_ROWS) {
        double ra, dec, z;
        if (sscanf(line, "%lf,%lf,%lf", &ra, &dec, &z) == 3) {
            points[num_rows].ra = ra;
            points[num_rows].dec = dec;
            points[num_rows].z = z;
            num_rows++;
        }
    }
    fclose(file);
    printf("Finished reading %d rows from %s\n", num_rows, INPUT_NAME);


    FILE *output_file_distance = fopen(OUTPUT_NAME, "w");
    if (output_file_distance == NULL) {
        perror("Error opening output file");
        return 1;
    }

    fprintf(output_file_distance, "RA,DEC,Redshift,Comoving_dist,X,Y,Z\n");
  

    for (int i = 0; i < num_rows; i++) {
	    if(i%10000==0)printf("processing outer loop%d\n",i);
   		double d_C1 = comoving_distance(points[i].z, Omega_m, Omega_L, H0);
   		double x1,y1,z1;
        cartesian_coordinates(points[i].ra, points[i].dec,d_C1,&x1,&y1,&z1);
        fprintf(output_file_distance, "%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",points[i].ra, points[i].dec,points[i].z,d_C1,x1,y1,z1);
    }

    fclose(output_file_distance);
    free(points);
    return 0;
}
