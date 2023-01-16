#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define NBLIG 6
#define NBCOL 7

const char PION_A = 'X';
const char PION_B = 'O';
const char VIDE = ' ';
const char INCONNU = ' ';
const int COLONNE_DEBUT = NBCOL / 2;
const char GAUCHE = 'q';
const char DROITE ='d';

typedef char tab[NBLIG][NBCOL];

void initGrille(tab);
void afficher(tab,char,int);
bool grillePleine(tab);
void jouer(tab,char,int *,int *);
int choisirColonne(tab,char,int);
int trouverLigne(tab,int);
bool estVainqueur(tab,int,int);
void finDePartie(char);


int main(void)
{
    char vainqueur;
    int ligne, colonne;
    tab grille;

    initGrille(grille);
    vainqueur = INCONNU;
    afficher(grille, PION_A, COLONNE_DEBUT);


    while ((vainqueur == INCONNU) && (!grillePleine(grille)))
    {
        jouer(grille, PION_A, &ligne, &colonne);
        afficher(grille, PION_B, COLONNE_DEBUT);
        if (estVainqueur(grille, ligne, colonne))
        {
            vainqueur = PION_A;
        }
        else if (!grillePleine(grille))
        {
            jouer(grille, PION_B, &ligne, &colonne);
            afficher(grille, PION_A, COLONNE_DEBUT);
            if (estVainqueur(grille, ligne, colonne))
            {
                vainqueur = PION_B;
            }
        }
    }
    finDePartie(vainqueur);
    
}


//Met dans toutes les cases de la grille le caracteère VIDE (" ")
void initGrille(tab grille1)
{
    int i, j;
    for (i = 0; i < NBLIG; i++)
    {
        for (j = 0; j < NBCOL; j++)
        {
            grille1[i][j] = VIDE;
        }
    }
}


//Affiche la grille avec ses éléments et également le jeton du joueur actuellement en train de jouer au dessus
void afficher(tab grille, char pion, int colonne)
{
    system("cls");
    int i, j;
    if (colonne == 0)
    {
        printf("  %c\n", pion);//Si c'est sur la première colonne on met 2 espaces pour ne pas créer un décalage et on affiche le jeton
    }
    else
    {
        printf("  ");
        for (i = 0; i < colonne; i++)//Sinon on met 2 espaces puis on rajoute 4 espaces à chaque instance de la boucle puis on fini avec le jeton
        {
            printf("    ");
        }
        printf("%c\n", pion);
    }

    for (i = 0; i < NBLIG; i++)//Affichage de la grille
    {
        for (j = 0; j < NBCOL; j++)
        {
            if (j == 0)
            {
                printf("| %c |", grille[i][j]);//On affiche "|   |" seulement au premier élement d'une ligne pour ne pas afficher 2 "|" d'affilé
            }
            else
            {
                printf(" %c |", grille[i][j]);
            }
        }
        if (i!=NBLIG-1) {
            printf("\n+---+---+---+---+---+---+---+\n");
        }
        else {
            printf("\n-----------------------------\n");//Affiche une ligne droite si on est à la dernière ligne 
        }
    }
}


//Renvoie true si la grille est pleine, false sinon
bool grillePleine(tab grille)
{
    bool resultat = true;
    int i, j;
    for (i = 0; i < NBLIG; i++)
    {
        for (j = 0; j < NBCOL; j++)
        {
            if (grille[i][j] == VIDE)
            {
                resultat = false;
            }
        }
    }
    printf("%d",resultat);
    return resultat;
}


//procédure principale, réutilise chosirColonne pour savoir la colonne choisie par le joueur puis utilise trouver ligne pour savoir si la colonne a au moins une case vide, si oui on met dans
//la première case vide sinon on reboucle et réutilise chosiirColonne
void jouer(tab grille,char pion,int *ligne,int *colonne)
{
    bool colonnePleine = false;

    while(!colonnePleine)
    {
        *colonne = 0;
        *ligne = 0;
        
        *colonne = choisirColonne(grille,pion,COLONNE_DEBUT);
        *ligne = trouverLigne(grille, *colonne);
        
        if (*ligne != -1 )
        {
            colonnePleine = true;
        }
    }
    grille[*ligne][*colonne]=pion;
}


//Le joueur choisi sa colonne avec "q" et "d", on vérifie bien qu'il marque seulement "q","d" et " " sinon rien ne se passe et il doit juste rejouer
int choisirColonne(tab grille,char pion,int colonne) {
    char reponse,poubelle;//variable poubelle pour absorber le caractere entree dans le scanf
    int indice = colonne;
    afficher(grille,pion,colonne);

    do {
        printf("Deplacement gauche : 'q'\nDeplacement droite : 'd'\nEntrer le jeton : 'espace'\nValider son choix : 'entree'\n");//Affichage des commandes possibles
        scanf("%c%c",&reponse,&poubelle);
    } while (reponse!=VIDE && reponse!=GAUCHE && reponse!=DROITE);

    while (reponse!=VIDE) {
        do {
            if (reponse==GAUCHE) {
                if (indice>0) {
                    indice--;
                }
            }
            else {
                if (indice<6) {
                    indice++;
                }
            }
        } while ((reponse!=VIDE && reponse!=GAUCHE && reponse!=DROITE));
        afficher(grille,pion,indice);
        do {
            printf("Deplacement gauche : 'q'\nDeplacement droite : 'd'\nEntrer le jeton : 'espace'\nValider son choix : 'entree'\n");//Affichage des commandes possibles dans la boucle
            scanf("%c%c",&reponse,&poubelle);//On absorbe le caractere Entree avec poubelle
        } while ((reponse!=VIDE && reponse!=GAUCHE && reponse!=DROITE));
    }

    return indice;
}


//Donne l'indice de la première ligne disponible sur une colonne, si elle est pleine on renvoie -1
int trouverLigne(tab grille, int colonne)
{
    int compteur = NBLIG-1;
    int resultat = -1;
    while (resultat == -1 && compteur >= 0)
    {
        if (grille[compteur][colonne] == VIDE)
        {
            resultat = compteur;
        }
        compteur--;
    }
    return resultat;
}


//Fonction de test, test si il y a 4 jetons rentrés en paramètres d'alignés 
bool estVainqueur(tab grille, int ligne, int colonne)
{
    char pion;
    bool victoire;
    int nbrPion, i;
    
    pion = grille[ligne][colonne];
    victoire = false;
    
    //Test si il y a 4 jetons alignés verticalement
    i= 1;
    nbrPion = 1;
    while(grille[ligne + i][colonne] == pion && ligne + i < NBLIG)
    {
        nbrPion++;
         i++;
    }
    i= 1;
    while(grille[ligne - i][colonne] == pion && ligne  - i >=0)
    {
        nbrPion++;
         i++;
    }
    if(nbrPion == 4)
     {
         victoire = true;
     }

    //Test si il y a 4 jetons alignés horizontalement
    if(!victoire)
    {
        i= 1;
        nbrPion = 1;
        while(grille[ligne][colonne + i] == pion && colonne + i < NBCOL)
        {
            nbrPion++;
            i++;
        }
        i= 1;
        while(grille[ligne][colonne - i] == pion && colonne  - i >=0)
        {
            nbrPion++;
            i++;
        }
        
        if(nbrPion == 4)
        {
            victoire = true;
        }
    }
    
    
    
    //Test si il y a 4 jetons alignés en diagonale
    if(!victoire)
    {
        i = 1;
        nbrPion = 1;
        while(grille[ligne - i][colonne + i] == pion && ligne - i >= 0 && colonne + i < NBCOL)
        {
            nbrPion++;
            i++;
        }
        
        i= 1;
        while(grille[ligne + i][colonne - i] == pion && ligne + i < NBLIG && colonne - i >= 0)
        {
            nbrPion++;
            i++;
        }
        
        if(nbrPion == 4)
        {
            victoire = true;
        }
    }
    
    if(!victoire)
    {
        i= 1;
        nbrPion = 1;
        while(grille[ligne + i][colonne + i] == pion && ligne + i < NBLIG && colonne + i < NBCOL)
        {
            
            nbrPion++;
            i++;
        }
        
        i= 1;
        while(grille[ligne - i][colonne - i] == pion && ligne - i >= 0 && colonne - i >= 0)
        {
            
            nbrPion++;
            i++;
        }
        if(nbrPion == 4)
        {
            victoire = true;
        }
    }
    return victoire;
}


//Affiche les résultats en fonction de variable vainqueur qui peut être soit PION_A, PION_B ou INCONNU
void finDePartie(char vainqueurfin) {
    if (vainqueurfin == PION_A) {
        printf("le vainqueur est le joueur A");
    }
    else if (vainqueurfin== PION_B) {
        printf("le vainqueur est le joueur B");
    }
    else {
        printf("Pas de vainqueur, egalite");
    }
}