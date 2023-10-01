int randd(int a, int b);
int valeur_tableau(int* tab, int val, int taille);
void obtenir_tableau_aleatoire(int* tab, int n, int max);
int place_dans_grille(int x, int y, int tx, int ty);
int place_vers_coordonnes(int place, int*x, int *y, int tx, int ty);
void actualiser_nombre_voisin(Case *c, int tx, int ty);
void appliquer_regles(Case* c , int taille);