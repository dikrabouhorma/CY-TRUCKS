#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
const int DIM_STR = 100 ;
const int DIM_ARRAY = 35000;
#define MAX_TRAJETS 300000
#define MAX_ID_COUNT 35000

// Structure Trajets (routeid unique)
typedef struct Trajets {
    int routeid;
    int count; // combien de stepid
    int d_min;
    int d_max
    int moy
} Trajets;

// Calculer le nombre de trajet par routeid
void getNbrTrajets( char  *filename ){
    FILE *file = fopen(filename, "r");
    char line[256];
    // Read and discard lines until the desired line
    for (int currentLineNumber = 1; currentLineNumber < count; ++currentLineNumber) {
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
    }
    while (fgets(line, sizeof(line), file)) {
        // splite line
        char* idroute   = strtok(line, ";");
        char* stepid    = strtok(NULL, ";");
        char* townDeb   = strtok(NULL, ";");
        char* townFin   = strtok(NULL, ";");

        if( idroute == idroute_  ) {
            (*count)++ ;
            //printf("%d\n", &count);
        }
    }
    fclose(file);
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
    
    int routId = 0 ;
    int stepid = 0 ;
    float distance = 0.0;
    int count = 0 ;
    
    Trajets allTrajets[MAX_TRAJETS];

    // Lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), file)) {
        char townA[DIM_STR], townB[DIM_STR] ,drivername[DIM_STR];

        sscanf(line, "%d;%d;%[^;];%[^;];%f;%[^\n]", &routId, &stepid,  townA, townB, &distance, drivername);

        int existsA = 0;
        for (int i = 0; i < cityCount; i++) {
            if ( city.routeid == routId) {
                existsA = 1;
                break;
            }
        }
        if (!existsA) {

            getNbrTrajetsByCity("data.csv" , city.routeid , count) ;

            (cityCount)++;
            printf("count : %d \n",cityCount);
        }



       if(cityCount > 20 )
            break ;


    }
    fclose(file);

    for (int i = 0; i < cityCount; i++) {
            printf(" id : %d, count : %d \n", TopcityCounts[i].routeid, TopcityCounts[i].count)
    }

    printf("cities count %d \n" , cityCount) ;

    return 0;
}
