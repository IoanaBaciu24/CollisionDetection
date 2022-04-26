
#include <stdio.h>
#include "mc48.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "dataStructures.h"
#include "xoshiro256starstar.h"
#include <time.h>






void find_col_old(uint8_t h[6], uint8_t m1[16], uint8_t m2[16])
{

    node_t *root = (node_t*)malloc(sizeof(node_t));

    root->zero = NULL;
    root->one = NULL;
    root->index = -1;

    int n = (long) pow((double) 2,25);

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
            return;
        }

    }
        // if we get here, the birthday paradox betrayed us and we didn't find a collision yet
        // double the array that keeps the messages
        printf("resizing\n");
        index = n;
        n*=2;
        arr  = (message_t *) realloc(arr, n);


    }


}

void find_message(llnode_t *head, int index, uint8_t m[16], uint8_t h[6], uint8_t h_comp[6])
{
    llnode_t *current = head;
    uint8_t haux[6];
    while (current!=NULL)
    {
        for(int j=0;j<6;j++)
            haux[j] = h[j];
        tcz48_dm(current->arr[index].m,  &haux);

        int idem = 1;
        for(int i=0;i<6;i++)
            if(haux[i]!=h_comp[i]) idem=0;

        if(idem == 1)
        {
            for(int j=0;j<16;j++){
                m[j] = current->arr[index].m[j];
            }
            return;
        }

        current = current->next;
    }
}


void find_col(uint8_t h[6], uint8_t m1[16], uint8_t m2[16])
{

    node_t *root = (node_t*)malloc(sizeof(node_t));

    root->zero = NULL;
    root->one = NULL;
    root->index = -1;

    int n = (long) pow((double) 2, 25);

    message_t *arr = (message_t *)malloc(sizeof(message_t)*n);
    llnode_t *head = (llnode_t *) malloc(sizeof(llnode_t));
    head->arr = arr;
    head->next = NULL;
    uint64_t rnd[2];
    uint8_t message[16];
    uint8_t haux[6], horig[6];
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
//                for(int j=0;j<16;j++){
//                    m1[j] = arr[out].m[j];
//                }

                for(int j=0;j<6;j++)
                    horig[j] = h[j];
                find_message(head, out, m1, horig, haux );

                for(int j=0;j<16;j++){
                    m2[j] = message[j];
                }
//                free(arr);
                while (head!=NULL)
                {
                    free(head->arr);
                    llnode_t *cur = head;
                    head = head->next;
                    free(cur);
                }
                return;
            }

        }
        // if we get here, the birthday paradox betrayed us and we didn't find a collision yet
        // add another node in the list, reset index, woohoo
        printf("resizing\n");
        index = 0;

        llnode_t *next = (llnode_t *) malloc(sizeof(llnode_t));
        arr = (message_t *)malloc(sizeof(message_t)*n);
        next->arr = arr;
        llnode_t *current = head;
        while (current->next!=NULL)
            current = current->next;

        current->next = next;


    }


}


void attack(int d)
{
    uint8_t *h = (uint8_t *) malloc(6);
    h[0] = IVB0; h[1] = IVB1; h[2] = IVB2; h[3] = IVB3; h[4] = IVB4; h[5] = IVB5;
    uint8_t *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8;
    m1 = (uint8_t*) malloc(sizeof (uint8_t)*16);
    m2 = (uint8_t*) malloc(sizeof (uint8_t)*16);

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
        clock_t begin = clock();
        find_col(h, m1, m2);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//        printf("%f\n", time_spent);
        tcz48_dm(m1, h);
        remaining = 0;
        flag = 0;
        for(int a=0;a<16;a++)
            printf("%02X ", m1[a]);
        printf("\n");
        for(int a=0;a<16;a++)
            printf("%02X ", m2[a]);
        printf("\n%f\n", time_spent);

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



    attack(1);
    attack(2);
    attack(3);
    attack(4);

    return 0;
}
