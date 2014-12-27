#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "routine.h"
using namespace std;

////////////////////////////////
//  Author: Kyle Zampell
//  Connect Four Game
//  CS 420, Spring 2014
//  Professor Tang
///////////////////////////////

void routine(){
	//This is the main logic that powers the decision-making process.
	int board[8][8]; //The game board, stored as an array
	int maxdepth; //The maximum number of moves to check ahead.
	//Note that testing was done with a maxdepth of 4, and a time limit of 30 seconds.
	bool iscomputerfirst = false; //Who makes the first move?
	int timeLimit;  //Prevent the computer from taking all day
	time_t start, check;  //Uses the internal clock to measure elapsed time.
	int compare; //This is Beta, or the baseline value for the ultimate board state.  Serves as a way to skip over inferior options.
	bool firstMove = true; //Is this the first move the computer makes?
	
	cout << "Would you like to go first? (y/n): "; //PC asks if it goes first
    char input;
    cin >> input;
	tolower(input);
    if(input == 'y') { iscomputerfirst = false;} //If a y character is given, the player wishes to make the first move
    else {iscomputerfirst = true;}
    cout << endl << "Enter a time limit per move (in seconds): ";
	//This program can run for a long time if allowed.  This time limit was to establish a limit on runtime.
    cin >> timeLimit;
	cout << endl << "How many moves ahead would you like the computer to think? (Will do one further than entered): ";
	cin >> maxdepth;  //Asks the player how smart they want the computer to be.  Fewer is faster.
    printboard(); //Display the current game board
	if(iscomputerfirst){
        while (true) //If the PC goes first, it executes operations in this order.
        {
            makemove();
            checkGameOver(); //Checks if the game is over after each move.
            getamove();
            checkGameOver();
        }
    } else {
        while (true){
            getamove();
            checkGameOver();
            makemove();
            checkGameOver();
        }
    }
}

//This method is about finding the best move the player can make. Lower score is better.
int min(int depth)
{   
	int best=20000,score,points,repeat=0;
    time(&check);
    if(check - start >= timeLimit){return best;}
    points = evaluate(board);
	if (depth == 0 || points == -5 || points == 5)  return (points);
	for(int radius=0; radius<4; radius++){
		for(int vertical=0; vertical<=radius; vertical++){
			for(int horizontal=0; horizontal<=radius; horizontal++){
				if(vertical!=radius && horizontal!=radius){ horizontal=radius;} //Forces the program to skip all values not on the outer edge (radius).
				
				if (board[3-vertical][3-horizontal]==0){ //Upper Left Quad
					board[3-vertical][3-horizontal]=2; // make move on board
					score = max(depth-1);
					if (score < best){ best=score; }
					board[3-vertical][3-horizontal]=0; // undo move
					if(compare && depth==1 && score < compare){ repeat++;}
					time(&check);
					if(check - start >= timeLimit){return best;}
				}
				
				if (board[4+vertical][4+horizontal]==0){ //Bottom Right Quad
					board[4+vertical][4+horizontal]=2; // make move on board
					score = max(depth-1);
					if (score < best){ best=score; }
					board[4+vertical][4+horizontal]=0; // undo move
					if(compare && depth==1 && score < compare){ repeat++;}
					time(&check);
					if(check - start >= timeLimit){return best;}
				}
				
				if (board[4+vertical][3-horizontal]==0){ //Bottom Left Quad
					board[4+vertical][3-horizontal]=2; // make move on board
					score = max(depth-1);
					if (score < best){ best=score; }
					board[4+vertical][3-horizontal]=0; // undo move
					if(compare && depth==1 && score < compare){ repeat++;}
					time(&check);
				   if(check - start >= timeLimit){return best;}
				}
				
				if (board[3-vertical][4+horizontal]==0){ //Upper Right Quad
					board[3-vertical][4+horizontal]=2; // make move on board
					score = max(depth-1);
					if (score < best){ best=score; }
					board[3-vertical][4+horizontal]=0; // undo move
					if(compare && depth==1 && score < compare){ repeat++;}
					time(&check);
					if(check - start >= timeLimit){return best;}
				}
				
				if(repeat > 24){ return best;} //Delayed alpha-beta pruning, see explanation under max section.
			}
		}
  }
  if(depth==1 && !compare){compare = best;} //Checks if the compare value has been initialized.
  else if(depth==1 && compare){if(compare > best){compare=best;}} //Initializes Beta for the Alpha-Beta Pruning
  return(best); //Returns minimum value found
}

//This method seeks the best possible move the computer can make.
int max(int depth)
{
    int best=-20000,score,points, repeat=0;
    time(&check);
    if(check - start >= timeLimit){return best;}
    points = evaluate(board);
    if (depth == 0 || points == -5 || points == 5)  return (points); //Checks for Leaf status, Win or Loss board state.
    for(int radius=0; radius<4; radius++){     //This expands the search outwards from the central four spots
        for(int vertical=0; vertical<=radius; vertical++){ //Active Row
            for(int horizontal=0; horizontal<=radius; horizontal++){    //Active Column
                if(horizontal!=radius && vertical!= radius){horizontal=radius;}
				
                if (board[3-vertical][3-horizontal]==0){ //Upper Left Quadrant
                    board[3-vertical][3-horizontal]=1; //Test a move on the board
                    score = min(depth-1); //Evaluate how desirable it is
                    if (score > best){ best=score; } //If it's good, remember how good it was.
                    board[3-vertical][3-horizontal]=0; //Undo test move
                    if (compare && score < compare){ repeat++;} //Count number of times a result was less than Beta (compare).
                    time(&check);
                    if(check - start >= timeLimit){return best;} //Checks to make sure there's still time left.
                }
				
                if (board[4+vertical][4+horizontal]==0){ //Lower Right Quadrant
                    board[4+vertical][4+horizontal]=1; // make move on board
                    score = min(depth-1);
                    if (score > best){ best=score; }
                    board[4+vertical][4+horizontal]=0; // undo move
                    if (compare && score < compare){ repeat++; }
                    time(&check);
                    if(check - start >= timeLimit){return best;}
                }
				
                if (board[4+vertical][3-horizontal]==0){ //Lover Left Quadrant
                    board[4+vertical][3-horizontal]=1; // make move on board
                    score = min(depth-1);
                    if (score > best){ best=score; }
                    board[4+vertical][3-horizontal]=0; // undo move
                    if (compare && score < compare){ repeat++; }
                    time(&check);
                    if(check - start >= timeLimit){return best;}
                }
				
                if (board[3-vertical][4+horizontal]==0){ //Upper Right Quadrant
                    board[3-vertical][4+horizontal]=1; // make move on board
                    score = min(depth-1);
                    if (score > best){ best=score; }
                    board[3-vertical][4+horizontal]=0; // undo move
                    if (compare && score < compare){ repeat++; }
                    time(&check);
                    if(check - start >= timeLimit){return best;}
                }
				
                if(repeat > 24){ return best;} //If the same heuristic occurred too many times, then prune the remainder of the possible game boards.
            } //The above limit was determined by testing, where a lower limit would result in inability to detect a success play, even with 3 in a row.
        }
    }
  return(best); //Returns largest result.
}

void makemove()
{
    int best=-200000,depth=maxdepth,score,myVertical,myHorizontal,f=3,g=3;
    while(firstMove){ //If the program gets to place the first piece
        if(board[f][g]==0){myVertical=f; myHorizontal=g; firstMove=false; goto terminus;} //Forces to play towards the center of the board.
        f++;
        g++; //It takes either the spot at D4 or E5.
    }
    time (&start); //Records start time, to keep track of elapsed time.
    for(int radius=0; radius <4; radius++){ //Starts in the center, then expands outward like a ripple on a pond.  4 total rings.
        for(int vertical=0; vertical <=radius; vertical++){
            for(int horizontal=0; horizontal<=radius; horizontal++){
                if(horizontal==radius || vertical==radius){ //Only works on spots along the current ring (radius) to save time (as it's an iterative method).
				
                    if (board[3-vertical][3-horizontal]==0){ //Upper left quadrant
                        board[3-vertical][3-horizontal]=1; // make move on board
                        score = min(depth-1); //Evaluates the heuristic value of possible boards if the computer places its piece here.
                        if (score > best) { myVertical=(3-vertical); myHorizontal=(3-horizontal); best=score; } //If it's a better score than previous, remember it.
                        board[3-vertical][3-horizontal]=0; // undo move
                        time(&check);  //Time elapsed check.
                        if(check - start >= timeLimit){cout << "Time Limit Breached" << endl; goto terminus;} //Move this section into a new method, return boolean for time limit
                    }
					
                    if (board[4+vertical][4+horizontal]==0){ //Bottom Right Quadrant
                        board[4+vertical][4+horizontal]=1; // make move on board
                        score = min(depth-1);
                        if (score > best) { myVertical=(4+vertical); myHorizontal=(4+horizontal); best=score; }
                        board[4+vertical][4+horizontal]=0; // undo move
                        time(&check);
                        if(check - start >= timeLimit){cout << "Time Limit Breached" << endl; goto terminus;}
                    }
					
                    if (board[3-vertical][4+horizontal]==0){ //Bottom Left Quadrant
                        board[3-vertical][4+horizontal]=1; // make move on board
                        score = min(depth-1);
                        if (score > best) { myVertical=(3-vertical); myHorizontal=(4+horizontal); best=score; }
                        board[3-vertical][4+horizontal]=0; // undo move
                        time(&check);
                        if(check - start >= timeLimit){cout << "Time Limit Breached" << endl; goto terminus;}
                    }
					
                    if (board[4+vertical][3-horizontal]==0){ //Upper Right Quadrant
                        board[4+vertical][3-horizontal]=1; // make move on board
                        score = min(depth-1);
                        if (score > best) { myVertical=(4+vertical); myHorizontal=(3-horizontal); best=score; }
                        board[4+vertical][3-horizontal]=0; // undo move
                        time(&check);
                        if(check - start >= timeLimit){cout << "Time Limit Breached" << endl; goto terminus;}
                    }
                }
            }
        }
  }
  terminus: //If the time limit (set at the beginning) is reached/breached, program immediately jumps down here.
  char rowHeader;
  switch(myVertical){ //Purely for display purposes.
        case 0: rowHeader='A'; break;
        case 1: rowHeader='B'; break;
        case 2: rowHeader='C'; break;
        case 3: rowHeader='D'; break;
        case 4: rowHeader='E'; break;
        case 5: rowHeader='F'; break;
        case 6: rowHeader='G'; break;
        case 7: rowHeader='H'; break;
    }
  cout << "My move is:  " << rowHeader << " " << (myHorizontal+1) << endl;
  board[myVertical][myHorizontal]=1; //Places the computer's piece
}

void getamove()
{
	bool movenotgiven = true;
    while(movenotgiven){  //This loop runs until a proper input is given.
		int row, columnHeader;
		char rowHeader;
		cout << "Enter your move (Row then Column):  ";  //Asks player for their move input, specifying format
		cin >> rowHeader >> columnHeader;
		toupper(rowheader);
		columnHeader--; //Decrement to fit array format
		if(columnHeader < 0 || columnHeader>7){ cout<<"That is not a valid entry." << endl;} //If the column header does not fit within the bounds
		switch(rowHeader){ //Switch statement for the row header
			case 'A': row=0; break;
			case 'B': row=1; break;
			case 'C': row=2; break;
			case 'D': row=3; break;
			case 'E': row=4; break;
			case 'F': row=5; break;
			case 'G': row=6; break;
			case 'H': row=7; break;
			default: cout << "Invalid row."<< endl; //If the row header does not fit within the bounds
		}
		if(board[row][columnHeader] == 0){ board[row][columnHeader]=2; movenotgiven=false;} //Checks if the space is empty, and if so places the player's piece
		else{
			cout << "   That space is already occupied, try again." << endl; //Just in case the input is already taken
		}
	}
}

void printboard()
{
    char rowHeader;
    cout << endl;
    cout << "  1 2 3 4 5 6 7 8" << endl;
    for(int row = 0; row < 8; row++){
        switch(row){ //Determine which letter to output
            case 0: rowHeader='A'; break;
            case 1: rowHeader='B'; break;
            case 2: rowHeader='C'; break;
            case 3: rowHeader='D'; break;
            case 4: rowHeader='E'; break;
            case 5: rowHeader='F'; break;
            case 6: rowHeader='G'; break;
            case 7: rowHeader='H'; break;
        }
        cout << rowHeader;
        for(int col = 0; col < 8; col++){
                cout << " ";
                if(board[row][col]==1){cout << "X";} //Computer is always X, regardless of who goes first.
                else if(board[row][col]==2){cout << "O";}
                else {cout << "-";} //Blank space, no entry
        }
        cout << endl;
    }
}