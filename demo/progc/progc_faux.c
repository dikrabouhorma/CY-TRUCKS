#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

const int DIM_STR =100 ;



// Definition de la structure pour les donnees de la route

typedef struct Route {

    int routeid;

    int stepid;

    char *townA;

    char *townB;

    float distance;

    char *driverName;

} Route;



// Definition de la structure pour un noeud de l'arbre AVL

typedef struct AVLNode {

    Route data;

    struct AVLNode *left;

    struct AVLNode *right;

    int height;

} AVLNode;



// Prototypes des fonctions

AVLNode* insert(AVLNode* node, Route data);

int height(AVLNode *N);

int max(int a, int b);

AVLNode* newNode(Route data);

AVLNode* rightRotate(AVLNode *y);

AVLNode* leftRotate(AVLNode *x);

int getBalance(AVLNode *N);

void preOrder(AVLNode *root);



// Fonctions pour l'arbre AVL (insertion, rotation, etc.)

// Fonction pour obtenir la hauteur d'un noeud

int height(AVLNode *N) {

    if (N == NULL)

        return 0;

    return N->height;

}



// Fonction pour obtenir le maximum de deux entiers

int max(int a, int b) {

    return (a > b) ? a : b;

}



// Fonction pour creer un nouveau noeud de l'arbre AVL

AVLNode* newNode(Route data) {

    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));

    node->data = data;

    node->left = NULL;

    node->right = NULL;

    node->height = 1; // Le noeud est initialement ajoute a la feuille

    return(node);

}



// Fonction pour effectuer une rotation droite

AVLNode *rightRotate(AVLNode *y) {

    AVLNode *x = y->left;

    AVLNode *T2 = x->right;



    // Effectuer la rotation

    x->right = y;

    y->left = T2;



    // Mettre a jour les hauteurs

    y->height = max(height(y->left), height(y->right)) + 1;

    x->height = max(height(x->left), height(x->right)) + 1;



    // Retourner la nouvelle racine

    return x;

}



// Fonction pour effectuer une rotation gauche

AVLNode *leftRotate(AVLNode *x) {

    AVLNode *y = x->right;

    AVLNode *T2 = y->left;



    // Effectuer la rotation

    y->left = x;

    x->right = T2;



    // Mettre a jour les hauteurs

    x->height = max(height(x->left), height(x->right)) + 1;

    y->height = max(height(y->left), height(y->right)) + 1;



    // Retourner la nouvelle racine

    return y;

}



// Fonction pour obtenir le facteur d'equilibre d'un noeud

int getBalance(AVLNode *N) {

    if (N == NULL)

        return 0;

    return height(N->left) - height(N->right);

}



// Fonction recursive pour inserer une route dans l'arbre AVL et retourner la nouvelle racine de l'arbre

AVLNode* insert(AVLNode* node, Route data) {

    // 1. Effectuer l'insertion normale BST

    if (node == NULL)

        return(newNode(data));



    if (data.routeid < node->data.routeid)

        node->left = insert(node->left, data);

    else if (data.routeid > node->data.routeid)

        node->right = insert(node->right, data);

    else // Les cles identiques ne sont pas autorisees dans l'arbre AVL

        return node;



    // 2. Mettre a jour la hauteur de ce noeud ancetre

    node->height = 1 + max(height(node->left), height(node->right));



    // 3. Obtenir le facteur d'equilibre de ce noeud ancetre pour verifier si ce noeud est devenu desequilibre

    int balance = getBalance(node);



    // Si ce noeud devient desequilibre, alors il y a 4 cas



    // Cas Gauche Gauche

    if (balance > 1 && data.routeid < node->left->data.routeid)

        return rightRotate(node);



    // Cas Droite Droite

    if (balance < -1 && data.routeid > node->right->data.routeid)

        return leftRotate(node);



    // Cas Gauche Droite

    if (balance > 1 && data.routeid > node->left->data.routeid) {

        node->left = leftRotate(node->left);

        return rightRotate(node);

    }



    // Cas Droite Gauche

    if (balance < -1 && data.routeid < node->right->data.routeid) {

        node->right = rightRotate(node->right);

        return leftRotate(node);

    }



    // Retourner le pointeur de noeud (inchange)

    return node;

}



// Fonction pour parcourir l'arbre AVL en pre-ordre et afficher les noeuds

void preOrder(AVLNode *root) {

    if (root != NULL) {

        printf("%d \n", root->data.routeid);

        printf("%s \n", root->data.townA);

        printf("%s \n", root->data.townB);

        printf("%s \n", root->data.driverName);

        printf("%f \n", root->data.distance);

        printf("--------------------------------------\n");

        preOrder(root->left);

        preOrder(root->right);

    }

}


// Function to check if a tree is balanced and follows AVL properties
int isAVLTree(AVLNode *root) {
    if (root == NULL)
        return 1; // An empty tree is AVL

    // Check the balance factor of the current node
    int balance = getBalance(root);

    // Check if the balance factor is within the allowed range
    if (balance > 1 || balance < -1) {
        printf("AVL Tree Violation at Route ID %d\n", root->data.routeid);
        return 0;
    }

    // Recursively check the left and right subtrees
    return isAVLTree(root->left) && isAVLTree(root->right);
}



void countNodes(AVLNode *root  , int *count) {

    if (root != NULL) {

        //if( (strcmp(root->data.townA  , key ) == 0) || (strcmp(root->data.townB  , key ) == 0 ))
                (*count)++ ;

      

        countNodes(root->left , count);

        countNodes(root->right, count);

    }

}

void search(AVLNode *root , char *key , int *count) {

    if (root != NULL) {

        if( (strcmp(root->data.townA  , key ) == 0) || (strcmp(root->data.townB  , key ) == 0 ))
                (*count)++ ;

      

        search(root->left , key , count);

        search(root->right, key , count);

    }

}

//------------------------------------------------------------------------------------------------------------------------



// Structure pour stocker le nombre de trajets par ville

typedef struct CityCount {

    char *cityName;

    int count;

    int departCount; // Compter le nombre de fois où cette ville est une ville de départ

} CityCount;

// trim function to remove spaces from begining and end 
char * trim_space(char *str) {
    char *end;
    /* skip leading whitespace */
    while (isspace(*str)) {
        str = str + 1;
    }
    /* remove trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end = end - 1;
    }
    /* write null character */
    *(end+1) = '\0';
    return str;
}

// Fonction pour compter le nombre de trajets par ville

void countRoutesPerCity(AVLNode *root, CityCount cityCounts[], int *cityCountSize) {

    if (root != NULL) {

        // Verifier la ville A du trajet actuel

        int i;

        // count parcourir les villes qui existent puis incrementer si la ville courante existe dans la liste 

        for (i = 0; i < *cityCountSize; ++i) {

            if (strcmp(trim_space(root->data.townA), trim_space(cityCounts[i].cityName) )== 0) {

            	if (root->data.stepid == 1) {

                	cityCounts[i].departCount++;

            	}

                cityCounts[i].count++;

                break;

            }

        }



         // Si la ville A n'est pas encore présente dans la liste, l'ajouter

        if (i == *cityCountSize) {

            //allocation memoire pour string 

            cityCounts[*cityCountSize].cityName = (char *) malloc( DIM_STR * sizeof(char));

            strcpy(trim_space(cityCounts[*cityCountSize].cityName), trim_space(root->data.townA));

            cityCounts[*cityCountSize].count = 1;

            if (root->data.stepid == 1) {

                	cityCounts[*cityCountSize].departCount = 1;

            } else {

                        cityCounts[*cityCountSize].departCount = 0;

            }

            

            (*cityCountSize)++;

        }



        // Verifier la ville B du trajet actuel

        for (i = 0; i < *cityCountSize; ++i) {

            if (strcmp(trim_space(root->data.townB), trim_space(cityCounts[i].cityName)) == 0) {

                cityCounts[i].count++;

                break;

            }

        }

        // Si la ville B n'est pas encore presente dans la liste, l'ajouter

        if (i == *cityCountSize) {

            //allocation memoire pour string 

            cityCounts[*cityCountSize].cityName = (char *) malloc( DIM_STR * sizeof(char));

            strcpy(trim_space(cityCounts[*cityCountSize].cityName), trim_space(root->data.townB));

            cityCounts[*cityCountSize].count = 1;

            cityCounts[*cityCountSize].departCount = 0;

            (*cityCountSize)++;

        }

        

        //printf("%s \n" , root->data.townA);

        //printf("%s \n" , root->data.townB);

       //printf(" =============================  \n");



        // Parcourir les sous-arbres gauche et droit

        countRoutesPerCity(root->left, cityCounts, cityCountSize);

        countRoutesPerCity(root->right, cityCounts, cityCountSize);

    }

}



// Fonction pour échanger deux éléments dans le tableau

void swap(CityCount *a, CityCount *b) {

    CityCount temp = *a;

    *a = *b;

    *b = temp;

}



// Fonction pour trier le tableau (décroissant) en utilisant le tri à bulles

void bubbleSort_count(CityCount arr[], int size) {

    for (int i = 0; i < size - 1; ++i) {

        for (int j = 0; j < size - i - 1; ++j) {

            if (arr[j].count < arr[j + 1].count) {

                swap(&arr[j], &arr[j + 1]);

            }

        }

    }

}



void bubbleSort_names(CityCount arr[], int size) {

    for (int i = 0; i < size - 1; ++i) {

        for (int j = 0; j < size - i - 1; ++j) {

            // Utiliser strcmp pour comparer les chaînes (noms de villes) ordre croissant

            if (strcmp(arr[j].cityName, arr[j + 1].cityName) > 0) {

                swap(&arr[j], &arr[j + 1]);

            }

        }

    }

}



//------------------------------------------------------------------------------------------------------------------------



// Fonction principale

int main() {

    

    FILE *file = fopen("./data/data.csv", "r");

    if (!file) {

        printf("Impossible d'ouvrir le fichier.\n");

        return 1;

    }



    AVLNode *root = NULL;

   

    char line[256];

    int countlines =0 ;

    Route route;

    // mapping line form the  csv fie and route strecture  then add to AVL

    while (fgets(line, sizeof(line), file)) {


        // allocation pour string dynamique  

        route.townA = (char *)malloc( DIM_STR * sizeof(char) );

        route.townB = (char *)malloc( DIM_STR * sizeof(char) );

        route.driverName = (char *)malloc( DIM_STR * sizeof(char) );

        

        sscanf(line, "%d;%d;%[^;];%[^;];%f;%[^\n]", &route.routeid, &route.stepid, route.townA, route.townB, &route.distance, route.driverName);

        root = insert(root, route);

        countlines++;

    }



    fclose(file);

    printf("Count lines  = %d \n" , countlines);
    printf("H = %d \n" , root->height);  // maximum nodes 2 puisance H 

    int count = 0 ;

    countNodes(root ,&count );

    printf("Total : %d \n" , count) ;

    int found = 0 ;
    search(root , "ANDILLY" , &found);

    printf("Found : %d \n" , found) ;

    
// is Tree AVL or note 1 ok 0 No
    printf("IsAvl : %d \n" , isAVLTree(root)) ;
    

    //preOrder(root) ;

    

// -------------------- option -t ---------------------------------------------------------

    // Initialiser un tableau pour stocker les comptages par ville

    CityCount cityCounts[500000];  // Assumant un maximum de 100 000 villes !!!!!! ????? <<<=============

    int cityCountSize = 0;

    // Compter les trajets par ville

    countRoutesPerCity(root, cityCounts, &cityCountSize);

 

    // Afficher les resultats

    for (int i = 0; i < cityCountSize; ++i) {

        printf("Ville: %s, Nombre de trajets: %d\n", cityCounts[i].cityName, cityCounts[i].count);

    }

    

   

    // Enregistrer le resultat dans un fichier

    FILE *ff = fopen("trajets-ville.txt", "w");  // Ouvrir le fichier en mode écriture

    if (ff == NULL) {

        perror("Erreur lors de l'ouverture du fichier trajets-ville.txt");

        return 1;

    }

    

    // Trier le tableau en utilisant le tri à bulles décroissant

    bubbleSort_count(cityCounts, cityCountSize);

    

    CityCount top_10[10];

    int top_10_Size = 10;

    // sauvegarder que 10 !!

    for (int i = 0; i < top_10_Size; ++i) {

        top_10[i] = cityCounts[i];

    }

    // Par ordre Alphabétique

    bubbleSort_names(top_10, top_10_Size);

    

    // Sauvegarder les résultats dans le fichier le résultat final

    for (int i = 0; i < top_10_Size; ++i) {

        fprintf(ff, "%s, %d , %d \n", top_10[i].cityName, top_10[i].count, top_10[i].departCount);

    }

    fclose(ff);  // Fermer le fichier

    

     

    

    

    

    return 0;

}

