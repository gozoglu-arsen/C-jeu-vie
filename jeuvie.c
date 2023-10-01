int randd(int a, int b){
    /*retourne une valeur entre a et b*/
    return rand() % (b-a) + a;
}



int valeur_tableau(int* tab, int val, int taille){
    // Retourne 1 si val est présent dans le tabelau.
    int i;
    for(i=0;i<taille;++i){
        if(tab[i] == val) return 1;
    }
    return 0; 
}
void obtenir_tableau_aleatoire(int* tab, int n, int max)
{

    /*Permet d'obtenir un tableau aléatoire
    tab contiendera des valeur aleatoire
    n est la taille du tableau
    max : valeur max souhaité
    */
    int i;
    for(i=0;i<n;++i)
    {
        tab[i] = randd(0,max);
    }
}
int place_dans_grille(int x, int y, int tx, int ty){
    /*  place_dans_grille() - fonction du projet LIFE
        Convertie des coordonnées (x,y) vers un nombre unique (la place de la case dans le tableau).
        Prend en arguments:
            - x et y, les coordonnées à convertir
            - tx et ty: les dimensions de la grille
        Retourne:
            - -1 en cas d'erreur
            -  */

    int cx = 0, cy = 0, n = tx - 1, c = 0;

    for(cx = 0; cx <= tx; ++cx){
        if(cx > tx || cy > ty) return -1;
        if(!y){
            if(cx == x && cy == y) return c;
        }
        if(c == n){
            n += tx;
            cx = 0;
            cy++;
        }
        c++;
        if(cx == x && cy == y) return c;
    }
}


int place_vers_coordonnes(int place, int*x, int *y, int tx, int ty){
    /*Convertie une place du tableau(une valeur unique) en coordonnées (x,y)
    Il s'agit du pointeur x,y
    */

    int x1, y1;
        for(x1=0;x1<tx;++x1)
        {
            for(y1=0;y1<ty; ++y1){
                if(place_dans_grille(x1,y1,tx,ty) == place){
                    *x = x1; *y = y1;
                    return 1;
                }
            }
        }
        return 0;
}
void actualiser_nombre_voisin(Case *c, int tx, int ty)
{
    int i, nombre_voisin = 0;
    int x,y;

    for(i=0; i<tx*ty; ++i){
        place_vers_coordonnes(i,&x,&y,tx,ty);
        nombre_voisin = 0;

    if((x-1) >= 0 && (y-1)>=0){
    if(c[place_dans_grille(x-1 , y-1 ,tx,ty)].vie == 1) nombre_voisin++;
    }
    if(y-1 >= 0){
        if(c[place_dans_grille(x , y-1 ,tx,ty)].vie == 1) nombre_voisin++;
    }
    if((x+1) <= tx && (y-1) >= 0){
        if(c[place_dans_grille(x+1 , y-1 ,tx,ty)].vie == 1) nombre_voisin++;
    }
    if((x-1) >= 0){
        if(c[place_dans_grille(x-1 , y ,tx,ty)].vie == 1) nombre_voisin++;
    }
    if((x+1) <= tx){
        if(c[place_dans_grille(x+1 , y ,tx,ty)].vie == 1) nombre_voisin++;
    }

    if((y+1) <= ty){
        if(c[place_dans_grille(x , y+1 ,tx,ty)].vie == 1) nombre_voisin++;
    }
    if( (x-1) >= 0 && (y+1) <= ty){
        if(c[place_dans_grille(x-1 , y+1 ,tx,ty)].vie == 1) nombre_voisin++;
    }
    if( (x+1) <= tx && (y+1) <= ty){
        if(c[place_dans_grille(x+1 , y+1 ,tx,ty)].vie == 1) nombre_voisin++;
    }


        c[i].nombre_voisin = nombre_voisin;
    }
}


void appliquer_regles(Case* c , int taille)
{
    /*Applique les regles du jeu*/
    int* in = malloc(sizeof(int)*taille);
    int* out = malloc(sizeof(int)* taille);
    int i;
    for(i=0; i<taille; ++i){
        in[i] = c[i].vie;
        out[i] = 0;
    }

    for(i=0; i<taille; ++i)
    {
        if(c[i].nombre_voisin == 3) out[i] = 1;
        else if(c[i].nombre_voisin == 2) out[i] = in[i];
        else if(c[i].nombre_voisin < 2 || c[i].nombre_voisin<3) out[i] = 0;

    }

    for(i=0; i<taille; ++i)
    {
        c[i].vie = out[i];
    }
}