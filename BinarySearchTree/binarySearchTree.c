#include <stdio.h>
#include <stdlib.h>

typedef struct BST
{
    int value;
    struct BST *left;
    struct BST *right;
} tree;

tree *insert_node(tree *t, int data)
{
    if (t == NULL)
    {
        t = (tree *)malloc(sizeof(tree));
        t->value = data;
        t->left = NULL;
        t->right = NULL;
    }
    else if (t->value > data)
        t->left = insert_node(t->left, data);

    else if (t->value < data)

        t->right = insert_node(t->right, data);

    return t; // 지금 노드의 주소값을 리턴
}

tree *get_node(tree *t, int target)
{
    if (t == NULL)
        return NULL;

    if (t->value == target)
        return t;

    else if (t->value > target)
        return get_node(t->left, target);

    else if (t->value < target)
        return get_node(t->right, target);
}

tree *get_min(tree *t)
{
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    else
        return get_min(t->left);
}
tree *get_max(tree *t)
{
    if (t == NULL)
        return NULL;
    if (t->right == NULL)
        return t;
    else
        return get_max(t->right);
}

tree *delete_node(tree *t, int target){
    tree *temp =NULL;
    if (t == NULL)
        return NULL;
    if (t->value == target){
        // 자식 없음
        if (t->left == NULL && t->right == NULL){
            free(t);
            return NULL;
        }
        // 자식 하나 인데 왼쪽
        else if (t->left == NULL){
            temp = t->right;
            free(t);
            return temp;
        }
        // 자식 하나인데 오른쪽
        else if (t->right == NULL){
            temp = t->left;
            free(t);
            return temp;
        }
        // 자식 둘다 존재
        else {
            temp = get_min(t->right);
            t->value = temp->value;
            delete_node(t->right,temp->value);
            return temp;
        }
    }
    else if (t->value < target){
        t->right = delete_node(t->right,target);
    }
    else {
        t->left = delete_node(t->left,target);
    }
    return t;
}
void display(tree *t)
{
    if (t != NULL)
    {
        display(t->left);
        printf("%d ", t->value);
        display(t->right);
    }
}

int main()
{
    tree *root = NULL;
    tree *findNode = NULL;
    int data;

    root = insert_node(root, 5);
    root = insert_node(root, 3);
    root = insert_node(root, 8);
    root = insert_node(root, 12);
    root = insert_node(root, 15);
    root = insert_node(root, 20);
    root = insert_node(root, 1);
    display(root);
    printf("\n");

    findNode = get_node(root, 8);
    printf("%d\n", findNode->value);

    findNode = get_max(root);
    printf("%d\n", findNode->value);

    findNode = get_min(root);
    printf("%d\n", findNode->value);

    root = delete_node(root, 20);
    display(root);
    printf("\n");
    root = insert_node(root, 19);
    display(root);
    return 0;
}