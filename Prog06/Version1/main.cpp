//
//  main.c
//  Cellular Automaton
//
//  Created by Jean-Yves Hervé on 2018-04-09.
//	C++ version 2020-11-06

 /*-------------------------------------------------------------------------+
 |	A graphic front end for a grid+state simulation.						|
 |																			|
 |	This application simply creates a glut window with a pane to display	|
 |	a colored grid and the other to display some state information.			|
 |	Sets up callback functions to handle menu, mouse and keyboard events.	|
 |	Normally, you shouldn't have to touch anything in this code, unless		|
 |	you want to change some of the things displayed, add menus, etc.		|
 |	Only mess with this after everything else works and making a backup		|
 |	copy of your project.  OpenGL & glut are tricky and it's really easy	|
 |	to break everything with a single line of code.							|
 |																			|
 |	Current keyboard controls:												|
 |																			|
 |		- 'ESC' --> exit the application									|
 |		- space bar --> resets the grid										|
 |																			|
 |		- 'c' --> toggle color mode on/off									|
 |		- 'b' --> toggles color mode off/on									|
 |		- 'l' --> toggles on/off grid line rendering						|
 |																			|
 |		- '+' --> increase simulation speed									|
 |		- '-' --> reduce simulation speed									|
 |																			|
 |		- '1' --> apply Rule 1 (Conway's classical Game of Life: B3/S23)	|
 |		- '2' --> apply Rule 2 (Coral: B3/S45678)							|
 |		- '3' --> apply Rule 3 (Amoeba: B357/S1358)							|
 |		- '4' --> apply Rule 4 (Maze: B3/S12345)							|
 |																			|
 +-------------------------------------------------------------------------*/

#include <vector>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <cstring>
//
#include "gl_frontEnd.h"
#include "validation.h"

using namespace std;

#if 0
//==================================================================================
#pragma mark -
#pragma mark Custom data types
//==================================================================================
#endif

// captures the location of a cell in a 2D Array
typedef struct Cell 
{
	int i;
	int j;
} Cell;

typedef struct ThreadInfo
{
	//	you probably want these
	pthread_t threadID;
	int threadIndex;
	//
	//	whatever other input or output data may be needed
	//
	// vector of cell locations that a thread is responsible to update
	std::vector<Cell> cellsToUpdate;
} ThreadInfo;

#if 0
//==================================================================================
#pragma mark -
#pragma mark Function prototypes
//==================================================================================
#endif

void displayGridPane(void);
void displayStatePane(void);
void initializeApplication();
void cleanupAndquit(void);
void* threadFunc(void*);
void swapGrids(void);
unsigned int cellNewState(unsigned int i, unsigned int j);

/**
*	Summary: Computes and returns the cell locations all threads are responsible for. Called once in main.
*	@return: Returns a 2d vector of cells where vec[n] == std::vector<Cells> that Thread[n] is responsible to update.
*/
std::vector<std::vector<Cell>> getCellLocationsToUpdate();

/**
 * Summary: Calculates how many Cells each through should be responsible for based on the number of cells in the grid and the number of threads.
 * @param numOfCells: Number of cells in the grid.
 * @param threadCount: Number of computation threads that will be working on the grid.
 * @return: vector<int> where vector[i] = # of cells that computation thread[i] will update
 */ 
std::vector<int> getIndexRangesForThreads(int numOfCells, int threadCount);

/**
 * Summary: prints cell locations to update for debugging purposes.
 * @param v: Cell locations to update, stored in horizontalBands global variable.
 * @return: void.
 */ 
void printSplitWork(std::vector<std::vector<Cell>> v);

/**
 * Summary: Launches the master computation thread.
 * @return: void.
 */ 
void createMasterComputationThread();

/**
 * Summary: Thread function run by the master computation thread, launches a generation, and sleeps in a loop until app is exited.
 * @param args: Void pointer to argument.
 * @return: void*, returns NULL.
 */ 
void* masterComputationThreadFunc(void* args);

/**
 * Summary: Reads a message from named pipe.
 * @return string value of message received.
 */ 
std::string ReadFromPipe();

/**
 * Summary: Writes a message to a named pipe.
 * @return void.
 */ 
void WriteToPipe();

/**
 * Summary: Thread function for the pipe communication thread, reads from pipe in a loop and performs actions.
 * @param args: Void pointer to argument.
 * @return: void*, returns NULL.
 */ 
void* pipeCommunicationThreadFunc(void *args);

/**
 * Summary: Launches pipe communication thread.
 * @return void.
 */ 
void createPipeCommunicationThread();


//==================================================================================
//	Precompiler #define to let us specify how things should be handled at the
//	border of the frame
//==================================================================================

#if 0
//==================================================================================
#pragma mark -
#pragma mark Simulation mode:  behavior at edges of frame
//==================================================================================
#endif

#define FRAME_DEAD		0	//	cell borders are kept dead
#define FRAME_RANDOM	1	//	new random values are generated at each generation
#define FRAME_CLIPPED	2	//	same rule as elsewhere, with clipping to stay within bounds
#define FRAME_WRAP		3	//	same rule as elsewhere, with wrapping around at edges

//	Pick one value for FRAME_BEHAVIOR
#define FRAME_BEHAVIOR	FRAME_DEAD

#if 0
//==================================================================================
#pragma mark -
#pragma mark Application-level global variables
//==================================================================================
#endif

//	Don't touch
extern int gMainWindow, gSubwindow[2];

//	The state grid and its dimensions.  We use two copies of the grid:
//		- currentGrid is the one displayed in the graphic front end
//		- nextGrid is the grid that stores the next generation of cell
//			states, as computed by our threads.
unsigned int* currentGrid;
unsigned int* nextGrid;
unsigned int** currentGrid2D;
unsigned int** nextGrid2D;

//	Piece of advice, whenever you do a grid-based (e.g. image processing),
//	implementastion, you should always try to run your code with a
//	non-square grid to spot accidental row-col inversion bugs.
//	When this is possible, of course (e.g. makes no sense for a chess program).
// const unsigned int numRows = 400, numCols = 420;

unsigned int numRows; // number of rows in grid
unsigned int numCols; // number of columns in grid
unsigned int maxNumThreads; // number of threads working on the grid
std::vector<std::vector<Cell>> horizontalBands; // holds cells that each thread is responsible for
pthread_t masterComputationThread; // master computation thread
bool quit = false; // stops computation threads computations when set to true
int microSecondsBetweenGenerations = 100000; // init value 100,000 micro seconds
pthread_mutex_t userControlsLock; // locks global variable set by keyboard controls and pipe to avoid race condition
bool stopFromBash = false; // stops pipe communication thread when set

//	the number of live computation threads (that haven't terminated yet)
unsigned short numLiveThreads = 0;

unsigned int rule = GAME_OF_LIFE_RULE;

unsigned int colorMode = 0;

unsigned int generation = 0;


//------------------------------
//	Threads and synchronization
//	Reminder of all declarations and function calls
//------------------------------
//pthread_mutex_t myLock;
//pthread_mutex_init(&myLock, NULL);
//int err = pthread_create(pthread_t*, NULL, threadFunc, ThreadInfo*);
//int pthread_join(pthread_t , void**);
//pthread_mutex_lock(&myLock);
//pthread_mutex_unlock(&myLock);

#if 0
//==================================================================================
#pragma mark -
#pragma mark Computation functions
//==================================================================================
#endif


//------------------------------------------------------------------------
//	You shouldn't have to change anything in the main function
//------------------------------------------------------------------------
int main(int argc, const char* argv[])
{
	// validate arguments
	if(!validateArguments(argv, argc))
	{
		return 1;
	}

	// init rows and columns and horizontal bands
	numRows = atoi(argv[1]);
	numCols = atoi(argv[2]);
	maxNumThreads = atoi(argv[3]);
	horizontalBands = getCellLocationsToUpdate();
	
	// init user controls lock
	pthread_mutex_init(&userControlsLock, NULL);

	//	This takes care of initializing glut and the GUI.
	//	You shouldn’t have to touch this
	initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);
	
	//	Now we can do application-level initialization
	initializeApplication();

	// run pipe communication thread
	createPipeCommunicationThread();

	// run master computation thread
	createMasterComputationThread();

	//	Now would be the place & time to create mutex locks and threads

	//	Now we enter the main loop of the program and to a large extend
	//	"lose control" over its execution.  The callback functions that 
	//	we set up earlier will be called when the corresponding event
	//	occurs
	glutMainLoop();
	
	//	In fact this code is never reached because we only leave the glut main
	//	loop through an exit call.
	//	Free allocated resource before leaving (not absolutely needed, but
	//	just nicer.  Also, if you crash there, you know something is wrong
	//	in your code.
	// free(currentGrid2D);
	// free(currentGrid);
		
	//	This will never be executed (the exit point will be in one of the
	//	call back functions).
	return 0;
}


//==================================================================================
//
//	This is a part that you have to edit and add to.
//
//==================================================================================

void initializeApplication(void)
{
    //--------------------
    //  Allocate 1D grids
    //--------------------
    currentGrid = new unsigned int[numRows*numCols];
    nextGrid = new unsigned int[numRows*numCols];

    //---------------------------------------------
    //  Scaffold 2D arrays on top of the 1D arrays
    //---------------------------------------------
    currentGrid2D = new unsigned int*[numRows];
    nextGrid2D = new unsigned int*[numRows];
    currentGrid2D[0] = currentGrid;
    nextGrid2D[0] = nextGrid;
    for (unsigned int i=1; i<numRows; i++)
    {
        currentGrid2D[i] = currentGrid2D[i-1] + numCols;
        nextGrid2D[i] = nextGrid2D[i-1] + numCols;
    }
	
	//---------------------------------------------------------------
	//	All the code below to be replaced/removed
	//	I initialize the grid's pixels to have something to look at
	//---------------------------------------------------------------
	//	Yes, I am using the C random generator after ranting in class that the C random
	//	generator was junk.  Here I am not using it to produce "serious" data (as in a
	//	simulation), only some color, in meant-to-be-thrown-away code
	
	//	seed the pseudo-random generator
	srand((unsigned int) time(NULL));
	
	resetGrid();
}

//---------------------------------------------------------------------
//	You will need to implement/modify the two functions below
//---------------------------------------------------------------------

void WriteToPipe()
{
	std::string pipe = "/tmp/c_to_bash";

	// prepare message
	std::string message = "done";
	const char* message_cstring = message.c_str();
	char buffer[500];
	sprintf(buffer, "%s\n", message_cstring);
	buffer[strlen(buffer)] = '\0';

	// open pipe
	int fd = open(pipe.c_str(), O_WRONLY);
	write(fd, buffer, strlen(buffer) + 1);

	// close pipe
	close(fd);
}

std::string ReadFromPipe()
{
    std::string pipe = "/tmp/bash_to_c";

    char buffer[500];

	//open pipe
    int fd = open(pipe.c_str(), O_RDONLY);

	// read message from pipe
    read(fd, buffer, 500);

	// remove \n and add NULL character
    buffer[strcspn(buffer, "\n")] = 0;
    buffer[strlen(buffer)] = '\0';

	// close pipe
    close(fd);

    return std::string(buffer);
}

void* pipeCommunicationThreadFunc(void *args)
{
	bool continueReadingFromPipe = true;
	while(continueReadingFromPipe)
	{
		// read message from pipe
		std::string message = ReadFromPipe();

		// ignore empty message 
		if(message.empty()) 
		{
			continue;
		} 

		// lock updating of global variables to avoid race condition with keyboard controls
		pthread_mutex_lock(&userControlsLock);

		// decode message and update appropriate variable

		if(message == "rule 1")
		{
			rule = GAME_OF_LIFE_RULE;
		}
		else if(message == "rule 2")
		{
			rule = CORAL_GROWTH_RULE;
		}
		else if(message == "rule 3")
		{
			rule = AMOEBA_RULE;
		}
		else if(message == "rule 4")
		{
			rule = MAZE_RULE;
		}
		else if(message == "color on")
		{
			colorMode = !colorMode;
		}
		else if(message == "color off")
		{
			colorMode = !colorMode;
		}
		else if(message == "slowdown")
		{
			microSecondsBetweenGenerations += 100000 /* 100,000 */;
		}
		else if(message == "speedup")
		{
			if(microSecondsBetweenGenerations == 100000)
				break; // cap at 0.1 second for slowest speed
			else
				microSecondsBetweenGenerations -= 100000 /* 100,000 */;
		}
		else if(message == "end")
		{
			stopFromBash = true;
			continueReadingFromPipe = false;
		}

		// unclock user controls lock
		pthread_mutex_unlock(&userControlsLock);

		// tell bash we are done with this command and that it can accept another
		WriteToPipe();
	}
	return NULL;
}

void createPipeCommunicationThread()
{
	pthread_t pipeCommunicationThread;
	pthread_create(&pipeCommunicationThread, NULL, pipeCommunicationThreadFunc, NULL);
}

void createMasterComputationThread()
{
	pthread_create(&masterComputationThread, NULL, masterComputationThreadFunc, NULL);
}

void* masterComputationThreadFunc(void* args)
{
	while(!quit)
	{
		// launch new generation of computation threads
		oneGeneration();
		// sleep for a bit
		usleep(microSecondsBetweenGenerations);
	}
	return NULL;
}

void printSplitWork(std::vector<std::vector<Cell>> v)
{
	for(unsigned int i = 0; i < maxNumThreads; i++)
	{
		std::cout << "Thread[" << i << "] Will Work On " << v[i].size() << " cells: " << std::endl;

		for(unsigned int j = 0; j < v[i].size(); j++)
		{
			std::cout << "[" << v[i][j].i << "," << v[i][j].j << "] ";
 		}
 		std::cout << std::endl << std::endl;
	}
}

std::vector<int> getIndexRangesForThreads(int numOfCells, int threadCount) 
{
    std::vector<int> processIndexes;

    int i;
    
    // if number of cells divides evenly into the number of threads
    // then each thread will process the same number of cells
    if(numOfCells % threadCount == 0)
    {
        for(i = 0; i < threadCount; i++)
        {
            processIndexes.push_back(numOfCells/threadCount);
        }
    }

    // split the cells into as evenly as possible
    else
    {
		// the first 0 ... upTo threads will be give the value of quotient
		// the next upTo + 1 ..... will be given quotient + 1 to keep difference between number of cells to lowest as possible
        int upTo = threadCount - (numOfCells % threadCount);
        int quotient = numOfCells/ threadCount;
        for(i = 0; i < threadCount; i++)
        {
            if(i >= upTo)
            {
                processIndexes.push_back(quotient + 1);
            }
            else
            {
                processIndexes.push_back(quotient);
            }
        
        }
    }   
    return processIndexes;
}

std::vector<std::vector<Cell>> getCellLocationsToUpdate()
{
	std::vector<std::vector<Cell>> outerVector;

	// create thread vectors of Cell
	for(unsigned int k = 0; k < maxNumThreads; k++)
	{
		std::vector<Cell> vec;
		outerVector.push_back(vec);
	}

	// get number of cells each threads should work on
	std::vector<int> splitWork = getIndexRangesForThreads((numRows*numCols) /* num of cells */, maxNumThreads);
	int currentThread = 0;
	
	// assign each thread the number of threads it should work on
	for(unsigned int i = 0; i < numRows; i++)
	{
		for(unsigned int j = 0; j < numCols; j++)
		{
			// check if currentThread has all it's cells assigned
			// if no start assigning to the next thread
			if(splitWork[currentThread] == 0)
			{
				currentThread++;
			}

			// add cell to threads vector
			Cell cell;
			cell.i = i;
			cell.j = j;
			outerVector[currentThread].push_back(cell);

			// minus 1 from cells left to assign to thread
			splitWork[currentThread]--;
		}
	}
	return outerVector;
}

void* threadFunc(void* arg)
{
	ThreadInfo info = *(ThreadInfo*)arg;

	// loop through cells this computation thread is responsible for
	for(Cell cell : info.cellsToUpdate)
	{
		int i = cell.i;
		int j = cell.j;

		// update state of cell
		unsigned int newState = cellNewState(i, j);
	
		//	In black and white mode, only alive/dead matters
		//	Dead is dead in any mode
		if (colorMode == 0 || newState == 0)
		{
			nextGrid2D[i][j] = newState;
		}
		//	in color mode, color reflext the "age" of a live cell
		else
		{
			//	Any cell that has not yet reached the "very old cell"
			//	stage simply got one generation older
			if (currentGrid2D[i][j] < NB_COLORS-1)
				nextGrid2D[i][j] = currentGrid2D[i][j] + 1;
			//	An old cell remains old until it dies
			else
				nextGrid2D[i][j] = currentGrid2D[i][j];
		}	
	}	
	
	return NULL;
}

void oneGeneration(void)
{
	// create update threads and thread infos
	pthread_t* updateThreads = new pthread_t[maxNumThreads]();
	ThreadInfo* updateThreadInfos = new ThreadInfo[maxNumThreads]();

	// init thread infos
	for(unsigned int k = 0; k < maxNumThreads; k++)
	{		
		updateThreadInfos[k].threadIndex = k;
		updateThreadInfos[k].threadID = updateThreads[k];
		updateThreadInfos[k].cellsToUpdate = horizontalBands[k];
	}
	
	// launch threads
	for(unsigned int i = 0; i < maxNumThreads; i++)
	{
		pthread_create(&updateThreads[i], NULL, threadFunc, &updateThreadInfos[i]);
	}

	// wait for threads to end
	for(unsigned int j = 0; j < maxNumThreads; j++)
	{
		pthread_join(updateThreads[j], NULL);
	}

	delete[] updateThreads;
	delete[] updateThreadInfos;

	generation++;
	
	swapGrids();
}

//	This is the function that determines how a cell update its state
//	based on that of its neighbors.
//	1. As repeated in the comments below, this is a "didactic" implementation,
//	rather than one optimized for speed.  In particular, I refer explicitly
//	to the S/B elements of the "rule" in place.
//	2. This implentation allows for different behaviors along the edges of the
//	grid (no change, keep border dead, clipping, wrap around). All these
//	variants are used for simulations in research applications.
unsigned int cellNewState(unsigned int i, unsigned int j)
{
	//	First count the number of neighbors that are alive
	//----------------------------------------------------
	//	Again, this implementation makes no pretense at being the most efficient.
	//	I am just trying to keep things modular and somewhat readable
	int count = 0;

	//	Away from the border, we simply count how many among the cell's
	//	eight neighbors are alive (cell state > 0)
	if (i>0 && i<numRows-1 && j>0 && j<numCols-1)
	{
		//	remember that in C, (x == val) is either 1 or 0
		count = (currentGrid2D[i-1][j-1] != 0) +
				(currentGrid2D[i-1][j] != 0) +
				(currentGrid2D[i-1][j+1] != 0)  +
				(currentGrid2D[i][j-1] != 0)  +
				(currentGrid2D[i][j+1] != 0)  +
				(currentGrid2D[i+1][j-1] != 0)  +
				(currentGrid2D[i+1][j] != 0)  +
				(currentGrid2D[i+1][j+1] != 0);
	}
	//	on the border of the frame...
	else
	{
		#if FRAME_BEHAVIOR == FRAME_DEAD
		
			//	Hack to force death of a cell
			count = -1;
		
		#elif FRAME_BEHAVIOR == FRAME_RANDOM
		
			count = rand() % 9;
		
		#elif FRAME_BEHAVIOR == FRAME_CLIPPED
	
			if (i>0)
			{
				if (j>0 && currentGrid2D[i-1][j-1] != 0)
					count++;
				if (currentGrid2D[i-1][j] != 0)
					count++;
				if (j<numCols-1 && currentGrid2D[i-1][j+1] != 0)
					count++;
			}

			if (j>0 && currentGrid2D[i][j-1] != 0)
				count++;
			if (j<numCols-1 && currentGrid2D[i][j+1] != 0)
				count++;

			if (i<numRows-1)
			{
				if (j>0 && currentGrid2D[i+1][j-1] != 0)
					count++;
				if (currentGrid2D[i+1][j] != 0)
					count++;
				if (j<numCols-1 && currentGrid2D[i+1][j+1] != 0)
					count++;
			}
			
	
		#elif FRAME_BEHAVIOR == FRAME_WRAPPED
	
			unsigned int 	iM1 = (i+numRows-1)%numRows,
							iP1 = (i+1)%numRows,
							jM1 = (j+numCols-1)%numCols,
							jP1 = (j+1)%numCols;
			count = currentGrid2D[iM1][jM1] != 0 +
					currentGrid2D[iM1][j] != 0 +
					currentGrid2D[iM1][jP1] != 0  +
					currentGrid2D[i][jM1] != 0  +
					currentGrid2D[i][jP1] != 0  +
					currentGrid2D[iP1][jM1] != 0  +
					currentGrid2D[iP1][j] != 0  +
					currentGrid2D[iP1][jP1] != 0 ;

		#else
			#error undefined frame behavior
		#endif
		
	}	//	end of else case (on border)
	
	//	Next apply the cellular automaton rule
	//----------------------------------------------------
	//	by default, the grid square is going to be empty/dead
	unsigned int newState = 0;
	
	//	unless....
	
	switch (rule)
	{
		//	Rule 1 (Conway's classical Game of Life: B3/S23)
		case GAME_OF_LIFE_RULE:

			//	if the cell is currently occupied by a live cell, look at "Stay alive rule"
			if (currentGrid2D[i][j] != 0)
			{
				if (count == 3 || count == 2)
					newState = 1;
			}
			//	if the grid square is currently empty, look at the "Birth of a new cell" rule
			else
			{
				if (count == 3)
					newState = 1;
			}
			break;
	
		//	Rule 2 (Coral Growth: B3/S45678)
		case CORAL_GROWTH_RULE:

			//	if the cell is currently occupied by a live cell, look at "Stay alive rule"
			if (currentGrid2D[i][j] != 0)
			{
				if (count > 3)
					newState = 1;
			}
			//	if the grid square is currently empty, look at the "Birth of a new cell" rule
			else
			{
				if (count == 3)
					newState = 1;
			}
			break;
			
		//	Rule 3 (Amoeba: B357/S1358)
		case AMOEBA_RULE:

			//	if the cell is currently occupied by a live cell, look at "Stay alive rule"
			if (currentGrid2D[i][j] != 0)
			{
				if (count == 1 || count == 3 || count == 5 || count == 8)
					newState = 1;
			}
			//	if the grid square is currently empty, look at the "Birth of a new cell" rule
			else
			{
				if (count == 3 || count == 5 || count == 7)
					newState = 1;
			}
			break;
		
		//	Rule 4 (Maze: B3/S12345)							|
		case MAZE_RULE:

			//	if the cell is currently occupied by a live cell, look at "Stay alive rule"
			if (currentGrid2D[i][j] != 0)
			{
				if (count >= 1 && count <= 5)
					newState = 1;
			}
			//	if the grid square is currently empty, look at the "Birth of a new cell" rule
			else
			{
				if (count == 3)
					newState = 1;
			}
			break;

			break;
		
		default:
			cout << "Invalid rule number" << endl;
			exit(5);
	}

	return newState;
}

void cleanupAndquit(void)
{
	quit = true;
	//	join the master computation thread
	pthread_join(masterComputationThread, NULL);
	
	//	free the grids
	delete[] currentGrid2D;
	delete[] nextGrid2D;

	exit(0);
}



#if 0
#pragma mark -
#pragma mark GUI functions
#endif

//==================================================================================
//	GUI functions
//	These are the functions that tie the simulation with the rendering.
//	Some parts are "don't touch."  Other parts need your intervention
//	to make sure that access to critical section is properly synchronized
//==================================================================================


void displayGridPane(void)
{
	//	This is OpenGL/glut magic.  Don't touch
	glutSetWindow(gSubwindow[GRID_PANE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------------------------------
	//	This is the call that makes OpenGL render the grid.
	//
	//---------------------------------------------------------
	drawGrid(currentGrid2D, numRows, numCols);
	
	//	This is OpenGL/glut magic.  Don't touch
	glutSwapBuffers();
	
	glutSetWindow(gMainWindow);
}

void displayStatePane(void)
{
	//	This is OpenGL/glut magic.  Don't touch
	glutSetWindow(gSubwindow[STATE_PANE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------------------------------
	//	This is the call that makes OpenGL render information
	//	about the state of the simulation.
	//	You may want to add stuff to display
	//---------------------------------------------------------
	drawState(numLiveThreads);
	
	//	This is OpenGL/glut magic.  Don't touch
	glutSwapBuffers();
	glutSetWindow(gMainWindow);
}


//	This callback function is called when a keyboard event occurs
//
void myKeyboardFunc(unsigned char c, int x, int y)
{
	(void) x; (void) y;
	
	pthread_mutex_lock(&userControlsLock);
	switch (c)
	{
		//	'ESC' --> exit the application
		case 27:
			cleanupAndquit();
			break;

		//	spacebar --> resets the grid
		case ' ':
			resetGrid();
			break;

		//	'+' --> reduce simulation speed
		case '-':
			microSecondsBetweenGenerations += 100000 /* 100,000 */;
			break;

		//	'-' --> increase simulation speed
		case '+':
			if(microSecondsBetweenGenerations == 100000)
				break; // cap at 0.1 second for slowest speed
			else
				microSecondsBetweenGenerations -= 100000 /* 100,000 */;
			break;

		//	'1' --> apply Rule 1 (Game of Life: B23/S3)
		case '1':
			rule = GAME_OF_LIFE_RULE;
			break;

		//	'2' --> apply Rule 2 (Coral: B3_S45678)
		case '2':
			rule = CORAL_GROWTH_RULE;
			break;

		//	'3' --> apply Rule 3 (Amoeba: B357/S1358)
		case '3':
			rule = AMOEBA_RULE;
			break;

		//	'4' --> apply Rule 4 (Maze: B3/S12345)
		case '4':
			rule = MAZE_RULE;
			break;

		//	'c' --> toggles on/off color mode
		//	'b' --> toggles off/on color mode
		case 'c':
		case 'b':
			colorMode = !colorMode;
			break;

		//	'l' --> toggles on/off grid line rendering
		case 'l':
			toggleDrawGridLines();
			break;

		default:
			break;
	}
	pthread_mutex_unlock(&userControlsLock);
	
	glutSetWindow(gMainWindow);
	glutPostRedisplay();
}

void myTimerFunc(int value)
{
	//	value not used.  Warning suppression
	(void) value;
	
	// checks if stopFromBash
	if(stopFromBash)
	{
		cleanupAndquit();
	}

    //  possibly I do something to update the state information displayed
    //	in the "state" pane
	
	
	//	This is not the way it should be done, but it seems that Apple is
	//	not happy with having marked glut as deprecated.  They are doing
	//	things to make it break
    //glutPostRedisplay();
    myDisplayFunc();
    
	//	And finally I perform the rendering
	glutTimerFunc(100, myTimerFunc, 0);
}

//---------------------------------------------------------------------
//	You shouldn't need to touch the functions below
//---------------------------------------------------------------------

void resetGrid(void)
{
	for (unsigned int i=0; i<numRows; i++)
	{
		for (unsigned int j=0; j<numCols; j++)
		{
			nextGrid2D[i][j] = rand() % 2;
		}
	}
	swapGrids();
}

//	This function swaps the current and next grids, as well as their
//	companion 2D grid.  Note that we only swap the "top" layer of
//	the 2D grids.
void swapGrids(void)
{
	//	swap grids
	unsigned int* tempGrid;
	unsigned int** tempGrid2D;
	
	tempGrid = currentGrid;
	currentGrid = nextGrid;
	nextGrid = tempGrid;
	//
	tempGrid2D = currentGrid2D;
	currentGrid2D = nextGrid2D;
	nextGrid2D = tempGrid2D;
}

