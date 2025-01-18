#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// allocate 2D array
double** allocate2D(int row, int col)
{
    double** arr = (double**)malloc(row * sizeof(double*));
    for (int i = 0; i < row; i++)
    {
        arr[i] = (double*)malloc(col * sizeof(double));
    }
    return arr;
}

// de-allocate 2D array
void deallocate2D(double** arr, int row)
{
    for (int i = 0; i < row; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

// function print to file
void print_to_file(int n, double** A, int format_flag)
{
    FILE* out;
    char fileName[100];

    if (format_flag == 1)
    {
        // format file name
        sprintf(fileName, "array_%06d_bin.out", n);

        // open the file
        out = fopen(fileName, "wb");
        if (!out)
        {
            printf("Error: Could not open file %s\n", fileName);
            exit(EXIT_FAILURE);
        }
        
        // write into it
        for (int i = 0; i < n; i++)
        {
            fwrite(A[i], sizeof(double), n, out);
        }
    }
    else
    {
        // format file name
        sprintf(fileName, "array_%06d_asc.out", n);

        // open the file
        out = fopen(fileName, "w");
        if (!out)
        {
            printf("Error: Could not open file %s\n", fileName);
            exit(EXIT_FAILURE);
        }

        // write into it
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                fprintf(out, "%.15e ", A[i][j]);
            }
            fprintf(out, "\n");
        }
    }

    fclose(out);
}

int main()
{
    // declare variables
    int n;
    double** A;

    // read input from file
    FILE* inp = fopen("input_q1.in", "r");
    if (!inp)
    {
        printf("Error opening input file.\n");
        return 1;
    }

    // read and close the file
    fscanf(inp, "%d", &n);
    fclose(inp);

    // allocate memory for the matrix A
    A = allocate2D(n, n);

    // fill the 2D array
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = i + j;
        }
    }

    // write into binary file
    print_to_file(n, A, 1);

    printf("Writing into binary file completed. Kindly check the current directory to access the file.\n");

    // write into ASCII file
    print_to_file(n, A, 0);

    printf("Writing into ASCII file completed. Kindly check the current directory to access the file.\n");

    // free memories
    deallocate2D(A, n);

    return 0;
}

/*
    # part (a) 

    file size of asc format : 351 MB
    file size of bin format : 122 MB

    # part (b)

    size of the array in memory : 8 (size of double) * 4000 * 4000 = 128 MB (approx)
    size in disk for asc format : 351 MB
    size in disk for bin format : 122 MB

    o  Bin format takes less storage in disk that asc format because it's a direct dump of memory       
       representation. The asc format takes more memory due to the text representation of numbers,
       decimal places, line endings, etc.

    o  The binary format is better suited for large data because:

       - It's more space-efficient 
       - Faster to read and write
       - Maintains full numerical precision
       - No risk of floating-point representation errors
       - Can be easily compressed for further storage and transmission
*/
