#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} node;

void insert_node(node *head, const int index, const int value)
{
    int k = index;
    node *pre_node = head;
    node *insert = (node *)malloc(sizeof(node));
    insert->data = value;

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
    node *garbage = NULL;
    node *pre_node = head;

    while (k-- && pre_node != NULL)
        pre_node = pre_node->next;
    if (pre_node == NULL || pre_node->next == NULL) // 리스트 범위를 넘어 갈 경우 and 찾는 인덱스 값이 없으면 리턴
        return;
    garbage = pre_node->next;
    pre_node->next = garbage->next;
    free(garbage);
}
// 찾는 값을 가진 인덱스
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
// 찾는 인덱스의 값
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

void display(node *head)
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

    insert_node(head, 0, 2); // 2
    display(head);
    insert_node(head, 1, 7); // 2 7
    display(head);
    insert_node(head, 2, 6); // 2 7 6
    display(head);
    insert_node(head, 3, 1); // 2 7 6 1
    display(head);
    insert_node(head, 2, 3); // 2 7 3 6 1
    display(head);
}