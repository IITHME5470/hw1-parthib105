#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define eps 1e-8

// function to allocate 1D array
double* allocate1D(int size)
{
    double* arr = (double*)malloc(size * sizeof(double));
    if (arr == NULL) 
    {
        printf("Memory allocation failed for the 1D array.\n");
        exit(1);
    }
    return arr;
}

// function to allocate 2D array
double** allocate2D(int row, int col)
{
    double** arr = (double**)malloc(row * sizeof(double*));
    if (arr == NULL) 
    {
        printf("Memory allocation failed for the 2D array.\n");
        exit(1);
    }

    for (int i = 0; i < row; i++) 
    {
        arr[i] = (double*)malloc(col * sizeof(double));
        if (arr[i] == NULL) 
        {
            printf("Memory allocation failed for row %d.\n", i);
            for (int j = 0; j < i; j++)
            {
                free(arr[j]);
            }
            free(arr);
            exit(1);
        }
    }
    return arr;
}

// function to free memory for 2D array
void free2D(double** arr, int row)
{
    for (int i = 0; i < row; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

// function to read matrix from file
void read_matrix(int row, int col, char* fileName, double** mat)
{
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("Error opening file %s.\n", fileName);
        exit(1);
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (fscanf(fp, "%lf,", &mat[i][j]) != 1)
            {
                printf("Error reading file %s at row %d, column %d.\n", fileName, i, j);
                fclose(fp);
                exit(1);
            }
        }
    }

    fclose(fp);
}

// function to read vector file
int read_vector(int size, char* fileName, double* vec)
{
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return 0;
    }

    for (int i = 0; i < size; i++)
    {
        if (fscanf(fp, "%lf,", &vec[i]) != 1)
        {
            printf("Error reading file %s at row %d.\n", fileName, i);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}

// function for matrix vector multiplication
void mat_vec_multiply(int row, int col, double* x, double* y, double** A)
{
    for (int i = 0; i < row; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < col; j++)
        {
            sum += A[i][j] * x[j];
        }
        y[i] = sum;
    }
}

// function to calculate the dot product
double dot_product(int size, double* a, double* b)
{
    double sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        sum += a[i] * b[i];
    }
    return sum;
}

// function to check if a vector is an eigen vector
int isEigenVector(int row, int col, int vecSize, double* eigVal, double* x, double** A)
{
    // check size compatibility
    if (col != vecSize) return 0;

    // get the matrix vector multiplication
    double* y = allocate1D(row);
    mat_vec_multiply(row, col, x, y, A);

    // calculate lambda
    double lambda = dot_product(vecSize, y, x) / dot_product(vecSize, x, x);

    // check if x is a eigen vector
    for (int i = 0; i < vecSize; i++)
    {
        if (fabs(y[i] - lambda * x[i]) > eps)
        {
            free(y);
            return 0;
        }
    }
    
    *eigVal = lambda;
    free(y);
    return 1;
}

int main()
{
    // declare the variables
    int n, vecnum;
    double** A, *x, eigVal;
    char matFile[100], vecFile[100];
    FILE* inp;

    // Open the input file
    inp = fopen("input_q2.in", "r");
    if (inp == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    fscanf(inp, "%d", &n);
    fclose(inp);

    // allocate memory for arrays
    A = allocate2D(n, n);
    x = allocate1D(n);

    // Read matrix from file
    sprintf(matFile, "inputfiles/mat_%06d.in", n);
    read_matrix(n, n, matFile, A);

    // perform the task for each given vector
    for (vecnum = 1;; vecnum++) 
    {
        // Read the vector from file
        sprintf(vecFile, "inputfiles/vec_%06d_%06d.in", n, vecnum);
        int rv = read_vector(n, vecFile, x);
        if (!rv)
        {
            break;
        }

        // check eigen vector
        int check_eigen = isEigenVector(n, n, n, &eigVal, x, A);
        if (check_eigen)
        {
            printf("vec_%06d_%06d.in : Yes : %8e\n", n, vecnum, eigVal);
        }
        else
        {
            printf("vec_%06d_%06d.in : Not an eigenvector\n", n, vecnum);
        }

        printf("\n");
    }

    // free memories
    free(x);
    free2D(A, n);
    return 0;
}