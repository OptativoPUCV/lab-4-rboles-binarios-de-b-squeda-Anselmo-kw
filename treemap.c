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
    //TreeNode * hijoDer = node->right; ok
    //hijoDer = minimum(node); ok

    /*node->pair->key = hijoDer->pair->key;
    node->pair->value = hijoDer->pair->value;*/

    
    if(node->left != NULL && node->right != NULL)
    {
        //TreeNode * hijoDer = minimum(node->right);
        TreeNode * hijoDer = node->right;
        hijoDer = minimum(hijoDer);      

        node->pair->key = hijoDer->pair->key;
        node->pair->value = hijoDer->pair->value;

        removeNode(tree, hijoDer);
    }

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

/* La función Pair* upperBound(TreeMap* tree, void* key) retorna el **Pair** con clave 
igual a key. En caso de no encontrarlo retorna el primer par asociado a una clave mayor 
o igual a key. Para implementarla puede realizar una búsqueda normal y usar un puntero 
a nodo auxiliar ub_node que vaya guardando el nodo con la menor clave *mayor o igual a 
key*. Finalmente retorne el par del nodo ub\_node. */
Pair * upperBound(TreeMap * tree, void* key) {
    if(tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    TreeNode * guardado = NULL; //parte en nulo porque no sabemos si vamos a tener candidatos o no

    while(aux != NULL)
    {
        if(is_equal(tree, aux->pair->key, key))
            return aux->pair;
        else{
            //key < aux, 1 si key es menor (<) que aux
            if (tree->lower_than(key, aux->pair->key)) {
                // key < aux->key  == posible sucesor
                guardado = aux;
                aux = aux->left;
            }
            else{// key > aux
                aux = aux->right;
            }
        }

    }

    if(guardado == NULL) //si no tuvimos candidatos, retornamos NULL;
        return NULL;
    return guardado->pair;
}

/* Pair* firstTreeMap(TreeMap* tree) retorna el primer **Pair** del mapa (el menor). */
Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->root;
    if(aux == NULL) return NULL;

    while(aux->left != NULL){
        aux = aux->left;
    }

    //Actualizo el current SIEMPRE RECORDARLO
    tree->current = aux;
    return aux->pair;
}

/* Pair* nextTreeMap(TreeMap* tree)  retornar el siguiente **Pair** 
del mapa a partir del puntero TreeNode* current. Recuerde actualizar 
este puntero. */
Pair * nextTreeMap(TreeMap * tree) {
    if(tree == NULL || tree->current == NULL) return NULL;
    TreeNode * actual = tree->current;

    //Si tiene hijo derecho, retorna el dato minimo del sub arbol
    if(actual->right != NULL) //Tiene hijo derecho
    {   
        actual = actual->right;
        while (actual->left != NULL)
        {
            actual = actual->left; //buscamos el más peque
        }

        tree->current = actual;
        return actual->pair;
    }
    else{//Si current no tiene hijo derecho, retorna el primer padre que sea de clave mayor
        void * key = tree->current->pair->key; //clave del actual (void porque la clave en al estructura es de tipo void)
        TreeNode * aux = tree->current->parent; // papa del actual

        //SIEMPRE que comparo claves, usar tree->lower_than(clave1, clave2);
        while(aux != NULL && tree->lower_than(aux->pair->key, key)){
            aux = aux->parent;
        }

        tree->current = aux;
        if(aux == NULL) return NULL;
        else
            return aux->pair;
    }
}
