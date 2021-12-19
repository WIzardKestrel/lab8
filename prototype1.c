
#include <stdio.h>
#include <stdlib.h>

#define null NULL
#define red 1
#define black 0
typedef struct node {
    int data;
    int color;
    struct node* parent;
    struct node* right;
    struct node* left;
}node;
node* root = NULL;

node * find_grandparent(node * temp){
    return temp -> parent -> parent;
}
node * find_uncle(node * temp){
    node * grandparent = find_grandparent(temp);
    if(!grandparent)
        return grandparent;
    if(temp -> parent == grandparent -> left)
        return  grandparent -> right;
    else
        return grandparent -> left;
}

node * init_temp(int data){
    node * temp = (node *)malloc(sizeof(struct node));
    temp->right = NULL;
    temp->left = NULL;
    temp->parent = NULL;
    temp->data = data;
    temp->color = red;
}

node * insert(node * root, node * new_node){
    int data = new_node -> data;
    if(root == null){
        return new_node;
    }
    if (data > root -> data) {                                      // standard BST insertion.
        root->right = insert(root->right, new_node);
        root -> right -> parent = root;
    }
    else if (data < root -> data) {
        root -> left = insert(root -> left, new_node);
        root -> left -> parent = root;
    }
    return root;
}


void rotate_roight(struct node* crit)
{
    node * crit_parent = crit -> parent; // critical node's parent
    node * crit_left = crit -> left; // the soon-to-be critical node's parent ( left child of crit)
    crit -> left = crit_left -> right; // make the left child of the critical node the right child of
                                            // the critical node's left child
    if(crit_left != null && crit -> left != null) // if the right child of the left child's parent
        crit_left -> right -> parent = crit;            // is not null, assign its new parent as the crit node
    crit_left -> right = crit;
    crit -> parent = crit_left; // these two lines switch parents
    crit_left -> parent = crit_parent; // between the critical node and the left child of the critical node.

    if(crit_left -> parent == null){ // if the critical node was root then we assign the newly rotated
        root = crit_left;           // node to the root (global variable)
    }
    else if(crit_parent -> left == crit) // else if the crit node was a left child, we make crit_parent's
            crit_parent -> left = crit_left; // left child point to the new child
    else
            crit_parent -> right = crit_left; // otherwise we make it's right child point to it


}
void rotate_left(struct node* crit)
{
    node * crit_parent = crit -> parent;
    node * crit_right = crit -> right;
    crit -> right = crit_right -> left;
    // the critical node's left child

    if(crit_right != null && crit -> right != null)
        crit_right -> left -> parent = crit;
    crit_right -> left = crit;
    crit -> parent = crit_right;
    crit_right -> parent = crit_parent;

    if(crit_right -> parent == null){
        root = crit_right;
    }
    else if(crit_parent -> right == crit)
        crit_parent -> right = crit_right;
    else
        crit_parent -> left = crit_right;

}

void balance(struct node* node_to_balance)
{
    struct node* parent = NULL;
    struct node* grandparent = NULL;

    while ((node_to_balance != root) && (node_to_balance->color != black) && (node_to_balance->parent->color == red)){
        parent = node_to_balance->parent;
        grandparent = find_grandparent(node_to_balance);

        struct node* uncle_pt = find_uncle(node_to_balance);
        if (uncle_pt != NULL && uncle_pt->color == red){
            grandparent->color = red;
            parent->color = black;
            uncle_pt->color = black;
            node_to_balance = grandparent;
        }else{
            if (parent == grandparent->left){
                if (node_to_balance == parent->right) {
                    rotate_left(parent);
                    node_to_balance = parent;
                    parent = node_to_balance->parent;
                }
//                printf("right rotate\n");
                rotate_roight(grandparent);
                int t = parent->color;
                parent->color = grandparent->color;
                grandparent->color = t;
                node_to_balance = parent;
                }
            else {
                if (node_to_balance == parent->left) {
//                    printf("right rotate\n");
                    rotate_roight(parent);
                    node_to_balance = parent;
                    parent = node_to_balance->parent;
                }
//                printf("left rotate\n");
                rotate_left(grandparent);
                int t = parent->color;
                parent->color = grandparent->color;
                grandparent->color = t;
                node_to_balance = parent;
            }
        }
       }


    if(root -> parent == null)
        root -> color = black;
}

node * print_in_order(node * head){
    if(head == null)
        return head;
    print_in_order(head->left);
    printf("%d\n", head -> data);
    print_in_order(head->right);
}

int find_height(node * root){
    if(root == null)
        return 0;
    int left_height = find_height(root->left);
    int right_height = find_height(root->right);
    return (right_height > left_height)? right_height + 1 : left_height + 1;
}
void prt_lvl(node * root, node * org, int h){
    node * parent = null;
    if(root == null)
        return;
    if(h == 1) {
        printf("%d ", root->data);
        if(root -> color == black)
            printf("B ");
        else
            printf("R ");
        parent = root -> parent;
        if (parent != null) {
            //printf("(censored) powder\n"); // for debugging
            if (parent == root) {
                parent = root -> parent;
            }
            if (parent -> data > root -> data)
                printf("(%d L) ", parent -> data);
            else if (parent -> data < root -> data) {
                printf("(%d R) ", parent -> data);
            }
        }

    }else if(h > 1){
        prt_lvl(root -> left, org, h - 1);
        prt_lvl(root -> right, org, h - 1);
    }
}
void disp_tree(node * org, node * root){
    int height = find_height(root);
    for (int i = 1; i <= height; ++i) {
        prt_lvl(root, org, i);
        if(i != height)
            printf("\n");
    }
}

int main()
{

    int data;
    scanf("%d", &data);
    while(data != -1){
        struct node* temp = init_temp(data);
        root = insert(root, temp);
        balance(temp);
//        disp_tree(root, root);
//        printf("\n\n");
        scanf("%d", &data);
    }

    print_in_order(root);
    printf("\n");
    disp_tree(root, root);

    return 0;
}
