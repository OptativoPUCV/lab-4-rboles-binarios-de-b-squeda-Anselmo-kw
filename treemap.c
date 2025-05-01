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
    TreeNode * newNodo = createTreeNode(key, value);
    TreeNode * aux = tree->root; //Siempre empezamos por la raiz
    TreeNode * parent = NULL; // al parecer sirve para en un rato

    //Si root es null, significa que es el primero
    if(aux == NULL){
        tree->root = newNodo;
        tree->current = newNodo;
    }

    //Si no, hay que buscar donde insertarlo
    
    while (aux != NULL)
    {   
        //Como aux llega hasta NULL, no me sirve para hacer los enlace, entonces ocupo 
        parent = aux; //parent para quedarme con el ultimo valor de aux antes de ser nulo

        //Si key < aux
        if(tree->lower_than(key, aux->pair->key)){
            aux = aux->left; //me muevo a la izquierda
        }
        //Si aux < key
        else{
            if(tree->lower_than(aux->pair->key, key)){
                aux = aux->right; //me muevo a la derecha
            }
            else{//si son iguales psss no c hace nada
                return;
            }
        }

    }

    //Ahora enlazamos //pero devo ver donde insertarlo
    //Si key < aux
    if(tree->lower_than(key, parent->pair->key)){
        parent->left = newNodo;
        newNodo->parent = parent;
    }
    else{
        parent->right = newNodo;
        newNodo->parent = parent;
    }

    //Actualizar el curret
    tree->current = newNodo;
}

TreeNode * minimum(TreeNode * x){//es sub raiz, osea no es la raiz
    TreeNode * nodo = x;
    if(nodo->left == NULL)
        return nodo;

    while (nodo->left != NULL)
    {
        nodo = nodo->left;
    }
    
    return nodo;
}


void removeNode(TreeMap * tree, TreeNode* node) {

    // Nodo sin hijos //
    if(node->left == NULL && node->right == NULL){
        if(node->parent->left == NULL) //Signfica que que padre está a la derecha
        {
            node->parent->right = NULL;
        }
        else{
            node->parent->left = NULL;
        }
        return;
    }

    // Nodo con un hijo //
    if(node->left == NULL || node->right == NULL) //Sino tiene la izquierda, tiene a la derecha
    {   
        TreeNode * hijo = NULL;//node->right;

        if (node->left == NULL)
            hijo = node->right;
        else
            hijo = node->left;
        
        //Vemos donde está el padre
        if(node->parent->left == node) {
            node->parent->left = hijo;
            hijo->parent = node->parent;
        }
        else{
            node->parent->right = hijo;
            hijo->parent = node->parent;
        }
    }

    // Nodo con dos hijos //
/*Descienda al hijo derecho y obtenga el menor nodo del subárbol (con la función minimum). 
Reemplace los datos (key,value) de *node* con los del nodo "minimum". Elimine el nodo 
minimum (para hacerlo puede usar la misma función *removeNode*). */
    TreeNode * hijoDer = node->right;

    hijoDer = minimum(node);
    /*node->pair->key = hijoDer->pair->key;
    node->pair->value = hijoDer->pair->value;*/

    
    /*if(node->left != NULL && node->right != NULL)
    {
        TreeNode * hijoDer = minimum(node->right);

        free(node->pair->key);
        free(node->pair->value);        

        node->pair->key = hijoDer->pair->key;
        node->pair->value = hijoDer->pair->value;

        removeNode(tree, hijoDer);
    }*/

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
        /* lower_than (key, cosa)
        si key < cosa, retorna 1, si no, retorna 0;
        */
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
