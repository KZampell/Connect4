#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "main.h"
using namespace std;

////////////////////////////////
//  Author: Kyle Zampell
//  Connect Four Game
//  CS 420, Spring 2014
//  Professor Tang
///////////////////////////////



int main ()
{
    setup();
    routine();
}

void setup()
{ for (int i=0; i<8; i++) for (int j=0; j<8; j++) { board[i][j]=0; } } //Initializes a blank board

void checkGameOver()
{
    string x; //Added as a way to allow player to see the last move made for the win state.
    printboard();
    int points = evaluate(board);
    if (points == -5) { cout << "You Win! Press any key to continue." << endl; cin >> x;  exit(0); }
    if (points == 5)  { cout << "I Win! Press any key to continue."   << endl; cin >> x; exit(0); }
}
