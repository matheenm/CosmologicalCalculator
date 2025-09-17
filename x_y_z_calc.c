#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX_ROWS_GREEN 871419 // Adjust as needed
#define GREEN_NAME "data.csv" // Adjust as needed
#define OUT_NAME_DIST "data_x_y_z.csv"
#define NUM_CLOSEST 500
#define NUM_THREADS_PAR 8

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



int main() {
    printf("read files %s\n",GREEN_NAME);

    Point *points1;

    printf("pre malloc 1\n");
    // Allocate memory for the array of Person structs
    points1 = (Point *)malloc(MAX_ROWS_GREEN * sizeof(Point));
    printf("finished malloc 1\n");


    int num_rows1 = MAX_ROWS_GREEN ; // read_csv(GREEN_NAME, points1, MAX_ROWS_GREEN);

    FILE *file = fopen(GREEN_NAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int num_rows = 0;
    double ra,dec,z;
    char line[100];

    while (fgets(line, sizeof(line), file) != NULL && num_rows < MAX_ROWS_GREEN) {
        sscanf(line, "%lf,%lf,%lf",&ra,&dec,&z);
       	points1[num_rows].ra=ra;
	    points1[num_rows].dec=dec;
	    points1[num_rows].z=z;
        num_rows++;
    }
    fclose(file);
    printf("finished read files %s\n",GREEN_NAME);

    FILE *output_file_distance = fopen(OUT_NAME_DIST, "w");
    if (output_file_distance == NULL) {
        perror("Error opening output file");
        return 1;
    }

    fprintf(output_file_distance, "RA,DEC,Redshift,Comoving_dist,X,Y,Z\n");
  

    for (int i = 0; i < num_rows1; i++) {
	    if(i%10000==0)printf("processing outer loop%d\n",i);
   		double d_C1 = comoving_distance(points1[i].z, Omega_m, Omega_L, H0);
   		double x1,y1,z1;
        cartesian_coordinates(points1[i].ra, points1[i].dec,d_C1,&x1,&y1,&z1);
        fprintf(output_file_distance, "%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",points1[i].ra, points1[i].dec,points1[i].z,d_C1,x1,y1,z1);
    }

    fclose(output_file_distance);
    free(points1);
    return 0;
}
