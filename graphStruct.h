typedef struct {
    int nbTaches ;
    int nbContraintes ;
    int* durees ;
    t_contrainte* contraintes ;
    int nbSommets ;
    bool** MAdj ;
    int** MVal ;
    int* MRang;
    int* datePlusTot;
    int* datePlusTard;
    int* marges;
} t_graphe ;
