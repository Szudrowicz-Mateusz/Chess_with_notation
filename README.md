Chess with chess notation saved in DataBase.
To this project you need a SQLite3 and SFML.

I used Leftis's project as blueprint to avoid creating a chess again so in the subfolder we have second README.md written by Lefti.

I created files "database.h" and "database.cpp" and modfied "chessGame.cpp" a lot so if someone want to see original project is here :
    ## **https://github.com/Lefti97/Chess-SFML**

I also change the structure of CMake files so intruction given by Lefti will be usless.
I use CMake and I will provide here how to do it:

    0) cd Chess_withDB
    1) mkdir build
    2) cd build
    3) cmake ..
    4) make
    5) ./p

For looking into DB you can use variety of programs but I ryly recomend using **DB Browser for SQLite**. Easily access to data. Only thing that you need use refresh button if you want to see new moves if you have opend DB.

This is pretty neat project based on talking on vacation about chess. I wanted to create my DB. In near futer probably I will want to check whole game move by move thanks to this notation.

