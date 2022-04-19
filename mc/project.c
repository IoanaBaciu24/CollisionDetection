
#include <stdio.h>
#include "mc48.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "dataStructures.h"
#include "xoshiro256starstar.h"


// first attempt to okay generation of bitstrings of size n : https://stackoverflow.com/questions/9767321/how-to-generate-all-possible-combinations-n-bit-strings



//
//void find_col(uint8_t h[6], uint8_t m1[16], uint8_t m2[16])
//{
//
//    //1. declare a root for the hash tree
//    //2. for 2^24 +1 times at most generate random numbers
//    //3. use the compression function to obtain h
//    //4. insert the message in an array and the hash in the tree
//
//
//    node_t *root = (node_t*)malloc(sizeof(node_t));
//
//    root->zero = NULL;
//    root->one = NULL;
//    root->parent = NULL;
//    root->index = -1;
//
//    int n = (int) pow((double) 2,24) + 500;
//
//    message_t *arr = (message_t *)malloc(sizeof(message_t)*n);
//    uint64_t rnd[2];
//    uint8_t *message;
//    uint8_t *haux;
//    message = (uint8_t *) malloc(16);
//    haux = (uint8_t *) malloc(6);
//    int out;
//    printf("let's goo\n");
//    for(int i=0;i<n;i++)
//    {
//        node_t *current = root;
//        rnd[0] = xoshiro256starstar_random();
//        rnd[1] = xoshiro256starstar_random();
//        from64_to8(rnd, message);
//
//        for(int j=0;j<6;j++)
//            haux[j] = h[j];
//
//        tcz48_dm(message,  haux);
//        for(int j=0;j<16;j++)
//            arr[i].m[j] = message[j];
//
//        out = add_bits_to_tree(current, haux, 6, i);
//        if(out != -1)
//        {
//            for(int j=0;j<16;j++){
//                m1[j] = arr[out].m[j];
//                }
//
//            for(int j=0;j<16;j++){
//                m2[j] = message[j];
//                }
//            free(arr);
//            for(int i=0;i<6;i++)
//                printf("%hhu ", haux[i]);
//            printf("\n");
////            for(int i=0;i<16;i++)
////                printf("%hhu ", m2[i]);
//            printf("got here");
//	    //TODO would be nice to free the tree too
//            return;
//        }
//
//        if(i%100 == 0) printf("iteration %d \n", i);
//    }
//
//
//}



void test_add_to_tree()
{
    node_t *root = (node_t*)malloc(sizeof(node_t));
    root->zero = NULL;
    root->one = NULL;
    root->index = -1;

    uint8_t h[6] = {IVB0,IVB1,IVB2,IVB3,IVB4,IVB5};
    int out = add_bits_to_tree(root, h, 6, 1);
    int out2 = add_bits_to_tree(root, h, 6, 2);

    printf("%d %d\n", out, out2);
}



void find_col(uint8_t h[6], uint8_t m1[16], uint8_t m2[16])
{

    node_t *root = (node_t*)malloc(sizeof(node_t));

    root->zero = NULL;
    root->one = NULL;
//    root->parent = NULL;
    root->index = -1;

    int n = (int) pow((double) 2,25);

    message_t *arr = (message_t *)malloc(sizeof(message_t)*n);
    uint64_t rnd[2];
    uint8_t message[16];
    uint8_t haux[6];
    int out, index = 0;
    while (1){
    for(int i=index;i<n;i++)
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

//        if(i%10000 == 0) printf("iteration %d \n", i);
    }
        // if we get here, the birthday paradox betrayed us and we didn't find a collision yet
        // double the array that keeps the messages
        index = n;
        n*=2;
        arr  = (message_t *) realloc(arr, n);


    }


}


// The function takes d at most 4

void attack(int d)
{
    uint8_t *h = (uint8_t *) malloc(6);
    h[0] = IVB0; h[1] = IVB1; h[2] = IVB2; h[3] = IVB3; h[4] = IVB4; h[5] = IVB5;
    uint8_t *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8;
    m1 = (uint8_t*) malloc(sizeof (uint8_t)*16);
    m2 = (uint8_t*) malloc(sizeof (uint8_t)*16);
    // if d=1: 2 messages, if d=2: 4 messages, if d = 3: 8 messages, if d = 4: 16 messages

    int numberOfMessages = pow((double )2, d);
    int lengthOfOneMessage = 16*d;

    uint8_t **listOfMessages = (uint8_t**) malloc(numberOfMessages* sizeof(uint8_t*));
    for(int i=0;i<numberOfMessages;i++)
    {
        listOfMessages[i] = (uint8_t*) malloc(sizeof(uint8_t)*lengthOfOneMessage);
    }
    int chunk, remaining = 0, flag = 0, index = 0;
    for(int i = 0;i<d;i++) {
        printf("Finding collision %d\n", i);
        chunk = numberOfMessages / pow((double) 2, (i + 1));
        printf("%d , %d, %d\n", numberOfMessages, lengthOfOneMessage, chunk);
        find_col(h, m1, m2);
        tcz48_dm(m1, h);
        remaining = 0;
        flag = 0;
        for(int a=0;a<16;a++)
            printf("%02X ", m1[a]);
        printf("\n");
        for(int a=0;a<16;a++)
            printf("%02X ", m2[a]);
        printf("\n");

        while (remaining < numberOfMessages) {
            if (flag == 0) {
                for (int j = 0; j < chunk; j++) {
                    for (int k = 0; k < 16; k++) { listOfMessages[remaining][index + k] = m1[k]; }
                    remaining++;
                    }
                }
            else {
                for (int j = 0; j < chunk; j++) {
                    for (int k = 0; k < 16; k++) { listOfMessages[remaining][index + k] = m2[k]; }
                    remaining++;
                    }
                 }
            flag = 1 - flag;

        }
        index += 16;

    }
    uint8_t *h2 = (uint8_t *) malloc(6);
    for(int i=0;i<numberOfMessages;i++)
    {
        h2[0] = IVB0; h2[1] = IVB1; h2[2] = IVB2; h2[3] = IVB3; h2[4] = IVB4; h2[5] = IVB5;
        for(int j=0;j<lengthOfOneMessage;j++)
            printf("%02X ", listOfMessages[i][j]);
        printf("\nHashes to: \n");
        ht48(listOfMessages[i], lengthOfOneMessage, h2);
        for(int j=0;j<6;j++)
            printf("%02X ", h2[j]);
        printf("\n");
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

//     node_t *root = (node_t*)malloc(sizeof(node_t));
//
//     root->zero = NULL;
//     root->one = NULL;
//     root->parent = NULL;
//     root->index = -1;
//
//     uint8_t val[5];
//     val[0] = 137;
//     node_t *root1 = root, *root2 = root;
    // printf("%p %p \n", root1, root2);
    // int out = add_bits_to_tree(root1, val, 1,1);
    // printf("%d\n", out);
    // printf("%p %p \n", root1, root2);
//
//     out = add_bits_to_tree(root2, val, 1,2);
//     printf("%d\n", out);
    // printf("%p %p \n", root1, root2);

//    uint8_t h[6] = {1,2,3,4,5,6};
//    uint8_t h[6] = {IVB0,IVB1,IVB2,IVB3,IVB4,IVB5};
////
//    uint8_t *m1, *m2;
//    m1 = (uint8_t*) malloc(sizeof (uint8_t)*16);
//    m2 = (uint8_t*) malloc(sizeof (uint8_t)*16);
////
//    find_col(h, m1, m2);
//    printf("done this\n");
//    for(int i=0;i<6;i++)
//        printf("%hhu ", h[i]);
//    printf("\n");
//    for(int i=0;i<16;i++)
//        printf("%hhu ", m1[i]);
//    printf("\n");
//    for(int i=0;i<16;i++)
//        printf("%hhu ", m2[i]);
//    printf("\n");
//
//    uint8_t *compr1, *compr2;
//    compr1 = (uint8_t*) malloc(sizeof (uint8_t)*6);
//    for(int i=0;i<6;i++)
//        compr1[i] = h[i];
//    compr2 = (uint8_t*) malloc(sizeof (uint8_t)*6);
//    for(int i=0;i<6;i++)
//        compr2[i] = h[i];
//    ht48(m1, 16, compr1);
//    ht48(m2, 16,  compr2);

    //uint8_t m1[16] = {179, 137, 231, 105, 39, 245, 28, 206, 236, 42, 49, 243, 73, 79, 42, 224};
    //uint8_t m2[16] = {186, 229, 247, 166, 221, 116, 102, 48, 104, 233, 23, 58, 158, 17, 178, 165};
//    uint8_t *compr1, *compr2;
//    compr1 = (uint8_t*) malloc(sizeof (uint8_t)*6);
//    for(int i=0;i<6;i++)
//        compr1[i] = h[i];
//    compr2 = (uint8_t*) malloc(sizeof (uint8_t)*6);
//    for(int i=0;i<6;i++)
//        compr2[i] = h[i];
//    tcz48_dm(m1, compr1);
//    tcz48_dm(m2,  compr2);
//
//
//
//    for(int i=0;i<6;i++)
//        printf("%hhu ", compr1[i]);
//
//    printf("\n");
//
//    for(int i=0;i<6;i++)
//        printf("%hhu ", compr2[i]);
//
//    printf("\n");
//    for(int i=0;i<6;i++)
//        compr1[i] = h[i];
//    for(int i=0;i<6;i++)
//        compr2[i] = h[i];
//    ht48(m1, 16, compr1);
//    ht48(m2, 16,  compr2);
//    for(int i=0;i<6;i++)
//        printf("%hhu ", compr1[i]);
//
//    printf("\n");
//
//    for(int i=0;i<6;i++)
//        printf("%hhu ", compr2[i]);


    attack(2);
//    test_add_to_tree();
//hash();
    return 0;
}
