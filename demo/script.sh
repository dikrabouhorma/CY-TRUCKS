#!/bin/bash

# Vérification du nombre d'arguments
if [ "$#" -lt 2 ]; then
	# Si le premier argument est -h, afficher l'aide et ignorer les autres arguments
	if [ "$1" == "-h" ]; then
	    echo "Options possibles :"
	    echo "	-h  : Affiche l'aide"
	    echo "	-d1 : Conducteurs avec le plus de trajets"
    	    echo "	-d2 : Conducteurs avec la plus grande distance"
     	    echo "	-l  : Les 10 trajets les plus longs"
     	    echo "	-t  : Les 10 villes les plus traversées"
     	    echo "	-s  : Statistiques sur les étapes"
	    exit 0
	fi
	echo "Usage: $0 chemin_vers_fichier_csv [-h]help [-d1] [-d2] [-l] [-t] [-s]"
	exit 1
fi

# Récupération du chemin du fichier CSV
fichier_csv="$1"

# Récupération de l'argument passé au script
option="$2"

# Fonction pour démarrer le chrono qui va calculer la durée des traitements
chrono_start() {
    start_time=$(date +%s%N)  # Enregistre le temps de départ en nanosecondes
}

# Fonction pour arrêter le chrono et afficher la durée
chrono_stop() {
    end_time=$(date +%s%N)    # Enregistre le temps d'arrêt en nanosecondes
    duration=$((end_time - start_time))  # Calcule la durée en nanosecondes
    # Convertit la durée en secondes avec deux décimales
    duration_seconds=$(echo "scale=2; $duration / 1000000000" | bc)
    echo "Durée du traitement : ${duration_seconds} secondes"
}
# --------------- Fonction pour le traitement d1 -------------------------------------
Option_d1() {
	chrono_start
	#Compter le nombre de trajets pour chaque conducteur d1
	awk -F";" '/;1;/ {compteur[$6] +=1} END {for (nom in compteur) print compteur[nom] ";" nom }' data/data.csv | sort -nrk1,1 | head -n 10 > temp/d1_t.csv

	awk -F";" '{print NR ";" $0}' temp/d1_t.csv > temp/d1.csv
	rm ./temp/d1_t.csv
	chrono_stop

	# Commandes GNUplot pour générer l'histogramme horizontal
	gnuplot <<- GNU_PLOT
		set term pngcairo size 500,500 enhanced font 'Arial,8'
		set output './images/d1.png'
		set datafile separator ";"
		set ylabel 'DRIVER NAMES'
		set xlabel 'NB ROUTES'
		set title 'Option -d1 : Nb routes = f(Driver)'
		binwidth = 0.5
		bin(x) = binwidth * floor(x/binwidth)
		set style fill solid 1
		set yrange reverse
		plot 'temp/d1.csv' u (\$2/2):1:(\$2/2):(binwidth/2.):yticlabels(3) w boxxy notitle linecolor rgb "green"
	GNU_PLOT
	# Nettoyage des fichiers temporaires
	# rm ./temp/d1.csv
}

# --------------- Fonction pour le traitement d2 -------------------------------------
Option_d2() {
	chrono_start
	# Récupérer la distance totale parcourue par chaque conducteur
	awk -F";" '{compteur[$6] += $5} END {for (nom in compteur) print nom ";" compteur[nom]}' data/data.csv | sort -t";" -k2,2nr | head -n 10 > temp/d2_t.csv

	awk -F";" '{print NR ";" $0}' temp/d2_t.csv > temp/d2.csv
	rm ./temp/d2_t.csv
	chrono_stop
	
	# Commandes GNUplot pour générer l'histogramme horizontal
	gnuplot <<- GNU_PLOT
		set term pngcairo size 500,500 enhanced font 'Arial,8'
		set output './images/d2.png'
		set datafile separator ";"
		set ylabel 'DRIVER NAMES'
		set xlabel 'DISTANCE (Km)'
		set title 'Option -d2 : Distance = f(Driver)'
		binwidth = 0.5
		bin(x) = binwidth * floor(x/binwidth)
		set style fill solid 1
		set yrange reverse
		plot 'temp/d2.csv' u (\$3/2):1:(\$3/2):(binwidth/2.):yticlabels(2) w boxxy notitle linecolor rgb "red"
	GNU_PLOT
	# Nettoyage des fichiers temporaires
	# rm ./temp/d2.csv
}

# --------------- Fonction pour le traitement l -------------------------------------
Option_l() {
	chrono_start
	# Calculer la distance total de chaque trajet
	awk -F';' '{journey[$1]+=$5} END {for (j in journey) print j, journey[j]}' $fichier_csv | sort -k2,2nr | head -10 | sort -k1,1n > './temp/option_l.txt'
	chrono_stop

	gnuplot <<- GNU_PLOT
		set terminal pngcairo size 500,500 enhanced font 'Arial,8'
		set output './images/L.png'
		set xlabel 'ROUTE ID'
		set ylabel 'Distance (km)'
		set title 'Option -l : Distance=f(Route)'
		set style data histograms
		set boxwidth 1	 		# largeur de la barre
		set style histogram gap 1.5 	# définir la distance vide entre les barres
		set yrange [0:*]
		set style fill solid 1.00 border -1
		set xrange reverse
		plot './temp/option_l.txt' using 2:xtic(1) notitle linecolor rgb 'blue'
	GNU_PLOT
	# Nettoyage des fichiers temporaires
	#rm ./temp/option_l.txt
}

Option_t(){
	input_file="./temp/option-t.txt"

	gnuplot <<- GNU_PLOT
		set term pngcairo size 500,500 enhanced font 'Arial,7'
		set output './images/T2.png'
		set title 'Option -t : Nb routes = f(Towns)'
		set style data histograms
		set style histogram cluster gap 3
		set style fill solid border -1
		set boxwidth 2
		set xlabel 'VILLES'
		set ylabel 'NB ROUTES'
		set yrange [0:*]
		set datafile separator ","
		set xtics rotate by -45
		plot '$input_file' using 2:xtic(1) title 'Total trajets', '' using 3 title 'Villes de départ'
	GNU_PLOT
	# Nettoyage des fichiers temporaires
	#rm ./temp/option-t.txt
}
# ---------------------- Les options selon le choix de l'utilisateur -----------------------------
if [ "$option" == "-d1" ]; then
	Option_d1
elif [ "$option" == "-d2" ]; then
	Option_d2d
elif [ "$option" == "-l" ]; then
	Option_l
elif [ "$option" == "-t" ]; then
	# Makefile pour compiler et exécuter le programme C
	# --------------------------------------------------------------
	# La partie AVL prend 21 heures !!! vous pouvez enlever le commentaire pour l'exécuter
	# --------------------------------------------------------------
	# echo "Exécution du Makefile..."
	# Se déplacer dans le répertoire contenant le Makefile
	cd "./progc"
	# Exécution du Makefile
	make
	# Revenir au répertoire précédent
	cd ..
	# echo "Fin de l'exécution du Makefile."
	# --------------------------------------------------------------
	# Utiliser le résultat du make pour créer le graphe
	Option_t
elif [ "$option" == "-s" ]; then
	echo "-s option ... [en cours] "
else
    echo "Option non reconnue !"
fi
