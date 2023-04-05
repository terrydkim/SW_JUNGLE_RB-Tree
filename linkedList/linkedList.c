#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} node;

void insert_node(node *head, const int index, const int data)
{
    int k = index;
    node *pre_node = head;
    node *insert = (node *)malloc(sizeof(node));
    insert->data = data;

    while (k-- && pre_node != NULL)
        pre_node = pre_node->next;

    if (pre_node == NULL)
    {
        free(insert);
        return;
    }
    insert->next = pre_node->next;
    pre_node->next = insert;
}

void delete_node(node *head, const int index)
{
    int k = index;
    node *temp = head;
    node *garbage = NULL;
    while (k-- && temp != NULL)
        temp = temp->next;

    if (temp == NULL || temp->next == NULL)
        return;

    garbage = temp->next;
    temp->next = garbage->next;
    free(garbage);
}

int search(node *head, const int value)
{
    if (head->next == NULL)
        return -1;

    int index = 0;
    node *temp = head->next;

    while (temp->data != value)
    {
        ++index;
        temp = temp->next;
        if (temp == NULL)
            return -1;
    }
    return index;
}

int get_data(node *head, const int index)
{
    if (head->next == NULL)
        return -1;
    int k = index;
    node *temp = head->next;

    while (k--)
        temp = temp->next;
    return temp->data;
}

void update_node(node *head, const int index, const int value)
{
    if (head->next == NULL)
        return ;
    int k = index;
    node *temp = head->next;

    while (k--)
        temp = temp->next;
    temp->data = value;
}

void printLL(node *head)
{
    if (head->next == NULL)
        return;

    node *temp = head->next;
    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main()
{
    node *head = (node *)malloc(sizeof(node));
    head->data = 0;
    head->next = NULL;

    insert_node(head, 0, 12); // 12
    printLL(head);

    insert_node(head, 0, 23); // 23 12
    printLL(head);

    insert_node(head, 2, 43); // 23 12 43
    printLL(head);

    insert_node(head, 1, 56); // 23 56 12 43
    printLL(head);

    printf("%d\n", get_data(head, 2)); // 12

    delete_node(head, 2); // 23 56 43
    printLL(head);

    delete_node(head, 0); // 56 43
    printLL(head);

    update_node(head, 0, 98);
    printLL(head);
}
