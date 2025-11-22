#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    float or_w1;
    float or_w2;
    float or_b;

    float nand_w1;
    float nand_w2;
    float nand_b;

    float and_w1;
    float and_w2;
    float and_b;
} Xor;

typedef float sample[3];

// XOR-GATE
sample xor_train[] = {
    {0, 0, 0},
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 0},
};

// NOR-GATE
sample nor_train[] = {
    {0, 0, 1},
    {0, 1, 0},
    {1, 0, 0},
    {1, 1, 0},
};

#define train_count 4
sample *train = xor_train;

float sigmoidf(float x) {
    return 1.f / (1.f + expf(-x));
}

float rand_float(void)
{
    // RAND_MAX is a constant which is the max integer which
    // can be returned by the rand() function.
    return (float) rand()/ (float) RAND_MAX;
}

float forward(Xor m, float x1, float x2) {
    float a = sigmoidf(m.or_w1 * x1 + m.or_w2 * x2 + m.or_b);
    float b = sigmoidf(m.nand_w1 * x1 + m.nand_w2 * x2 + m.nand_b);
    return sigmoidf(m.and_w1 * a + m.and_w2 * b + m.and_b);
}

float cost(Xor m) {
    float result = 0.0f;
    for (size_t i=0; i<train_count; i++) {
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = forward(m, x1, x2);
        float d = y - train[i][2];
        result += d*d;
    }
    result /= train_count;
    return result;
}

Xor rand_xor() {
    Xor m = {
        .or_w1 = rand_float(),
        .or_w2 = rand_float(),
        .or_b = rand_float(),

        .nand_w1 = rand_float(),
        .nand_w2 = rand_float(),
        .nand_b = rand_float(),

        .and_w1 = rand_float(),
        .and_w2 = rand_float(),
        .and_b = rand_float()
    };
    return m;
}

void print_xor(Xor m) {
    printf("or_w1 = %f\n", m.or_w1);
    printf("or_w2 = %f\n", m.or_w2);
    printf("or_b = %f\n", m.or_b);
    printf("nand_w1 = %f\n", m.nand_w1);
    printf("nand_w2 = %f\n", m.nand_w2);
    printf("nand_b = %f\n", m.nand_b);
    printf("and_w1 = %f\n", m.and_w1);
    printf("and_w2 = %f\n", m.and_w2);
    printf("and_b = %f\n", m.and_b);
}

Xor finite_difference(Xor m, float eps) {
    Xor delta;
    float saved;
    float c = cost(m);

    saved = m.or_w1;
    m.or_w1 += eps;
    delta.or_w1 = (cost(m) - c)/eps;
    m.or_w1 = saved;

    saved = m.or_w2;
    m.or_w2 += eps;
    delta.or_w2 = (cost(m) - c)/eps;
    m.or_w2 = saved;

    saved = m.or_b;
    m.or_b += eps;
    delta.or_b = (cost(m) - c)/eps;
    m.or_b = saved;

    saved = m.nand_w1;
    m.nand_w1 += eps;
    delta.nand_w1 = (cost(m) - c)/eps;
    m.nand_w1 = saved;

    saved = m.nand_w2;
    m.nand_w2 += eps;
    delta.nand_w2 = (cost(m) - c)/eps;
    m.nand_w2 = saved;

    saved = m.nand_b;
    m.nand_b += eps;
    delta.nand_b = (cost(m) - c)/eps;
    m.nand_b = saved;

    saved = m.and_w1;
    m.and_w1 += eps;
    delta.and_w1 = (cost(m) - c)/eps;
    m.and_w1 = saved;

    saved = m.and_w2;
    m.and_w2 += eps;
    delta.and_w2 = (cost(m) - c)/eps;
    m.and_w2 = saved;

    saved = m.and_b;
    m.and_b += eps;
    delta.and_b = (cost(m) - c)/eps;
    m.and_b = saved;

    return delta;
}

Xor learn(Xor m, Xor dXor, float rate) {
    m.or_w1 -= rate*dXor.or_w1;
    m.or_w2 -= rate*dXor.or_w2;
    m.or_b -= rate*dXor.or_b;

    m.nand_w1 -= rate*dXor.nand_w1;
    m.nand_w2 -= rate*dXor.nand_w2;
    m.nand_b -= rate*dXor.nand_b;

    m.and_w1 -= rate*dXor.and_w1;
    m.and_w2 -= rate*dXor.and_w2;
    m.and_b -= rate*dXor.and_b;
    return m;
}

int main(void) {
    srand(time(NULL));
    float rate = 1e-1;
    float eps = 1e-3;
    float c;

    Xor m = rand_xor();
    for (size_t i=0; i<100000; i++) {
        c = cost(m);
        //printf("cost: %f\n", c);
        Xor dXor = finite_difference(m, eps);
        m = learn(m, dXor, rate);
    }
    printf("cost: %f\n", c);
    //print_xor(m);

    for (size_t i = 0; i < 2; i++)
        for(size_t j=0; j<2; j++)
            printf("%zu ^ %zu = %f\n", i, j, forward(m, i, j));

   printf("------------------------------------------------\n");
   printf("\"OR\" Neuron\n");
   for (size_t i=0; i<2; i++) {
    for(size_t j=0; j<2; j++) {
        printf("%zu | %zu = %f\n", i, j, sigmoidf(i*m.or_w1 + j*m.or_w2 + m.or_b));
    }
   }
   printf("------------------------------------------------\n");
   printf("\"NAND\" Neuron\n");
   for (size_t i=0; i<2; i++) {
    for(size_t j=0; j<2; j++) {
        printf("%zu | %zu = %f\n", i, j, sigmoidf(i*m.nand_w1 + j*m.nand_w2 + m.nand_b));
    }
   }
   printf("------------------------------------------------\n");
   printf("\"AND\" Neuron\n");
   for (size_t i=0; i<2; i++) {
    for(size_t j=0; j<2; j++) {
        printf("%zu | %zu = %f\n", i, j, sigmoidf(i*m.and_w1 + j*m.and_w2 + m.and_b));
    }
   }
}
