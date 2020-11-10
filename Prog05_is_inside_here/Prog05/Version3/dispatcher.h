#pragma once

#include "pipe.h"

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
 * Summarry: Lauches all resident dispatchers with new processes. 
 * @param pipes: All pipes used to send messages to resident dispathcers.
 * @param pathToExecs: Path to executables files for image library.
 * @return Vector of process ids of resident dispatchers.
 */
std::vector<pid_t> LaunchResidentDispatchers(PipeManager* pipes, std::string pathToExecs);