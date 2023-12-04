#include<string>
#include<iostream>
#include <fstream>
#include <vector>

int const nbre_client_max = 300;
int const nbre_vehicule_max = 100;
int const infini = 999999;
int const nb_voisin = 4;
int const nb_label_max=200;
int const nbre_tournee_max=200;
int const iter_max=20;
int const nbre_voisin=20;



using namespace std;

typedef struct T_instance{
	int nbre_client;								
	int nbre_type_vehicule;	
	double distance[nbre_client_max+1][nbre_client_max+1];			
	double demande[nbre_client_max];					
	double Capacite[nbre_vehicule_max];			
	double cout_variable[nbre_vehicule_max];		
	double cout_fixe[nbre_vehicule_max];			
	int vehicule_par_type[nbre_vehicule_max];					
}T_instance;

typedef struct T_tour_geant{
	int nbre_client; //nombre de clents
	int list_clt[nbre_client_max + 2];	
	int vol; //volume total transporte
	double camion; //camion choisi 
	double cout=0;
}T_tour_geant;

typedef struct T_label {
	int nbre_client;
	int vehicule_dispo[nbre_vehicule_max];
	int sommet;
	int camion;
	int position_pere;
	double cout;
} T_label;

typedef struct T_list_label {
	int nbre_liste;
	int sommet;
	vector<T_label> liste;
} T_list_label;

typedef struct T_solution {
	int nt; // nombre de tournée présents dans la solution
	T_tour_geant liste[nbre_tournee_max]; // La liste des tournées
	float cout; // le cout de la solution (Moyenne des couts des petites tournées
	int PRED[nbre_client_max]; // Le tableau des peres pour remonter les tournees generes par le VRP split
	int M[nbre_client_max]; //tableau des marqueurs sur les sommets pour l'algo disjktra like
	std::vector<T_label> liste_label[nbre_client_max]; // La matrice des labels : chaque sommet dispose d'un vecteur de label
}T_solution;


void lecture(string nom, T_instance& nom_instance);
void heuristique_plus_proche_voisin(T_instance& nom_instance,T_tour_geant& solution_initiale);
void heuristique_plus_proche_voisin_randomise(T_instance& nom_instance,T_tour_geant& solution_initiale);
void heuristique_troisieme_plus_proche_voisin(T_instance& nom_instance,T_tour_geant& solution_initiale);
void _2_opt_intra_tournee(T_instance& nom_instance,T_tour_geant& solution_initiale,int nb_max);
void deplacement(T_instance& nom_instance,T_tour_geant& solution_initiale);
void swap(T_instance& nom_instance,T_tour_geant& solution_initiale);
void split(T_instance& nom_instance, T_tour_geant& tournee,  T_solution& solution, T_list_label(&labels)[nbre_client_max + 1]);
void grasp(T_instance& nom_instance,T_solution& solution);