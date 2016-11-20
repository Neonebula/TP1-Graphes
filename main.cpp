#include <fstream>
#include <iostream>
#include "constraintStruct.h"
#include "graphStruct.h"

#define FICHIER_GRAPHE "C01.txt"
using namespace std;

int getNbAntecedants(t_graphe *G, int tache) {
    int nbAntecedants = 0;
    for(int s = 0; s < G->nbSommets; s++) {
        if(G->MAdj[s][tache])
          nbAntecedants++;
    }
    return nbAntecedants;
}
void fillTabAntecedants(t_graphe *G, int tache, int *tabAntecedant) {
    int k = 0;
    for(int s = 0; s < G->nbSommets; s++) {
        if(G->MAdj[s][tache]) {
            tabAntecedant[k]=s;
            k++;
        }
    }
}

int getNbSucesseurs(t_graphe *G, int tache) {
    int nbSucesseurs = 0;
    for(int s = 0; s < G->nbSommets; s++) {
        if(G->MAdj[tache][s])
          nbSucesseurs++;
    }
    return nbSucesseurs;
}
void fillTabSucesseurs(t_graphe *G, int tache, int *tabSucesseurs) {
    int k = 0;
    for(int s = 0; s < G->nbSommets; s++) {
        if(G->MAdj[tache][s]) {
            tabSucesseurs[k]=s;
            k++;
        }
    }
}

void calculRang(t_graphe *G) {
    G->MRang = new int [ G->nbSommets ];
    //On part du principe que 0 est le premier sommet donc sans antécedant et par consequant le rang 0
    G->MRang[0] = 0;

    //On initialise les autres rangs a -1 pour les comparaisons
    for(int r = 1; r < G->nbSommets-1; r++){
      G->MRang[r] = -1;
    }

    for (int ligne = 0; ligne < G->nbSommets-1; ligne++){
      for (int colonne = 1; colonne < G->nbSommets; colonne++){
        if(G->MAdj[ligne][colonne]){
          int nbAntecedants = getNbAntecedants(G, colonne);
          if(nbAntecedants==1){
            G->MRang[colonne] = G->MRang[ligne] + 1;
          }
          else{
            int tabAntecedant[nbAntecedants];
            fillTabAntecedants(G, colonne, tabAntecedant);
            int rangMax = G->MRang[tabAntecedant[0]];
            for(int a = 1; a < nbAntecedants; a++){
              if(G->MRang[tabAntecedant[a]]>rangMax) rangMax=G->MRang[tabAntecedant[a]];
            }
            G->MRang[colonne] = rangMax+1;
          }
        }
      }
    }
}

void calculCalendrier(t_graphe * G){
  G->datePlusTot = new int[G->nbSommets];
  G->datePlusTard = new int[G->nbSommets];

  for (int t = 0; t < G->nbSommets; t++) {
      G->datePlusTot[t]=0;
      G->datePlusTard[t]=0;
  }
  //Calcul de la date au plus tot
  for (int ligne = 1; ligne < G->nbSommets; ligne++){
    for (int colonne = 1; colonne < G->nbSommets; colonne++){
      if(G->MAdj[ligne][colonne]){
        int nbAntecedants = getNbAntecedants(G, colonne);
        if(nbAntecedants==1){
          G->datePlusTot[colonne] = G->durees[ligne];
        }
        else{
          int tabAntecedant[nbAntecedants];
          fillTabAntecedants(G, colonne, tabAntecedant);
          int dureeMax = G->durees[tabAntecedant[0]];
          int tacheLaPlusLongue = 0;
          for(int a = 1; a < nbAntecedants; a++){
            if( G->durees[tabAntecedant[a]]>dureeMax)
            {
              dureeMax = G->durees[tabAntecedant[a]];
              tacheLaPlusLongue = tabAntecedant[a];
            }
          }
          G->datePlusTot[colonne] = dureeMax + G->datePlusTot[tacheLaPlusLongue];
        }
      }
    }
  }

  //Calcul de la date au plus tard

}


void initGraphe(t_graphe * G){
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
      int nbAntecedants = 0;
      G->durees[tacheCourante] = dureeTache ;
  } ;

  for ( int c = 1 ; c <= G->nbContraintes ; c++ ) {
      // 1 ligne pour chaque contrainte
      fg >> G->contraintes[c].code ;
      switch ( G->contraintes[c].code ) {
          case 1 : // la t�che sommetX
          // ne peut commencer que si
          // la t�che sommetY est termin�e
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
  //On vérifie les sommets qui n'ont pas d'antecedants
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
  //On vérifie les sommets qui n'ont pas de successeurs
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
}

void afficheMatriceAdjacence(t_graphe * G){
  cout << "Adj" << "\t";
  for(int ligne = 0; ligne < G->nbSommets; ligne++) cout << ligne << "\t";
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
}

void afficheMatriceValeurs(t_graphe * G){
  cout << "Val" << "\t";
  for(int ligne = 0; ligne < G->nbSommets; ligne++) cout << ligne << "\t";
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
}

int main () {
    // Déclaration graphe
    t_graphe * G = new t_graphe ;
    initGraphe(G);
    afficheMatriceAdjacence(G);
    cout << "\n" << endl;
    afficheMatriceValeurs(G);
    cout << "\n" << endl;
    calculRang(G);
    cout << "\n" << endl;
    calculCalendrier(G);

    return 1 ;
}
