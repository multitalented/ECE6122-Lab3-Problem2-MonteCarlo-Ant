# ECE6122-Lab3-Problem2-MonteCarlo-Ant

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
    
    
    Example execution in command line:
    ~$  g++ .\*.cpp a.exe
    ~$  a.exe -N 10000 -g 8
    ~$ cat Lab3Prob2.txt
   
    
