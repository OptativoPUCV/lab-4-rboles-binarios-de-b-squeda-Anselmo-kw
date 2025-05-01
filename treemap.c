#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

/*typedef struct Pair {
        void * key;
        void * value;
    } Pair;

    struct TreeNode {
        Pair* pair
        TreeNode * left;
        TreeNode * right;
        TreeNode * parent;
    };

    struct TreeMap {
        TreeNode * root;
        TreeNode * current;
        int (*lower_than) (void* key1, void* key2);
    };*/

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *new = (TreeMap*) malloc(sizeof(TreeMap));
    if(new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    
}

TreeNode * minimum(TreeNode * x){

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root; //siempre se empieza por la raiz verdad 
    if(aux == NULL) return NULL;
    while (aux != NULL)
    {   
        //Usar la funcion de lower_than?// ta rara jnajsa
        // key < aux
        if(tree->lower_than(key, aux->pair->key))
        {
            aux = aux->left; //porque mi aux es mayor
        }

        // aux < key
        if(tree->lower_than(aux->pair->key, key))
        {
            aux = aux->right;//Aux en este caso es menor que la clave
        }

        //En caso contrario son iguales
        else
        {   
            //Importante actualizar el current
            tree->current = aux;
            return aux->pair; // y retornamos el pair
        }

        /*if(buscar->pair->key == key) 
        {   
            tree->current = buscar->pair;
            return buscar->pair;
        }
        if(buscar->pair->key > key)
        {
            buscar->left = nextTreeMap(tree);
        }
        if(buscar->pair->key < key)
        {
            buscar->right = nextTreeMap(tree);
        }*/
    }
    
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
