#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float train[][2] = {
    {0, 0},
    {1, 2},
    {2, 4},
    {3, 6},
    {4, 8},
};

#define train_count (sizeof(train) / sizeof(train[0]))

float rand_float(void)
{
    // RAND_MAX is a constant which is the max integer which
    // can be returned by the rand() function.
    return (float) rand()/ (float) RAND_MAX;
}

float cost(float w, float b) {
    float result = 0.0f;
    for (size_t i=0; i<train_count; i++) {
        float x = train[i][0];
        float y = x*w + b;
        float d = y - train[i][1];
        result += d*d;
    }
    result /= train_count;
    return result;
}

int main()
{
    //srand(time(NULL));
    srand(69);
    // y = x*w;
    float w = rand_float() * 10.0f; // by default 10.0 is treated as a double, 10.0f ensures that 10.0 is treated as float.
    float b = rand_float() * 5.0f;
    float eps = 1e-3;
    float rate = 1e-3;
    printf("result = %f\n", cost(w, b));
    for (size_t i=0; i<5000; i++) {
        float c = cost(w, b);
        float dw = (cost(w+eps, b) - c)/eps;
        float db = (cost(w, b+eps) - c)/eps;
        w -= rate*dw;
        b -= rate*db;
        printf("result = %f, w = %f, b = %f\n", cost(w, b), w, b);
    }
    printf("------------------------------\n");
    printf("w = %f\n", w);
    return 0;
}
