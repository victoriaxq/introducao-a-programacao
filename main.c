#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 500

typedef struct {
    Rectangle position;
    float speed;
    int health, atkspeed;
} Player; 

typedef struct {
    Rectangle position;
    float speed;
    int health, type;
} Enemy;

void Kill(Enemy * enemies, int id, int * numinimigos) {
    for(int i = id; i < *numinimigos - 1; i++) {
        Enemy temp = enemies[i];
        enemies[i] = enemies[i + 1];
        enemies[i + 1] = temp;
    }
    *numinimigos -= 1;
}

int main() {

    int gamestate = 0;
    int numinimigos = 0;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "teste");
    SetTargetFPS(60);
    // Jogador
    Player player;
    player.speed = 12.0f;
    player.position.x = 250;
    player.position.y = 250;
    player.position.height = 60;
    player.position.width = 60;
    player.atkspeed = 30;
    int atkdelay = 0, atktime = 0;
    // Ataque do Jogador
    Rectangle playerattack;
    playerattack.height = 60;
    playerattack.width = 60;
    playerattack.x = 0;
    playerattack.y = 0;
    // Inimigo teste
    Enemy enemies[5];
    for(int i = 0; i < 5; i++) {
        enemies[i].speed = 4.0f;
        enemies[i].position.height = 50;
        enemies[i].position.width = 50;
        enemies[i].position.x = 0 - 100*i;
        enemies[i].position.y = 0 - 100*1;
        numinimigos++;
    }
    while(!WindowShouldClose()) {
        if(gamestate == 0) {
            // Codigo

            // Jogador
            if(IsKeyDown(KEY_S) && player.position.y < WINDOW_HEIGHT - 60) {
                player.position.y += player.speed;
                playerattack.y += player.speed;
            }
            if(IsKeyDown(KEY_W) && player.position.y > 60) {
                player.position.y -= player.speed;
                playerattack.y -= player.speed;
            }
            if(IsKeyDown(KEY_D) && player.position.x < WINDOW_WIDTH - 60) {
                player.position.x += player.speed;
                playerattack.x += player.speed;
            }
            if(IsKeyDown(KEY_A) && player.position.x > 60) {
                player.position.x -= player.speed;
                playerattack.x -= player.speed;
            }
            if(IsKeyDown(KEY_DOWN) && atkdelay <= 0) {
                // valores importantes, nao alterar
                playerattack.x = player.position.x;
                playerattack.y = player.position.y + 60;
                // valores importantes
                atkdelay = player.atkspeed;
                atktime = 10;
            }
            if(IsKeyDown(KEY_UP) && atkdelay <= 0) {
                playerattack.x = player.position.x;
                playerattack.y = player.position.y - 60;
                atkdelay = player.atkspeed;
                atktime = 10;
            }
            if(IsKeyDown(KEY_RIGHT) && atkdelay <= 0) {
                playerattack.x = player.position.x + 60;
                playerattack.y = player.position.y;
                atkdelay = player.atkspeed;
                atktime = 10;
            }
            if(IsKeyDown(KEY_LEFT) && atkdelay <= 0) {
                playerattack.x = player.position.x - 60;
                playerattack.y = player.position.y;
                atkdelay = player.atkspeed;
                atktime = 10;
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
                if(player.position.x > enemies[i].position.x) {
                    for(int x = i + 1; x < numinimigos && flag1; x++) {
                        if(CheckCollisionRecs(enemies[x].position, enemies[i].position)) {
                            flag1 = 0;
                        } 
                    }
                    if(flag1) {
                        enemies[i].position.x += enemies[i].speed;
                    }
                }
                else if(player.position.x < enemies[i].position.x) {
                    for(int x = i + 1; x < numinimigos && flag1; x++) {
                        if(CheckCollisionRecs(enemies[x].position, enemies[i].position)) {
                            flag1 = 0;
                        } 
                    }
                    if(flag1) {
                        enemies[i].position.x -= enemies[i].speed;
                    }
                }
                if(player.position.y > enemies[i].position.y) {
                    for(int x = i + 1; x < numinimigos && flag1; x++) {
                        if(CheckCollisionRecs(enemies[x].position, enemies[i].position)) {
                            flag1 = 0;
                        } 
                    }
                    if(flag1) {
                        enemies[i].position.y += enemies[i].speed;
                    }
                }
                else if(player.position.y < enemies[i].position.y){
                    for(int x = i + 1; x < numinimigos && flag1; x++) {
                        if(CheckCollisionRecs(enemies[x].position, enemies[i].position)) {
                            flag1 = 0;
                        } 
                    }
                    if(flag1) {
                        enemies[i].position.y -= enemies[i].speed;
                    }
                }

                if(CheckCollisionRecs(enemies[i].position, player.position)) {
                    gamestate = 1;
                }
                if(CheckCollisionRecs(enemies[i].position, playerattack) && atktime > 0) {
                    Kill(enemies, i, &numinimigos);
                    if(numinimigos <= 0) {
                        gamestate = 2; 
                    }
                }
            }
            // Desenho 
            BeginDrawing(); 
                ClearBackground(RAYWHITE);
                DrawText(TextFormat("%d", atktime), 0, 0, 30, BLACK);
                if(atktime > 0) {
                    DrawRectangleRec(playerattack, GREEN);
                }
                DrawRectangleRec(player.position, BLUE);
                for(int i = 0; i < numinimigos; i++) {
                    DrawRectangleRec(enemies[i].position, RED);
                }    
            EndDrawing();
        }

        else if(gamestate == 1) {
            if(IsKeyPressed(KEY_ENTER)) {
                player.position.x = 250;
                player.position.y = 250;
                for(int i = 0; i < 5; i++) {
                    enemies[i].speed = 4.0f;
                    enemies[i].position.height = 50;
                    enemies[i].position.width = 50;
                    enemies[i].position.x = 0 - 100*i;
                    enemies[i].position.y = 0 - 100*1;
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
                player.position.x = 250;
                player.position.y = 250;
                for(int i = 0; i < 5; i++) {
                    enemies[i].speed = 4.0f;
                    enemies[i].position.height = 50;
                    enemies[i].position.width = 50;
                    enemies[i].position.x = 0 - 100*i;
                    enemies[i].position.y = 0 - 100*1;
                    numinimigos++;
                }
                atkdelay = 0;
                atktime = 0;
                gamestate = 0;
            }
            BeginDrawing();
                ClearBackground(GREEN);
                DrawText("LESGO", player.position.x, player.position.y, 30.0f, DARKGREEN) ;
            EndDrawing();
        }

    }
    
    CloseWindow();

    return 0;
}
