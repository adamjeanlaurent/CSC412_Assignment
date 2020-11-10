#pragma once

#include "pipe.h"
#include "job.h"

/**
 * Summary: Wrapper method that read a job file, and processes all tasks within that job file.
 * @param jobPath: Path to job file to process.
 * @param imagesPath: Path to directory with image files.
 * @param outputPath: Path to Output folder to put processed images.
 * @param execPath: Path to executables files for image library.
 * @return Returns bool indicating whether an end command was found.
 */ 
bool ProcessJobFile(char* jobPath, char* imagesPath, char* outputPath, char* execPath);

/**
 * Summary: Wrapper method that reads a job file, and sends tasks to appropriate resident dispatcher through pipe.
 * @param jobPath: Path to job file to process.
 * @param imagesPath: Path to directory with image files.
 * @param outputPath: Path to Output folder to put processed images.
 * @param execPath: Path to executables files for image library.
 * @param pipes: All pipes used to send messages to resident dispatchers.
 * @return Returns bool indicating whether an end command was found.
 */
bool ProcessJobFileWithPipes(const char* jobPath, char* imagesPath, char* outputPath, char* execPath, PipeManager* pipes);

/**
 * Summary: Lauches all resident dispatchers with new processes. 
 * @param pipes: All pipes used to send messages to resident dispathcers.
 * @param pathToExecs: Path to executables files for image library.
 * @return Vector of process ids of resident dispatchers.
 */
std::vector<pid_t> LaunchResidentDispatchers(PipeManager* pipes, std::string pathToExecs);

/**
 * Summary: Ran by a resident dispatcher, waits in a loop for messages from pipe and upon a read processes the task.
 * @param pipe: Pipe to read from
 * @param pathToExecs: Path to image library executables
 * @param task: Task for resident dispatcher to perform
 * @return void.
 */ 
void ResidentDispatcherProcessTask(char* pipe, char* pathToExecs, Task task);