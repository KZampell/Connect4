#include "evaluate.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

////////////////////////////////
//  Author: Kyle Zampell
//  Connect Four Game
//  CS 420, Spring 2014
//  Professor Tang
///////////////////////////////

int evaluate(int board[][8])
{
    bool endState = true; //This boolean says the board is full, no more possible moves
	bool isvertical = true;
	int totalPoints=0;
	totalpoints = logic(isvertical);
	isvertical = false;
	totalpoints = logic(isvertical);
    string x; //Player-confirmation input value.
    if(endState){cout << "DRAW! (Enter any key to continue)"; cin >> x; exit(0);} //Tie game
    else {return totalPoints;} //Returns the heuristic value of the game board
}

int logic(bool isvertical){
	//Checks the board in order, depending on what value was passed.
	int opponentPoints, myPoints, blanks, inARow, last, firstSeen;
	
	for (int primary=0; primary<8; primary++){
		if(isvertical) {int col = primary;} //This boolean check allows me to use the same method for checking vertically and horizontally.
		else {int row = primary;}
        blanks = 0; //Empty Spaces
        inARow = 0; //Pieces of the same type in series
        last = 0; //Last type of piece seen
        myPoints=0;	//Computer Points
        opponentPoints=0; //Player points
        firstSeen = -1; //Type of piece previously seen in a series, initially is negative to represent outside of the board.
		
        for (int secondary=0; secondary<8; secondary++){
			if(isvertical) {int row = secondary;}
			else {int col = secondary;}
            if(endState == true && board[secondary][primary]==0){endState = false;} //If an opening is found, there are moves that can be made.
			
            if (board[row][col]==1){ //Computer piece
                if(last == 2 && (firstSeen==1 || firstSeen==-1)){ opponentPoints = 0; inARow=0; firstSeen=2;} //if I did place a chip before theirs, end their streak and remove their points for this primary.
                else if(last==2 && firstSeen==0){ inARow=0; firstSeen=2;} //If there was a blank spot before their chip(s), just end their streak.
                else if(last==0 && blanks==1){
                        if(firstSeen==1){myPoints += pow(10.0, inARow+1);}
                        inARow=0;  //For the case of having a space between two computer pieces.
                }
                else if(last==1){ blanks=0;}
                inARow++; //Increment number of pieces in a series
                last = 1; //Previous piece seen was Computer's
                if (inARow==4) return 5; //Computer wins
                myPoints += pow(10.0, inARow); //More points for placing towards the center of the board
				
            } else if(board[row][col]==2){ //Player Piece
                if(last == 1 && (firstSeen==2 || firstSeen==-1)){ myPoints = 0; inARow=0; firstSeen=1;}
                else if(last==1 && firstSeen==0){ inARow=0; firstSeen=1;}
                else if(last==0 && blanks==1){
                        if(firstSeen==2){opponentPoints += pow(10.0, inARow+1);}
                        inARow=0;
                }
                else if(last==2){blanks=0;}
                inARow++;
                last = 2;  //Previous piece seen was Player's
                if(inARow==4) return -5; //Player wins
                opponentPoints += pow(10.0, inARow); //More points for placing towards the center of the board
				
            } else { //Blank space
                if(blanks == 0 && inARow > 0){ //If first empty space after a series
                    if(last == 1){firstSeen=1;}
                    else if (last == 2){firstSeen=2;}
                }
                if(firstSeen==-1){firstSeen=0;} //Case for edge of the board
                if(blanks ==1 && inARow > 0){ inARow=0;}
                blanks++; //Number of empty spaces in series
                last=0;
            }
        }
        if(inARow>0){//If there was a series up to the end of the board
            if(last==1 && firstSeen==2){//If the series was the computer's, and isn't 4 in a row
                while(inARow>0){
                    myPoints -= pow(10.0, inARow); //Take away points, because it's now a worthless series
                    inARow--;
                }
            }
            else if(last==2 && firstSeen==1){ //If the series was the Player's, and isn't 4 in a row
                while(inARow>0){
                    opponentPoints -= pow(10.0, inARow); //Take away their points
                    inARow--;
                }
            }
        }
        totalPoints = totalPoints + myPoints; //Points for the board (add computer's)
        totalPoints = totalPoints - opponentPoints; //Points for the board (remove player's)
    }
}
