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

void *check_rows(void *arg) {
    for (int i = 0; i < N; i++) {
        bool seen[N+1] = {false};
        for (int j = 0; j < N; j++) {
            int num = grid[i][j];
            if (num < 1 || num > 9 || seen[num]) { valid = false; return NULL; }
            seen[num] = true;
        }
    }
    return NULL;
}

void *check_cols(void *arg) {
    for (int j = 0; j < N; j++) {
        bool seen[N+1] = {false};
        for (int i = 0; i < N; i++) {
            int num = grid[i][j];
            if (num < 1 || num > 9 || seen[num]) { valid = false; return NULL; }
            seen[num] = true;
        }
    }
    return NULL;
}

void *check_subgrid(void *arg) {
    int idx = *(int*)arg;
    int row_start = (idx / 3) * 3;
    int col_start = (idx % 3) * 3;
    bool seen[N+1] = {false};
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) {
        int num = grid[row_start + i][col_start + j];
        if (num < 1 || num > 9 || seen[num]) { valid = false; return NULL; }
        seen[num] = true;
      }
    return NULL;
}

int main() {
    pthread_t tid[11];
    int indices[9];

    pthread_create(&tid[0], NULL, check_rows, NULL);

    pthread_create(&tid[1], NULL, check_cols, NULL);

    for (int i = 0; i < 9; i++) {
        indices[i] = i;
        pthread_create(&tid[i+2], NULL, check_subgrid, &indices[i]);
    }

    for (int i = 0; i < 11; i++)
        pthread_join(tid[i], NULL);

    printf("Sudoku is %s\n", valid ? "valid" : "invalid");
    return 0;
}
