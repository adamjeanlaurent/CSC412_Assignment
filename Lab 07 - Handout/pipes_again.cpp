//C++ includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>

//Linux includes
#include <fcntl.h>              // Obtain O_* constant definitions
#include <unistd.h>

struct Coord
{
	int x;
	int y;
};

void printCoord(const Coord* c)
{
	printf("(%d, %d)", c->x, c->y);
}

int main(int argc, char** argv)
{
	/*
	0 is always the reading end
	1 is always the writing end
	*/
	const int flags = 0; //flags of 0 creates a blocking pipe
	int fd[2];
	srand(time(NULL));
	
	//read from the pipe
	int error_code = pipe2(fd, flags);
	if (error_code == 0)
	{
		//create two processes
		pid_t pid = fork();
		if (pid == 0) //child is 0
		{
			//create the data
			std::vector<Coord> coords;
			int count = rand() % 10 + 10;
			for (int i = 0; i < count; i++)
			{
				Coord c;
				c.x = rand() % 200 - 100;
				c.y = rand() % 200 - 100;
				coords.push_back(c);
			}
			
			//print the data
			printf("Coords to write: ");
			for (unsigned int i = 0; i < coords.size(); i++)
			{
				printCoord(&coords[i]);
				printf(" ");
			}
			printf("\n");
			
			//write the data to the pipe
			unsigned int number_of_items = coords.size();
			write(fd[1], &number_of_items, sizeof(unsigned int)); //write size to pipe
			for (unsigned int i = 0; i < number_of_items; i++)
			{
				Coord c = coords[i];
				//write(fd[1], &c.x, sizeof(int));
				//write(fd[1], &c.y, sizeof(int));
				write(fd[1], &c, sizeof(Coord));
			}
		}
		else if (pid > 0) //parent
		{
			unsigned int size = 0;
			std::vector<Coord> coord_list;
			
			//read from the pipe
			read(fd[0], &size, sizeof(unsigned int));
			for (unsigned int i = 0; i < size; i++)
			{
				Coord c;
				read(fd[0], &c, sizeof(Coord));
				coord_list.push_back(c);
			}
			
			//print the data
			printf("Coords read: ");
			for (unsigned int i = 0; i < coord_list.size(); i++)
			{
				printCoord(&coord_list[i]);
				printf(" ");
			}
			printf("\n");
		}
		else if (pid < 0)
			printf("ERROR: Failure forking the process\n");
	}
	else
		printf("ERROR: Failure creating pipe with error_code=%d\n", error_code);
	
	return 0;
}