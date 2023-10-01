#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <time.h>

typedef struct Case Case;
struct Case{

    SDL_Rect position;
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int nombre_voisin;
    int vie;
};
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
int main(int argc, char *argv[])
{
    int i,j=0,v=0;
    if(argc == 3){ // S'il y a bien 3 arguments spécifiés (l'ouverture du fichier en CLI compte comme un argument)
        for(j=1 ; j<3 ; ++j){
            for( i=0 ; i<strlen(argv[j]) ; ++i){
                if(!isdigit(argv[j][i])){ // Si les paramètres stockés dans le tableau de caractères ne sont pas des nombres
                    printf("Les arguments spécifiés ne sont pas corrects. Indiquez la taille souhaitée pour le tableau en chiffres.\n");
                    return 0;
                }
            }
        }
    }

    int x = (argc == 3) ? atoi(argv[1]) : 15; // Si l'utilisateur a spécifié la dimension du tableau, on alloue
    int y = (argc == 3) ? atoi(argv[2]) : 10; // à x et y les valeurs spécifiées, sinon 15 et 10 respectivement.
    int n = 25 * (x*y) / 100;
    int* tableau_aleatoire;
    int etat_evenement = 1;
    SDL_Surface *ecran = NULL, *splash_screen = NULL, *surface_jeu = NULL, *image_splash = NULL;
    SDL_Rect position_nul;
    Case* jeu;
    jeu = malloc(sizeof(Case) * (x*y));

    SDL_Init(SDL_INIT_VIDEO);

    ecran = SDL_SetVideoMode(590,400, 32, SDL_NOFRAME);
    surface_jeu = SDL_CreateRGBSurface(SDL_HWSURFACE, 29 * x - 4, 29 * y - 4, 32, 0, 0, 0, 0);
    splash_screen = SDL_CreateRGBSurface(SDL_HWSURFACE, 590, 400, 32, 0, 0, 0, 0);
    SDL_FillRect(splash_screen, NULL, SDL_MapRGB(ecran->format, 250, 250, 250));
    image_splash = SDL_LoadBMP("splash.bmp");
    SDL_WM_SetCaption("Jeu de la Vie", NULL);
    position_nul.x = 0;position_nul.y = 0;

    SDL_BlitSurface(image_splash, NULL, splash_screen, &position_nul);
    SDL_BlitSurface(splash_screen, NULL, ecran, &position_nul);
    SDL_Flip(ecran);
    SDL_Delay(2100);
    ecran = SDL_SetVideoMode(29 * x - 4, 29 * y - 4, 32, SDL_HWSURFACE);

    j=0;
    for(i=0;i<x*y;i++){
        // ici, j correspond à la valeur de x tandis que v correspond à la valeur y;
        jeu[i].position.x=j;
        jeu[i].position.y=v;
        jeu[i].position.w = 25;
        jeu[i].position.h = 25;
        jeu[i].x_min = j;
        jeu[i].y_min = v;
        jeu[i].x_max = j+25;
        jeu[i].y_max = v+25;

         if(j+29 >= 29*x-4){ // Quand on arrive à la fin de la ligne, on change de ligne.
            j=0;
            v+= 29;
        }
        else j+=29;

        SDL_FillRect(surface_jeu, &jeu[i].position, SDL_MapRGB(ecran->format, 255, 255, 255)); 
    }

    SDL_BlitSurface(surface_jeu, NULL, ecran, &position_nul);
    SDL_Flip(ecran); 

    SDL_Event evenement;

    while(etat_evenement)
    {   
        SDL_WaitEvent(&evenement);
        switch(evenement.type)
        {
        case SDL_QUIT: etat_evenement = 0; break;

        case SDL_KEYDOWN:
            if(evenement.key.keysym.sym == SDLK_a)
            {
                tableau_aleatoire = malloc(sizeof(int) * n);
                obtenir_tableau_aleatoire(tableau_aleatoire, n, x*y);
                for(i=0;i<n; ++i)
                {
                    jeu[tableau_aleatoire[i]].vie = 1;
                    SDL_FillRect(surface_jeu, &jeu[tableau_aleatoire[i]].position, SDL_MapRGB(ecran->format, 50, 50, 50));
                }

                SDL_BlitSurface(surface_jeu, NULL, ecran, &position_nul);
                SDL_Flip(ecran); 
                actualiser_nombre_voisin(jeu,x,y);
                free(tableau_aleatoire);
            }

            if(evenement.key.keysym.sym == SDLK_RIGHT)
            {
                appliquer_regles(jeu,x*y);
                for(i=0;i<x*y;++i){
                    if(jeu[i].vie == 1) SDL_FillRect(surface_jeu, &jeu[i].position, SDL_MapRGB(ecran->format, 50, 50, 50)); 
                    else if( jeu[i].vie == 0)  SDL_FillRect(surface_jeu, &jeu[i].position, SDL_MapRGB(ecran->format, 255, 255, 255));


                }

                SDL_BlitSurface(surface_jeu, NULL, ecran, &position_nul);
                SDL_Flip(ecran); 
                actualiser_nombre_voisin(jeu,x,y);
            }
        
        break;

        case SDL_MOUSEBUTTONUP:

        if(evenement.button.button == SDL_BUTTON_LEFT)
            {
                for(i=0; i<x*y;++i){
                    if(evenement.button.x >= jeu[i].x_min && evenement.button.x <= jeu[i].x_max && evenement.button.y >= jeu[i].y_min && evenement.button.y <= jeu[i].y_max){
                        jeu[i].vie = 1;
                        SDL_FillRect(surface_jeu, &jeu[i].position, SDL_MapRGB(ecran->format, 50, 50, 50));
                        SDL_BlitSurface(surface_jeu, NULL, ecran, &position_nul);
                        SDL_Flip(ecran);
                        actualiser_nombre_voisin(jeu, x,y);
                        printf("%d\n", jeu[i].nombre_voisin);
                    }

                }
            }
        break;
        }
    }

    SDL_FreeSurface(surface_jeu);
    SDL_Quit();

    return EXIT_SUCCESS;
}