#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _noeud {
    char *mot;               // Mot à inserer dans l'arbre 
    int nb_occ;              // Nombre d'occurrences du mot (priorité)
    struct _noeud *fg;       // Fils gauche
    struct _noeud *fd;       // Fils droit
} Noeud, *ABRnois;

typedef struct _cell {
    Noeud *n;                // Pointeur vers le nœud de l’ABRnois
    struct _cell *suivant;  // Élément suivant de la liste
} Cell, *Liste;

//Crée sur le tas un nouveau Noeud avec une occurence initialisée à 1.
Noeud * alloue_noeud(char * mot) {
    Noeud * nouv_noeud = (Noeud*)malloc(sizeof(Noeud));
    if(nouv_noeud == NULL){
        return NULL;
    }
    nouv_noeud->mot = (char*)malloc(strlen(mot)+1);
    if( nouv_noeud->mot == NULL){
        free(nouv_noeud);
        return NULL;
    }
    strcpy(nouv_noeud->mot, mot);
    nouv_noeud->nb_occ = 1;
    nouv_noeud->fg = NULL;
    nouv_noeud->fd = NULL;
    
    return nouv_noeud;
}
//crée une représentation graphique d’un arbre de type ABRnois dans un fichier pdf

//effectue une rotation gauche sur l’arbre *A lorsque celui-ci possède un enfant droit non nul
void rotation_gauche(ABRnois * A){
    if( *A == NULL || (*A)->fd == NULL ){ 
        return; 
    }
      
    ABRnois tmp = (*A)->fd;  
    (*A)->fd = (tmp)->fg;//on acroche A avec fd de fg
    tmp->fg = (*A);//A doit devenir le fg de fd 
    *A = tmp; //MAJ
}

//effectue une rotation droite sur l’arbre *A lorsque celui-ci possède un enfant droit non nul
void rotation_droite(ABRnois * A){
    if( *A == NULL || (*A)->fg == NULL ){ 
        return; 
    }
      
    ABRnois tmp = (*A)->fg;  
    (*A)->fg = (tmp)->fd;
    tmp->fd = (*A);//A doit devenir le fd de fg
    *A = tmp; //MAJ
}
/*Insère un mot dans l'arbre ABRnois.
Si le mot existe déjà : incrémente nb_occ.
Sinon : insère un nouveau nœud avec nb_occ = 1, puis remonte pour rétablir la priorité.*/
// int insert_ABRnois(ABRnois * A, char * mot){
//     if(*A == NULL ){//si l'arbre vid
//         *A = alloue_noeud(mot);
//         return( *A != NULL);//1 si c'est bon sinnon 0
//     }else if( *A->mot == mot){
//         *A->nb_occ +=1 ;
//         return 1;
//     }else if( strcmp(*A->mot, mot) < 0){//strcmp ppur avoir l'ordre lexico
//         return insert_ABRnois(&(*A)->fg, mot);
//     }
//     return insert_ABRnois(&(*A)->fd, mot);
// }
// int insert_ABRnois(ABRnois *A, char *mot) {
//     if (*A == NULL) {
//         *A = alloue_noeud(mot);
//         return (*A != NULL); // 1 succes, 0  échec
//     }

//     int cmp = strcmp((*A)->mot, mot);

//     if (cmp == 0) {
//         // Le mot existe déjà
//         (*A)->nb_occ += 1;
//         return 1;
//     } else if (cmp > 0) {
//         // mot < (*A)->mot → aller à gauche
//         int res = insert_ABRnois(&((*A)->fg), mot);
//         if (res && (*A)->fg->nb_occ > (*A)->nb_occ) {
//             rotation_droite(A);
//         }
//         return res;
//     } else {
//         // mot > (*A)->mot → aller à droite
//         int res = insert_ABRnois(&((*A)->fd), mot);
//         if (res && (*A)->fd->nb_occ > (*A)->nb_occ) {
//             rotation_gauche(A);
//         }
//         return res;
//     }
// }

int insert_ABRnois(ABRnois *A, char *mot) {
    if (*A == NULL) {
        *A = alloue_noeud(mot);
        return (*A != NULL);
    }

    int cmp = strcmp((*A)->mot, mot);

    if (cmp == 0) {
        // le mot existe déjà
        (*A)->nb_occ += 1;

        // Verifier si l’enfant gauche ou droit doit remonter
        // Si fg existe et est plus prioritaire que A → faire remonter fg
        if ((*A)->fg && (*A)->fg->nb_occ > (*A)->nb_occ)
            rotation_droite(A);
        // Si fd existe et est plus prioritaire que A → faire remonter fd
        if ((*A)->fd && (*A)->fd->nb_occ > (*A)->nb_occ)
            rotation_gauche(A);

        return 1;
    } else if (cmp > 0) {
        int res = insert_ABRnois(&((*A)->fg), mot);
        if (res && (*A)->fg->nb_occ > (*A)->nb_occ) {
            rotation_droite(A);
        }
        return res;
    } else {
        int res = insert_ABRnois(&((*A)->fd), mot);
        if (res && (*A)->fd->nb_occ > (*A)->nb_occ) {
            rotation_gauche(A);
        }
        return res;
    }
}



Noeud *extraire_noeud_racine(ABRnois *A) {
    if (A == NULL || *A == NULL) return NULL;

    // On garde un pointeur vers le nœud à extraire (celui pointé par A au départ)
    Noeud *extrait = *A;
    ABRnois *courant = A;  // pointeur vers le pointeur à modifier

    //printf("[DEBUG] Début extraction : %s (%d)\n", extrait->mot, extrait->nb_occ);

    while ((*courant)->fg != NULL || (*courant)->fd != NULL) {
        // Cas : un seul enfant droit
        if ((*courant)->fg == NULL) {
            //printf("[DEBUG] Rotation gauche sur %s (%d)\n", (*courant)->mot, (*courant)->nb_occ);
            rotation_gauche(courant);
        }
        // Cas : un seul enfant gauche
        else if ((*courant)->fd == NULL) {
            //printf("[DEBUG] Rotation droite sur %s (%d)\n", (*courant)->mot, (*courant)->nb_occ);
            rotation_droite(courant);
        }
        // Cas : deux enfants
        else {
            int g = (*courant)->fg->nb_occ;
            int d = (*courant)->fd->nb_occ;

            if (g > d) {
                //printf("[DEBUG] Priorité gauche > droite → rotation droite\n");
                rotation_droite(courant);
            } else {
                //printf("[DEBUG] Priorité droite >= gauche → rotation gauche\n");
                rotation_gauche(courant);
            }
        }

        // Tant qu’on fait des rotations, le nœud extrait reste à sa place dans l’arbre
        // On continue à le suivre 
        if (*courant == extrait) {
            // on fait rien , toujours dans le meme noeud 
        } else if ((*courant)->fg == extrait || (*courant)->fd == extrait) {
            // On avance dans la branche ou il a été poussé
            if ((*courant)->fg == extrait)
                courant = &((*courant)->fg);
            else
                courant = &((*courant)->fd);
        } else {
            // 
        }
    }
    //printf("[DEBUG] Nœud à extraire devenu feuille : %s (%d)\n", extrait->mot, extrait->nb_occ);
    *courant = NULL;  // suppression
    return extrait;
}



int extrait_priorite_max(ABRnois *A, Liste *lst) {
    if (A == NULL || *A == NULL) return 0;

    int max_prio = (*A)->nb_occ; // priorité de la racine
    int nb_extraits = 0;

    while (*A != NULL && (*A)->nb_occ == max_prio) {
        //printf("Racine : %s (%d)\n", (*A)->mot, (*A)->nb_occ);

        // Extraire la racine (priorité max)
        Noeud *extrait = extraire_noeud_racine(A);
        if (extrait == NULL) return 0;
        //printf("Mot extrait : %s\n", extrait->mot);

        // Allouer une cellule pour la liste chaînée
        Cell *new_cell = malloc(sizeof(Cell));
        if (new_cell == NULL) {//si echec on free 
            free(extrait->mot);
            free(extrait);
            return 0;
        }

        new_cell->n = extrait;
        new_cell->suivant = NULL;

        // Insertion triee dans la liste chaînée 
        Cell **courant = lst;
        while (*courant != NULL && strcmp((*courant)->n->mot, extrait->mot) < 0) {
            courant = &((*courant)->suivant);
        }
        new_cell->suivant = *courant;
        *courant = new_cell;

        nb_extraits++;
    }

    return nb_extraits;
}
///////

// afficher l'arbre 
void ecrireDebut(FILE *f) {
    fprintf(f, "digraph Arbre {\n");
    fprintf(f, "node [shape=record];\n");
    fprintf(f, "edge [dir=none];\n");
}

void ecrireArbre(FILE *f, ABRnois a) {
    if (a != NULL) {
        fprintf(f, "\"%s\\n%d\";\n", a->mot, a->nb_occ);
        if (a->fg != NULL) {
            fprintf(f, "\"%s\\n%d\" -> \"%s\\n%d\";\n", a->mot, a->nb_occ, a->fg->mot, a->fg->nb_occ);
            ecrireArbre(f, a->fg);
        }
        if (a->fd != NULL) {
            fprintf(f, "\"%s\\n%d\" -> \"%s\\n%d\";\n", a->mot, a->nb_occ, a->fd->mot, a->fd->nb_occ);
            ecrireArbre(f, a->fd);
        }
    }
}

void ecrireFin(FILE *f) {
    fprintf(f, "}\n");
}

int exporte_arbre(char *nom_pdf, ABRnois A) {
    char *dotfile = "temp.dot";

    FILE *out = fopen(dotfile, "w");
    if (!out) {
        perror("Erreur ouverture fichier dot");
        return 0;
    }

    ecrireDebut(out);
    ecrireArbre(out, A);
    ecrireFin(out);
    fclose(out);

    // commande system pour générer le PDF
    int len = strlen(dotfile) + strlen(nom_pdf) + 20;
    char *cmd = malloc(len);
    if (!cmd) return 0;

    sprintf(cmd, "dot -Tpdf %s -o %s", dotfile, nom_pdf);
    system(cmd);
    free(cmd);

    return 1;
}

//fonction pour liberer 
void free_arbre(ABRnois A) {
    if (A == NULL) return;
    free_arbre(A->fg);
    free_arbre(A->fd);
    free(A->mot);
    free(A);
}

    
void affiche_prefixe(ABRnois a) {
       
    if (a != NULL) {
        printf("%s (%d)\n", a->mot, a->nb_occ);
        affiche_prefixe(a->fg);
        affiche_prefixe(a->fd);
     }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage : %s frequents.txt corpus1.txt [corpus2.txt ...] [-g] [-n p]\n", argv[0]);
        return 1;
    }

    // prise en compte des arguments 
    char *nom_fichier_sortie = argv[1];
    int export_pdf = 0;
    int limite_mots = -1;

    char *fichiers_corpus[argc];
    int nb_fichiers_corpus = 0;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-g") == 0) {
            export_pdf = 1;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            limite_mots = atoi(argv[++i]);
        } else {
            fichiers_corpus[nb_fichiers_corpus++] = argv[i];
        }
    }

    // construction de l'arbee 
    ABRnois arbre = NULL;
    int total_mots = 0;
    int insertion_num = 1;

    for (int i = 0; i < nb_fichiers_corpus; i++) {
        FILE *f = fopen(fichiers_corpus[i], "r");
        if (!f) {
            perror(fichiers_corpus[i]);
            continue;
        }
        char mot[256];
        while (fscanf(f, "%255s", mot) == 1) {
            insert_ABRnois(&arbre, mot);
            total_mots++;

            if (export_pdf) {
                char nom[64];
                sprintf(nom, "insertion_%d.pdf", insertion_num++);
                exporte_arbre(nom, arbre);
            }
        }
        fclose(f);
    }

    // extraction 
    FILE *out = fopen(nom_fichier_sortie, "w");
    if (!out) {
        perror(nom_fichier_sortie);
        return 1;
    }

    Liste lst = NULL;
    int nb_extraits = 0;
    int suppression_num = 1;
    int stop_apres_ce_nb_occ = -1;

    while (arbre != NULL) {
        int nb = extrait_priorite_max(&arbre, &lst);
        if (nb == 0) break;

        int current_priority = lst->n->nb_occ;

        for (Cell *cur = lst; cur != NULL; cur = cur->suivant) {
            double freq = 100.0 * cur->n->nb_occ / total_mots;
            fprintf(out, "%s %.2f  (%d) \n", cur->n->mot, freq, cur->n->nb_occ);
            nb_extraits++;
        }
        if (export_pdf) {
            char nom[64];
            sprintf(nom, "suppression_%d.pdf", suppression_num++);
            exporte_arbre(nom, arbre);
        }

        while (lst != NULL) {
            Cell *tmp = lst;
            lst = lst->suivant;
            free(tmp->n->mot);
            free(tmp->n);
            free(tmp);
        }

        if (limite_mots > 0 && nb_extraits >= limite_mots) {
            stop_apres_ce_nb_occ = current_priority;
            break;
        }
    }

    // si -n 
    while (arbre != NULL && stop_apres_ce_nb_occ != -1 && (arbre)->nb_occ == stop_apres_ce_nb_occ) {
        int nb = extrait_priorite_max(&arbre, &lst);
        if (nb == 0) break;

        for (Cell *cur = lst; cur != NULL; cur = cur->suivant) {
            double freq = 100.0 * cur->n->nb_occ / total_mots;
            fprintf(out, "%s %.2f\n", cur->n->mot, freq);
        }

        if (export_pdf) {
            char nom[64];
            sprintf(nom, "suppression_%d.pdf", suppression_num++);
            exporte_arbre(nom, arbre);
        }

        while (lst != NULL) {
            Cell *tmp = lst;
            lst = lst->suivant;
            free(tmp->n->mot);
            free(tmp->n);
            free(tmp);
        }
    }

    // free 
    fclose(out);
    free_arbre(arbre);
    return 0;
}
