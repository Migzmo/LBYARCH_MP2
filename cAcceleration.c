#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// Function prototype for ASM.
extern int* assemblyComputeAccelerations(int Y, float *inputArray);

// Function that compute accelerations in C.
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

// Function that generates random floats.
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
        
        // Generate random input data for this test scenario (same set of data for C and asm to be fair).
        for (int i = 0; i < Y; i++) {
            input[i][0] = randomFloat(0.0f, 100.0f);    
            input[i][1] = randomFloat(input[i][0], 200.0f);
            input[i][2] = randomFloat(1.0f, 10.0f);     
        }
        
        // We take note of the C function execution time for 30 runs.
        double totalCTime = 0.0;
        int *cResult = NULL;
        
        for (int run = 0; run < numRuns; run++) {
            LARGE_INTEGER frequency, start, end;

            // We get how many ticks per second there is for the CPU.
            QueryPerformanceFrequency(&frequency);

            // Start time.
            QueryPerformanceCounter(&start);
            
            // We time only the C function.
            cResult = cComputeAccelerations(Y, input);
            
            // End time.
            QueryPerformanceCounter(&end);

            double timeInSeconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
            totalCTime += timeInSeconds;
            
            // Free the result if not the last run.
            if (run < numRuns - 1 && cResult) {
                free(cResult);
                cResult = NULL;
            }
        }
        
        if (!cResult) {
            free(input);
            return 1;
        }
        
        double avgCTime = totalCTime / numRuns;
        
        // We take note of the assembly function execution time for 30 runs.
        double totalAsmTime = 0.0;
        int *assemblyResult = NULL;

        for (int run = 0; run < numRuns; run++) {
            LARGE_INTEGER frequency, start, end;

            // We get how many ticks per second there is for the CPU.
            QueryPerformanceFrequency(&frequency);

            // Start time.
            QueryPerformanceCounter(&start);
            
            // We time only the assembly function.
            assemblyResult = assemblyComputeAccelerations(Y, (float*)input);
            
            // End time.
            QueryPerformanceCounter(&end);

            double timeInSeconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
            totalAsmTime += timeInSeconds;
        }
        
        double avgAsmTime = totalAsmTime / numRuns;
        
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
        printf("\nC Function Performance:\n");
        printf("  Total time (30 runs): %.9f seconds\n", totalCTime);
        printf("  Average time per run (in seconds): %.9f seconds\n", avgCTime);
        printf("  Average time per run (in milliseconds): %.6f milliseconds\n", avgCTime * 1000);
        
        printf("\nAssembly Function Performance:\n");
        printf("  Total time (30 runs): %.9f seconds\n", totalAsmTime);
        printf("  Average time per run (in seconds): %.9f seconds\n", avgAsmTime);
        printf("  Average time per run (in milliseconds): %.6f milliseconds\n", avgAsmTime * 1000);
        
        printf("\nCorrectness check: %s\n", correct ? "PASS" : "FAIL");
        
        // Show sample outputs for only the first 10 rows.
        printf("\nSample Output (first 10 rows):\n");
        printf("ROW\tINIT_VEL\tFINAL_VEL\tTIME\tC OUTPUT\tASM OUTPUT\n");
        for (int i = 0; i < (Y < 10 ? Y : 10); i++) {
            printf("%d\t%.2f\t\t%.2f\t\t%.2f\t%d\t\t%d\n", i + 1, input[i][0], input[i][1], input[i][2], cResult[i], assemblyResult[i]);
        }
        printf("\n");
        
        free(input);
        free(cResult);
        
        printf("----------------------------------------\n\n");
    }
    
    printf("=== ALL TESTS COMPLETED ===\n");
    return 0;
}