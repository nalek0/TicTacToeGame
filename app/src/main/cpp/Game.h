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

class RenderButton {
public:
    virtual bool contains(float x, float y) = 0;
    virtual void onClick(void * context) = 0;
};

class TableCell {
private:
    CellState _state;
public:
    TableCell() : _state(TOE) { }

    CellState getState();
    void setState(CellState state);
};

class TableCellButton : public RenderButton {
private:
    float x_, y_, width_, height_;
    std::size_t x_ind_, y_ind_;
public:
    TableCellButton(float x, float y, float width, float height, std::size_t x_ind, std::size_t y_ind) :
            x_(x), y_(y), width_(width), height_(height), x_ind_(x_ind), y_ind_(y_ind) { }

    bool contains(float x, float y);
    void onClick(void * context);
};

class RestartButton : public RenderButton {
private:
    float x_, y_, width_, height_;
public:

    RestartButton(float x, float y, float width, float height) :
            x_(x), y_(y), width_(width), height_(height) { }

    bool contains(float x, float y);
    void onClick(void * context);

};

class ChangeLevelButton : public RenderButton {
private:
    float x_, y_, width_, height_;
    int levelDiff_;
public:

    ChangeLevelButton(float x, float y, float width, float height, int levelDiff) :
            x_(x), y_(y), width_(width), height_(height), levelDiff_(levelDiff) { }

    bool contains(float x, float y);
    void onClick(void * context);

};

enum TableState {
    TIC_WIN,
    TAC_WIN,
    PLAYING,
    DRAW,
};

class TableData {
private:
    std::vector<std::vector<TableCell>> _table;
    std::size_t width_, height_, win_length_;
public:

    TableData(int level);

    TableState getState();

    std::size_t getWidth();
    std::size_t getHeight();

    void setCell(std::size_t x, std::size_t y, CellState state);
    CellState getCell(std::size_t x, std::size_t y);

};

class Game {
private:
    int level_;
    std::size_t step_;
    android_app * app_;
public:
    TableData tableData;

    Game(android_app * app);
    CellState eventState();
    void click(std::size_t x_ind, std::size_t y_ind);
    void restart();
    void addLevel(int levelDiff);

};

#endif //MYGAME_GAME_H
