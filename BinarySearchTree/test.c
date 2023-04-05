#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
    newNode->color = RBTREE_BLACK;
    p->root = p->nil = newNode;
    return p;
}

void delete_node(rbtree *t, node_t *delNode)
{
    if (delNode == t->nil)
        return;
    // 왼자식 nil 아니면 찾아가기
    if (t->nil != delNode->left)
        delete_node(t, delNode->left);
    // 오른자식 nil 아니면 찾아가기
    if (t->nil != delNode->right)
        delete_node(t, delNode->right);
    // 지우기
    free(delNode);
}

void delete_rbtree(rbtree *t)
{
    delete_node(t, t->root);
    // 트리 지우고, nil 노드도 지우기
    free(t->nil);
    free(t);
}

void rotate_right(rbtree *t, node_t *targetNode)
{
    node_t *y = targetNode->left;
    targetNode->left = y->right; // 오른쪽에 자식이 없으면 nil 값을 가짐
    if (y->right != t->nil)      // 오른쪽에 자식이 있으면 떼서 타겟노드로 옮김
        y->right->parent = targetNode;
    y->parent = targetNode->parent;
    if (targetNode->parent == t->nil) // 타겟노드가 루트인 경우
        t->root = y;
    else if (targetNode == targetNode->parent->right) // 오른트리에 있는 경우
        targetNode->parent->right = y;
    else // 왼 트리에 있는 경우
        targetNode->parent->left = y;
    y->right = targetNode;
    targetNode->parent = y;
}
void rotate_left(rbtree *t, node_t *targetNode)
{
    node_t *y = targetNode->right;
    targetNode->right = y->left;
    if (y->left != t->nil)
        y->left->parent = targetNode;
    y->parent = targetNode->parent; // 이거 if문 위로 옮겨도 상관 없나? // 굳이 옮기면 귀찮아질듯 차례대로 그냥 생각
    if (targetNode->parent == t->nil)
        t->root = y;
    else if (targetNode == targetNode->parent->right)
        targetNode->parent->right = y;
    else
        targetNode->parent->left = y;
    y->left = targetNode;
    targetNode->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *targetNode)
{
    while (targetNode->parent->color == RBTREE_RED)
    {
        if (targetNode->parent == targetNode->parent->parent->left) // 부모가 조상 왼 트리일 때
        {
            node_t *y = targetNode->parent->parent->right; // 삼촌
            if (y->color == RBTREE_RED)                    // 삼촌이 레드
            {                                              // 삼촌과 부모의 색과 조상의 색을 바꾸고 타겟노드를 조상으로 바꿈
                targetNode->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                targetNode->parent->parent->color = RBTREE_RED;
                targetNode = targetNode->parent->parent; // 조상노드 다시 확인하기 위한 절차
            }
            else
            {                                                // 부모는 빨강 삼촌은 검정
                if (targetNode == targetNode->parent->right) // 부모의 오른쪽 자식일 때 // 꺾여 있을 때
                {
                    targetNode = targetNode->parent; // 바까야 안 꺾였을때랑 if 밖 코드 같이 사용
                    rotate_left(t, targetNode);      // 꺾인거 풀기
                }
                targetNode->parent->color = RBTREE_BLACK; // 안 꺾여있을 때
                targetNode->parent->parent->color = RBTREE_RED;
                rotate_right(t, targetNode->parent->parent);
            }
        }
        else // 부모가 조상 오른 트리일 때 // 왼 오른만 다름
        {
            node_t *y = targetNode->parent->parent->left; // 삼촌
            if (y->color == RBTREE_RED)
            {
                targetNode->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                targetNode->parent->parent->color = RBTREE_RED;
                targetNode = targetNode->parent->parent; // 이거 왜함? // while 문으로 다시 들어가서 문제가 있나 없나 확인
            }
            else
            {                                               // 부모 레드 삼촌 블랙
                if (targetNode == targetNode->parent->left) // 꺾임
                {
                    targetNode = targetNode->parent;
                    rotate_right(t, targetNode);
                }
                targetNode->parent->color = RBTREE_BLACK;
                targetNode->parent->parent->color = RBTREE_RED;
                rotate_left(t, targetNode->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
    node_t *targetNode = (node_t *)calloc(1, sizeof(node_t));
    targetNode->key = key;
    node_t *parent_targetNode = t->nil;
    node_t *x = t->root;

    while (x != t->nil)
    {
        parent_targetNode = x;
        if (x->key > targetNode->key)
            x = x->left;
        else
            x = x->right;
    }
    targetNode->parent = parent_targetNode;
    // 검색해서 nil 자리에 자식으로 할당

    if (parent_targetNode == t->nil)
        t->root = targetNode;
    else if (targetNode->key < parent_targetNode->key)
        parent_targetNode->left = targetNode;
    else
        parent_targetNode->right = targetNode;

    targetNode->left = t->nil;
    targetNode->right = t->nil;
    targetNode->color = RBTREE_RED;
    rbtree_insert_fixup(t, targetNode);

    return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
    node_t *p = t->root;
    while (p != t->nil)
    {
        if (p->key == key)
            return p;
        else if (p->key > key)
            p = p->left;
        else
            p = p->right;
    }
    return NULL;
}
node_t *node_min(const rbtree *t, node_t *root)
{
    while (root->left != t->nil)
        root = root->left;
    return root;
}
node_t *rbtree_min(const rbtree *t)
{
    node_t *p = t->root;
    while (p->left != t->nil)
        p = p->left;
    return p;
}

node_t *rbtree_max(const rbtree *t)
{
    node_t *p = t->root;
    while (p->right != t->nil)
        p = p->right;
    return p;
}

void rbtree_erase_fixup(rbtree *t, node_t *target)
{
    while ((target != t->root) && (target->color == RBTREE_BLACK))
    {
        if (target == target->parent->left) // 왼족 트리일 때
        {
            node_t *bro = target->parent->right;
            if (bro->color == RBTREE_RED) // case 1 처리
            {
                bro->color = RBTREE_BLACK;
                target->parent->color = RBTREE_RED;
                rotate_left(t, target->parent);
                bro = target->parent->right;
            }
            if ((bro->left->color == RBTREE_BLACK) && (bro->right->color == RBTREE_BLACK)) // case 2 처리
            {
                bro->color = RBTREE_RED;
                target = target->parent;
            }
            else
            {
                if (bro->right->color == RBTREE_BLACK) // case 3 처리
                {
                    bro->left->color = RBTREE_BLACK;
                    bro->color = RBTREE_RED;
                    rotate_right(t, bro);
                    bro = target->parent->right;
                }
                bro->color = target->parent->color; // case 4 처리
                target->parent->color = RBTREE_BLACK;
                bro->right->color = RBTREE_BLACK;
                rotate_left(t, target->parent);
                target = t->root; // 나중에 검은색으로 처리하려고
            }
        }
        else // 오른쪽 트리일 때
        {
            node_t *bro = target->parent->left;
            if (bro->color == RBTREE_RED) // case 1
            {
                bro->color = RBTREE_BLACK;
                target->parent->color = RBTREE_RED;
                rotate_right(t, target->parent);
                bro = target->parent->left;
            }
            if ((bro->right->color == RBTREE_BLACK) && (bro->left->color == RBTREE_BLACK)) // case 2
            {
                bro->color = RBTREE_RED;
                target = target->parent;
            }
            else // bro 자녀가 하나라도 red
            {
                if (bro->left->color == RBTREE_BLACK) // case 3
                {
                    bro->color = RBTREE_RED;
                    bro->right->color = RBTREE_BLACK;
                    rotate_left(t, bro);
                    bro = target->parent->left;
                }
                // case 4
                bro->color = target->parent->color;
                target->parent->color = RBTREE_BLACK;
                bro->left->color = RBTREE_BLACK;
                rotate_right(t, target->parent);
                target = t->root;
            }
        }
    }
    target->color = RBTREE_BLACK;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{                            // u와 v 바꾸기
    if (u->parent == t->nil) // 바꾸려는게 루트
        t->root = v;
    else if (u == u->parent->left) // 왼쪽 트리
        u->parent->left = v;
    else // 오른 트리
        u->parent->right = v;
    v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p)
{
    node_t *y = p;
    color_t p_original_color;
    p_original_color = y->color;
    node_t *x;
    if (p->left == t->nil) // 왼 자식 없다면
    {
        x = p->right;
        rbtree_transplant(t, p, p->right); // 삭제하기 위해
    }
    else if (p->right == t->nil) // 오른 자식 없음
    {
        x = p->left;
        rbtree_transplant(t, p, p->left);
    }
    else // 자식이 둘 다 존재
    {
        node_t *y = node_min(t, p->right);
        p_original_color = y->color;
        x = y->right;
        if (y->parent == p) // 최소가 바로 붙어있는 경우
            x->parent = y;
        else
        {
            rbtree_transplant(t, y, y->right);
            y->right = p->right;
            y->right->parent = y;
        }
        rbtree_transplant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }
    if (p_original_color == RBTREE_BLACK)
        rbtree_erase_fixup(t, x); // y의 자리에 x가 대체하므로
    free(p);
    return 0;
}

void rbtree_inorder(const rbtree *t, node_t *root, int *index, key_t *arr, int n)
{
    if (root == t->nil || *index >= n)
        return;

    rbtree_inorder(t, root->left, index ,arr, n);
    arr[(*index)++] = root->key;
    rbtree_inorder(t, root->right, index, arr, n);
}

// void rbtree_inorder(const rbtree *t, node_t *root, int *index, key_t *arr, int n)
// {
//   if (root == t->nil)
//     return;

//   rbtree_inorder(t, root->left, index, arr, n);
//   if (*index < n)
//   {
//     arr[(*index)++] = root->key;
//   }
//   else
//     return;
//   rbtree_inorder(t, root->right, index, arr, n);
// }

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
    int index = 0;
    rbtree_inorder(t, t->root, &index, arr, n);

    return 0;
}
