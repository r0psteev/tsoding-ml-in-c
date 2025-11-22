#include <stdio.h>

int main() {
    for (size_t x=0; x<2; x++)
        for (size_t y=0; y<2; y++)
            printf("%zu ^ %zu = %zu\n", x, y, (x|y) & (~(x&y)));
}
