typedef struct {
    int nbTaches ;
    int nbContraintes ;
    int* durees ;
    t_contrainte* contraintes ;
    int nbSommets ;
    bool** MAdj ;
    int** MVal ;
    int* MRang;
    int* MCalPlusTot;
    int* MCalPlusTard;
} t_graphe ;
