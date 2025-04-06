//
// Created by Nalek0 on 06.04.2025.
//

#ifndef MYGAME_GAME_H
#define MYGAME_GAME_H

#include <EGL/egl.h>
#include <array>
#include "Model.h"

struct android_app;

enum CellState {
    TIC,
    TAC,
    TOE,
};

class TableCell {
private:
    CellState _state;
public:

    TableCell();

    CellState getState();
    void setState(CellState state);

};

#define TABLE_SIZE 3
#define WIN_LENGTH 3

enum TableState {
    TIC_WIN,
    TAC_WIN,
    PLAYING,
    DRAW,
};

class TableData {
private:
    std::array<std::array<TableCell, TABLE_SIZE>, TABLE_SIZE> _table;
    android_app * app_;
public:

    TableData();
    TableData(android_app * app);

    TableState getState();

    std::size_t getWidth();
    std::size_t getHeight();

    void setCell(std::size_t x, std::size_t y, CellState state);
    CellState getCell(std::size_t x, std::size_t y);

};

class Game {
private:
    android_app * app_;
public:
    TableData tableData;

    Game(android_app * app);

};

#endif //MYGAME_GAME_H
