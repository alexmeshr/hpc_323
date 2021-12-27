#include <inttypes.h>
#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

typedef struct matrix
{
    char type;
    int32_t size;
    int32_t* data32;
    int64_t* data64;
} matrix;

void fill_matrix(matrix* matr)
{
    srand(time(NULL));
    for (int i = 0; i < matr->size; i++)
        for (int j = 0; j < matr->size; j++)
            if (matr->type == 'd')
                matr->data32[i * matr->size + j] = rand();
            else
                matr->data64[i * matr->size + j] = (((int64_t)rand()) << 32) + rand();
}

matrix* create_matrix(char type, int n)
{
    matrix* new_matrix = (matrix*)malloc(sizeof(matrix));
    new_matrix->type = type;
    new_matrix->size = n;
    if (type == 'd')
        new_matrix->data32 = (int32_t*)malloc((int64_t)n * n * sizeof(int32_t));
    else if (type == 'l')
        new_matrix->data64 = (int64_t*)malloc((int64_t)n * n * sizeof(int64_t));
    return new_matrix;
}

void write_matrix(matrix* matr, char* file)
{
    FILE* output = fopen(file, "wb");
    fwrite(&(matr->type), sizeof(char), 1, output);
    fwrite(&(matr->size), sizeof(int32_t), 1, output);
    if (matr->type == 'd')
        fwrite(matr->data32, sizeof(int32_t), matr->size * matr->size, output);
    else
        fwrite(matr->data64, sizeof(int64_t), matr->size * matr->size, output);
    fclose(output);
}

void matrix_zero(matrix* matr)
{
    int size = matr->size;
    if (matr->type == 'd')
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                matr->data32[i * matr->size + j] = 0;
    else
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                matr->data64[i * matr->size + j] = 0;
}

matrix* read_matrix(char* file)
{
    FILE* input = fopen(file, "rb");
    char type;
    int size;
    fread(&type, sizeof(char), 1, input);
    fread(&size, sizeof(int32_t), 1, input);
    matrix* new_matrix = create_matrix(type, size);
    if (new_matrix->type == 'd')
        fread(new_matrix->data32, sizeof(int32_t),
              new_matrix->size * new_matrix->size, input);
    else
        fread(new_matrix->data64, sizeof(int64_t),
              new_matrix->size * new_matrix->size, input);
    fclose(input);
    return new_matrix;
}

void matrix_add(matrix* matr, int idx, int64_t val)
{
    if (matr->type == 'd')
        matr->data32[idx] += (int32_t)val;
    else
        matr->data64[idx] += val;
}

int64_t matrix_get(matrix* matr, int idx)
{
    if (matr->type == 'd')
        return (int64_t)matr->data32[idx];
    else
        return matr->data64[idx];
}

void mult(matrix* a, matrix* b, matrix* c, int style)
{
    int size = a->size;
    matrix_zero(c);
    switch (style)
    {
    case 0:  // ijk
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
            {
                for (int k = 0; k < size; k++)
                    matrix_add(c, i * size + j,  matrix_get(a, i * size + k)
                               * matrix_get(b, j * size + k));
            }
        printf("%c - ijk\n", a->type);
        break;
    case 1:  // ikj
        for (int i = 0; i < size; i++)
            for (int k = 0; k < size; k++)
            {
                for (int j = 0; j < size; j++)
                    matrix_add(c, i * size + j, matrix_get(a, i * size + k)
                               * matrix_get(b, j * size + k));
            }
        printf("%c - ikj\n", a->type);
        break;
    case 2:  // kij
        for (int k = 0; k < size; k++)
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                {
                    matrix_add(
                        c, i * size + j,
                        matrix_get(a, i * size + k) * matrix_get(b, j * size + k));
                }
        printf("%c - kij\n", a->type);
        break;
    case 3:  // jik
        for (int j = 0; j < size; j++)
            for (int i = 0; i < size; i++)
            {
                for (int k = 0; k < size; k++)
                    matrix_add(c, i * size + j, matrix_get(a, i * size + k)
                               * matrix_get(b, j * size + k));
            }
        printf("%c - jik\n", a->type);
        break;
    case 4:  // jki
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++)
            {
                for (int i = 0; i < size; i++)
                    matrix_add(c, i * size + j, matrix_get(a, i * size + k)
                               * matrix_get(b, j * size + k));
            }
        printf("%c - jki\n", a->type);
        break;
    case 5:  // kji
        for (int k = 0; k < size; k++)
            for (int j = 0; j < size; j++)
                for (int i = 0; i < size; i++)
                {
                    matrix_add(
                        c, i * size + j,
                        matrix_get(a, i * size + k) * matrix_get(b, j * size + k));
                }
        printf("%c - kji\n", a->type);
        break;
    }
    printf("size: %d\n", size);
}

void handle_error(int retval)
{
    printf(" Error %d\n", retval);
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argv[1][0] == '+')
    {
        matrix* new_matrix = create_matrix(argv[3][0], atoi(argv[4]));
        fill_matrix(new_matrix);
        write_matrix(new_matrix, argv[2]);
        free(new_matrix);
    }
    else
    {
        int EventSet = PAPI_NULL;

        if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
            handle_error(1);

        long_long start_virt, end_virt, start_real, end_real, values[1];

        matrix *a = NULL, *b = NULL, *c = NULL;
        a = read_matrix(argv[1]);
        b = read_matrix(argv[2]);
        c = read_matrix(argv[3]);

        start_real = PAPI_get_real_usec();
        start_virt = PAPI_get_virt_usec();

        if (PAPI_create_eventset(&EventSet) != PAPI_OK)
            handle_error(1);
        if (PAPI_add_event(EventSet, PAPI_L1_DCM) != PAPI_OK)
            handle_error(2);
        if (PAPI_start(EventSet) != PAPI_OK)
            handle_error(3);

        mult(a, b, c, argv[4][0] - '0');

        end_real = PAPI_get_real_usec();
        end_virt = PAPI_get_virt_usec();

        if (PAPI_read(EventSet, values) != PAPI_OK)
            handle_error(4);
        if (PAPI_stop(EventSet, values) != PAPI_OK)
            handle_error(5);

        printf("Real clock time: %llf\n", (double)(end_real - start_real)/1000000);
        printf("Virtual clock time: %llf\n", (double)(end_virt - start_virt)/1000000);
        printf("L1 cashe misses: %lld\n", values[0]);

        free(a);
        free(b);
        free(c);
    }
    return 0;
}
