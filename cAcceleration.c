#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern int* assemaccel(int n, float *inputArray);
int computeAcceleration(float initialVelocity, float finalVelocity, float time) {
    return (int) roundf((finalVelocity - initialVelocity) * 1000 / 3600 / time);
}

int main(void) {
    // Get input.
    int Y;
    scanf("%d", &Y);

    float (*input)[3] = malloc(Y * sizeof *input);

    if (input == NULL) {
        perror("malloc failed");
        return 1;
    }
    
    for (int i = 0; i < Y; i++) {
        scanf("%f, %f, %f", &input[i][0], &input[i][1], &input[i][2]);
    }

    // Compute accelerations.
    float *resultArray;
    resultArray = malloc(Y * sizeof(float));
    if (resultArray == NULL) {
        perror("malloc failed");
        free(input);
        return 1;
    }

    for (int i = 0; i < Y; i++) {
        resultArray[i] = computeAcceleration(input[i][0], input[i][1], input[i][2]);
    } 
       int *assemblyResult = assemaccel(Y,(float*)input);
    printf("ROW\tC OUTPUT\tASMB OUTPUT\n");  // table header
    for (int i = 0; i < Y; i++) {
        printf("%d\t%.2f\t\t%d\n", i, resultArray[i], assemblyResult[i]);
    }

    free(input);
    free(resultArray);
    return 0;   
}


