/**
*\file offre.c
*\brief Fichier contenant le code relatif aux Offres
*\author Erwan Mechoud
*\author Samuel pinto
*/


#include "offre.h"

/*
###################################################
    Partie File
###################################################
*/

/**
*\brief calcule la longueur d'une liste de devis
*\param [ in ] l la liste de devis
*\return la longueur dela liste
*/
int longListe(ListeDevis l)
{
    if(l==NULL)
        return 0;
    return longListe(l->suiv)+1;
}

/**
*\brief insère un devis au début d'une liste de devis
*\param [ in , out ] l la liste de devis
*\param [ in ] devis le devis à inserer dans la liste
*\param new maillon crée dans lequel on va mette le devis et qu'on va ajouter à la liste
*\return la liste mise à jour avec le nouveau devis au début
*/

//priver
ListeDevis insererEnTete(ListeDevis l, Devis devis){
    Maillon *new;
    new=(Maillon *)malloc(sizeof(Maillon));
    if (new==NULL){
        printf("Erreur: creation maillon (enliste)\n");
        exit(1);
    }
    new->devis=devis;
    new->suiv=l;
    l=new;
    return l;
}

/**
*\brief permet d'insérer (à l'aide de la fonction : insererEnTete) un devis dans la liste des devis en gardant un ordre alphabétique
*\param [ in , out ] l la liste de devis
*\param [ in ] devis le devis à inserer dans la liste
*\return la liste mise à jour avec le nouveau devis au bon endroit
*/
ListeDevis enliste(ListeDevis l, Devis devis)
{
    if(l==NULL)
        return insererEnTete(l, devis);
    if(strcmp(devis.entreprise , ((l->devis).entreprise))<0)
        return insererEnTete(l, devis);
    l->suiv=enliste(l->suiv, devis);
    return l;
}

/**
*\brief dit si une liste de devis est null ou non
*\param [ in ] l la liste de devis
*\return 0 si la liste est null et 1 dans le cas contraire
*/
//vrai si 0 sinon 1
int estVide(ListeDevis l){
    if (l==NULL) return 0;
    return 1;
}

/**
*\brief permet de supprimer le 1er devis d'une liste
*\param [ in , out ] l la liste de devis
*\param old maillon qui permet de libérer l'espace mémoire
*\return la liste sans son 1er devis
*/
ListeDevis delTete(ListeDevis l){
    Maillon *old;
    if (l==NULL){
        printf("Erreur: impossible de delister une liste vide");
        return l;
    }
    old=l;
    l=l->suiv;
    free(old);
    return l;
}

/**
*\brief supprime le devis d'une certaine entreprise dans une liste
*\param [ in ] l la liste de devis
*\param start liste auxiliaire crée pour supprimer le devis
*\param entreprise nom de l'entreprise dont on veut supprimer le devis
*\return une liste de devis mis à jour sans le devis qu'on voulait enlever
*/
/*
Liste deliste(Liste l, char *entreprise){
    Liste start=l;
    if (l==NULL){
        printf("Erreur: impossible de delister une liste vide\n");
        return l;
    }
    if (strcmp((start->devis).entreprise, entreprise)==0)
        return delTete(l);
    while (strcmp(((l->suiv)->devis).entreprise, entreprise)==0){
        if (l->suiv==NULL){
            printf("Erreur: entreprise non présente dans les donnée\n");
            return start;
        }
        l=l->suiv;
    }
}
*/

/**
*\brief ajoute dans le tableau une tache qui n'y était pas
*\param [ in , out] tab tableau contenant les différentes taches à réaliser
*\param [ in , out ] size taille logique du tableau des taches
*\param [ in , out] tMax taille physique du tableau des taches
*\param [ in , out] travaux nom de la tache que l'on ajoute
*\param new Offre crée pour mettre la nouvelle tache dedans
*\param aux Offre auxiliaire crée pour agrandir la taille physique du tableau
*\return la nouvelle taille logique du tableau
*/
/*
Fonction qui ajoute un devis dans le tableau quand le travail n'est pas encore présent dans le tableau
*/
int newOffre(Offre *tab[], int size, int *tMax, char *travaux){
    Offre *new, **aux;
    new=(Offre *)malloc(sizeof(Offre));
    if (new==NULL){
        printf("Erreur: création structure offre\n");
        exit(1);
    }
    if (size==(*tMax)){
        (*tMax)+=5;
        aux=(Offre **)realloc(tab, (*tMax)*sizeof(Offre *));
        if (aux==NULL){
            printf("Erreur: création tableaux aux\n");
            exit(1);
        }
        tab=aux;
        free(aux);
    }
    strcpy(new->travaux, travaux);
    new->ldevis=NULL;
    tab[size]=new;
    size+=1;
    return size;
}

/**
*\brief ajoute un devis dans la bonne tache
*\param [ in , out ] of tableau des taches
*\param [ in , out ] size taille logique du tableau des taches
*\param [ in , out] max taille physique du tableau des taches
*\param [ in , out] travaux nom de la tache que l'on ajoute
*\param [ in , out ] devis le devis que l'on souhaite ajouter
*\param i compteur pour faire une recherche de la bonne tache
*\return la nouvelle taille logique du tableau des taches
*/
int addDevis(Offre *of[], int size, int *max, char *travaux, Devis devis){
    for (int i=0; i<size; i++){
        if (strcmp(of[i]->travaux, travaux)==0){
            of[i]->ldevis=enliste(of[i]->ldevis, devis); 
            return size;
        }
    }
    size=newOffre(of, size, max, travaux);
    of[size-1]->ldevis=enliste(of[size-1]->ldevis, devis);
    return size;
}

/**
*\brief lit dans un fichier toute les informations d'un devis
*\param [ in ] flot entrée dans laquelle on lit
*\param [ out ] d devis dans lequel on met les informations lu dans le fichier
*\return le devis qui vient de lu
*/
Devis Lire1Devis(FILE *flot)
{
    Devis d;
    fgets(d.entreprise,31, flot);
    d.entreprise[strlen(d.entreprise)-1]='\0';
    fgets(d.adresse,51, flot);
    d.adresse[strlen(d.adresse)-1]='\0';
    fscanf(flot,"%d",&d.capital);
    fscanf(flot,"%d",&d.duree); 
    fscanf(flot,"%f%*c",&d.cout);
    return d;
}

/**
*\brief affiche toute les informations d'un seul devis 
*\param [ in ] d le devis qu'on souhaite afficher
*/
void afficher1Devis(Devis d)
{
    printf("Entreprise : %s\tAdresse : %s\nCapital : %d\tDuree : %d\tCout : %.2f\n", d.entreprise, d.adresse, d.capital, d.duree, d.cout);
}

/**
*\brief affiche le devis d'une certaine entreprise
*\param [ in ] l liste de devis
*\param [ in ] entreprise nom de l'entreprise que l'on souhaite afficher
*\param longueur longueur de la liste l
*\param a compteur dans une boucle
*/
void afficherDevisEntreprise(ListeDevis l, char entreprise[])
{
    int longueur;
    longueur=longListe(l);
    for(int a=0; a<longueur; a++)
    {
        if(strcmp(entreprise, (l->devis).entreprise)==0)
        {
            afficher1Devis(l->devis);
            return ; 
        }
    }
    printf("Cette entreprise n'existe pas ! \n");
}

/**
*\brief cherche dans un tableau une certaine tache
*\param [ in ] o tableau de tache
*\param [ in ] travaux la tache qu'on cherche dans le tableau
*\param [ in ] nb taille logique du tableau o
*\param [in , out ] trouve indicateur égal à 1 si la tache existe et 0 si l'inverse
*\param [ out ] i compteur dans une boucle
*\return la position de la tache dans le tableau si elle existe, sinon renvoie la position ou elle devrait être
*/
int rechercheTravaux(Offre **o, char travaux[], int nb, int *trouve)
{
    int i;
    for(i=0; i<nb; i++)
    {
        if(strcmp(travaux, o[i]->travaux)==0)
        {
            *trouve=1;
            return i;
        }
        if(strcmp(travaux, o[i]->travaux)<0)
        {
            *trouve=0;
            return i;
        }
    }
    *trouve=0;
    return i;
}

/**
*\brief affiche tous les devis d'une certaine tache
*\param [ in , out ] o tableau des taches
*\param [ in , out ] travaux tache que l'on souhaite afficher
*\param [ in , out ] nb taille logique du tableau o
*\param pos position dela tache dans le tableau o
*\param  [out] trouve indice pour savoir si la tache existe ou non
*\param longueur taille de la liste des devis de la tache
*\param [ out ] l liste des devis de la tache
*/
void afficher1Travaux(Offre **o, char travaux[], int nb)
{
    int pos, trouve, longueur;
    ListeDevis l;
    pos=rechercheTravaux(o, travaux, nb, &trouve);
    l=o[pos]->ldevis;
    if(trouve==0)
    {
        printf("Ce type de travail n'existe pas !\n");
        return;
    }
    longueur=longListe(l);
    printf("\n%s :\n", travaux);
    for(int a=0; a<longueur; a++){
        printf("\n");
        afficher1Devis(l->devis);
        l=l->suiv;
    }

}

/**
*\brief afficher tous les devis de toutes les taches
*\param [ in , out ] o tableau des taches
*\param [ in , out ] nb taille du tableau o
*\param i compteur dans une boucle
*/
void afficherTout(Offre **o, int nb)
{
    int i;
    for(i=0; i<=nb-1; i++){
        afficher1Travaux(o, o[i]->travaux, nb);
        printf("\n-------------------------------------------------------------------------------------------------------------\n");
    }
}

/**
*\brief lit toutes les informations d'un fichier pour ajoutes les taches dans le tableau et les devis dans leurs listes
*\param [ in , out] of tableau des taches
*\param [ in , out] size taille logique du tableau of
*\param [ in , out] max taille physique du tableau of
*\param [ in ] filename nom du fichier que l'on va lire
*\param [ out ] file entrée vers le fichier
*\param [ out ] devis nouveaux devis qu'on lit avec la fonction Lire1Devis puit qu'on ajoute dans les bonnes listes à l'aide de la fonction addDevis
*\param [ out ] travaux la tache du devis
*\return la nouvelle taille logique du tableau des taches après avoir ajouter tous les devis
*/
int readOffre(Offre *of[], int size, int *max, char *fileName){
    FILE *file;
    Devis devis;
    char travaux[30];
    file = fopen(fileName, "r");
    if (file==NULL){
        printf("Erreur: ouverture de '%s'\n", fileName);
        return -1;
    }
    fscanf(file, "%s%*c", travaux);
    while (feof(file)==0){
        devis=Lire1Devis(file);
        size=addDevis(of, size, max, travaux, devis);
        fscanf(file, "%s%*c", travaux);
    }
    fclose(file);
    return size;
}

/*
       PARTIE 2
*/

/**
*\brief garde le meilleur devis, c'est à dire le moins cher et si 2 ont le même prix alors c'est le capital le plus haut qui est gardé
*\param [ in ] of tableau de toutes les taches
*\param [ in ] size taille logique du tableau of
*\param [ in , out ] save devis dans lequel on sauvegarde le meilleur
*\param [ out ] l nouvelle liste de devis dans laquelle on met le meilleur devis
*\param i compteur dans la boucle
*/
//Fonction qui garde les moins chère
void sortByCost(Offre *of[], int size){
    Devis save;
    ListeDevis l;
    for (int i=0; i<size; i++){
        l=of[i]->ldevis;
        save=l->devis;
        l=delTete(l);
        while (estVide(l)!=0){
            if(l->devis.cout < save.cout)
                save = l->devis;
            else if (l->devis.cout == save.cout && l->devis.capital > save.capital)
                save = l->devis;
            l=delTete(l);
        }
        l=enliste(l, save);
        of[i]->ldevis=l;
    }
}


/*
*       PARTIE 3
*/

/**
*\brief 
*\param
*\param
*\return
*/
bool ListeVide(Liste l)
{
    if (l==NULL) return 0;
    return 1;
}

/**
*\brief 
*\param
*\param
*\return
*/
void afficherSuccesseur(Liste l)
{
    while(ListeVide(l)==1)
    { 
        printf("%s\t",l->nom);
        l=l->suiv;
    }
}

/**
*\brief 
*\param
*\param
*\return
*/
void afficherTaches(Tache *t[], int size)
{
    int a;
    for(a=0; a<size; a++)
    {
        printf("\n1\n");
        printf("nom : %s      duree : %d      nbPred : %d\n",t[a]->tache, t[a]->duree, t[a]->nbPred);
        printf("Noms des successeur :" afficherSuccesseur(t[a]->succ));
    }
}

/**
*\brief 
*\param
*\param
*\return
*/
int rechercheTache(Tache *t[], int size, char successeur[])
{
    int a;
    for(a=0; a<size; a++)
        if(strcmp(successeur, t[a]->tache)==0)
            return a;
    return -1;
}

/**
*\brief 
*\param
*\param
*\return
*/
Liste EnTeteTache(Liste l, char successeur[])
{
    Maillon2 *new;
    new=(Maillon2 *)malloc(sizeof(Maillon2));
    if (new==NULL){
        printf("Erreur: creation maillon (enliste)\n");
        exit(1);
    }
    strcpy(new->nom,successeur);
    new->suiv=l;
    l=new;
    return l;
}

/**
*\brief 
*\param
*\param
*\return
*/
Liste enlisteTache(Liste l, char successeur[])
{
    if(l==NULL)
        return EnTeteTache(l, successeur);
    if(strcmp(l->nom , successeur)!=0)
        return EnTeteTache(l, successeur);
    l->suiv=enlisteTache(l->suiv, successeur);
    return l;
}

/**
*\brief 
*\param
*\param
*\return
*/
Tache ** chargerTache(Offre *o[], int size)
{
    FILE *flot;
    flot=fopen("précédents.txt","r");
    if(flot==NULL)
    {
        printf("Pb avec l'ouverture de précédent.txt !\n");
        exit(1);
    }
    char predecesseur[20], successeur[20];
    int a, pos;
    Tache **t;
    t=(Tache **)malloc(size*sizeof(Tache *));
    if(t==NULL){printf("PB de malloc pour Tache !\n");exit(1);}
    for(a=0;a<size;a++)
    {
        strcpy(t[a]->tache , o[a]->travaux);
        t[a]->duree=(o[a]->ldevis->devis).duree;
    }
    while(!feof(flot))
    {
        fscanf(flot,"%s",predecesseur);
        fscanf(flot,"%s",successeur);
        pos=rechercheTache(t, size, successeur);
        if(pos!=-1)
            t[pos]->nbPred=t[pos]->nbPred+1;
        enlisteTache(t[pos]->succ, successeur);
    }
    return t;
}
