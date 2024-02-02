#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define DIM_STR 100
#define MAX_TRAJETS 300000


// *********************************************************************
// Structure Trajets (routeid unique)
typedef struct Trajets {
    int routeid;
    int total;  // Number of steps
    int distance;
    int d_min, d_max, d_moy;
} Trajets;

int MAX(int a, int b){
	if (a>=b) return a;
	else return b;
}
int MIN(int a, int b){
	if (a<=b) return a;
	else return b;
}

//------------------------------------------------------------------------------------------------------------------------
// Fonction principale
int main() {

    FILE *file = fopen("data.csv", "r");
    if (!file) {
        printf("Unable to open the file.\n");
        exit(1);
    }

    char line[DIM_STR];

    Trajets allTrajets[MAX_TRAJETS];
    int count = 0 ; // indice du tableau

    // initialiser les trajets
    for (int i=0 ; i< MAX_TRAJETS ; i++){
        allTrajets[i].routeid = 0 ;
        allTrajets[i].total = 0 ;
        allTrajets[i].distance = 0.0 ;
    }

    // Lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), file)) {
        char townA[DIM_STR], townB[DIM_STR] ,drivername[DIM_STR];
        int routId , stepid ;
        int distance;

        sscanf(line, "%d;%d;%[^;];%[^;];%d;%[^\n]", &routId, &stepid,  townA, townB, &distance, drivername);

        int exist = 0;
        // chercher si le routeid existe dans le tableau
        for (int i = 0; i < count; i++) {
            if ( allTrajets[i].routeid == routId) {
                exist = 1;
                allTrajets[i].total++;
              	allTrajets[i].distance = distance;
              	int tmp = allTrajets[i].d_max;
              	allTrajets[i].d_max = MAX(tmp, distance);
              	tmp = allTrajets[i].d_min;
              	allTrajets[i].d_min = MIN(tmp, distance);
                break;
            }
        }

        if (exist == 0) {
        // si routeid n'existe pas dans le tableau il faut l'ajouté
            allTrajets[count].routeid = routId ;
            allTrajets[count].total++;
            allTrajets[count].distance = distance;
            allTrajets[count].d_max = distance;
            allTrajets[count].d_min = distance;
            
            count++;
        }

    printf("Routeid found : %d \n", count);

    //if (count == 1000) break;

    }
    fclose(file);


    FILE *f2 = fopen("result.txt", "w");
    if (!f2) {
        printf("Unable to open the file.\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
            //printf(" RouteID : %d, total : %d , distance = %d \n", allTrajets[i].routeid , allTrajets[i].total, allTrajets[i].distance);
             fprintf(f2, "%d, %d , %d \n", allTrajets[i].routeid , allTrajets[i].d_min, allTrajets[i].d_max);
    }

    //printf("Routeid count %d \n" , count) ;

    return 0;
}
