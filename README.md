Chess with chess notation saved in DataBase.
To this project you need a SQLite3 and SFML.

I used Leftis's project as blueprint to avoid creating a chess again so in the subfolder we have second README.md written by Lefti.

I created files "database.h" and "database.cpp" and modfied "chessGame.cpp" a lot so if someone want to see original project is here :
    ## **https://github.com/Lefti97/Chess-SFML**

I also change the structure of CMake files so intruction given by Lefti will be usless.

CMake:

    1) mkdir build && cd build
    2) cmake ..
    3) make
    4) ./p

For looking into DB you can use variety of programs but I ryly recomend using **DB Browser for SQLite**. Easily access to data. Only thing that you need use refresh button if you want to see new moves if you have opend DB.

This is pretty neat project based on talking on vacation about chess. I wanted to create my DB. In near futer probably I will want to check whole game move by move thanks to this notation.

