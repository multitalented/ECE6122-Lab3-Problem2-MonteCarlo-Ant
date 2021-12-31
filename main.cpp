/*
Author: Deanna Gierzak
Class: ECE 6122
Last Date Modified: 10/13/2020

Description:

	This program uses takes in command line arguments of -N (# of trials) and -g (size of grid)
		and uses these to create a grid for Brilli the ant to move along. Her goal is to get 
		from her starting point at the top left of the grid to the bottom right corner of the 
		grid. When she makes it, it is counted as a success. If she gets stuck, it is a failure.

	She can make a move along the grid any time she stays within the bounds of the grid and when
		the move is not a point she has already visited.

		START = (1,1) starting point
			--->._._._._._._.
				|_|_|_|_|_|_|
				|_|_|_|_|_|_|
				|_|_|_|_|_|_|
				|_|_|_|_|_|_|
				|_|_|_|_|_|_|<---GOAL = (n, n) size of grid

	The program uses Monte Carlo Simulation (MCS) technique to find the probability that the ant 
		will make it to the goal point on the grid. MCS uses a sampling of random values to 
		determine probability (typically high for greater accuracy). Multi-threading is used 
		with OpenMP directives to create threads that each carry part of the computational load 
		for the N # of MCS trials.

	For each trial of the ant, a random value is generated and checked if valid to determine the 
		ant's next move. Values are generated in a uniform distributioncfrom int's 1 to 4 to 
		represent the up, down, left, and right directions.

	At the end of the program, the probability is calculated from (# of success) / (N # of trials)
		and output to the text file Lab3Prob2.txt
*/

#include <omp.h>
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
	//double start_time = omp_get_wtime();	// get the start time
	int gridSize = 0;
	int N = 0;
	int routeSuccess = 0;				// count the number of successful routes
	//int routeFail = 0;					// count the number of failed routes
	double PP = 0.0;					// used to calculate the probability of success from #routeSuccess / #routeFail

	for (int ii = 1; ii < argc; ++ii)						// starting from the first commandline argument after the executable location at [0]
	{
		if (!strcmp(argv[ii], "-N") && (argc >= ii + 1))	// check that the current argument is -N for number of trials and that there is a value after it
		{
			N = atoi(argv[ii + 1]);						// next argument is the number of trials
			ii++;
		}
		// next one is number of trials
		else if (!strcmp(argv[ii], "-g") && (argc >= ii + 1))  // check that the current argument is -g for gridsize and that there is a value after it
		{
			gridSize = atoi(argv[ii + 1]);				// next argument is the grid size
			ii++;
		}
	}

	if (N == 0 || gridSize == 0)
	{
		cout << "Must enter both number of runs and grid size.";	// user made an error in command line arguments
	}

	// The start of the parallel region where threads are created... from here, all variables declared after are private/local to each thread
	//		and any variables declared before are public variables for all of the threads to access
#pragma omp parallel
	{
		int k = 0;
		bool foundMove = false;										// boolean to tell whether a valid move was found, used for randomly selecting direction until true
		bool validMove = true;										// boolean to tell whether there is a valid move available, used to determine whether to search
		std::default_random_engine generator(time(0));				//  generator(time(0)); for a new random each time
		std::uniform_int_distribution<int> distribution(1, 4);		// uniform distribution for generating the values 1-4 to determine direction
		int direction = 0;											// the int value of direction that's generated from the random generator and distribution
		int i = 0, j = 0;											// the value of the point on the grid that the ant is located
		bool upMove = false, downMove = false, leftMove = false, rightMove = false;		// booleans to tell whether there is a valid move per direction
		// removed the following for attempted program speed-up
		//int up = 0;				// up direction, value as an int along y-axis
		//int down = 0;				// down direction, value as an int along y-axis
		//int left = 0;				// left direction, value as an int along x-axis
		//int right = 0;			// right direction, value as an int along x-axis
		int ii = 0;				// counter for resetting hasBeenVisited vector to false
		int jj = 0;				// counter for resetting hasBeenVisited vector to false
		vector< vector< bool > > hasBeenVisited(gridSize, vector<bool>(gridSize, false));			// grid of booleans to keep track of what points have been visited by the ant
#pragma omp for reduction(+ : routeSuccess)
		for (int numTrials = 0; numTrials < N; ++numTrials)
		{
			for (ii = 0; ii < gridSize; ++ii)
			{
				for (jj = 0; jj < gridSize; ++jj)
				{
					hasBeenVisited[ii][jj] = false; // reset the hasBeenVisited boolean matrix to false for each new trial of the ant moving through the grid
				}
			}
			i = 0;								// using (0,0) as starting point for the ant to keep aligned with 0-ordered indexing in C++ to avoid confusion 
			j = 0;								// using (0,0) as starting point for the ant to keep aligned with 0-ordered indexing in C++ to avoid confusion 
			validMove = true;					// must always start the trial with validMove = true to start off the while loop
			hasBeenVisited[0][0] = true;		// must always set the first point the ant is on to true to avoid errors

			while (validMove == true)			// while there is known to be a valid move...
			{
				// reset the boolean variables for checking the next point
				foundMove = false; upMove = false; downMove = false; leftMove = false; rightMove = false; validMove = false;

				// tried removing these variable declarations for speed-up, not totally sure it made a difference
				// set the up, down, left, right coordinates of possible directions the ant can go
				//up = i - 1; 
				//down = i + 1;
				//left = j - 1;
				//right = j + 1;

				// UP option
				if(i-1 >= 0 && !hasBeenVisited[i-1][j])		// if moving up, the ant would be within bounds of grid AND that point has not been visited...
				{
					upMove = true;		// ...then up is a valid move --> true!
				}
				// DOWN option
				if(i+1 < gridSize && !hasBeenVisited[i+1][j])	// if moving down, the ant would be within bounds of grid AND that point has not been visited...
				{
					downMove = true;	// ...then down is a valid move --> true!
				}
				// LEFT
				if(j-1 >= 0 && !hasBeenVisited[i][j-1])	// if moving left, the ant would be within bounds of grid AND that point has not been visited...
				{
					leftMove = true;	// ...then left is a valid move --> true!
				}
				//RIGHT
				if(j+1 < gridSize && !hasBeenVisited[i][j+1])	// if moving right, the ant would be within bounds of grid AND that point has not been visited...
				{
					rightMove = true;	// ...then right is a valid move --> true!
				}

				if (rightMove || downMove || leftMove || upMove) // if any of these moves were true...
				{
					validMove = true;	// ...then we know there is a valid move!
				}

				if (validMove == true)							// if there is a valid move...
				{
					while (foundMove == false)					// ...then while a move has not been found yet...
					{
						direction = distribution(generator);	// ...generate a random value for the direction until a valid move is found

						switch (direction)
						{
						case 1:			// UP					// if the generated value is 1
							if (upMove == true)					// and up was a valid move
							{
								//cout << "Option UP";
								i -= 1;							// then set the new point to be up one in y-direction
								foundMove = true;
							}
							break;
						case 2:			// DOWN					// if the generated value is 2
							if (downMove == true)				// and down was a valid move
							{
								//cout << "Option DOWN";
								i += 1;							// then set the new point to be down one in y-direction
								foundMove = true;
							}
							break;
						case 3:			// LEFT					// if the generated value is 3
							if (leftMove == true)				// and left was a valid move
							{
								//cout << "Option LEFT";
								j -= 1;							// then set the new point to be left one in x-direction
								foundMove = true;
							}
							break;
						case 4:			// RIGHT				// if the generated value is 4
							if (rightMove == true)				// and right was a valid move
							{
								//cout << "Option RIGHT";
								j += 1;							// then set the new point to be right one in x-direction
								foundMove = true;
							}
							break;
						}
					}
					hasBeenVisited[i][j] = true;				// and now set the new point to true for having been visited!
				}
				/*
				else		// removed because unnecessary, and to speed up program
				{
					routeFail += 1;				// if there was no valid move for the ant to make, then this is a failed route (she got stuck)
					//validMove = false;			// set validMove to
				}
				*/
				if (i == gridSize-1 && j == gridSize-1)
				{
//#pragma omp critical					// this ended up being slower than using the reduction
//					{
						routeSuccess += 1;			// if this new point is = to the extent of the grid, then she has successfully made it to the end
						validMove = false;			// set validMove to false to stop the while loop
//					}
					// the more threads you have, the better a reduction is
					// confirmed after testing on pace-ice that the reduction is 2x as fast for user time with time command
				}
			}
		}
	}

	PP = double(routeSuccess)/double(N);		// calculate the probability from the # of successes and total # of trials

	fstream outFile("Lab3Prob2.txt", fstream::out);

	outFile << fixed << setprecision(7) << PP << endl;
	outFile.close();
	//cout << routeSuccess << endl;
	//cout << N << endl;

	return 0;
}