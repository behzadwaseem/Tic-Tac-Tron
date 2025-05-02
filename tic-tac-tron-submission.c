// MTE100 FINAL PROJECT 2024 - TIC-TAC-TRON
// GROUP 27 - BEHZAD WASEEM, BENJAMIN PABLO TRAN, SHILO JEYARAJASINGAM, PARSA AHMADI
/********************************************************************************* */

// motorA is forward and backward robot movement
// motorB is vertical movement of stamping arm
// motorD is sideways movement of stamping arm

// CONSTANTS:
const float CELL_RIGHT = 560.0 / 3.0;					// moves to the next cell on the right
const float COLOUR_DOWN = 119;						// make speed positive to go down
const float STAMP_DOWN = 141;						// distance for stamping
const float BACK_WHEEL_CIRCUMF = 2 * PI * 4.4;				// measurement of back tractor wheel (in cm)
const float CELL_WIDTH = (3.0 / BACK_WHEEL_CIRCUMF) * 360.0;		// enough distance to move up a cell
const float RETURN_REST = ((2 * 3.2) / BACK_WHEEL_CIRCUMF) * 360.0; 	// moves robot out of way for human to play
const float MOVE_START = (0.5 * 5.5 / BACK_WHEEL_CIRCUMF) * 360.0;	// move back distance for makeMove function

// GLOBAL 2D ARRAYS:

// 2D BOARD ARRAY: (records the game state as the game progresses)
int board[3][3] = {{0, 0, 0},
		   {0, 0, 0},
		   {0, 0, 0}};
// 2D POSSIBLE MOVES ARRAY (keeps track of the free cells where moves can be made)
int possibleMoves[9][2] = {{-2, -2},
			   {-2, -2},
			   {-2, -2},
			   {-2, -2},
			   {-2, -2},
			   {-2, -2},
			   {-2, -2},
			   {-2, -2},
			   {-2, -2}};

void clearScreen()
{
	/*
		HELPER FUNCTION - Clears all lines on the robot's display.
	*/
	for (int i = 1; i <= 9; i++)
	{
		displayTextLine(i, "");
	}
}

void stopAllTasks(string message)
{
	/*
		HELPER FUNCTION - Used to completely stop the robot when the emergency stop (enter button) is pressed.
	*/
	while (true)
	{
		motor[motorA] = 0;
		motor[motorB] = 0;
		motor[motorD] = 0;
		displayTextLine(6, message);
	}
}

void moveMotor(int speed, int encoderCounts, tMotor specifiedMotor)
{
	/*
		1. Moves the specified motor at the desired speed and for the desired encoderCounts (distance).
	*/
	nMotorEncoder[specifiedMotor] = 0;
	motor[specifiedMotor] = speed;
	while (abs(nMotorEncoder(specifiedMotor)) < encoderCounts)
	{
		// Speed Control System - reduce speed as robot gets closer to desired encoder count:
		float error = 1.05 - abs(nMotorEncoder[specifiedMotor]) / encoderCounts;
		motor[specifiedMotor] = speed * error;
	}
	motor[specifiedMotor] = 0;
}

void scanBoard()
{
	/*
		2. Moves the robot around the board and scans each cell using the colour sensor
		to determine the current game state.
	*/
	moveMotor(15, COLOUR_DOWN, motorB); // move arm down to get colour sensor in position
	wait1Msec(1000);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// move to cell j in current col only for first two iterations
			if (j != 0) // skip first one
			{
				moveMotor(15, CELL_RIGHT, motorD);
				wait1Msec(1000);
			}

			displayTextLine(6, "OLD CELL VALUE %d", board[i][j]);
			int oldCellValue = board[i][j];
			displayTextLine(7, "");

			// check cell for player's marking:
			if (SensorValue[S4] == (int)colorBlack)
			{
				displayTextLine(5, "BLACK DETECTED");
				board[i][j] = -1;
				if (oldCellValue == 1 && board[i][j] == -1)
				{
					clearScreen();
					displayTextLine(3, "CHEATING DETECTED!");
					string message = "You must exit";
					stopAllTasks(message);
				}
				else
				{

					displayTextLine(7, "NEW CELL VALUE %d", board[i][j]);
					wait1Msec(2000);
				}
			}
			else
			{
				displayTextLine(5, "NOT BLACK!!");
				if (oldCellValue == -1) // if no black was detected at this cell, then its current value is 0 (empty cell) but if its old value was -1; cheating happened
				{
					clearScreen();
					displayTextLine(3, "CHEATING DETECTED!");
					string message = "You must exit!";
					stopAllTasks(message);
				}
			}
		}
		// move arm back to first cell in col:
		moveMotor(-15, 2 * CELL_RIGHT, motorD);
		wait1Msec(1000);
		if (i != 2)
		{
			// move robot to next col:
			moveMotor(15, CELL_WIDTH, motorA);
			wait1Msec(1000);
		}
		else if (i == 2)
		{
			// move robot to rest:
			moveMotor(-15, RETURN_REST, motorA);
		}
	}
	moveMotor(-15, 125, motorB); // move arm back up
	wait1Msec(1000);
}

int makeMove(int cell_x, int cell_y)
{
	/*
		3. Moves robot's arm to specified cell and stamps it to mark its move.
	*/
	displayTextLine(6, "Making Move");

	// move robot back a little to account for stamp distances
	moveMotor(-15, MOVE_START, motorA);
	wait1Msec(1000);

	for (int i = 0; i < 3; i++)
	{
		if (cell_x == i)
		{
			// move arm to cell in the current col:
			moveMotor(15, (CELL_RIGHT * cell_y), motorD);
			wait1Msec(2000);

			// Mark move with stamp:
			moveMotor(15, STAMP_DOWN, motorB); // move arm down to stamp
			wait1Msec(1000);
			moveMotor(-15, STAMP_DOWN, motorB); // move arm back up
			wait1Msec(1000);

			motor[motorB] = 0; // lock arm's vertical movement

			// move arm back to right side of board:
			moveMotor(-15, (CELL_RIGHT * cell_y), motorD);
			wait1Msec(1000);

			// return robot to rest position:
			displayTextLine(2, "Returning to rest");
			wait1Msec(1000);
			moveMotor(-15, CELL_WIDTH * cell_x, motorA); // works!
			displayTextLine(2, "BACK AT REST");
			wait1Msec(1000);
			// Record move in board array:
			displayTextLine(7, "Move made");
			wait1Msec(1000);
			displayTextLine(7, "Updating board");
			board[cell_x][cell_y] = 1; // update board to indicate that robot has occupied this spot

			// move robot forward a little to account for stamp distances
			moveMotor(15, MOVE_START, motorA);
			wait1Msec(1000);
			return 1;
		}
		else
		{
			// move robot to the next col:
			moveMotor(15, CELL_WIDTH, motorA);
			wait1Msec(1000);
		}
	}
	return -1;
}

void findEmpty()
{
	/*
		4. Finds all the currently empty places on the board where the robot can make a legal move.
	*/
	int n = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == 0) // indicates empty
			{
				// record empty position as a possible move:
				possibleMoves[n][0] = i;
				possibleMoves[n][1] = j;
				n++;
			}
			else // board is occupied at this spot
			{
				// update to indicate occupied cell:
				possibleMoves[n][0] = -1;
				possibleMoves[n][1] = -1;
				n++;
			}
		}
	}
}

void findRandom(int *currRandomP)
{
	/*
		5. Gives the coordinates of a random free spot on the board.
	*/
	int flag = -1;
	displayTextLine(8, "Checking for randoms");
	int rand_i = -3;
	int rand_j = -3;
	wait1Msec(1000);

	while (flag == -1)
	{
		int rand_i = random(2);
		int rand_j = random(2);
		displayTextLine(1, "%d", rand_i);
		displayTextLine(2, "%d", rand_j);

		for (int n = 0; n < 9; n++)
		{
			displayTextLine(9, "searching for randoms");
			if (possibleMoves[n][0] == rand_i && possibleMoves[n][1] == rand_j)
			{
				displayTextLine(3, "FOUND RANDOMS");
				*currRandomP = rand_i;
				displayTextLine(4, "%d", *currRandomP);
				currRandomP++;
				*currRandomP = rand_j;
				flag = 1;
				wait1Msec(2000);
			}
		}
	}
}

int evaluateBoard()
{
	/*
		6. Determines if a terminal state has been reached (game ends) by checking for 3 of the same
		moves in a row, or a full board with no winners (draw).
	*/
	for (int i = 0; i < 3; i++)
	{
		// check row:
		if ((board[i][0] == board[i][1] && board[i][0] == board[i][2]) && board[i][0] != 0)
		{
			return board[i][0];
		}
		// check cols:
		else if ((board[0][i] == board[1][i] && board[0][i] == board[2][i]) && board[0][i] != 0)
		{
			return board[0][i];
		}
	}

	// check diagonals:
	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2]) && board[0][0] != 0)
	{
		return board[0][0];
	}
	else if ((board[0][2] == board[1][1] && board[0][2] == board[2][0]) && board[0][2] != 0)
	{
		return board[0][2];
	}

	// check for draw:
	int numZeroes = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == 0)
			{
				numZeroes++;
			}
		}
	}
	if (numZeroes == 0)
	{
		return 0; // indicates a draw
	}
	return -2; // if none of the game-ending conditions are met (game continues)
}

task main()
{
	// CALIBRATE SENSORS:
	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S4] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Color_Color;
	wait1Msec(50);

	bool emergency = true;	// flag to indicate emergency stop is intended
	int currPlayer = 0;		// keeps track of whose turn it currently is (-1 represents human's turn while 1 is for AI)
	int winner = -2;		// keeps track of who wins the game
	bool firstRound = true; // used to check for cheating at game start

	// START UP:
	while (!getButtonPress(buttonEnter))
	{
		displayTextLine(5, "Press left for player first.");
		displayTextLine(6, "Press right for Robot first.");
		// displayTextLine(8, "PS - ur gonna get smoked either way.");

		while (!getButtonPress(buttonAny))
		{
		}
		int currPlayer = 0;
		if (getButtonPress(buttonLeft))
		{
			displayTextLine(5, "Player goes first.");
			displayTextLine(6, "");
			displayTextLine(8, "");
			currPlayer = -1; // indicate that human goes first
			wait1Msec(2000);
		}
		else if (getButtonPress(buttonRight))
		{
			displayTextLine(5, "Robot goes first.");
			displayTextLine(6, "");
			displayTextLine(8, "");
			wait1Msec(2000);
			currPlayer = 1; // indicate that AI goes first
		}

		// MAIN GAME LOOP:
		while (winner == -2 && !getButtonPress(buttonEnter))
		{
			if (currPlayer == -1) // HUMAN'S TURN
			{

				// Wait for touch sensor press:
				while (SensorValue[S1] != 1 && !getButtonPress(buttonEnter))
				{
					displayTextLine(5, "Player's turn.");
				}
				currPlayer = 1;

				// Check for Game End:
				winner = evaluateBoard();
				clearScreen();
			}
			else if (currPlayer == 1) // ROBOT'S TURN
			{
				displayTextLine(5, "Robot's turn.");
				// Keep track of time:
				clearTimer(T1);
				wait1Msec(1000);

				// Scan the Board:
				displayTextLine(8, "Scanning Board...");
				scanBoard();
				clearScreen();

				// Check for cheating on first move:
				if (firstRound == true)
				{
					firstRound = false;
					int boardCount = 0;
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							boardCount += abs(board[i][j]);
						}
						if (boardCount != 0)
						{
							clearScreen();
							displayTextLine(3, "CHEATING DETECTED!");
							string message = "You must exit";
							stopAllTasks(message);
						}
					}
				}

				// Check for Game End:
				winner = evaluateBoard();
				wait1Msec(1000);

				if (winner != 0 && winner != 1 && winner != -1) // only proceed to make a move if a game end is not reached
				{
					// Choose Random Cell to Mark:
					int currRandom[2] = {0, 0};
					findEmpty();
					wait1Msec(1000);
					findRandom(currRandom);
					makeMove(currRandom[0], currRandom[1]);
					wait1Msec(1000);

					currPlayer = -1; // indicate that human goes next

					// winner = 1 // for testing
					winner = evaluateBoard();
					clearScreen();
				}
				// OUTPUT TIME TAKEN FOR ROBOT'S TURN:
				float robotTime = time1[T1] / 1000.0; // convert milliseconds to seconds
				displayTextLine(4, "Robot's Time: %0.2f s", robotTime);
				wait1Msec(2000);
			}
		}

		// SHUTDOWN CODE:
		string winnerTitle = "";
		if (winner == 1)
		{
			winnerTitle = "Robot Won.";
		}
		else if (winner == -1)
		{
			winnerTitle = "You Won. Good job.";
		}
		else if (winner == 0)
		{
			winnerTitle = "Draw. GG.";
		}

		while (!getButtonPress(buttonDown) && !getButtonPress(buttonEnter))
		{
			displayTextLine(1, "Game Ended");
			displayTextLine(5, "%s", winnerTitle);
		}
		if (getButtonPress(buttonDown))
		{
			emergency = false;
		}
	}

	clearScreen();

	if (emergency) // indicate the emergency stop was pressed.
	{
		displayTextLine(1, "GAME ABORTED!");
		wait1Msec(2000);
	}
}
