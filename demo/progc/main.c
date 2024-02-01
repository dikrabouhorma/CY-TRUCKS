#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "avl.h"

#define DIM_STR 100 
#define DIM_ARRAY 35000
#define MAX_ID_COUNT 35000


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


