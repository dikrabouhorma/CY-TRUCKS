#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define DIM_STR 100 
#define DIM_ARRAY 35000
#define MAX_ID_COUNT 35000


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
AVLNode* insert(AVLNode* node, CityTrajets data);
int height(AVLNode *N);
int max(int a, int b);
AVLNode* newNode(CityTrajets data);
AVLNode* rightRotate(AVLNode *y);
AVLNode* leftRotate(AVLNode *x);
int getBalance(AVLNode *N);

void preOrder(AVLNode *root);
//void getNbrTrajetsByCity(char  *filename , char *cityname , int *nbrtrajet , int *nbrDepart);



// Fonctions pour la gestion de l'arbre AVL (insertion, rotation, etc.)
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
AVLNode* newNode(CityTrajets data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // Le noeud est initialement ajoute a la feuille
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
AVLNode* insert(AVLNode* node, CityTrajets data) {
    // 1. Effectuer l'insertion normale BST
    if (node == NULL)
        return(newNode(data));
        
    if (  strcmp(data.cityName, node->data.cityName) < 0 )
        node->left = insert(node->left, data);
        
    else if ( strcmp(data.cityName, node->data.cityName) > 0)
        node->right = insert(node->right, data);
        
    else // Les cles identiques ne sont pas autorisees dans l'arbre AVL
        return node;

    // 2. Mettre a jour la hauteur de ce noeud ancetre
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Obtenir le facteur d'equilibre de ce noeud ancetre pour verifier si ce noeud est devenu desequilibre
    int balance = getBalance(node);

    // Si ce noeud devient desequilibre, alors il y a 4 cas ::
    // Cas Gauche Gauche
    if (balance > 1 &&  strcmp (data.cityName , node->left->data.cityName)<0)
        return rightRotate(node);

    // Cas Droite Droite
    if (balance < -1 &&  strcmp(data.cityName , node->right->data.cityName )>0 )
        return leftRotate(node);

    // Cas Gauche Droite
    if (balance > 1 &&  strcmp(data.cityName ,node->left->data.cityName) >0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Cas Droite Gauche
    if (balance < -1 &&   strcmp(data.cityName, node->right->data.cityName) <0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Retourner le pointeur de noeud
    return node;
}


// Fonction pour parcourir l'arbre AVL en pre-ordre et afficher les noeuds
void preOrder(AVLNode *root) {
    if (root != NULL) {
        printf("%s \n", root->data.cityName);
        printf("%d \n", root->data.count);
        printf("%d \n", root->data.departCount);
        printf("--------------------------------------\n");

        preOrder(root->left);
        preOrder(root->right);
    }
}

// Fonction pour parcourir l'arbre AVL en in-ordre et afficher les noeuds
void inOrder(AVLNode *root) {
    if (root != NULL) {
        inOrder(root->left);
        
        printf("%s \n", root->data.cityName);
        printf("%d \n", root->data.count);
        printf("%d \n", root->data.departCount);
        printf("--------------------------------------\n");

        inOrder(root->right);
    }
}

// Fonction Récursive qui parcours l'AVL in-order et sauvegarde le résultat dans un fichier txt
// Cette fonction récursive ne fait pas la tâche prévu
void inOrderSaveRec(FILE *file ,  AVLNode *root) {
    if (root != NULL) {
        inOrder(root->left);
        fprintf(file, "%s ; %d ; %d \n", root->data.cityName, root->data.count, root->data.departCount);
        inOrder(root->right);
    }
}

// Fonction d'enregistrement inOrder NON RECURSIVE dans un fichier txt
void inOrderSave(FILE* file, AVLNode* root) {
    AVLNode* current = root;
    AVLNode* stack[MAX_ID_COUNT];
    int top = -1;

    while (current != NULL || top >= 0) {
        while (current != NULL) {
            stack[++top] = current;
            current = current->left;
        }
        
        current = stack[top--];
        fprintf(file, "%s ; %d ; %d \n", current->data.cityName, current->data.count, current->data.departCount);
        current = current->right;
    }
}

int myindex = 0 ;
CityTrajets TopcityCounts[10];

// traveersal from higher to lower according to inorder manner with limit
void doit(AVLNode *root , int limit) {
    if (root != NULL && myindex < limit) {
        doit(root->right , limit );
        
        printf("%s \n", root->data.cityName);
        printf("%d \n", root->data.count);
        printf("%d \n", root->data.departCount);
        printf("--------------------------------------\n");

        TopcityCounts[myindex] = root->data ;
        myindex++ ;

        doit(root->left , limit );
    }
}

// Fonction pour verifier si AVL est balancer ou pas
int isAVLTree(AVLNode *root) {
    if (root == NULL)
        return 1; 	// Arbre vide est un AVL

    // Verifier le facteur d equilibre du noeud courant
    int balance = getBalance(root);

    // Verifier est-ce que le facteur d equilibre est acceptable ou non
    if (balance > 1 || balance < -1) {
        printf("AVL Tree Violation at Route ID %s\n", root->data.cityName);
        return 0;
    }

    // Recursivite pour continuer a droite et gauche
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
        if( strcmp(root->data.cityName ,  key) == 0  )
                (*count)++ ;
        search(root->left , key , count);
        search(root->right, key , count);
    }
}

int isNodeExist(AVLNode *root , char *key ) {
    if (root != NULL) {
        if( strcmp(root->data.cityName ,  key) == 0  )
                return 1 ;
        return isNodeExist(root->left , key );
        return isNodeExist(root->right, key );
    }
    return 0 ;
}

//------------------------------------------------------------------------------------------------------------------------
// Structure pour stocker le nombre de trajets par ville
void getNbrTrajetsByCity(char  *filename , char *cityname , int *nbrtrajetDeb , int *nbrtrajetFin, int *nbrDepart , int start){
    FILE *file = fopen(filename, "r");
    char line[256];
    
    // Read and discard lines until the desired line
    for (int currentLineNumber = 1; currentLineNumber < start; ++currentLineNumber) {
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            // Handle end of file or read error
            break;
        }
    }

    while (fgets(line, sizeof(line), file)) {
            // splite line
            char* idroute = strtok(line, ";");
            char* stepid = strtok(NULL, ";");
            char* townDeb = strtok(NULL, ";");
            char* townFin = strtok(NULL, ";");

            if( strcmp(cityname , townDeb) == 0  )
                        (*nbrtrajetDeb)++ ;

            if(strcmp(cityname , townFin) == 0)
                    (*nbrtrajetFin)++ ;

            if(strcmp(cityname , townDeb) == 0 && atoi(stepid) == 1)
                        (*nbrDepart)++ ;
    }
    fclose(file);
}
//------------------------------------------------------------------------------------------------------------------------

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


// Fonction pour échanger deux éléments dans le tableau
void swap(CityTrajets *a, CityTrajets *b) {
    CityTrajets temp = *a;
    *a = *b;
    *b = temp;
}

// Fonction pour trier le tableau (décroissant) en utilisant le tri à bulles
void bubbleSort_count(CityTrajets arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j].count < arr[j + 1].count) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void bubbleSort_names(CityTrajets arr[], int size) {
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

	/* ------------------------------------------------------------------------------------------------------ 
	Dans cette première partie, on insere dans l'AVL les villes par ordre alphabetique et on compte à chaque fois, le nombre de fois parcouru ainsi que le nombe de fois où ces villes on ete des villes de depart.
	
	L'execution de ce traitement prend environ 21 heures.
	C'est pourquoi on a conserve le resultat dans un fichier txt pour qu'on puisse continuer le traitement plus rapidement.
	Si vous avez besoin de refaire la partie AVL il faut juste enlever les commentaires de cette partie ci-dessous.
	------------------------------------------------------------------------------------------------------ */
	/*
	char *cities[MAX_ID_COUNT] ;
	AVLNode *root = NULL;

	clock_t start, end;
	double cpu_time_used;
	start = clock();

	int cityCount = 0 ;

	FILE *f1 = fopen("../data/data.csv", "r");
	if (!f1) {
		printf("Erreur de l'ouverture du fichier data.csv.\n");
		exit(1);
	}

	char line[DIM_STR];
	int count = 0 ;

	CityTrajets city;

	int routId = 0 ;
	float distance ;
	int stepid = 0;
	int limitcount = 0 ;

	while (fgets(line, sizeof(line), f1)) {

		int countTrajetDeb = 0 ;
		int countCityDeb = 0 ;
		int countTrajetFin = 0 ;

		char townA[DIM_STR], townB[DIM_STR] ,drivername[DIM_STR];
		sscanf(line, "%d;%d;%[^;];%[^;];%f;%[^\n]", &routId, &stepid,  townA, townB, &distance, drivername);

		// Check townA
		int existsA = 0;
		for (int i = 0; i < cityCount; i++) {
		    if (strcmp(townA, cities[i]) == 0) {
			existsA = 1;
			break;
		    }
		}
		if (!existsA) {
		   cities[cityCount] = strdup(townA);

		   city.cityName = strdup(townA) ;
		   getNbrTrajetsByCity("../data/data.csv" , city.cityName , &countTrajetDeb ,&countTrajetFin  ,&countCityDeb , count) ;
		   
		   // probleme des villes dupliques dans un trajet
		   int intercitydebut =  countTrajetDeb - countCityDeb ;
		   int intercityfin = countTrajetFin - intercitydebut ;

		   city.count = countCityDeb + intercitydebut + intercityfin ;
		   city.departCount = countCityDeb ;

		   // Inserer dans AVL
		   root = insert(root, city);

		   (cityCount)++;
		   //printf("Node : %d \n",cityCount);
		}

		// Check townB
		int existsB = 0;
		for (int i = 0; i < cityCount; i++) {
		    if (strcmp(townB, cities[i]) == 0) {
			existsB = 1;
			break;
		    }
		}
		if (!existsB) {
		    cities[cityCount] = strdup(townB);

		    city.cityName = strdup(townB) ;
		    getNbrTrajetsByCity("../data/data.csv" , city.cityName , &countTrajetDeb ,&countTrajetFin  ,&countCityDeb , count) ;

		    // probleme des villes dupliques dans un trajet
		    int intercitydebut =  countTrajetDeb - countCityDeb ;
		    int intercityfin = countTrajetFin - intercitydebut ;
		    city.count = countCityDeb + intercitydebut + intercityfin ;
		    city.departCount = countCityDeb ;

		    root = insert(root, city); // Inserer dans AVL
		    (cityCount)++;
		}

		 printf("check point : line : %d  city : %d \n" , count , cityCount );
		 limitcount = 0;

		 count++;
		 limitcount ++ ;
	}
	fclose(f1);


	// calcule de temps en second concernnant ce traitement
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("took %f seconds to execute \n", cpu_time_used);
	printf("cities count %d \n" , cityCount) ;


	int countnodes = 0 ;
	countNodes(root ,&countnodes );
	printf("Total : %d \n" , countnodes) ;

	// Liberer la mémoire
	for (int i = 0; i < cityCount; i++) {
		free(cities[i]);
	}

	FILE *ff = fopen("trajets-ville.txt", "w+");  // Ouvrir le fichier en mode écriture
	if (ff == NULL) {
	    perror("Erreur lors de l'ouverture du fichier trajets-ville.txt");
	    return 1;
	}
	inOrderSave(ff, root );
	
        fclose(ff);  // Fermer le fichier ff
        */
        
        /* -------------------------------------------------------------------------------------------
        A ce stade, le calcules de toutes les villes sont dans le fichier : trajets-ville.txt
        Donc on va lire à partir de ce fichier pour trier selon le nombre de trajet (colonne 2) ensuite garder les 10 premiers et les trier encore une fois par ordre alphabétique. Et sauvegarder le résultat final dans le fichier option-t.txt
        -------------------------------------------------------------------------------------------*/
	
	// lire les informations des villes
	CityTrajets T[DIM_ARRAY];
	int c = 0; // indice de T

	FILE *file = fopen("trajets-ville-toutlesvilles.txt", "r");
	if (!file) {
	printf("Unable to open the file.\n");
	exit(1);
	}
	char line[DIM_STR];

	while (fgets(line, sizeof(line), file)) {

		int count=0, countdepart=0;
		char town[DIM_STR];

		sscanf(line, "%[^;]; %d ; %d\n", town, &count, &countdepart);
		
		strcpy(T[c].cityName, town);
		T[c].count = count;
		T[c].departCount = countdepart;
		
		//printf ("-----> %s , %d , %d \n", T[c].cityName, T[c].count, T[c].departCount);
		c++ ;
		//if (c == 10) break;
	}
     
	// Trie a bulle par count
	bubbleSort_count(T, c);

	// Sauvegarder top10 selon le count : ordre croissant
	CityTrajets TopcityCounts[10];
	for (int i = 0; i<10 ; i++){
	TopcityCounts[i] = T[i];
	}

	// Trie a bulle par nom decroissant des top10
	bubbleSort_names(TopcityCounts, 10);

	// Enregistrer le résultat FINAL dans un fichier f3
	FILE *f3 = fopen("option-t.txt", "w");  // Ouvrir le fichier en mode écriture
	if (f3 == NULL) {
	    perror("Erreur lors de l'ouverture du fichier option-t.txt");
	    return 1;
	}
	for (int i=0;i<10;i++){
		fprintf(f3, "%s,%d,%d\n", TopcityCounts[i].cityName, TopcityCounts[i].count , TopcityCounts[i].departCount);
	}
	fclose(f3);  // Fermer le fichier

    return 0;
}


