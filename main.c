#include <stdio.h>
#include <stdlib.h>

struct matrix {
    int n;
    int m;
    int *arrays;
};

struct matrix *createMatrix(int n, int m);

int *access(struct matrix *matrix, int i, int j);

void editMatrix(struct matrix *matrix, int i, int j, int val);

int dotMultiplier(int size_n, int size_m, int aj, int bi, struct matrix *aMatrix, struct matrix *bMatrix);

struct matrix *matrixMultiplier(struct matrix *aMatrix, struct matrix *bMatrix);

struct matrix *fileToMatrix(char *fileName);

void fileWriter(struct matrix *matrix, char *newFileName);

void printMat(struct matrix *matrix);


int main() {
    printf("Hello, World!\n");

    //full test
    printf("\n=================\nfull test\n=================\n");

    struct matrix m23 = *createMatrix(2, 3);
    struct matrix m32 = *createMatrix(3, 2);
    struct matrix m22_2 = *createMatrix(2, 2);

    char *fa = "../data/matrix_a.txt";
    char *fb = "../data/matrix_b.txt";
    char *fRes = "../data/matrix_res.txt";


    struct matrix m42_a = *fileToMatrix(fa);
    struct matrix m24_b = *fileToMatrix(fb);

    for (int l = 0; l < 6; l += 1) {
        m32.arrays[l] = l + 1;
    }
    for (int m = 0; m < 6; m += 1) {
        m23.arrays[m] = m + 7;
    }

    for (int n = 0; n < 2; ++n) {
        for (int i = 0; i < 2; ++i) {
            editMatrix(&m22_2, n, i, n + i);
        }
    }

//    printMat(&m32);
//    printMat(&m23);
//    printMat(&m22_2);
    printMat(&m42_a);
    printMat(&m24_b);

    printf("\n");

    struct matrix m22 = *matrixMultiplier(&m42_a, &m24_b);

    printMat(&m22);

    fileWriter(&m22, fRes);


    return 0;
}

/**
     * Function to perform the dot multiplication of a given row and column of the two matrices.
     *
     * parm-> size_n: The n dimension of the matrix a
     * parm-> size_m: The m dimension of the matrix b
     * parm-> aj: The j'th row of the matrix a that is currently being multiplied
     * parm-> bi: The i'th column of the matrix b that is currently being multiplied
     * parm-> *aMatrix: A pointer to the matrix a to be multiplied
     * parm-> *bMatrix: A pointer to the matrix b to be multiplied
     *
     * return-> res: The int result of the dot multiplication of a given row and column.
     * */
int dotMultiplier(int size_n, int size_m, int aj, int bi, struct matrix *aMatrix, struct matrix *bMatrix) {

    int res = 0;

    if (size_n == size_m) {
        // A loop to track the matching elements in the selected row and column
        for (int i = 0; i < size_m; i += 1) {
            int a = *access(aMatrix, i, aj);        // The current element of the row from matrix a
            int b = *access(bMatrix, bi, i);        // The current element of the column from matrix b
            res += a * b;                           // The accumulation of products to be put into the resulting matrix.
        }
    } else {
        printf("Error! incompatible size\n");
        res = 0;
    }

    printf("%d\n", res);
    return res;
};

/**
     * A function to perform the matrix multiplication of the two given matrices.
     * /Note/   - The matrices must be placed in the order one wishes to perform the operation.
     *          - A compatibility check is done before the operation is performed to reduce the likelihood of error.
     *          - An incompatible operation will result in an exit(0) operation closing the program.
     *          - Compatible matrices are defied as two matrices whose n and m dimensions are equal (an == bm for a(n-by-m) & b(n-by-m))
     *          - The resulting matrix is defined as having dimension bn-by-am.
     *
     * parm-> *aMatrix: A pointer to the matrix a to be multiplied.
     * parm-> *bMatrix: A pointer to the matrix b to be multiplied.
     *
     * return-> &resMatrix: The address of the resulting matrix struct modeling the answer to the matrix multiplication operation.
     * */
struct matrix *matrixMultiplier(struct matrix *aMatrix, struct matrix *bMatrix) {

    static struct matrix resMatrix;

    int an = aMatrix->n;                        // The n value of matrix a
    int bm = bMatrix->m;                        // The m value of matrix b

    if (an == bm) {
        int n = aMatrix->m;                     // The n value of the new result matrix
        int m = bMatrix->n;                     // The m value of the new result matrix
        resMatrix = *createMatrix(n, m);        // Creating an empty matrix struct of the appropriate dimension.

        // for loop inserting the result of an appropriate dot multiplication in proper position in the
        //  result matrix
        for (int i = 0; i < n; i += 1) {
            for (int j = 0; j < m; j += 1) {
                editMatrix(&resMatrix, j, i, dotMultiplier(an, bm, i, j, aMatrix, bMatrix));
            }
        }
    } else {
        printf("incompatible matrices");
        exit(0);
    }

    return &resMatrix;
};

/**
     * A function to convert a .txt file based matrix to a struct matrix to be used within the program.
     * The function calculates the dimensions of the incoming matrix and creates a new matrix with the proper data.
     *
     * parm-> *fileName: A pointer to the char array containing the file path as a string.
     *
     * return-> *matrix: A new matrix struct containing the data from the file in the correct order.
     * */
struct matrix *fileToMatrix(char *fileName) {

    static struct matrix matrix;
    int n = 0;      // x: number of chars in a line
    int m = 1;      // y: number of '\n' characters

    FILE *matrixFile;

    matrixFile = fopen(fileName, "r");
    if (matrixFile == NULL) {
        printf("file %s not available", fileName);
        exit(0);
    } else {

        // get m dimension
        char mc;
        while (fscanf(matrixFile, "%c", &mc) == 1) {
            if (mc == '\n')
                m += 1;
        }

        fclose(matrixFile);
        matrixFile = fopen(fileName, "r");

        // get n dimension
        int ni;
        while (fscanf(matrixFile, "%d", &ni) == 1) {
            n += 1;
        }
        n /= m;

        fclose(matrixFile);
        matrixFile = fopen(fileName, "r");

        // create matrix struct and read content of file into allocated memory.
        matrix = *createMatrix(n, m);
        int val;
        int i = 0;
        while (fscanf(matrixFile, "%d", &val) == 1) {
            matrix.arrays[i] = val;
            i += 1;
        }

    }

    fclose(matrixFile);
    return &matrix;
}

void fileWriter(struct matrix *matrix, char *newFileName) {

    FILE *matrixFile;

    matrixFile = fopen(newFileName, "w");
    if (matrixFile == NULL) {
        printf("file %s not available", newFileName);
        exit(0);
    } else {
        for (int i = 0; i < matrix->m; i += 1) {
            for (int j = 0; j < matrix->n; j += 1) {
                int r = matrix->arrays[(i * matrix->n) + j];
                fprintf(matrixFile, "%d ", r);
            }
            fprintf(matrixFile, "\n");
        }
        fclose(matrixFile);

        printf("Write Successful: %s", newFileName);
    }
}

struct matrix *createMatrix(int n, int m) {
    struct matrix *matrix = NULL;

    int *arr = (int *) calloc(n * m, sizeof(int));

    struct matrix mat = {n, m, arr};
    matrix = &mat;
    return matrix;
};

int *access(struct matrix *matrix, int i, int j) {
    int *res = NULL;

    int index = (j * matrix->n) + i;
    res = &matrix->arrays[index];

    return res;
}

void editMatrix(struct matrix *matrix, int i, int j, int val) {

    int index = (j * matrix->n) + i;
    matrix->arrays[index] = val;
};

void printMat(struct matrix *matrix) {
    printf("\n");
    for (int i = 0; i < matrix->m; i += 1) {
        for (int j = 0; j < matrix->n; j += 1) {
            int r = matrix->arrays[(i * matrix->n) + j];
            printf("%d ", r);
        }
        printf("\n");
    }
}


