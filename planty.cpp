#include "dolphin/dolphin.h"
#include <furi.h>
#include <gui/gui.h>
#include <stdlib.h>

constexpr int BOARD_WIDTH = 9;
constexpr int BOARD_HEIGHT = 4;

const char* sun = "50";
int sun_array_count = 2;
const char* sun_array[41] = {
    "0","25","50","75","100","125","150","175","200","225","250","275",
    "300","325","350","375","400","425","450","475","500","525","550","575",
    "600","625","650","675","700","725","750","775","800","825","850","875",
    "900","925","950","975","999"
};

int choice = -1;

int player_x = 6;
int player_y = 6;

int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};

// Plant coordinates
int plants[][28][2] = {
    {{2,2},{4,2},{5,2},{6,2},{3,3},{7,3},{9,3},{3,4},{8,4},{9,4},{4,5},{5,5},{6,5},{7,5},{9,5},{5,6},{2,7},{3,7},{5,7},{7,7},{8,7},{3,8},{4,8},{5,8},{6,8},{7,8},{5,9}},
    {{4,2},{5,2},{6,2},{3,3},{7,3},{2,4},{4,4},{6,4},{8,4},{3,5},{5,5},{7,5},{4,6},{5,6},{6,6},{2,7},{3,7},{5,7},{7,7},{8,7},{3,8},{4,8},{5,8},{6,8},{7,8},{5,9}},
    {{2,2},{3,2},{4,2},{7,2},{8,2},{4,3},{5,3},{6,3},{9,3},{4,4},{7,4},{3,5},{8,5},{3,6},{4,6},{8,6},{2,7},{5,7},{7,7},{9,7},{2,8},{5,8},{8,8},{3,9},{4,9}},
    {{4,2},{5,2},{6,2},{7,2},{3,3},{8,3},{2,4},{9,4},{2,5},{4,5},{7,5},{9,5},{2,6},{5,6},{6,6},{9,6},{2,7},{9,7},{3,8},{8,8},{4,9},{5,9},{6,9},{7,9}},
    {{5,2},{6,2},{7,2},{5,3},{6,3},{7,3},{6,4},{6,5},{5,6},{6,6},{7,6},{4,7},{8,7},{3,8},{5,8},{7,8},{9,8},{3,9},{4,9},{5,9},{6,9},{7,9},{8,9},{9,9}}
};

void calc_sun()
{
    if((rand() % 90 + 1) == 1)
    {
        if (sun_array_count + 1 <= 40)
        {
            sun_array_count++;
            sun = sun_array[sun_array_count];
        }
    }
}

void draw_plants(Canvas* canvas)
{
    
    for(int y = 0; y < BOARD_HEIGHT; y++)
    {
        for(int x = 0; x < BOARD_WIDTH; x++)
        {
            int plant = board[y][x];
            if(plant)
            {
                for(int i = 0; i < 26; i++)
                {
                    int a = plants[plant-1][i][0];
                    int b = plants[plant-1][i][1];
                    if(a != 0 && b != 0)
                    {
                        canvas_draw_dot(canvas, x*12 + a, y*12 + b);
                    }
                }
            }
        }
    }
}

void draw_player(Canvas* canvas)
{
    
    canvas_draw_circle(canvas, player_x, player_y, 6);
}

void draw_options(Canvas* canvas)
{ 
    
    int offset = 0;
    for(int i = 0; i < 5; i++)
    {
        int array_size = sizeof(plants[i]) / sizeof(plants[i][0]);
        for(int j = 0; j < array_size; j++)
        {
            int x = plants[i][j][0];
            int y = plants[i][j][1];
            if(x != 0 && y != 0)
            {
                canvas_draw_dot(canvas, x + offset, y + 52);
            }
        }
        offset += 12;
    }
}

void draw_board(Canvas* canvas)
{
    
    for(int x = 0; x < 108; x += 12)
    {
        for(int y = 0; y < 48; y += 12)
        {
            canvas_draw_frame(canvas, x, y, 12, 12);
        }
    }
    for(int x = 0; x < 60; x += 12)
    {
        canvas_draw_frame(canvas, x, 52, 12, 12);
    }
}

static void input_callback(InputEvent* event, void* context)
{
    FuriMessageQueue* queue = (FuriMessageQueue*)context;
    if(event->type == InputTypeShort || event->type == InputTypeRepeat)
    {
        if (event->key == InputKeyUp)
        {
            if (player_y > 6 && player_y <= 42)
            {
                player_y -= 12;
            }
            else if (player_y > 42)
            {
                player_y -= 16;
            }
        }

        if (event->key == InputKeyDown)
        {
            if (player_y < 42)
            {
                player_y += 12;
            }
            else if (player_y < 58)
            {
                player_y += 16;
            }
        }

        if (event->key == InputKeyLeft)
        {
            if (player_x > 6)
            {
                player_x -= 12;
            }
        }

        if (event->key == InputKeyRight)
        {
            if (player_x < 96)
            {
                player_x += 12;
            }
        }

        if (event->key == InputKeyOk)
        {
            if (player_y > 48)
            {
                choice = (player_x - 6) / 12 + 1;
            }
            else
            {
                if (choice == 1 && sun_array_count >= 4)
                {
                    sun_array_count -= 4;
                    sun = sun_array[sun_array_count];
                    board[player_y / 12][player_x / 12] = choice;
                }

                if ((choice == 2 && sun_array_count >= 2) || (choice == 4 && sun_array_count >= 2))
                {
                    sun_array_count -= 2;
                    sun = sun_array[sun_array_count];
                    board[player_y / 12][player_x / 12] = choice;
                }

                if (choice == 3 && sun_array_count >= 6)
                {
                    sun_array_count -= 6;
                    sun = sun_array[sun_array_count];
                    board[player_y / 12][player_x / 12] = choice;
                }

                if (choice == 5 && sun_array_count >= 1)
                {
                    sun_array_count -= 1;
                    sun = sun_array[sun_array_count];
                    board[player_y / 12][player_x / 12] = choice;
                }
            }
        }
    }
    furi_message_queue_put(queue, event, FuriWaitForever);
}


static void draw_callback(Canvas* canvas, void* context)
{
    UNUSED(context);
    canvas_clear(canvas);
    draw_board(canvas);   
    draw_options(canvas);
    draw_plants(canvas);
    draw_player(canvas);
    calc_sun();
    canvas_draw_str(canvas, 96, 56, sun);
    canvas_commit(canvas);
}

int main()
{
    FuriMessageQueue* queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, draw_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, queue);
    Gui* gui = (Gui*)furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    dolphin_deed(DolphinDeedPluginGameStart);
    InputEvent event;
    bool running = true;
    while(running) {
        if(furi_message_queue_get(queue, &event, FuriWaitForever) == FuriStatusOk) {
            if(event.type == InputTypeShort && event.key == InputKeyBack) {
                running = false;
            }
        }
        view_port_update(view_port);
    }

    view_port_enabled_set(view_port, false);
    furi_message_queue_free(queue);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);
    return 0;
}
