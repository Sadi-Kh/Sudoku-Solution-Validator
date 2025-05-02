#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define N 9

int grid[N][N] = {
    {5,3,4,6,7,8,9,1,2},
    {6,7,2,1,9,5,3,4,8},
    {1,9,8,3,4,2,5,6,7},
    {8,5,9,7,6,1,4,2,3},
    {4,2,6,8,5,3,7,9,1},
    {7,1,3,9,2,4,8,5,6},
    {9,6,1,5,3,7,2,8,4},
    {2,8,7,4,1,9,6,3,5},
    {3,4,5,2,8,6,1,7,9}
};

bool valid = true;

typedef struct { int type, index; } params;
// type = 0: row, 1: col, 2: subgrid

void *validator(void *arg) {
    params *p = (params*)arg;
    bool seen[N+1] = {false};

    if (p->type == 0) { // row
        int i = p->index;
        for (int j = 0; j < N; j++) {
            int num = grid[i][j];
            if (num < 1 || num > 9 || seen[num]) { valid = false; return NULL; }
            seen[num] = true;
        }
    }
    else if (p->type == 1) { // col
        int j = p->index;
        for (int i = 0; i < N; i++) {
            int num = grid[i][j];
            if (num < 1 || num > 9 || seen[num]) { valid = false; return NULL; }
            seen[num] = true;
        }
    }
    else { // subgrid
        int idx = p->index;
        int row_start = (idx / 3) * 3;
        int col_start = (idx % 3) * 3;
        for (int i = 0; i < 3; i++)
          for (int j = 0; j < 3; j++) {
            int num = grid[row_start + i][col_start + j];
            if (num < 1 || num > 9 || seen[num]) { valid = false; return NULL; }
            seen[num] = true;
          }
    }
    return NULL;
}

int main() {
    pthread_t tid[27];
    params args[27];
    int t = 0;

    for (int i = 0; i < N; i++) {
        args[t] = (params){0, i};
        pthread_create(&tid[t], NULL, validator, &args[t]);
        t++;
    }

    for (int j = 0; j < N; j++) {
        args[t] = (params){1, j};
        pthread_create(&tid[t], NULL, validator, &args[t]);
        t++;
    }

    for (int k = 0; k < 9; k++) {
        args[t] = (params){2, k};
        pthread_create(&tid[t], NULL, validator, &args[t]);
        t++;
    }

    for (int i = 0; i < t; i++)
        pthread_join(tid[i], NULL);

    printf("Sudoku is %s\n", valid ? "valid" : "invalid");
    return 0;
}
