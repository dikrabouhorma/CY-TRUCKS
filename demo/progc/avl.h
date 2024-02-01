#ifndef AVL_H
#define AVL_H

typedef struct CityTrajets {
    char *cityName;
    int count;
    int departCount; // Compter le nombre de fois où cette ville est une ville de départ
} CityTrajets;

// Definition de la structure pour un noeud de l'arbre AVL
typedef struct AVLNode {
    CityTrajets data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// Prototypes des fonctions
int height(AVLNode *N);
int max(int a, int b);
AVLNode* newNode(CityTrajets data);
AVLNode* rightRotate(AVLNode *y);
AVLNode* leftRotate(AVLNode *x);
int getBalance(AVLNode *N);
AVLNode* insert(AVLNode* node, CityTrajets data);

void preOrder(AVLNode *root);
void inOrder(AVLNode *root);
void inOrderSaveRec(FILE *file ,  AVLNode *root);
void inOrderSave(FILE* file, AVLNode* root);

void doit(AVLNode *root , int limit);
int isAVLTree(AVLNode *root);
void countNodes(AVLNode *root  , int *count);
void search(AVLNode *root , char *key , int *count);
int isNodeExist(AVLNode *root , char *key );

void getNbrTrajetsByCity(char  *filename , char *cityname , int *nbrtrajetDeb , int *nbrtrajetFin, int *nbrDepart , int start);
char* trim_space(char *str);

void swap(CityTrajets *a, CityTrajets *b);
void bubbleSort_count(CityTrajets arr[], int size);
void bubbleSort_names(CityTrajets arr[], int size);

#endif /* AVL_H */

