#define N 3
#include <stdio.h>
#include <stdlib.h>
typedef char morpion[N][N];
int *MinMax(morpion tabmorpion,int alpha,int beta);
int Min(morpion tabmorpion,int alpha,int beta);
int Max(morpion tabmorpion,int alpha, int beta);
int joueur0 = 0, joueur1 = 1;

int jouer(morpion morpion, int i, int j , int joueur) {
    if(morpion[i][j]!=-1) return 1;

    else {
        morpion[i][j]=joueur;
        return 0;
    }
}

int gagnant(morpion morpion, int i){
  int x,y,a,b=0,c,d,e,f=0,g=0,h=0,j=0;
  //x et y : coord, a : nbr de signes ligne, b : booléen de possibilité de victoire
  //c : devient 1 quand une ligne contient un signe opposé
  //d : nbr de signes colonne
  //e : devient 1 quand une colonne contient un signe opposé
  //f : nbr de signe diag descendante
  //g : nbr de signe diag montante
  //h : devient 1 quand une diagonale contient un signe opposé
  //j : devient 1 quand une diagonale contient un signe opposé
  for(x=0;x<N;x++){
    a=0;
    d=0;
    c=0;
    e=0;
    for(y=0;y<N;y++){
      //ligne
      if(morpion[x][y]==(i+1)%2) c=1;
      if(morpion[x][y]==i) a++;
      //colonne
      if(morpion[y][x]==(i+1)%2) e=1;
      if(morpion[y][x]==i) d++;
    }
    //diagonale descendante
    if(morpion[x][x]==i) f++;
    if(morpion[x][x]==(i+1)%2) h=1;
    //diagonale montante
    if(morpion[N-x-1][x]==i) g++;
    if(morpion[N-x-1][x]==(i+1)%2) j=1;
    //victoire diagonale
    if((f==N)||(g==N)) return 1;
    //victoire ligne/colonne
    if((a==N)||(d==N)) return 1;
    //si y'a au moins une ligne ou une colonne où y'a pas de signe opposé, alors c'est gagnable
    if((c==0)||(e==0)) b=1;
  }
  //si y'a au moins une diagonale où y'a pas de signe opposé, alors c'est gagnable
  if((h==0)||(j==0)) b=1;
  if(b==0) return -1;
  else return 0;
}

void afficher(int symb){
    if(symb == 0) {
        printf("| O |");
    }

    else if(symb == 1) {
        printf("| X |");
    }

    else if(symb == -1) {
        printf("|   |");
    }
}

int Max(morpion morpion,int alpha, int beta){
  int valeur;
  if(gagnant(morpion, 1)==1){
    return 10;
  }
  else if(gagnant(morpion,0)==1){
    return -10;
  }
  else if((gagnant(morpion,0)==-1)&&(gagnant(morpion,1)==-1)){
    return 0;
  }

  for (int i=0;i<N;i++){
    for (int j=0;j<N;j++){
      if(morpion[i][j]==-1){
        morpion[i][j]=1;
        valeur=Min(morpion,alpha,beta);
        morpion[i][j]=-1;
        if(alpha<valeur){
          alpha=valeur;
        }
        if(beta<=alpha){
          return alpha;
        }
      }
    }
  }
  return alpha;
}

int Min(morpion morpion,int alpha, int beta){
  int valeur;
  if(gagnant(morpion,1)==1){
    return 10;
  }
  else if(gagnant(morpion,0)==1){
    return -10;
  }
  else if((gagnant(morpion,1)==-1)&&(gagnant(morpion,0)==-1)){
    return 0;
  }

  for (int i=0;i<N;i++){
    for (int j=0;j<N;j++){
      if(morpion[i][j]==-1){
        morpion[i][j]=0;
        valeur=Max(morpion,alpha,beta);
        morpion[i][j]=-1;
        if(beta>valeur){
          beta=valeur;
        }
        if(beta<=alpha){
            return beta;
        }
      }
    }
  }
  return beta;
}

int *MinMax(morpion morpion,int alpha,int beta){
  int *coord=malloc(sizeof(int)*2);
  coord[0]=0;
  coord[1]=0;
  int valeur;

  for (int i=0;i<N;i++){
    for (int j=0;j<N;j++){
      if(morpion[i][j]==-1){
        morpion[i][j]=0;
        valeur=Max(morpion,alpha,beta);
        if(beta>valeur){
          beta=valeur;
          coord[0]=i;
          coord[1]=j;
        }
        morpion[i][j]=-1;
      }
    }
  }
  return coord;
}

void mainJeuIA() {
    int i,j,a,b,c,n_tour = 0,joueur;
    morpion tabmorpion;

    // initialisation du tableau à -1 (vide)
   for(i = 0;i<N;i++) {
        for(j = 0;j<N;j++) {
            tabmorpion[i][j] = -1;
        }
    }


  // Déroulé d'un tour du jeu
    while(gagnant(tabmorpion,(n_tour+1)%2)!=1){
      if((gagnant(tabmorpion,n_tour%2)==-1)&&(gagnant(tabmorpion,(n_tour+1)%2)==-1)){
        printf("Match nul. Aucune solution n'est possible");
        break;
      }
      if(n_tour%2==1){

        printf("Entrer le numéro de ligne, de 0 à N-1.");
        scanf("%d",&a);
        printf("Entrer le numéro de colonne, 0 à N-1.");
        scanf("%d",&b);
        //test de l'indice a et b
        if((a > -1 && a < N) && (b>-1 && b < N)){
          //test de si la case est vide
          if(jouer(tabmorpion,a,b,n_tour%2)){
            printf("Veuillez choisir une case vide. \n");
          }
          //on met la valeur du joueur dans la case
          else{
            jouer(tabmorpion,a,b,n_tour%2);
            n_tour++;
          }
        }
      }
      else if(n_tour%2==0){
        printf("Au tour de l'IA\n\n");
        int *c=MinMax(tabmorpion,-1000,1000);
        jouer(tabmorpion,c[0],c[1],0);
        n_tour++;
      }
    for(i = 0;i<N;i++) {
      for(j = 0;j<N;j++) {
          afficher(tabmorpion[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    printf("\n");
  }
  printf("Le joueur %d remporte la partie !\n",(n_tour+1)%2+1);
}

void mainJeuA2() {
    int i,j,a,b,c,n_tour,joueur;
    morpion tabmorpion;

    // initialisation du tableau à -1 (vide)
   for(i = 0;i<N;i++) {
        for(j = 0;j<N;j++) {
            tabmorpion[i][j] = -1;
        }
    }


    // Déroulé d'un tour du jeu
    for(n_tour = 1;n_tour<N*N+1;n_tour++) {
        joueur = n_tour % 2;
        c = 2;
            while(c != 0) {  
                printf("Entrer le numéro de ligne, de 0 à N-1.");
                scanf("%d",&a);
                printf("Entrer le numéro de colonne, 0 à N-1.");
                scanf("%d",&b);
                if((a > -1 && a < N) && (b>-1 && b < N)) {
                    c = jouer(tabmorpion,a,b,joueur);
                    if(c == 1) printf("Veuillez choisir une case vide. \n");
                }
                else printf("Entrez un chiffre entre 0 et N. \n");
            }
    
            for(i = 0;i<N;i++) {
                for(j = 0;j<N;j++) {
                    afficher(tabmorpion[i][j]);
                }
                printf("\n");
            }

            if(gagnant(tabmorpion,joueur) == 1) {  
                printf("Le joueur %d a gagné !",joueur);
                break;
            }

            else if(gagnant(tabmorpion,joueur) == -1) {  
                printf("Match nul. Aucune solution n'est possible");
                break;
            }

        printf("\n");
        printf("\n");
    }



}


int main() {
  int choix = 0;
  printf("Bienvenue dans le MORPION. Voulez vous jouer contre l'IA ou a 2 ? 1 pour IA, 2 pour a 2");
  scanf("%d",&choix);
  do {
    if(choix == 1) mainJeuIA();
    else if(choix == 2) mainJeuA2();
    else printf("Veuillez choisir : 1 ou 2.");
  } while((choix != 1) && (choix!=2)) ;


}