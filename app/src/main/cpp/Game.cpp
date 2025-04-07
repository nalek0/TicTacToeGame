//
// Created by Nalek0 on 06.04.2025.
//

#include <EGL/egl.h>
#include <cassert>
#include <string>
#include "Game.h"

CellState TableCell::getState() {
    return this->_state;
}

void TableCell::setState(CellState state) {
    this->_state = state;
}

bool TableCellButton::contains(float x, float y) {
    return x_ < x
           && x_ + width_ > x
           && y_ < y
           && y_ + height_> y;
}

void TableCellButton::onClick(void * context) {
    Game * context_game = reinterpret_cast<Game *>(context);

    context_game->click(x_ind_, y_ind_);
}

bool RestartButton::contains(float x, float y) {
    return x_ < x
           && x_ + width_ > x
           && y_ < y
           && y_ + height_> y;
}

void RestartButton::onClick(void * context) {
    Game * context_game = reinterpret_cast<Game *>(context);
    context_game->restart();
}

bool ChangeLevelButton::contains(float x, float y) {
    return x_ < x
           && x_ + width_ > x
           && y_ < y
           && y_ + height_> y;
}

void ChangeLevelButton::onClick(void * context) {
    Game * context_game = reinterpret_cast<Game *>(context);
    context_game->addLevel(levelDiff_);
}

TableData::TableData(int level) {
    switch (level) {
        case 1:
            width_ = 3;
            height_ = 3;
            win_length_ = 3;
            break;
        case 2:
            width_ = 5;
            height_ = 5;
            win_length_ = 4;
            break;
        case 3:
            width_ = 10;
            height_ = 10;
            win_length_ = 5;
            break;
        default:
            assert(false);
    }

    this->_table = std::vector<std::vector<TableCell>>(height_);

    for (std::size_t y = 0; y < height_; y++) {
        this->_table[y] = std::vector<TableCell>(width_);
    }

    for (std::size_t x = 0; x < width_; x++) {
        for (std::size_t y = 0; y < height_; y++) {
            this->_table[y][x] = TableCell();
        }
    }
}

TableState TableData::getState() {
    // Horizontal win
    for (std::size_t y = 0; y < this->getHeight(); y++) {
        for (std::size_t x_start = 0; x_start + win_length_ <= this->getWidth(); x_start++) {
            auto value = this->getCell(x_start, y);
            bool same = true;

            for (std::size_t i = 0; i < win_length_; i++) {
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
        for (std::size_t y_start = 0; y_start + win_length_ <= this->getHeight(); y_start++) {
            auto value = this->getCell(x, y_start);
            bool same = true;

            for (std::size_t i = 0; i < win_length_; i++) {
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
    for (std::size_t x_start = 0; x_start + win_length_ <= this->getHeight(); x_start++) {
        for (std::size_t y_start = 0; y_start + win_length_ <= this->getHeight(); y_start++) {
            auto value = this->getCell(x_start, y_start);
            bool same = true;

            for (std::size_t i = 0; i < win_length_; i++) {
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
    for (std::size_t x_start = 0; x_start + win_length_ <= this->getWidth(); x_start++) {
        for (std::size_t y_start = win_length_ - 1; y_start < this->getHeight(); y_start++) {
            auto value = this->getCell(x_start, y_start);
            bool same = true;

            for (std::size_t i = 0; i < win_length_; i++) {
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
    return width_;
}

std::size_t TableData::getHeight() {
    return height_;
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

Game::Game(android_app * app) :
        app_(app), step_(0), level_(1), tableData(TableData(1)) { }

CellState Game::eventState() {
    if (step_++ % 2 == 0) {
        return TIC;
    } else {
        return TAC;
    }
}

void Game::click(std::size_t x_ind, std::size_t y_ind) {
    if (tableData.getCell(x_ind, y_ind) == TOE) {
        tableData.setCell(x_ind, y_ind, eventState());
    }
}

void Game::restart() {
    for (std::size_t x = 0; x < tableData.getWidth(); x++) {
        for (std::size_t y = 0; y < tableData.getHeight(); y++) {
            tableData.setCell(x, y, TOE);
        }
    }
}

void Game::addLevel(int levelDiff) {
    step_ = 0;
    level_ = level_ + levelDiff;
    level_ = std::min(level_, 3);
    level_ = std::max(level_, 1);
    tableData = TableData(level_);
}
