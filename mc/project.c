
#include <stdio.h>
#include "mc48.h"
#include <stdint.h>
#include <math.h>
#include "dataStructures.h"
#include "xoshiro256starstar.h"
// Function that receives h, chaining key on 48 bits and 
// finds a collision of messages m1 and m2

// first attempt to okay generation of bitstrings of size n : https://stackoverflow.com/questions/9767321/how-to-generate-all-possible-combinations-n-bit-strings

int addHash(node_t root, uint8_t h[6], int message_identifier)
{
    int identical = 1;
    // TODO compute here the insertion of a hash in the graph
}



void find_col(uint8_t h[6], uint8_t m1[16], uint8_t m2[16])
{
    node_t root1;

    root1.zero = NULL;
    root1.one = NULL;
    root1.sample = NULL;
    root1.parent = NULL;
    node_t root0;

    root0.zero = NULL;
    root0.one = NULL;
    root0.sample = NULL;
    root0.parent = NULL;

    int n = pow(2,24)+1;



}



int main()
{
    uint64_t rnd[2];
    rnd[0] = xoshiro256starstar_random();
    rnd[1] = xoshiro256starstar_random();

    u_int8_t result[16];
    from64_to8(rnd, &result);

    printf("%llx %llx \n", rnd[0], rnd[1]);
    for(int i=0;i<16;i++)
    {
        printf("%02X ", result[i]);
    }

    uint64_t another_result[2];
    from8_to64(result, &another_result);
    printf("\n%llx %llx \n", another_result[0], another_result[1]);


    node_t *root = (node_t*)malloc(sizeof(node_t*));
    root->zero = NULL;
    root->one = NULL;
    root->parent = NULL;
    root->index = -1;

    uint8_t val[5];
    val[0] = 137;
    node_t *root1 = root, *root2 = root;
    printf("%p %p \n", root1, root2);
    int out = add_bits_to_tree(root1, val, 1,1);
    printf("%d\n", out);
    printf("%p %p \n", root1, root2);

    out = add_bits_to_tree(root2, val, 1,2);
    printf("%d\n", out);
    printf("%p %p \n", root1, root2);

    return 0;
}