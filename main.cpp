
#include <fstream>
#define FICHIER_GRAPHE �C01.txt�
#include <iostream>
#include "graphStruct.h"
#include "constraintStruct.h"

int main () {
    // D�claration graphe
    t_graphe * G = new t_graphe ;

    // Lecture du graphe sur fichier
    ifstream fg ( FICHIER_GRAPHE ) ;

    // FICHIER_GRAPHE est une variables (ou autre)
    // donnant le nom du fichier
    fg >> G->nbTaches ;
    fg >> G->nbContraintes ;
    G->nbSommets = G->nbTaches +2 ;
    G->durees = new int [ G->nbTaches +1 ] ;

    // T�ches num�rot�es � partir de 1
    // G->durees[0] non utilis�
    G->contraintes = new t_contrainte [ G->nbTaches +1 ] ;

    // Contraintes num�rot�es � partir de 1
    // G->contraintes[0] non utilis�
    G->MAdj = new bool * [ G->nbSommets ] ;
    G->MVal = new int * [ G->nbSommets ] ;

    // Sommet 0 : d�but de projet
    // Sommets 1 � nbTaches : t�ches
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
        // 1 ligne pour chaque t�che
        fg >> tacheCourante ;
        fg >> dureeTache ;
        G->durees[tacheCourante] = dureeTache ;
    } ;
    for ( int c = 1 ; c <= G->nbContraintes ; c++ ) {
        // 1 ligne pour chaque contrainte
        fg >> G->contraintes[c]->code ;
        switch ( G->contraintes[c]->code ) {
            case 1 : // la t�che sommetX
            // ne peut commencer que si
            // la t�che sommetY est termin�e
            fg >> G->contraintes[c]->sommetX ;
            fg >> G->contraintes[c]->sommetY ;
            break ;
        } ;
    }

    return 1 ;
}
