#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<memory>
#include<string>
#include<vector>
#include<ctime>
#include<algorithm>

using namespace std;

SDL_Renderer* render;
SDL_Window* window;

int yPlayer1=480/2;
int yPlayer2=480/2;

int point1=0;
int point2=0;

int xBola=640/2;
int yBola=480/2;
int Xspeed=5;
int Yspeed=5;

SDL_Rect player1,player2,bola;

SDL_Event event;

void initSDL(){
    int renderFlags,windowFlags;

    renderFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        cout<<"Error:"<<SDL_GetError();
        exit(1);
    }

    window = SDL_CreateWindow("Pong",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480,windowFlags);

    if(!window){
        cout<<"Error in Create Window!"<<SDL_GetError();
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"linear");

    render = SDL_CreateRenderer(window,-1,renderFlags);

    if(!render){
        cout<<"Error in Create Render!"<<SDL_GetError();
        exit(1);
    }

}

void Inputs(){
    
    if(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            exit(0);
        }

        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym==SDLK_w){
                if(!(yPlayer1<=0)){
                    yPlayer1-=20;
                }
            }else if(event.key.keysym.sym==SDLK_s){
                if(!(yPlayer1+50>=480)){
                    yPlayer1+=20;
                }
            }else if(event.key.keysym.sym==SDLK_UP){
                if(!(yPlayer2<=0)){
                    yPlayer2-=20;
                }
            }else if(event.key.keysym.sym==SDLK_DOWN){
                if(!(yPlayer2+50>=480)){
                    yPlayer2+=20;
                }
            }
        }
        
    }
}

void prepareScene(){
    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderClear(render);
}

void presentScene(){
    SDL_RenderPresent(render);
}

SDL_Rect drawRect(int x,int y,int w,int h,int r=255,int g=255,int b=255){
    SDL_SetRenderDrawColor(render,r,g,b,255);

    SDL_Rect player;
    player.x=x;
    player.y=y;
    player.w=w;
    player.h=h;
    SDL_RenderFillRect(render,&player);

    return player;
}

void loopGame(){
    xBola+=Xspeed;
    yBola+=Yspeed;

    if(SDL_HasIntersection(&bola, &player2)){
        Xspeed=-Xspeed;
        xBola-=10;
    }

    if(SDL_HasIntersection(&bola, &player1)){
        Xspeed=-Xspeed;
        xBola+=10;
    }

    if(yBola<=0){ //cima
        Yspeed = -Yspeed;
    }

    if(yBola+20>=480){  //baixo
        Yspeed = -Yspeed;
    }

    if(xBola+20>=640){ //lado direito
        point1++;
        xBola=640/2;
        yBola=480/2;
    }

    if(xBola<=0){  //lado esquerdo
        point2++;
        xBola=640/2;
        yBola=480/2;
    }

}

void displayText(string txt,int x,int y,int h,int w,int size){
    TTF_Init();
    SDL_SetRenderDrawColor(render,255,255,255,255);
    TTF_Font * font = TTF_OpenFont("font/font.ttf", size);
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid(font,txt.c_str(), color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(render, surface);
    
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dstrect = { x, y, w, h };
    SDL_RenderCopy(render, texture, NULL, &dstrect);

    TTF_Quit();
}

void drawLine(){
    SDL_SetRenderDrawColor(render,255,255,255,255);

    SDL_RenderDrawLine(render,640/2,0,640/2,480);

}

int main(int argv, char** args){
    initSDL();
    
    while(1){ //1=true
        player1=drawRect(0,yPlayer1,20,50,255,0,0);    //padle esquerdo
        player2=drawRect(620,yPlayer2,20,50,0,0,255);  //padle direito
        bola=drawRect(xBola,yBola,20,20,255,255,0); //bola

        drawLine();

        loopGame();

        displayText(to_string(point1),100,0,50,50,100);
        displayText(to_string(point2),500,0,50,50,100);

        Inputs();
        presentScene();
        prepareScene();

        SDL_Delay(20);
    }

    return 0;
}