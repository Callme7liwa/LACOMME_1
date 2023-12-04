#include "HVRP.hpp"
using namespace std;

void lecture(string nom_fichier, T_instance& nom_instance) {
	ifstream fichier(nom_fichier, ifstream::in);
	if (fichier) {
		fichier >> nom_instance.nbre_client;
		fichier >> nom_instance.nbre_type_vehicule;
		for(int i=0 ; i<nom_instance.nbre_type_vehicule ; i++){
			fichier >> nom_instance.vehicule_par_type[i];
			fichier >> nom_instance.Capacite[i];
			fichier >> nom_instance.cout_fixe[i];
			fichier >> nom_instance.cout_variable[i];
		}
		for(int i=0 ; i<nom_instance.nbre_client+1 ; i++){
			for(int j=0 ; j<nom_instance.nbre_client+1 ; j++){
				fichier >> nom_instance.distance[i][j];
			}
		}
		for(int i=0 ; i<nom_instance.nbre_client ; i++){
			int client;
			fichier >> client;
			fichier >> nom_instance.demande[client];
		}
	}
	else {
		cout << "impossible d'ouvrir le fichier" << endl;
	}
	fichier.close();
}

void heuristique_plus_proche_voisin(T_instance& nom_instance,T_tour_geant& solution_initiale){
	int list_clt[nom_instance.nbre_client];
	for(int i=0;i<nom_instance.nbre_client;i++){
		list_clt[i]=i+1;
	}
	solution_initiale.list_clt[0]=0;
	solution_initiale.list_clt[nom_instance.nbre_client+1]=0;
	int size=nom_instance.nbre_client;
	int courant = 0;
	while(size!=0){
		int client_plus_proche=0;
		int distance_min=infini;
		int indice;
		for(int i=0;i<size;i++){
			if (nom_instance.distance[courant][list_clt[i]]<distance_min){
				distance_min=nom_instance.distance[courant][list_clt[i]];
				client_plus_proche=list_clt[i];
				indice=i;
			}
		}
		solution_initiale.list_clt[nom_instance.nbre_client-size+1]=client_plus_proche;
		solution_initiale.cout+=nom_instance.distance[courant][client_plus_proche];
		courant=client_plus_proche;
		size--;
		list_clt[indice]=list_clt[size];
	}
	solution_initiale.cout+=nom_instance.distance[courant][0];
}

void heuristique_plus_proche_voisin_randomise(T_instance& nom_instance,T_tour_geant& solution_initiale){
	int list_clt[nom_instance.nbre_client];
	for(int i=0;i<nom_instance.nbre_client;i++){
		list_clt[i]=i+1;
	}
	solution_initiale.list_clt[0]=0;
	solution_initiale.list_clt[nom_instance.nbre_client+1]=0;
	int size=nom_instance.nbre_client;
	int courant = 0;
	while(size!=0){
		int indice,indicee,nbre_voisin=0;
		int voisin[nb_voisin];
		int list_clt_copy[size];
		int size_copy=size;
		int client_plus_proche=0;
		for(int i=0;i<size_copy;i++){
			list_clt_copy[i]=list_clt[i];
		}
		while(nbre_voisin<nb_voisin and size_copy!=0){
			int distance_min=infini;
			for(int i=0;i<size_copy;i++){
				if (nom_instance.distance[courant][list_clt_copy[i]]<distance_min){
					distance_min=nom_instance.distance[courant][list_clt_copy[i]];
					client_plus_proche=list_clt_copy[i];
					indice=i;
				}
			}
			voisin[nbre_voisin]=client_plus_proche;
			nbre_voisin++;
			size_copy--;
			list_clt_copy[indice]=list_clt_copy[size_copy];
		}
		int aleatoire =	rand()%nb_voisin;
		solution_initiale.list_clt[nom_instance.nbre_client-size+1]=voisin[aleatoire];
		solution_initiale.cout+=nom_instance.distance[courant][voisin[aleatoire]];
		courant=voisin[aleatoire];
		//find index
		for (int i=0;i<size;i++){
			if (list_clt[i]==courant){indicee=i;}
		}
		size--;
		list_clt[indicee]=list_clt[size];
	}
	solution_initiale.cout+=nom_instance.distance[courant][0];
}

void heuristique_troisieme_plus_proche_voisin(T_instance& nom_instance,T_tour_geant& solution_initiale){
	int list_clt[nom_instance.nbre_client];
	for(int i=0;i<nom_instance.nbre_client;i++){
		list_clt[i]=i+1;
	}
	solution_initiale.list_clt[0]=0;
	solution_initiale.list_clt[nom_instance.nbre_client+1]=0;
	int size=nom_instance.nbre_client;
	int courant = 0;
	while(size!=0){
		int indice,indicee;
		int list_clt_copy[size];
		int size_copy=size;
		int client_plus_proche=0;
		for(int i=0;i<size_copy;i++){
			list_clt_copy[i]=list_clt[i];
		}
		for (int j=0;j<3;j++){
			int distance_min=infini;
			for(int i=0;i<size_copy;i++){
				if (nom_instance.distance[courant][list_clt_copy[i]]<distance_min){
					distance_min=nom_instance.distance[courant][list_clt_copy[i]];
					client_plus_proche=list_clt_copy[i];
					indice=i;
				}
			}
			size_copy--;
			list_clt_copy[indice]=list_clt_copy[size_copy];
		}
		solution_initiale.list_clt[nom_instance.nbre_client-size+1]=client_plus_proche;
		solution_initiale.cout+=nom_instance.distance[courant][client_plus_proche];
		courant=client_plus_proche;
		//find index
		for (int i=0;i<size;i++){
			if (list_clt[i]==courant){indicee=i;}
		}
		size--;
		list_clt[indicee]=list_clt[size];
	}
	solution_initiale.cout+=nom_instance.distance[courant][0];
}

void _2_opt_intra_tournee(T_instance& nom_instance,T_tour_geant& solution_initiale,int nb_max){
	int j,diff;
	int l[nom_instance.nbre_client+2];
	int tmp,i=0,r=0;
	exit:
	while(i!=nom_instance.nbre_client-2 and r<nb_max){
		j=i+2;
		while(j!=nom_instance.nbre_client){
			diff=
			-nom_instance.distance[solution_initiale.list_clt[i]][solution_initiale.list_clt[i+1]]
			-nom_instance.distance[solution_initiale.list_clt[j]][solution_initiale.list_clt[j+1]]
			+nom_instance.distance[solution_initiale.list_clt[i]][solution_initiale.list_clt[j]]
			+nom_instance.distance[solution_initiale.list_clt[i+1]][solution_initiale.list_clt[j+1]];
			if(diff<0){
				solution_initiale.cout=solution_initiale.cout+diff;
				for(int k=0;k<i+1;k++){
					l[k]=solution_initiale.list_clt[k];
				}
				l[i+1]=solution_initiale.list_clt[j];
				int h=j-1
				for(int k=i+2;k<j+1;k++){
					l[k]=solution_initiale.list_clt[h];
					h--;
				}
				for(int k=j+1;k<nom_instance.nbre_client+2;k++){
					l[k]=solution_initiale.list_clt[k];
				}
				for(int k=0;k<nom_instance.nbre_client+2;k++){
					solution_initiale.list_clt[k]=l[k];
				}
				r++;
				i=0;	
				goto exit;						
			}
			j++;
		}
		i++;
	}
}

void deplacement(T_instance& nom_instance,T_tour_geant& solution_initiale){
	int diff,ancien_diff;
	for(int i=1;i<nom_instance.nbre_client+1;i++){
		int delete1=nom_instance.distance[solution_initiale.list_clt[i-1]][solution_initiale.list_clt[i]];
		int delete2=nom_instance.distance[solution_initiale.list_clt[i]][solution_initiale.list_clt[i+1]] ;
		for(int j=1;j<nom_instance.nbre_client+1;j++){
			int delete3=nom_instance.distance[solution_initiale.list_clt[j]][solution_initiale.list_clt[j+1]];
			int add1=nom_instance.distance[solution_initiale.list_clt[j]][solution_initiale.list_clt[i]];
			int add2=nom_instance.distance[solution_initiale.list_clt[i]][solution_initiale.list_clt[j+1]];
			if(i!=j){
				diff = - delete1 - delete2 - delete3 + add1 + add2;
				if(diff<ancien_diff){
					solution_initiale.cout=solution_initiale.cout+diff;
					int client_choisi=solution_initiale.list_clt[i];
					for (int k=i;k<j;k++){
						solution_initiale.list_clt[i]=solution_initiale.list_clt[i+1];
					}
					solution_initiale.list_clt[j]=client_choisi;
					ancien_diff=diff;
				}
			}
		}
	}
}

void swap(T_instance& nom_instance,T_tour_geant& solution_initiale){
	int inf, sup;
	do{
		inf=rand() % nom_instance.nbre_client + 1 ;
		sup=rand() % nom_instance.nbre_client + 1 ;
	}while(inf == sup);
	int inter;
	inter=solution_initiale.list_clt[inf];
	solution_initiale.list_clt[inf]=solution_initiale.list_clt[sup];
	solution_initiale.list_clt[sup]=inter;
}
void split(T_instance& nom_instance, T_tour_geant& tournee,  T_solution& solution, T_list_label(&labels)[nbre_client_max + 1]){
	int j = 0;
	bool stop = false;
	T_label courant;
	T_label suivant;
	for (int i = 1; i < tournee.nbre_client - 1; i++) {
		j = i + 1;
		double charge = 0;
		int clt_i = tournee.list_clt[i];
		while (j < tournee.nbre_client + 1 && stop == false) {
			int clt_j = tournee.list_clt[j];
			int clt_j_1 = tournee.list_clt[j - 1];
			for (int k = 1; k < labels[clt_i].nbre_liste+1; k++) {
				courant = labels[clt_i].liste[k];
				for (int p = 1; p < nom_instance.nbre_type_vehicule + 1; p++) {
					if (courant.vehicule_dispo[p] > 0) {
						suivant.sommet = i;
						suivant.position_pere = k;
						for (int i = 1; i < nom_instance.nbre_type_vehicule + 1; i++) {
							suivant.vehicule_dispo[i] = courant.vehicule_dispo[i];
						}
						suivant.vehicule_dispo[p] = courant.vehicule_dispo[p] - 1;
						suivant.camion = p;
						int cout;
						if (j == i + 1) {
							cout = nom_instance.distance[1][clt_j] * nom_instance.cout_variable[p] + nom_instance.distance[clt_j][1] * nom_instance.cout_variable[p] + 2 * nom_instance.cout_fixe[p];
							suivant.cout = cout;
							charge = nom_instance.demande[clt_j];
						}
						else {
							cout = nom_instance.distance[clt_j_1][clt_j] * nom_instance.cout_variable[p] + nom_instance.distance[clt_j][1] * nom_instance.cout_variable[p] - nom_instance.distance[clt_j_1][1] * nom_instance.cout_variable[p] + nom_instance.cout_fixe[p];
							suivant.cout += cout;
							charge += nom_instance.demande[clt_j];
						}

						if (charge < nom_instance.Capacite[p]+1 && labels[clt_j].nbre_liste > 1) {
							labels[clt_j].liste.push_back(suivant);
						}
						else if (charge < nom_instance.Capacite[p]+1) {
							labels[clt_j].liste.push_back(suivant);
						}

					}

				}
			}
			j++;
		}
	}	
}
void grasp(T_instance& nom_instance,T_solution& solution){
	T_solution Voisin,meilleurVoisin;
	for (int i = 1; i < iter_max; i++) { 
		T_solution sol = solution;
		meilleurVoisin.cout = infini;
		int compteur = 0;
		while (compteur < nbre_voisin){
			Voisin = sol;
			int n1=rand() %nom_instance.nbre_client + 1;
			int n2=rand() %nom_instance.nbre_client + 1;
		//	swap(Voisin,n1,n2);
		//	split(nom_instance,Voisin);
			compteur++;
			if (Voisin.cout < meilleurVoisin.cout){
				meilleurVoisin = Voisin;
			}
		}
		solution = meilleurVoisin;
	}
}

int main(){
	T_instance mon_instance;
	lecture("HFVRP/Puy_De_Domes.txt",mon_instance);
	T_tour_geant solution_initiale;
	T_solution solution;

	heuristique_plus_proche_voisin(mon_instance,solution_initiale);
	//deplacement(mon_instance,solution_initiale);
	_2_opt_intra_tournee(mon_instance,solution_initiale,50);
	cout << "cout = "<< fixed << solution_initiale.cout;

	return 0;
}