#define N 3
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>


typedef char morpion[N][N];
int *MinMax(morpion tabmorpion,int alpha,int beta);
int Min(morpion tabmorpion,int alpha,int beta);
int Max(morpion tabmorpion,int alpha, int beta);
int joueur0 = 0, joueur1 = 1;
int i,j;

SDL_Surface *texte = NULL;
TTF_Font *police = NULL;
SDL_Color couleurNoire = {255, 255, 255};
SDL_Rect Jeu;
SDL_Rect casenn;
SDL_Rect BoxMessages;
SDL_Point clicked;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *grille = NULL;
SDL_Surface *surface = NULL;
SDL_Surface *icon = NULL;
SDL_Event event;
SDL_Texture *pionCroix = NULL;
SDL_Texture *pionRond = NULL;
int continuer = 1;



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


int jouer(morpion morpion, int i, int j , int joueur) {
    if(morpion[i][j]!=-1) return 1;

    else {
        morpion[i][j]=joueur;
        return 0;
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

void afficherTab(morpion morpion) {
    
    
    

    surface = SDL_LoadBMP("grille.bmp");
    grille = SDL_CreateTextureFromSurface(renderer, surface);
    //if(surface != NULL) SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("croix.bmp");
    pionCroix = SDL_CreateTextureFromSurface(renderer, surface);
    //if(surface != NULL) SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("rond.bmp");
    pionRond = SDL_CreateTextureFromSurface(renderer, surface);
    //if(surface != NULL) SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, grille, NULL, &Jeu);

    

    for(i = 0;i<N;i++) {
      for(j = 0;j<N;j++) {
          casenn.x = j*100;
          casenn.y = i*100;
          casenn.w = 97;
          casenn.h = 97;

          if(morpion[i][j] == joueur0) {
              SDL_RenderCopy(renderer,pionRond,NULL,&casenn);
              
          }

          if(morpion[i][j] == joueur1) {
              SDL_RenderCopy(renderer,pionCroix,NULL,&casenn);
          }

      }
    }
    

    SDL_RenderPresent(renderer);
}

void afficherMsg(char * message,morpion morpion){
    police = TTF_OpenFont("angelina.ttf", 65);
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police,message, couleurNoire);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, texte);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, Message, NULL, &BoxMessages);
    afficherTab(morpion);
    if(Message != NULL) SDL_DestroyTexture(Message);
    //TTF_CloseFont(police);

}

void pause()
{
    int continuer = 1;
    SDL_Event event;
 
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}


void mainJeuA2(morpion tabmorpion)
{
    int n_tour = 0,x = -3,y = -3; //On initialise x et y en dehors de l'indice du tableau, pour éviter que le premier passage dans la boucle place un pion tout seul
    int compteur;
    // initialisation du tableau à -1 (vide)
   for(i = 0;i<N;i++) {
        for(j = 0;j<N;j++) {
            tabmorpion[i][j] = -1;
        }
    }
    
    Jeu.x = 0;
    Jeu.y = 0;
    Jeu.w = 300;
    Jeu.h = 300;

    BoxMessages.x = 0;
    BoxMessages.y = 300;
    BoxMessages.w = 300;
    BoxMessages.h = 200;
    
    afficherTab(tabmorpion);

    while (continuer == 1)
    {

      if((n_tour%2) == 1) afficherMsg("a toi, joueur 2 !",tabmorpion);
      else if((n_tour%2) == 0) afficherMsg("a toi, joueur 1 !",tabmorpion);

      compteur = 1;
        while(compteur == 1) {

          SDL_WaitEvent(&event);
          switch(event.type)
          {
              case SDL_QUIT:
                  continuer = 0;
                  return;
                  
              case SDL_MOUSEBUTTONUP:
                  clicked.x = event.button.x;
                  clicked.y = event.button.y;
                  clicked.x = (clicked.x) / 100;
                  clicked.y = (clicked.y) / 100;
                  x = clicked.x;
                  y = clicked.y;
                  compteur = 0;

                  if(jouer(tabmorpion,y,x,(n_tour)%2) == 1){
                      afficherMsg("Choisis une case vide ! ", tabmorpion);
                      compteur = 1;
                  }
                  else {
                    afficherTab(tabmorpion);
                    n_tour ++;

                  }
                  break;
          }
        }

        if(gagnant(tabmorpion,(n_tour-1)%2)==1) {
              afficherMsg("FIN !",tabmorpion);
              continuer = 0;
              
          }

          else if((gagnant(tabmorpion,(n_tour)%2)==-1)&&((gagnant(tabmorpion,(n_tour+1)%2)))== -1){
              afficherMsg("Match nul..",tabmorpion);
              continuer = 0;
              
          }
        
    }

    pause();
    return;
    
}

void mainJeuIA(morpion tabmorpion) {
    int n_tour = 1,x = 3,y = 3; //On initialise x et y en dehors de l'indice du tableau, pour éviter que le premier passage dans la boucle place un pion tout seul
    int compteur;
    int *c;
    

    Jeu.x = 0;
    Jeu.y = 0;
    Jeu.w = 300;
    Jeu.h = 300;

    BoxMessages.x = 0;
    BoxMessages.y = 300;
    BoxMessages.w = 300;
    BoxMessages.h = 200;
        
    afficherTab(tabmorpion);
    

    while (continuer == 1)
    {
        
      compteur = 1;

        if(n_tour%2 == 1) {
          
          while(compteur == 1) {
            

            afficherMsg("A ton tour ! ", tabmorpion);
            SDL_WaitEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT:
                    continuer = 0; 
                    return;
                    
                case SDL_MOUSEBUTTONUP:
                    clicked.x = event.button.x;
                    clicked.y = event.button.y;
                    clicked.x = (clicked.x) / 100;
                    clicked.y = (clicked.y) / 100;
                    x = clicked.x;
                    y = clicked.y;
                    compteur = 0;
                    if(jouer(tabmorpion,y,x,(n_tour)%2) == 1){
                      afficherMsg("Choisis une case vide ! ", tabmorpion);
                      compteur = 1;
                    }

                    else {
                      afficherTab(tabmorpion);
                      n_tour ++;
                    }
                    break;
            }
          }
     
        }
          
        if(gagnant(tabmorpion,(n_tour-1)%2)==1) {
                afficherMsg("VICTOIRE !",tabmorpion);
                
                break;
        }

        if((gagnant(tabmorpion,(n_tour)%2)==-1)&&((gagnant(tabmorpion,(n_tour+1)%2)))== -1){
              afficherMsg("Match nul..",tabmorpion);
              
              break;
        }     

        

        if(n_tour%2 == 0){
            
            afficherMsg("Au tour de l'IA ! ", tabmorpion);
            SDL_Delay(1000);
            c=MinMax(tabmorpion,-1000,1000);
            jouer(tabmorpion,c[0],c[1],0);
            n_tour++;
            
        }
        
        afficherTab(tabmorpion);
        
        

        if(gagnant(tabmorpion,(n_tour-1)%2)==1) {
            afficherMsg("Victoire de l'IA !",tabmorpion);
            break;
        }

        if((gagnant(tabmorpion,(n_tour)%2)==-1)&&((gagnant(tabmorpion,(n_tour+1)%2)))== -1){
              afficherMsg("Match nul..",tabmorpion);
              break;
        }  
    }


    pause();
    return;
}

int main() {

    int x,y,cont = 1;

     //Ouverture de fenêtre, création de l'icone et du rendu
    
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    morpion tabmorpion;

    Jeu.x = 0;
    Jeu.y = 0;
    Jeu.w = 300;
    Jeu.h = 300;

    BoxMessages.x = 0;
    BoxMessages.y = 300;
    BoxMessages.w = 300;
    BoxMessages.h = 200;

    // initialisation du tableau à -1 (vide)
   for(i = 0;i<N;i++) {
        for(j = 0;j<N;j++) {
            tabmorpion[i][j] = -1;
        }
    }

    window = SDL_CreateWindow("Le Jeu Du Morpion",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              300, 500, SDL_WINDOW_SHOWN);
    
    icon = SDL_LoadBMP("logoTTT.bmp");

    SDL_SetWindowIcon(window,icon);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    afficherTab(tabmorpion);
    afficherMsg("IA    1v1",tabmorpion);

    while(cont == 1) {
        SDL_WaitEvent(&event);
                switch(event.type)
                {
                    case SDL_QUIT:
                        cont = 0;
                        goto Quit;
                        break;
                        
                        
                    case SDL_MOUSEBUTTONUP:
                        clicked.x = event.button.x;
                        clicked.y = event.button.y;
                        x = clicked.x;
                        y = clicked.y;
                        
                        break;
                }

        if(y > 300) {
            if(x<150) {
                mainJeuIA(tabmorpion);
                cont = 0;

            }

            if(x>150) {
                mainJeuA2(tabmorpion);
                cont = 0;

            }
        }

    }

    

    Quit:
    if(NULL != grille) SDL_DestroyTexture(grille);
    if(NULL != pionRond) SDL_DestroyTexture(grille);
    if(NULL != pionCroix) SDL_DestroyTexture(grille);
    if(NULL != police) TTF_CloseFont(police);
    if(texte != NULL) SDL_FreeSurface(texte);
    SDL_QuitEvent(event);
    if(NULL != renderer) SDL_DestroyRenderer(renderer);
    if(NULL != window) SDL_DestroyWindow(window);
    

    
    TTF_Quit();
    SDL_Quit();
}


