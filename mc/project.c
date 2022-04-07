
#include <stdio.h>
#include "mc48.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "dataStructures.h"
#include "xoshiro256starstar.h"
// Function that receives h, chaining key on 48 bits and 
// finds a collision of messages m1 and m2

// first attempt to okay generation of bitstrings of size n : https://stackoverflow.com/questions/9767321/how-to-generate-all-possible-combinations-n-bit-strings




void find_col(uint8_t h[6], uint8_t m1[16], uint8_t m2[16])
{

    //1. declare a root for the hash tree 
    //2. for 2^24 +1 times at most generate random numbers 
    //3. use the compression function to obtain h
    //4. insert the message in an array and the hash in the tree


    node_t *root = (node_t*)malloc(sizeof(node_t));

    root->zero = NULL;
    root->one = NULL;
    root->parent = NULL;
    root->index = -1;

    int n = (int) pow((double) 2,24) + 1;

    message_t *arr = (message_t *)malloc(sizeof(message_t)*n);
    uint64_t rnd[2];
    uint8_t message[16];
    uint8_t haux[6];
    int out; 
    for(int i=0;i<n;i++)
    {
        rnd[0] = xoshiro256starstar_random();
        rnd[1] = xoshiro256starstar_random();
        from64_to8(rnd, &message);

        for(int j=0;j<6;j++)
            haux[j] = h[j];

        tcz48_dm(message,  &haux);
        for(int j=0;j<16;j++)
            arr[i].m[j] = message[j];

        out = add_bits_to_tree(root, haux, 6, i);
        if(out != -1)
        {
            for(int j=0;j<16;j++){
                m1[j] = arr[out].m[j];
                }
            
            for(int j=0;j<16;j++){
                m2[j] = message[j];
                }            
            free(arr);
            //TODO would be nice to free the tree too 
            return;
        }

        if(i%100 == 0) printf("iteration %d \n", i);
    }


}



int main()
{
    // uint64_t rnd[2];
    // rnd[0] = xoshiro256starstar_random();
    // rnd[1] = xoshiro256starstar_random();

    // u_int8_t result[16];
    // from64_to8(rnd, &result);

    // printf("%llx %llx \n", rnd[0], rnd[1]);
    // for(int i=0;i<16;i++)
    // {
    //     printf("%02X ", result[i]);
    // }

    // uint64_t another_result[2];
    // from8_to64(result, &another_result);
    // printf("\n%llx %llx \n", another_result[0], another_result[1]);

    // node_t *root = (node_t*)malloc(sizeof(node_t));

    // root->zero = NULL;
    // root->one = NULL;
    // root->parent = NULL;
    // root->index = -1;

    // uint8_t val[5];
    // val[0] = 137;
    // node_t *root1 = root, *root2 = root;
    // printf("%p %p \n", root1, root2);
    // int out = add_bits_to_tree(root1, val, 1,1);
    // printf("%d\n", out);
    // printf("%p %p \n", root1, root2);

    // out = add_bits_to_tree(root2, val, 1,2);
    // printf("%d\n", out);
    // printf("%p %p \n", root1, root2);

    uint8_t h[6] = {1,2,3,4,5,6};
    uint8_t m1[16], m2[16];
    find_col(h, &m1, &m2);
    printf("done this\n");
    for(int i=0;i<16;i++)
        printf("%s ", m1[i]);
    printf("\n");
    for(int i=0;i<16;i++)
        printf("%s ", m2[i]);

    uint8_t compr1[6], compr2[6];

    tcz48_dm(m1,  &compr1);
    tcz48_dm(m2,  &compr2);

    for(int i=0;i<6;i++)
        printf("%s ", compr1[i]);
    

    for(int i=0;i<6;i++)
        printf("%s ", compr2[i]);
    return 0;
}