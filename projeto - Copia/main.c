#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 528

typedef struct {
    Vector2 position;
    float speed, size, atkspeed;
    int health;
} Player; 

typedef struct {
    Vector2 position;
    float speed, size;
    int health, type;
} Enemy;

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

int *Kill(Enemy *enemies, int id, int * numinimigos) { 
    Enemy *temp = (Enemy*) calloc(*numinimigos, sizeof(Enemy));
    for(int i = id; i < *numinimigos - 1; i++) {
        *temp = enemies[i];
        enemies[i] = enemies[i + 1];
        enemies[i + 1] = *temp;
    }
    *numinimigos -= 1;
}

int main() {

    int gamestate = 0;
    int numinimigos = 0;
    int flagMovimento = 0;
    int flagAtaque = 0;
    int flagParado = 0;
    int contadorOndas = 1;

    Rectangle paredes[4] = {{0, 0, 215, WINDOW_HEIGHT},
    {0, 0, WINDOW_WIDTH, 140},
    {0, 425, WINDOW_WIDTH, WINDOW_HEIGHT - 425},
    {575, 0, WINDOW_WIDTH - 575, WINDOW_HEIGHT}};

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "teste");
    SetTargetFPS(60);
    Texture mapa = LoadTexture("assets/Level_0.png");
    //morcego
    Texture2D morcego = LoadTexture("assets/skeleton.png");
    int maxFramesMorcego = 3;
    float timer = 0.0f;
    int frameMorcego = 0;
    // Jogador
    Texture2D personagem = LoadTexture("assets/Player_Idle_Run_Stop.png");
    Texture2D personagemParado = LoadTexture("assets/Player_Idle_Run_Stop.png");
    Texture2D personagemInvertido = LoadTexture("assets/cavaleiro_invertido.png");
    int maxFramesPlayer = 4;
    int framePlayer = 0;
    Player player;
    player.speed = 2.0f;
    player.size = 8;
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
    playerattack.width = 30;
    playerattack.x = 0;
    playerattack.y = 0;
    // Inimigo teste
    Enemy *enemies = (Enemy*) calloc(5, sizeof(Enemy));
    for(int i = 0; i < 5; i++) {
        enemies[i].speed = 1.0f;
        enemies[i].size = 4;
        enemies[i].position.x = rand() % WINDOW_WIDTH;
        enemies[i].position.y = rand() % WINDOW_HEIGHT;
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
        }
         frameMorcego = frameMorcego % maxFramesMorcego;
         framePlayer = framePlayer % maxFramesPlayer;
        if(gamestate == 0) {
            // Codigo

            // Jogador
            if(IsKeyDown(KEY_S) && !CheckCollisionRecs(paredes[2], hitbox)) {
                player.position.y += player.speed;
                playerattack.y += player.speed;
                flagParado = 0;
            }
            if(IsKeyDown(KEY_W) && !CheckCollisionRecs(paredes[1], hitbox)) {
                player.position.y -= player.speed;
                playerattack.y -= player.speed;
                flagParado = 0;
            }
            if(IsKeyDown(KEY_D) && !CheckCollisionRecs(paredes[3], hitbox)) {
                player.position.x += player.speed;
                playerattack.x += player.speed;
                flagMovimento = 0;
                flagParado = 0;
            }
            if(IsKeyDown(KEY_A) && !CheckCollisionRecs(paredes[0], hitbox)) {
                player.position.x -= player.speed;
                playerattack.x -= player.speed;
                flagMovimento = 1;
                flagParado = 0;
            }
            if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && flagMovimento == 0){
                flagParado = 1;
            }
            if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && flagMovimento == 1){
                flagParado = 2;
            }
            if(IsKeyDown(KEY_DOWN) && atkdelay <= 0) {
                // valores importantes, nao alterar
                playerattack.x = player.position.x - 10;
                playerattack.y = player.position.y + 15;
                // valores importantes
                atkdelay = player.atkspeed;
                atktime = 20;
            }
            if(IsKeyDown(KEY_UP) && atkdelay <= 0) {
                playerattack.x = player.position.x - 10;
                playerattack.y = player.position.y - 35;
                atkdelay = player.atkspeed;
                atktime = 20;
            }
            if(IsKeyDown(KEY_RIGHT) && atkdelay <= 0) {
                playerattack.x = player.position.x + 45;
                playerattack.y = player.position.y + 20;
                atkdelay = player.atkspeed;
                atktime = 20;
            }
            if(IsKeyDown(KEY_LEFT) && atkdelay <= 0) {
                playerattack.x = player.position.x + 8;
                playerattack.y = player.position.y + 20;
                atkdelay = player.atkspeed;
                atktime = 20;
            }

            if(atkdelay) {
                atkdelay--;
            }

            if(atktime) {
                atktime--;
            }

            // Inimigo
            for(int i = 0; i < numinimigos; i++) {
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

                if(CheckCollisionCircles((Vector2){player.position.x + 30, player.position.y + 25} , player.size, enemies[i].position, enemies[i].size)) {
                    gamestate = 1;
                }
                if(CheckCollisionCircleRec(enemies[i].position, enemies[i].size, playerattack) && atktime > 0) {
                    enemies = remover(enemies, i, &numinimigos);
                    if(numinimigos <= 0) {
                        gamestate = 2; 
                    }
                }
            }
            // Desenho 
            BeginDrawing(); 
                ClearBackground(GRAY);
                DrawTexture(mapa, 0, 0, WHITE);
                DrawText(TextFormat("%d", atktime), 0, 0, 30, BLACK);
                if(atktime > 0) {
                    if(flagMovimento == 0)
                    DrawTextureRec(ataque, (Rectangle){(ataque.width/4)*framePlayer, 0, ataque.width/4, ataque.height/7},(Vector2){player.position.x, player.position.y}, WHITE);
                    else if (flagMovimento == 1)
                    DrawTextureRec(ataqueInvertido, (Rectangle){(ataqueInvertido.width/4)*framePlayer, 0, ataqueInvertido.width/4,ataqueInvertido.height/7},(Vector2){player.position.x, player.position.y}, WHITE);
                    // DrawRectangle(playerattack.x, playerattack.y, playerattack.width, playerattack.height, GREEN);
                }
                // DrawCircle(player.position.x, player.position.y, player.size, BLUE);
                if(flagMovimento == 0 && atktime <= 0 && flagParado == 0){
                DrawTextureRec(personagem, (Rectangle){(personagem.width/4)*framePlayer, personagem.height/3, personagem.width/4,personagem.height/3},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                else if (flagMovimento == 1 && atktime <= 0 && flagParado == 0){
                DrawTextureRec(personagemInvertido, (Rectangle){(personagemInvertido.width/4)*framePlayer, personagemInvertido.height/3, personagemInvertido.width/4,personagemInvertido.height/3},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                else if (flagMovimento == 1 && atktime <= 0 && flagParado == 2){
                DrawTextureRec(personagemInvertido, (Rectangle){(personagemInvertido.width/4)*framePlayer, 0, personagemInvertido.width/4,personagemInvertido.height/3},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                else if (flagMovimento == 0 && atktime <= 0 && flagParado == 1){
                DrawTextureRec(personagem, (Rectangle){(personagem.width/4)*framePlayer, 0, personagem.width/4,personagem.height/3},(Vector2){player.position.x, player.position.y}, WHITE);
                }
                for(int i = 0; i < numinimigos; i++) {
                    DrawTextureRec(morcego, (Rectangle){(morcego.width/3)*frameMorcego, 0, morcego.width/3,morcego.height},(Vector2){enemies[i].position.x, enemies[i].position.y}, WHITE);
                }    
            EndDrawing();
        }

        else if(gamestate == 1) {
            if(IsKeyPressed(KEY_ENTER)) {
                contadorOndas = 1;
                player.position.x = 250;
                player.position.y = 250;
                enemies = (Enemy*)realloc(enemies, 5 * sizeof(Enemy));
                if(enemies == NULL)exit(1);
                for(int i = 0; i < 5; i++) {
                    enemies[i].speed = 1.0f;
                    enemies[i].size = 8;
                    enemies[i].position.x = rand() % WINDOW_WIDTH;
                    enemies[i].position.y = rand() % WINDOW_HEIGHT;
                    numinimigos = 5;
                }
                atkdelay = 0;
                atktime = 0;
                gamestate = 0;
            }
            BeginDrawing();
                ClearBackground(RED);
                DrawText("bruh", player.position.x, player.position.y, 30.0f, MAROON) ;
            EndDrawing();
        }

        else if(gamestate == 2) {
            if(IsKeyPressed(KEY_ENTER)) {
                contadorOndas++;
                player.position.x = 250;
                player.position.y = 250;
                enemies = (Enemy*) realloc(enemies, (5*contadorOndas) * sizeof(Enemy));
                if(enemies == NULL) exit(1);
                for(int i = 0; i < 5*contadorOndas; i++) {
                    enemies[i].speed = 1.0f;
                    enemies[i].size = 8;
                    enemies[i].position.x = rand() % WINDOW_WIDTH;
                    enemies[i].position.y = rand() % WINDOW_HEIGHT;
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
                DrawText("LESGO", player.position.x, player.position.y, 30.0f, DARKGREEN) ;
            EndDrawing();
        }

    }
    UnloadTexture(personagem);
    UnloadTexture(personagemInvertido);
    UnloadTexture(personagemParado);
    UnloadTexture(morcego);
    UnloadTexture(mapa);
    free(enemies);
    CloseWindow();

    return 0;
}
