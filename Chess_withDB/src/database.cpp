#include "database.h"

Database::Database()
{
    if(init())
        std::cout<<"Your database is working \n";
    else
        std::cerr<<"Your database don't work \n";
}

bool Database::init()
{
    int rc = sqlite3_open("../database/mydatabase.db", &db);

    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << '\n';
        return false;
    }

const char* createGameTableSQL = "CREATE TABLE IF NOT EXISTS game ("
                                "Id_game INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "time INTEGER,"
                                "score TEXT,"
                                "date DATE DEFAULT CURRENT_DATE"
                                ");";

    rc = sqlite3_exec(db, createGameTableSQL, nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    } 

const char* createCourseTableSQL = "CREATE TABLE IF NOT EXISTS the_course_of_the_game ("
                                   "Id_move INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "Id_game INTEGER,"
                                   "move TEXT,"
                                   "left_time INTEGER,"
                                   "FOREIGN KEY (Id_game) REFERENCES game (Id_game)"
                                   ");";
    rc = sqlite3_exec(db, createCourseTableSQL, nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    } 

    return true;
}

int Database::callback_last_game(void* data, int argc, char** argv, char** azColName) {
    int* lastGameId = static_cast<int*>(data);
    *lastGameId = std::stoi(argv[0]);
    return 0; // Return 0 to indicate success
}

bool Database::getGame_id() {
    const char* getLastGameIdSQL = "SELECT Id_game FROM game ORDER BY Id_game DESC LIMIT 1;";
    

    int rc = sqlite3_exec(db, getLastGameIdSQL, callback_last_game, &game_id, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error retrieving last game ID: " << sqlite3_errmsg(db) << std::endl;
        return false; // Return an error value or handle the error accordingly
    }

    return true;
}

bool Database::startGame(int time, const std::string& score) {
    // Prepare the SQL statement for insertion
    std::string insertGameSQL = "INSERT INTO game (time, score) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insertGameSQL.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the parameters (time and score) to the SQL statement
    rc = sqlite3_bind_int(stmt, 1, time);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding time parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_bind_text(stmt, 2, score.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding score parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Execute the insertion statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finalize the statement and return success
    sqlite3_finalize(stmt);

    getGame_id();

    return true;
}

bool Database::insertMoves(std::string moves, int old_pos)
{
    /*
    Why this is so ugly ? Good question if you asekd.
    I used someone else code as blueprint for my project.
    I find it pretty useful that I have most of the information in one displayed string on screen.
    So I took this string ,,Black Pawn to position X: 1 Y: 8".
    From this I changed to notation as it should be.
    */

    if(!tmp.empty())
        if(tmp[0]=='0'){
            return false;
        }

    // Searching in string what kind of figure we are moving
    std::size_t found = moves.find("King");
    if (found!=std::string::npos)
        tmp.push_back('K');
    else{
        found = moves.find("Knight");
        if (found!=std::string::npos)
            tmp.push_back('N');
        else{
            found = moves.find("Bishop");
            if (found!=std::string::npos)
                tmp.push_back('B');
            else{
                found = moves.find("Rook");
                if (found!=std::string::npos)
                    tmp.push_back('R');
                else{
                    found = moves.find("Queen");
                    if (found!=std::string::npos)
                        tmp.push_back('Q');
                }
            }
        }
    }


    // This notation is for which column  or figure start their move
    // Finally I remove it beacuse it looks awful but if someone need it.
    /*
    switch (old_pos)
    {
    case 0: case 8: case 16: case 24: case 32: case 40: case 48: case 56:
        tmp.push_back('a');
        break;
    case 1: case 9: case 17: case 25: case 33: case 41: case 49: case 57:
        tmp.push_back('b');
        break;
    case 2: case 10: case 18: case 26: case 34: case 42: case 50: case 58:
        tmp.push_back('c');
        break;
    case 3: case 11: case 19: case 27: case 35: case 43: case 51: case 59:
        tmp.push_back('d');
        break;
    case 4: case 12: case 20: case 28: case 36: case 44: case 52: case 60:
        tmp.push_back('e');
        break;
    case 5: case 13: case 21: case 29: case 37: case 45: case 53: case 61:
        tmp.push_back('f');
        break;
    case 6: case 14: case 22: case 30: case 38: case 46: case 54: case 62:
        tmp.push_back('g');
        break;
    case 7: case 15: case 23: case 31: case 39: case 47: case 55: case 63:
        tmp.push_back('h');
        break;
    }
    */


    // There we have a column which our figure end on
    found = moves.find("X: ") + 3;

    switch(moves[found])
    {
        case '1':
            tmp.push_back('a');
            break;
        case '2':
            tmp.push_back('b');
            break;
        case '3':
            tmp.push_back('c');
            break;
        case '4':
            tmp.push_back('d');
            break;
        case '5':
            tmp.push_back('e');
            break;
        case '6':
            tmp.push_back('f');
            break;
        case '7':
            tmp.push_back('g');
            break;
        case '8':
            tmp.push_back('h');
            break;
    }

    found = moves.find("Y: ") + 3;

    // This is a row on which our figure end on
    tmp.push_back(moves[found]);

    return true;
}

void Database::pieceIsTaken()
{
    tmp.insert(1,":");
}

void Database::mate()
{
    tmp.push_back('+');
}

void Database::checkMate(bool black_win)
{
    tmp.push_back('#');

    if(black_win)
        updateGameScore("0-1");
    else
        updateGameScore("1-0");
}

void Database::draw()
{
    tmp.push_back('=');
    updateGameScore("0.5-0.5");
}

void Database::castling(bool is_short)
{
    if (is_short)
        tmp = "0-0";
    else
        tmp = "0-0-0";
}

void Database::flush(const int left_time)
{
    if(!tmp.empty())
        insertCourseData(left_time);
        
    tmp = "";
}

bool Database::insertCourseData(const int left_time){

    // Prepare the SQL statement for insertion
    std::string insertCourseSQL = "INSERT INTO the_course_of_the_game (Id_game, move, left_time) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insertCourseSQL.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the parameters (Id_game, move, and left_time) to the SQL statement
    rc = sqlite3_bind_int(stmt, 1, game_id);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding Id_game parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_bind_text(stmt, 2, tmp.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding move parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    
    rc = sqlite3_bind_int(stmt, 3, left_time);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding left_time parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Execute the insertion statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finalize the statement and return success
    sqlite3_finalize(stmt);
    return true;
}

bool Database::updateGameScore(const std::string& new_score) {
    // Prepare the SQL statement for update
    std::string updateScoreSQL = "UPDATE game SET score = ? WHERE Id_game = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, updateScoreSQL.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing update statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the parameters (new_score and game_id) to the SQL statement
    rc = sqlite3_bind_text(stmt, 1, new_score.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding new_score parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_bind_int(stmt, 2, game_id);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding game_id parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Execute the update statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Error updating data: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finalize the statement and return success
    sqlite3_finalize(stmt);
    return true;
}