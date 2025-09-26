let furi_record_open = ffi("void* furi_record_open(char*)");
let gui_direct_draw_acquire = ffi("void* gui_direct_draw_acquire(void*)");
let gui_direct_draw_release = ffi("void gui_direct_draw_release(void*)");
let commit = ffi("void canvas_commit(void*)");
let clear = ffi("void canvas_clear(void*)");
let draw_circle = ffi("void canvas_draw_circle(void*,int,int,int)");;
let draw_dot = ffi("void canvas_draw_dot(void*,int,int)");
let draw_frame = ffi("void canvas_draw_frame(void*,int,int,int,int)");
let draw_line = ffi("void canvas_draw_line(void*,int,int,int,int)");
let draw_str = ffi("void canvas_draw_str(void*, int, int, char*)");
let Math = require("math");


let gui = furi_record_open("gui");
let canvas = gui_direct_draw_acquire(gui);

let running = true;
let sun = 50;

let board = [];
for (let y = 0; y < 4; y++) {
    let row = [];
    for (let x = 0; x < 9; x++) {
        row.push(0);
    }
    board.push(row);
}

let player_x = 6;
let player_y = 6;
let choice = 0;

let plants = [
    [[2, 2], [4, 2], [5, 2], [6, 2], [3, 3], [7, 3], [9, 3], [3, 4], [8, 4], [9, 4], [4, 5], [5, 5], [6, 5], [7, 5], [9, 5], [5, 6], [2, 7], [3, 7], [5, 7], [7, 7], [8, 7], [3, 8], [4, 8], [5, 8], [6, 8], [7, 8], [5, 9]],
    [[4, 2], [5, 2], [6, 2], [3, 3], [7, 3], [2, 4], [4, 4], [6, 4], [8, 4], [3, 5], [5, 5], [7, 5], [4, 6], [5, 6], [6, 6], [2, 7], [3, 7], [5, 7], [7, 7], [8, 7], [3, 8], [4, 8], [5, 8], [6, 8], [7, 8], [5, 9]],
    [[4, 2], [5, 2], [6, 2], [7, 2], [3, 3], [8, 3], [2, 4], [9, 4], [2, 5], [4, 5], [7, 5], [9, 5], [2, 6], [5, 6], [6, 6], [9, 6], [2, 7], [9, 7], [3, 8], [8, 8], [4, 9], [5, 9], [6, 9], [7, 9]],
    [[5, 2], [6, 2], [7, 2], [5, 3], [6, 3], [7, 3], [6, 4], [6, 5], [5, 6], [6, 6], [7, 6], [4, 7], [8, 7], [3, 8], [5, 8], [7, 8], [9, 8], [3, 9], [4, 9], [5, 9], [6, 9], [7, 9], [8, 9], [9, 9]],
];

function calc_sun()
{
    if ((Math.floor(Math.random() * 100) + 1) === 1)
    {
        sun = Math.min(sun + 25, 999);
    }
}


function draw_plants()
{
    for (let x = 0; x < 9; x++)
    {
        for (let y = 0; y < 4; y++)
        {
            let plant = board[y][x];
            if (plant)
            {
                let coords = plants[plant - 1];
                for (let i = 0; i < coords.length; i++)
                {
                    let a = coords[i][0];
                    let b = coords[i][1];
                    draw_dot(canvas, x * 12 + a, y * 12 + b);
                }
            }
        }
    }
}

function draw_player()
{
    draw_circle(canvas, player_x, player_y, 6);
}

function options(canvas) {
    let offset = 0;
    for (let i = 0; i < plants.length; i++) {
        let plant = plants[i];
        for (let j = 0; j < plant.length; j++) {
            let x = plant[j][0];
            let y = plant[j][1];
            draw_dot(canvas, x + offset, y + 52);
        }
        offset += 12;
    }
}

function draw_board(canvas)
{
    for (let x = 0; x < 108; x += 12)
    {
        for (let y = 0; y < 48; y += 12)
        {
            draw_frame(canvas, x, y, 12, 12);
        }
    }

    for (let x = 0; x < 48; x += 12)
    {
        draw_frame(canvas, x, 52, 12, 12);
    }
}


function planty()
{
    clear(canvas);
    draw_board(canvas);
    options(canvas);
    draw_plants();
    draw_player();
    calc_sun();
    draw_str(canvas, 96, 56, sun.toString());
    commit(canvas);
}

function loop() {
    if (!running)
    {
        return;
    }
    planty();
    setTimeout(loop, 100);
}
loop();
gui_direct_draw_release(gui);
