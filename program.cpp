#include "splashkit.h"
#include <iostream>
#include <vector>

using namespace std;

enum Direction {
    STOP,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct point {
    int x;
    int y;
};

int x = 0, y = 0, width = 30, height = 30, score = 0, tile_size, last_x = 0, last_y = 0;
int food_x = rnd(width-1), food_y = rnd(height-1);
vector<point> tail;
Direction dir = STOP;
bool game_over = false;

point new_point(int x, int y) {
    point p;
    p.x = x;
    p.y = y;
    return p;
}

void update() {
    if (x == food_x && y == food_y) {
        score += 10;
        cout << score << endl;
        food_x = rnd(width-1);
        food_y = rnd(height-1);
        tail.insert(tail.begin(), new_point(last_x, last_y));
    }

    if (tail.size() > 0) {
        tail.insert(tail.begin(), new_point(last_x, last_y));
        tail.pop_back();
    }

    if (key_typed(W_KEY) || key_typed(UP_KEY)) {
        dir = UP;
    } else if (key_typed(S_KEY) || key_typed(DOWN_KEY)) {
        dir = DOWN;
    } else if (key_typed(A_KEY) || key_typed(LEFT_KEY)) {
        dir = LEFT;
    } else if (key_typed(D_KEY) || key_typed(RIGHT_KEY)) {
        dir = RIGHT;
    }

    /* TODO
    * run over tail
    * teleport across the map??
    * obstacles? */

    switch(dir) {
        case UP:
            if (y == 0) game_over = true;
            y--;
            break;
        case DOWN:
            if (y == height-1) game_over = true;
            y++;
            break;
        case LEFT:
            if (x == 0) game_over = true;
            x--;
            break;
        case RIGHT:
            if (x == width) game_over = true;
            x++;
            break;
        case STOP:
            break;
    };

    last_x = x, last_y = y;
}

void draw() {
    clear_screen(COLOR_BLACK);
    
    for (int i = 0; i < height-1; i++) {
        for (int j = 0; j < width-1; j++) {
            // DRAW GRID
            // draw_line(COLOR_DARK_GRAY, j*tile_size, 0, j*tile_size, screen_height());
            // draw_line(COLOR_DARK_GRAY, 0, j*tile_size, screen_width(), j*tile_size);
            
            // DRAW OUTTER BOX
            draw_rectangle(COLOR_DARK_GRAY, 0, 0, screen_width(), screen_height());

            if (j == x && i == y) {
                fill_rectangle(COLOR_RED, j*tile_size, i*tile_size, tile_size, tile_size);
            } else if (j == food_x && i == food_y) {
                fill_circle(COLOR_LIGHT_GREEN, (food_x*tile_size)+(tile_size/2), (food_y*tile_size)+(tile_size/2), tile_size/3);
            }

            for (int n = 0; n < tail.size(); n++) {
                if (tail[n].x == j && tail[n].y == i) {
                    fill_rectangle(COLOR_RED, (j*tile_size)+(tile_size/4), (i*tile_size)+(tile_size/4), tile_size/2, tile_size/2);
                }
            }
        }
    }
    refresh_screen(7);
}

int main() {
    open_window("Snake", 600, 600);
    tile_size = screen_width()/width;
    do {
        process_events();
        if (game_over) {
            //end_game_screen();
            return 0;
        } else {
            update();
            cout << score << endl;
            draw();
        }
    } while(!quit_requested());
    return 0;
}