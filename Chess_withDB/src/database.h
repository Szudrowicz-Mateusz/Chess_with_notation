#pragma once

#include <sqlite3.h>
#include <string>
#include <iostream>

class Database{
public:
    Database();
    ~Database() = default;

    // Game status 
    void mate();
    void checkMate(bool black_win);
    void draw();

    // Game updates
    bool insertMoves(std::string moves, int old_pos);
    void pieceIsTaken();
    void castling(bool is_short);

    // Function that triggers function that operates on Database
    void flush(const int left_time);

    // Creating first record that game has started
    bool startGame(int time, const std::string& score);

private:
    // Variables
    sqlite3* db;
    uint game_id;
    std::string tmp; // it should be called ,,move'' but to many lines to change

    // Initializers
    bool init();

    // Getters
    bool getGame_id();

    // Functions that operates on Database 
    // In used
    static int callback_last_game(void* data, int argc, char** argv, char** azColName);
    bool insertCourseData(const int left_time);
    bool updateGameScore(const std::string& new_score);

};