#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

extern int* assemblyComputeAccelerations(int Y, float *inputArray);

int* cComputeAccelerations(int Y, float (*input)[3]) {
    int *cResult = malloc(Y * sizeof(int));
    if (!cResult) {
        perror("malloc failed");
        return NULL;
    }

    for (int i = 0; i < Y; i++) {
        cResult[i] = (int) roundf((input[i][1] - input[i][0]) * 1000 / 3600 / input[i][2]);
    }

    return cResult;
}

float randomFloat(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

int main(void) {
    // To seed srand() with the current time.
    srand((unsigned int)time(NULL));

    int testSizes[] = {10, 100, 1000, 10000};
    int numTests = 4;
    int numRuns = 30;
    
    printf("=== PERFORMANCE TESTING ===\n\n");
    
    for (int t = 0; t < numTests; t++) {
        int Y = testSizes[t];
        printf("Testing with Y = %d\n", Y);
        
        // Dynamically allocate memory for input using Y.
        float (*input)[3] = malloc(Y * sizeof *input);
        if (!input) {
            perror("malloc failed");
            return 1;
        }
        
        // Generate random input data.
        for (int i = 0; i < Y; i++) {
            input[i][0] = randomFloat(0.0f, 100.0f);    
            input[i][1] = randomFloat(input[i][0], 200.0f);
            input[i][2] = randomFloat(1.0f, 10.0f);     
        }
        
        // Compute C results once for correctness check.
        int *cResult = cComputeAccelerations(Y, input);
        if (!cResult) {
            free(input);
            return 1;
        }
        
        // We take note of the assembly function execution time for 30 runs.
        double totalTime = 0.0;
        int *assemblyResult = NULL;

        for (int run = 0; run < numRuns; run++) {
            LARGE_INTEGER frequency, start, end;
            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&start);
            
            // We time only the assembly function.
            assemblyResult = assemblyComputeAccelerations(Y, (float*)input);
            
            QueryPerformanceCounter(&end);
            double timeInSeconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
            totalTime += timeInSeconds;
        }
        
        double avgTime = totalTime / numRuns;
        
        // We check correctness by comparing via the C output.
        int correct = 1;
        for (int i = 0; i < Y; i++) {
            if (cResult[i] != assemblyResult[i]) {
                correct = 0;
                printf("Mismatch at row %d: C=%d, ASM=%d\n", i, cResult[i], assemblyResult[i]);
                break;
            }
        }
        
        // Display results.
        printf("Average execution time (30 runs): %.9f seconds\n", avgTime);
        printf("Average execution time: %.6f milliseconds\n", avgTime * 1000);
        printf("Correctness check: %s\n", correct ? "PASS" : "FAIL");
        
        // Show sample outputs for only the first 10 rows.
        printf("\nSample Output (first 10 rows):\n");
        printf("ROW\tINIT_VEL\tFINAL_VEL\tTIME\tC OUTPUT\tASM OUTPUT\n");
        for (int i = 0; i < (Y < 10 ? Y : 10); i++) {
            printf("%d\t%.2f\t\t%.2f\t\t%.2f\t%d\t\t%d\n", i + 1, input[i][0], input[i][1], input[i][2], cResult[i], assemblyResult[i]);
        }
        printf("\n");
        
        free(input);
        free(cResult);
    }
    
    printf("=== ALL TESTS COMPLETED ===\n");
    return 0;
}