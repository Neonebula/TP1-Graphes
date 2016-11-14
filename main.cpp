#include <fstream>
#include <iostream>
#include "constraintStruct.h"
#include "graphStruct.h"

#define FICHIER_GRAPHE "C01.txt"
using namespace std;

int main () {
    // Déclaration graphe
    t_graphe * G = new t_graphe ;

    // Lecture du graphe sur fichier
    ifstream fg ( FICHIER_GRAPHE ) ;

    // FICHIER_GRAPHE est une variables (ou autre)
    // donnant le nom du fichier
    fg >> G->nbTaches ;
    fg >> G->nbContraintes ;
    G->nbSommets = G->nbTaches +2 ;
    G->durees = new int [ G->nbTaches +1 ] ;

    // Tâches numérotées à partir de 1
    // G->durees[0] non utilisé
    G->contraintes = new t_contrainte [ G->nbTaches +1 ] ;

    // Contraintes numérotées à partir de 1
    // G->contraintes[0] non utilisé
    G->MAdj = new bool * [ G->nbSommets ] ;
    G->MVal = new int * [ G->nbSommets ] ;

    // Sommet 0 : début de projet
    // Sommets 1 à nbTaches : tâches
    // Sommet nbTaches+1(=nbSommets-1) : fin de projet

    for ( int s = 0 ; s < G->nbSommets ; s++ ) {
        G->MAdj[s] = new bool [ G->nbSommets ] ;
        G->MVal[s] = new int [ G->nbSommets ] ;
        for ( int extTerm = 0 ; extTerm < G->nbSommets ; extTerm++ ) {
            G->MAdj[s][extTerm] = false ;
        };
    };

    int tacheCourante ;
    int dureeTache ;
    for ( int t = 1 ; t <= G->nbTaches ; t++ ) {
        // 1 ligne pour chaque tâche
        fg >> tacheCourante ;
        fg >> dureeTache ;
        G->durees[tacheCourante] = dureeTache ;
    } ;

    for ( int c = 1 ; c <= G->nbContraintes ; c++ ) {
        // 1 ligne pour chaque contrainte
        fg >> G->contraintes[c].code ;
        switch ( G->contraintes[c].code ) {
            case 1 : // la tâche sommetX
            // ne peut commencer que si
            // la tâche sommetY est terminée
            fg >> G->contraintes[c].sommetX ;
            fg >> G->contraintes[c].sommetY ;
            break ;
        } ;
    } ;
    //Initialisation du la matrice a zero
    for(int ligne=0; ligne < G->nbSommets; ligne++){
        for (int colonne=0; colonne < G->nbSommets; colonne++){
            G->MVal[ligne][colonne] = 0;
        }
    }
    //On rempli les matrices sauf pour la premiere ligne
    for ( int c = 1 ; c <= G->nbContraintes ; c++ ) {
         int sommetY = G->contraintes[c].sommetX;
         int sommetX = G->contraintes[c].sommetY;

         G->MVal[sommetX][sommetY] = G->durees[sommetX] ;
         G->MAdj[sommetX][sommetY] = true;
    } ;
    //Cas premiere ligne
    for(int colonne = 1; colonne < G->nbSommets-1; colonne++) {
        int ligne = 1;
        bool trouve = false;
        do {
            if(G->MAdj[ligne][colonne]) {
                trouve = true;
            }
            ligne++;
        } while(!trouve && ligne < G->nbSommets);
        if(!trouve) {
            G->MVal[0][colonne] = 0;
            G->MAdj[0][colonne] = true;
        }
    }
    //Cas derniere colonne
    for(int ligne = 1; ligne < G->nbSommets-1; ligne++) {
        int colonne = 1;
        bool trouve = false;
        do {
            if(G->MAdj[ligne][colonne]) {
                trouve = true;
            }
            colonne++;
        } while(!trouve && colonne < G->nbSommets);
        if(!trouve) {
            G->MVal[ligne][6] = G->durees[ligne];
            G->MAdj[ligne][6] = true;
        }
    }
    // Affichage Matrice valeur
    cout << "Val" << "\t";
    for(int i = 0; i < G->nbSommets; i++) cout << i << "\t";
    cout << endl;
    for (int ligne = 0 ; ligne < G->nbSommets; ligne++){
        cout << ligne << "\t";
        for (int colonne = 0 ; colonne < G->nbSommets; colonne++)
        {
            if(G->MAdj[ligne][colonne]) {
                cout << G->MVal[ligne][colonne];

            }
            cout << "\t";

        }
        cout << endl;
    }

    cout << endl;

     // Affichage Matrice adjacence
    cout << "Adj" << "\t";
    for(int i = 0; i < G->nbSommets; i++) cout << i << "\t";
    cout << endl;
    for (int ligne = 0 ; ligne < G->nbSommets; ligne++){
        cout << ligne << "\t";
        for (int colonne = 0 ; colonne < G->nbSommets; colonne++)
        {
            cout << G->MAdj[ligne][colonne];
            cout << "\t";

        }
        cout << endl;
    }

    return 1 ;
}
