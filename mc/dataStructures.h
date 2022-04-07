#include <string.h>
#include <stdlib.h>

typedef struct Node {

    struct Node *zero;
    struct Node *one;
    struct Node *sample;
    struct Node *parent;
    int index;
} node_t;

typedef struct Message{

    uint8_t m[16];

} message_t;


union t_convert
{ uint64_t bigvar;
    uint8_t  charvar[8]; }
        convert;

void from64_to8(uint64_t input[2], uint8_t output[])
{

       union t_convert aux1, aux2 ;
       aux1.bigvar = input[0];
       aux2.bigvar = input[1];

    for (int i=0;i<8;i++)
    {
        output[i] = aux1.charvar[i];
        output[i+8] = aux2.charvar[i];

    }

}

void from8_to64(uint8_t input[16], uint64_t output[])
{

    union t_convert aux1, aux2 ;
    aux1.bigvar = input[0];
    aux2.bigvar = input[1];

    for(int i=0;i<8;i++)
    {
        aux1.charvar[i] = input[i];
        aux2.charvar[i] = input[i+8];

    }

    output[0] = aux1.bigvar;
    output[1] = aux2.bigvar;

}

int add_bits_to_tree(node_t *root, uint8_t value[], int size, int identifier)
{
    uint8_t nr = 137;
    int b;
    int new_entry = 0;
//    int count = 0;
    node_t *current = root;
    node_t *last = NULL;
    for(int j=0;j<size;j++){
        nr = value[j];
        for(int i=0;i<8;i++)
        {
            b = nr & 0b10000000;
            b = b>>7;
            nr = nr<<1;
            // printf("%d",b);
            if(b == 1)
            {
                if(root->one == NULL)
                {
                    node_t *new_node = (node_t *)malloc(sizeof (node_t));
                    new_node->one = NULL;
                    new_node->zero = NULL;
                    new_node->parent = root;
                    root->one = new_node;
                    root = root->one;
                    last = root;
                    new_entry = 1;
                }
                else
                {
                    root = root->one;
                    last = root;
                }
            }
            if(b == 0)
            {
                if(root->zero == NULL)
                {
                    node_t *new_node = (node_t *)malloc(sizeof (node_t));
                    new_node->one = NULL;
                    new_node->zero = NULL;
                    new_node->parent = root;
                    root->zero = new_node;
                    root = root->zero;
                    last = root;
                    new_entry = 1;


                }
                else
                {
                    root = root->zero;
                    last = root;

                }
            }
//            count++;
        }

    }
    // printf("\n");
    if(new_entry == 1)
    {
        last->index = identifier;
        return -1;
    }
    else
    {
        return last->index;
    }
}
