#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


// Creating a 2d array
int** create2dArray(int rows, int columns)
{
    printf("Allocating memory for %i rows and %i columns\n", rows, columns);
    int** aa;
    aa = malloc(rows * sizeof(*aa));
    for (int i = 0; i < rows; i++)
    {
        aa[i] = malloc(columns*sizeof(aa[0]));
    }
    return aa;
}


// Freeing the memory used for the allocation of the array
int free2dArray(int** arr, int rows)
{
      for (int i = 0; i < rows; i++)
    {
        free(arr[i]);
    }
    free(arr);
    printf("memory is free now\n");
    return 0;
}


void fill2dArrayWithRandomIntegers(int** arr, int rows, int columns)
{
    srand(time(NULL));

    for (int i = 0; i < rows;i++){
        for (int j = 0; j < columns; j++){

            // We can use larger numbers if we want to. Just change 100 into a bigger or smaller number
            arr[i][j] = rand() % 100;
        }
    }
}


void print2dArray(int** arr, int rows, int columns)
{

    printf("Printing array of %i rows and %i columns\n", rows, columns);

    for (int i = 0; i < rows;i++){

        for (int j = 0; j < columns; j++){

            printf("%i,", arr[i][j]);
        }

        printf("\n");
    }
}


// Finds the sum for each row
void sumAll(int** arr, int* total, int rows, int columns)
{

    for (int i = 0; i < rows; i++){

        total[i] = 0;

        for (int j = 0; j < columns; j++){

            total[i] += arr[i][j];

        }
    }
}


int main(int argc, char *argv[])
{
    int rows, columns, i, j;
    int grandTotal = 0;

    rows = 10;
    columns = 4;

    if(argc == 1){

        printf("Enter array dimensions (Rows, collumns)\n");
        scanf("%d %d", &rows, &columns);

//Passing in n and m as arguments
    } else if (argc == 2){

        rows = strtol(argv[1], NULL, 10);


    } else if (argc == 3){

        rows = strtol(argv[1], NULL, 10);
        columns = strtol(argv[2], NULL, 10);


    }

// Creating the 2d array with dynamic memory allocation
    int** arr = create2dArray(rows, columns);
    // int* totals = malloc(rows*sizeof(int));

    fill2dArrayWithRandomIntegers(arr, rows, columns);

    // ftok to generate unique key
    key_t key = ftok("shmfile",65);
    int shm = shmget(key, rows * sizeof (int), IPC_CREAT | 0666);

    if (shm < 0)
    {
        perror("shmget");
        return 1;
    }
     // Attach the segment as an int array
    int *row = shmat(shm, NULL, 0);
    if (row < (int *) NULL)
    {
        perror("shmat");
        return 1;
    }

    for (i = 0; i < rows; ++i)
        // Create as many children processes as the rows are
        if (!fork())
        {
            for (j = 0; j < columns; ++j)
                row[i] += arr[i][j];
            return 0;
        }

    for(i = 0; i < rows; ++i)
        wait(&j);


    print2dArray(arr, rows, columns);

    //sumAll(arr, totals, rows, columns);

    // Printing of row sums and general sum of all sums
    for (int k = 0; k < rows; k++){
        grandTotal += row[k];
        printf("row %i total is: %i\n", k, row[k]);
    }

    printf("Total row sum : %i\n", grandTotal);

    // Detach the shared memory segment and delete its key for later reuse
    shmdt(row);
    shmctl(shm, IPC_RMID, NULL);
    // Freeing memory
    free2dArray(arr, rows);
    // free(totals);
    return 0;
}
