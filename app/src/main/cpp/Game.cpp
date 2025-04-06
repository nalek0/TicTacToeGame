//
// Created by Nalek0 on 06.04.2025.
//

#include <EGL/egl.h>
#include <cassert>
#include <string>
#include "Game.h"

TableCell::TableCell() {
    this->_state = CellState::TOE;
}

CellState TableCell::getState() {
    return this->_state;
}

void TableCell::setState(CellState state) {
    this->_state = state;
}

TableData::TableData() { }

TableData::TableData(android_app * app) {
    this->app_ = app;

    for (std::size_t x = 0; x < TABLE_SIZE; x++) {
        for (std::size_t y = 0; y < TABLE_SIZE; y++) {
            this->_table[y][x] = TableCell();
        }
    }
}

TableState TableData::getState() {
    // Horizontal win
    for (std::size_t y = 0; y < this->getHeight(); y++) {
        for (std::size_t x_start = 0; x_start + WIN_LENGTH < this->getWidth(); x_start++) {
            auto value = this->getCell(x_start, y);
            bool same = true;

            for (std::size_t i = 0; i < WIN_LENGTH; i++) {
                std::size_t x = x_start + i;
                if (this->getCell(x, y) != value) {
                    same = false;
                }
            }

            if (same && value == TIC) {
                return TIC_WIN;
            } else if (same && value == TAC) {
                return TAC_WIN;
            }
        }
    }

    // Vertical win
    for (std::size_t x = 0; x < this->getHeight(); x++) {
        for (std::size_t y_start = 0; y_start + WIN_LENGTH < this->getHeight(); y_start++) {
            auto value = this->getCell(x, y_start);
            bool same = true;

            for (std::size_t i = 0; i < WIN_LENGTH; i++) {
                std::size_t y = y_start + i;
                if (this->getCell(x, y) != value) {
                    same = false;
                }
            }

            if (same && value == TIC) {
                return TIC_WIN;
            } else if (same && value == TAC) {
                return TAC_WIN;
            }
        }
    }

    // Diagonal ++ win
    for (std::size_t x_start = 0; x_start + WIN_LENGTH < this->getHeight(); x_start++) {
        for (std::size_t y_start = 0; y_start + WIN_LENGTH < this->getHeight(); y_start++) {
            auto value = this->getCell(x_start, y_start);
            bool same = true;

            for (std::size_t i = 0; i < WIN_LENGTH; i++) {
                std::size_t x = x_start + i;
                std::size_t y = y_start + i;

                if (this->getCell(x, y) != value) {
                    same = false;
                }
            }

            if (same && value == TIC) {
                return TIC_WIN;
            } else if (same && value == TAC) {
                return TAC_WIN;
            }
        }
    }

    // Diagonal -- win
    for (std::size_t x_start = 0; x_start + WIN_LENGTH < this->getHeight(); x_start++) {
        for (std::size_t y_start = WIN_LENGTH; y_start < this->getHeight(); y_start++) {
            auto value = this->getCell(x_start, y_start);
            bool same = true;

            for (std::size_t i = 0; i < WIN_LENGTH; i++) {
                std::size_t x = x_start + i;
                std::size_t y = y_start - i;

                if (this->getCell(x, y) != value) {
                    same = false;
                }
            }

            if (same && value == TIC) {
                return TIC_WIN;
            } else if (same && value == TAC) {
                return TAC_WIN;
            }
        }
    }

    // Check draw
    bool found_toe = false;
    for (std::size_t x = 0; x < this->getWidth(); x++) {
        for (std::size_t y = 0; y < this->getHeight(); y++) {
            if (this->getCell(x, y) == TOE) {
                found_toe = true;
            }
        }
    }

    if (!found_toe) {
        return DRAW;
    } else {
        return PLAYING;
    }
}

std::size_t TableData::getWidth() {
    return TABLE_SIZE;
}

std::size_t TableData::getHeight() {
    return TABLE_SIZE;
}

void TableData::setCell(std::size_t x, std::size_t y, CellState state) {
    assert(x >= 0 && x < this->getWidth());
    assert(y >= 0 && y < this->getWidth());

    this->_table[y][x].setState(state);
}

CellState TableData::getCell(std::size_t x, std::size_t y) {
    assert(x >= 0 && x < this->getWidth());
    assert(y >= 0 && y < this->getHeight());

    return this->_table[y][x].getState();
}

Game::Game(android_app * app) {
    app_ = app;
    step_ = 0;
    tableData = TableData(app);
}

CellState Game::eventState() {
    if (step_++ % 2 == 0) {
        return TIC;
    } else {
        return TAC;
    }
}

