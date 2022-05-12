#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define WINDOW_WIDTH 1008
#define WINDOW_HEIGHT 608
#define PROJ_MAX 100

typedef struct {
    Vector2 position;
    float speed, size, atkspeed;
    int health;
} Player; 

typedef struct {
    Vector2 position;
    float speed, size;
    int health, type, atkspeed, enemystate;
} Enemy;

typedef struct {
    Vector2 position, speed;
    float size;
} Projectile;

Enemy *remover(Enemy *v, int index, int *nElementos){
    if(*nElementos == 0) return NULL; // Nesse caso não há o que remover.

    if(index >= *nElementos) return v; // Index inválido.


    /* Sobrepomos aquele elemento */
    for(int i = index; i < *nElementos - 1; i++) 
        v[i] = v[i+1];

    /* Realocamos */
    Enemy *temp = v;
    temp = (Enemy *) realloc(temp, (*nElementos - 1) * sizeof(Enemy));
    if(temp == NULL && *nElementos != 1) { free(v); exit(1);} 

    (*nElementos)--;
    return temp; // Retornamos o ponteiro
}

float norm(float v, float y) {
    float temp;
    temp = v / sqrt((v * v) + (y * y));
    return temp;
}

int main() {

    int gamestate = -1;
    int numinimigos = 0;
    int flagMovimento = 0;
    int flagAtaque = 0;
    int flagParado = 0;
    int contadorOndas = 1;
    int flagMorte = 0;
    int flagDefesa = 0;

    InitAudioDevice();

    Rectangle paredes[4] = {
    //esquerda
    {0, 0, 20, WINDOW_HEIGHT},
    //direita
    {WINDOW_WIDTH - 90, 0, 115, WINDOW_HEIGHT},
    //cima
    {0, 0, WINDOW_WIDTH, 65},
    //baixo
    {0, WINDOW_HEIGHT - 135, WINDOW_WIDTH, 100}
    };

    Projectile projectiles[PROJ_MAX];
    int projnum = 0;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "teste");
    SetTargetFPS(60);
    //menu
    Texture2D menu = LoadTexture("assets/menu.png");
    Texture2D perdeu = LoadTexture("assets/perdeu.png");
    Texture2D ganhou = LoadTexture("assets/ganhou.png");
    //mapa
    Texture mapa = LoadTexture("assets/Level_0.png");
    //barra de vida
    Texture2D vida = LoadTexture("assets/barra_de_vida_.png");
    int framesVida = 0;
    int maxFramesVida = 6;


    //efeitos sonoros
    Sound musicaFundo = LoadSound("assets/musicafundo.mp3");
    Sound playersoundAtaque =LoadSound("assets/playerataque.mp3");
    Sound morcegoAtingido = LoadSound("assets/morcegoatingido.mp3");
    Sound esqueletoAtingido =LoadSound("assets/esqueletoAtingido.mp3");
    Sound sapoAtingido =LoadSound("assets/sapoAtingido.mp3");
    Sound fantasmaAtingido = LoadSound("assents/fantasmaAtingido.mp3");
    PlaySound(musicaFundo);

    //morcego
    Texture2D morcego = LoadTexture("assets/bat.png");
    int maxFramesMorcego = 3;
    float timer = 0.0f;
    int frameMorcego = 0;

    // Jogador
    Texture2D personagem = LoadTexture("assets/Player_Idle_Run_Stop.png");
    Texture2D personagemParado = LoadTexture("assets/Player_Idle_Run_Stop.png");
    Texture2D personagemInvertido = LoadTexture("assets/cavaleiro_invertido.png");
    Texture2D mortePersonagem = LoadTexture("assets/PlayerHit_Death.png");
    int maxFramesPlayer = 4;
    int framePlayer = 0;
    int frameMorte = 0;
    int maxFramesMorte = 12;
    Player player;
    player.speed = 2.0f;
    player.size = 8;
    player.health = 5;
    player.position.x = 250;
    player.position.y = 250;
    player.atkspeed = 40;
    Rectangle hitbox;
    hitbox.height = 8;
    hitbox.width = 8;
    hitbox.x = player.position.x;
    hitbox.y = player.position.y;
    int atkdelay = 0, atktime = 0;
    // Ataque do Jogador
    Texture2D ataque = LoadTexture("assets/PlayerAttack.png");
    Texture2D ataqueInvertido = LoadTexture("assets/ataque_invertido.png");
    Rectangle playerattack;
    playerattack.height = 15;
    playerattack.width = 20;
    playerattack.x = 0;
    playerattack.y = 0;
    // Inimigo teste
    Enemy *enemies = (Enemy*) calloc(5, sizeof(Enemy));
    int atkdelayInimigo = 0;
    for(int i = 0; i < 5; i++) {
        enemies[i].speed = 1.0f;
        enemies[i].size = 4;
        enemies[i].position.x = GetRandomValue(20, WINDOW_WIDTH - 90);
        enemies[i].position.y = GetRandomValue(65, WINDOW_HEIGHT - 135);
        enemies[i].atkspeed = 40;
        enemies[i].type = i % 2;
        enemies[i].atkspeed = 60;
        numinimigos++;
    }
    while(!WindowShouldClose()) {

        
       
        hitbox.x = player.position.x;
        hitbox.y = player.position.y;
        timer += GetFrameTime();
          if(timer >= 0.2f){
            timer = 0.0f;
            frameMorcego += 1;
            framePlayer += 1;
            if(flagMorte == 1){
                frameMorte++;
            }
        }
         frameMorcego = frameMorcego % maxFramesMorcego;
         framePlayer = framePlayer % maxFramesPlayer;
         frameMorte = frameMorte % maxFramesMorte;

        if(gamestate == -1){
            BeginDrawing();
                DrawTexture(menu, 0, 0, WHITE);
            EndDrawing();

            if(IsKeyDown(KEY_ENTER)){
                gamestate = 0;
            }
        }

        if(gamestate == 0) {
            // Codigo

            // Jogador
            if(IsKeyDown(KEY_S) && !CheckCollisionRecs(paredes[3], hitbox)) {
                player.position.y += player.speed;
                playerattack.y += player.speed;
                flagParado = 0;
            }
            if(IsKeyDown(KEY_W) && !CheckCollisionRecs(paredes[2], hitbox)) {
                player.position.y -= player.speed;
                playerattack.y -= player.speed;
                flagParado = 0;
            }
            if(IsKeyDown(KEY_D) && !CheckCollisionRecs(paredes[1], hitbox)) {
                player.position.x += player.speed;
                playerattack.x += player.speed;
                // Mudei flagMovimento pra 1, representando a direita. A condicional é pra ele não virar durante o ataque.
                if(atktime == 0) {
                    flagMovimento = 1;
                }
                flagParado = 0;
            }
            if(IsKeyDown(KEY_A) && !CheckCollisionRecs(paredes[0], hitbox)) {
                player.position.x -= player.speed;
                playerattack.x -= player.speed;
                // Mudei flagMovimento pra -1, representando a esquerda
                if(atktime == 0) {
                    flagMovimento = -1;
                }
                flagParado = 0;
            }
            /*if(IsKeyDown(KEY_E)){
                flagDefesa = 1;
            }
            else{
                flagDefesa = 0;
            }*/
            if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && flagMovimento == 1){
                flagParado = 1;
            }
            if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && flagMovimento == -1){
                flagParado = 2;
            }
            
            if(IsKeyDown(KEY_SPACE) && atkdelay <= 0) {
                if(flagMovimento == 1) {
                    playerattack.x = player.position.x + 48;
                    playerattack.y = player.position.y + 20;
                }
                if(flagMovimento == -1) {
                    playerattack.x = player.position.x + 8;
                    playerattack.y = player.position.y + 20;
                }
                PlaySound(playersoundAtaque);
                atkdelay = player.atkspeed;
                atktime = 20;
                
            }

            if(atkdelay) {
                atkdelay--;
            }
            if(atkdelayInimigo) {
                atkdelayInimigo--;
            }

            if(atktime) {
                atktime--;
            }

            // Inimigos
            // Divididos em tipos diferentes
            for(int i = 0; i < numinimigos; i++) {
                // Inimigo de tipo 0: Persegue o jogador
                if(enemies[i].type == 0) {
                    int flag1 = 1;
                    if(player.position.x + 30 > enemies[i].position.x) {
                        for(int x = i + 1; x < numinimigos && flag1; x++) {
                            if(CheckCollisionCircles(enemies[x].position, enemies[x].size, enemies[i].position, enemies[i].size)) {
                                flag1 = 0;
                                
                            } 
                        }
                        if(flag1) {
                            enemies[i].position.x += enemies[i].speed;
                        }
                    }
                    else if(player.position.x - 30 < enemies[i].position.x) {
                        for(int x = i + 1; x < numinimigos && flag1; x++) {
                            if(CheckCollisionCircles(enemies[x].position, enemies[x].size, enemies[i].position, enemies[i].size)) {
                                flag1 = 0;
                            } 
                        }
                        if(flag1) {
                            enemies[i].position.x -= enemies[i].speed;
                        }
                    }
                    if(player.position.y + 25 > enemies[i].position.y) {
                        for(int x = i + 1; x < numinimigos && flag1; x++) {
                            if(CheckCollisionCircles(enemies[x].position, enemies[x].size, enemies[i].position, enemies[i].size)) {
                                flag1 = 0;
                            } 
                        }
                        if(flag1) {
                            enemies[i].position.y += enemies[i].speed;
                        }
                    }
                    else if(player.position.y - 25 < enemies[i].position.y){
                        for(int x = i + 1; x < numinimigos && flag1; x++) {
                            if(CheckCollisionCircles(enemies[x].position, enemies[x].size, enemies[i].position, enemies[i].size)) {
                                flag1 = 0;
                            } 
                        }
                        if(flag1) {
                            enemies[i].position.y -= enemies[i].speed;
                        }
                    }

                    if(CheckCollisionCircles((Vector2){player.position.x + 30, player.position.y + 25}, player.size, enemies[i].position, enemies[i].size) && atkdelayInimigo <= 0 && flagDefesa == 0) {\
                        atkdelayInimigo = 40;
                        player.health--;
                        framesVida++;
                        framesVida = framesVida % maxFramesVida;
                        if(player.health == 0){
                        gamestate = 1;
                        flagMorte = 1;
                        }
                    }
                    if(CheckCollisionCircleRec(enemies[i].position, enemies[i].size, playerattack) && atktime > 0) {
                        PlaySound(morcegoAtingido);
                        enemies = remover(enemies, i, &numinimigos);
                        if(numinimigos <= 0) {
                            gamestate = 2; 
                        }
                    }
                }
                // Tipo 1: Dispara projeteis no jogador
                else if(enemies[i].type == 1) {
                    if(enemies[i].atkspeed <= 0) {
                        if(projnum <= PROJ_MAX) {
                            projectiles[projnum].position = enemies[i].position;
                            projectiles[projnum].size = 5;
                            projectiles[projnum].speed.x = norm(player.position.x + 30 - enemies[i].position.x, player.position.y + 25 - enemies[i].position.y);
                            projectiles[projnum].speed.y = norm(player.position.y + 25 - enemies[i].position.y, player.position.x + 30 - enemies[i].position.x);
                            projnum++;
                            enemies[i].atkspeed = 120;
                        }
                    }
                    else {
                        enemies[i].atkspeed -= 1;
                    }
                    // Receber dano
                    if(CheckCollisionCircleRec(enemies[i].position, enemies[i].size, playerattack) && atktime > 0) {
                        PlaySound(morcegoAtingido);
                        enemies = remover(enemies, i, &numinimigos);
                        if(numinimigos <= 0) {
                            gamestate = 2; 
                        }
                    }
                }
            }

            //Projeteis
            for(int i = 1; i <= projnum; i++) {
                projectiles[i - 1].position.x += projectiles[i - 1].speed.x;
                projectiles[i - 1].position.y += projectiles[i - 1].speed.y;
                if(CheckCollisionCircles((Vector2){player.position.x + 30, player.position.y + 25}, player.size, projectiles[i - 1].position, projectiles[i - 1].size) && atkdelayInimigo <= 0 && flagDefesa == 0) {\
                    atkdelayInimigo = 40;
                    player.health--;
                    framesVida++;
                    framesVida = framesVida % maxFramesVida;
                    if(player.health == 0){
                    gamestate = 1;
                    flagMorte = 1;
                    }
                }
                if(projectiles[i - 1].position.x < 0 || projectiles[i - 1].position.x > WINDOW_WIDTH || projectiles[i - 1].position.y < 0 || projectiles[i - 1].position.y > WINDOW_HEIGHT) {
                    for(int x = i - 1; x <= projnum - 1; x++) {
                        Projectile temp = projectiles[x];
                        projectiles[x] = projectiles[x + 1];
                        projectiles[x + 1] = temp;
                    }
                    projnum--;
                }
            }
            // Desenho 
            BeginDrawing(); 
                ClearBackground(GRAY);
                DrawTexture(mapa, 0, 0, WHITE);
                if(atktime > 0) {
                    DrawRectangleRec(playerattack, BLUE);
                }
                for(int i = 0; i < projnum; i++) {
                    DrawCircle(projectiles[i].position.x, projectiles[i].position.y, projectiles[i].size, RED);
                }
                DrawTextureRec(vida, (Rectangle){60, (vida.height/6)*framesVida, vida.width, vida.height/6}, (Vector2){0, 0}, WHITE);
                // DrawText(TextFormat("%d", atktime), 0, 0, 30, BLACK);
                if(atktime > 0) {
                    if(flagMovimento == 1)
                    DrawTextureRec(ataque, (Rectangle){(ataque.width/4)*framePlayer, 0, ataque.width/4, ataque.height/7},(Vector2){player.position.x, player.position.y}, WHITE);
                    else if (flagMovimento == -1)
                    DrawTextureRec(ataqueInvertido, (Rectangle){(ataqueInvertido.width/4)*framePlayer, 0, ataqueInvertido.width/4,ataqueInvertido.height/7},(Vector2){player.position.x, player.position.y}, WHITE);
                    // DrawRectangle(playerattack.x, playerattack.y, playerattack.width, playerattack.height, GREEN);
                }
                // DrawCircle(player.position.x, player.position.y, player.size, BLUE);
                if(flagMovimento == 1 && atktime <= 0 && flagParado == 0 && flagDefesa == 0){
                DrawTextureRec(personagem, (Rectangle){(personagem.width/4)*framePlayer, personagem.height/3, personagem.width/4,personagem.height/3},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                else if (flagMovimento == -1 && atktime <= 0 && flagParado == 0 && flagDefesa == 0){
                DrawTextureRec(personagemInvertido, (Rectangle){(personagemInvertido.width/4)*framePlayer, personagemInvertido.height/3, personagemInvertido.width/4,personagemInvertido.height/3},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                else if (flagMovimento == -1 && atktime <= 0 && flagParado == 2 && flagDefesa == 0){
                DrawTextureRec(personagemInvertido, (Rectangle){(personagemInvertido.width/4)*framePlayer, 0, personagemInvertido.width/4,personagemInvertido.height/3},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                else if (flagMovimento == 1 && atktime <= 0 && flagParado == 1 && flagDefesa == 0){
                DrawTextureRec(personagem, (Rectangle){(personagem.width/4)*framePlayer, 0, personagem.width/4,personagem.height/3},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                else if (atktime <= 0 && flagDefesa == 1 && flagMovimento == 1){
                    DrawTextureRec(ataque, (Rectangle){(ataque.width/4)*3, (ataque.height/7) * 6, ataque.width/4, ataque.height/7},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                else if (atktime <= 0 && flagDefesa == 1 && flagMovimento == -1){
                    DrawTextureRec(ataqueInvertido, (Rectangle){0, (ataque.height/7) * 6, ataqueInvertido.width/4, ataqueInvertido.height/7},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                for(int i = 0; i < numinimigos; i++) {
                    DrawTextureRec(morcego, (Rectangle){(morcego.width/3)*frameMorcego, 0, morcego.width/3,morcego.height},(Vector2){enemies[i].position.x, enemies[i].position.y}, WHITE);
                }    
            EndDrawing();
        }

        else if(gamestate == 1) {
            if(IsKeyPressed(KEY_ENTER)) {
                contadorOndas = 1;
                flagMorte = 0;
                framesVida = 0;
                player.health = 5;
                player.position.x = 250;
                player.position.y = 250;
                enemies = (Enemy*)realloc(enemies, 5 * sizeof(Enemy));
                if(enemies == NULL)exit(1);
                for(int i = 0; i < 5; i++) {
                    enemies[i].speed = 1.0f;
                    enemies[i].size = 4;
                    enemies[i].position.x = GetRandomValue(20, WINDOW_WIDTH - 90);
                    enemies[i].position.y = GetRandomValue(65, WINDOW_HEIGHT - 135);
                    enemies[i].atkspeed = 40;
                    enemies[i].type = i % 2;
                    enemies[i].atkspeed = 60;
                    numinimigos++;
                }
                atkdelay = 0;
                atktime = 0;
                gamestate = 0;
            }
            BeginDrawing();
                ClearBackground(GRAY);
                DrawTexture(mapa, 0, 0, WHITE);
                DrawTextureRec(mortePersonagem, (Rectangle){(mortePersonagem.width/maxFramesMorte) * frameMorte, mortePersonagem.height/2, mortePersonagem.width/maxFramesMorte, mortePersonagem.height/2}, (Vector2){player.position.x, player.position.y}, WHITE);
                DrawTexture(perdeu, 0, 0, WHITE);
            EndDrawing();
        }

        else if(gamestate == 2) {
            if(IsKeyPressed(KEY_ENTER)) {
                contadorOndas++;
                player.position.x = 250;
                player.position.y = 250;
                enemies = (Enemy*) realloc(enemies, (5*contadorOndas) * sizeof(Enemy));
                if(enemies == NULL) exit(1);
                for(int i = 0; i < 5 * contadorOndas; i++) {
                    enemies[i].speed = 1.0f;
                    enemies[i].size = 4;
                    enemies[i].position.x = GetRandomValue(20, WINDOW_WIDTH - 90);
                    enemies[i].position.y = GetRandomValue(65, WINDOW_HEIGHT - 135);
                    enemies[i].atkspeed = 40;
                    enemies[i].type = i % 2;
                    enemies[i].atkspeed = 60;
                    numinimigos++;
                }
                atkdelay = 0;
                atktime = 0;
                gamestate = 0;
            if(contadorOndas == 10){
                gamestate = 3;
            }
            }
            BeginDrawing();
            DrawText("PROXIMA ONDA A CAMINHO", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 30, WHITE);
            EndDrawing();
        }
        else if(gamestate == 3){
            BeginDrawing();
                ClearBackground(GREEN);
                DrawTexture(ganhou, 0, 0, WHITE);
            EndDrawing();

            //voltar pro menu
            /* if(IsKeyDown(KEY_ENTER)){
                gamestate = -1;
            }  */
        }

    }
    UnloadTexture(menu);
    UnloadTexture(ganhou);
    UnloadTexture(perdeu);
    UnloadTexture(personagem);
    UnloadTexture(personagemInvertido);
    UnloadTexture(personagemParado);
    UnloadTexture(morcego);
    UnloadTexture(mapa);
    UnloadTexture(ataque);
    UnloadTexture(ataqueInvertido);
    UnloadTexture(mortePersonagem);
    free(enemies);
    UnloadSound(playersoundAtaque); 
    UnloadSound(morcegoAtingido);
    UnloadSound(fantasmaAtingido);
    UnloadSound(sapoAtingido);
    UnloadSound(esqueletoAtingido);
    UnloadSound(musicaFundo);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
