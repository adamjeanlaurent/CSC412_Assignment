#pragma once

/**
 * Summary: Wrapper method that read a job file, and processes all tasks within that job file.
 * @param jobPath: Path to job file to process.
 * @param imagesPath: Path to directory with image files.
 * @param outputPath: Path to Output folder to put processed images.
 * @param execPath: Path to executables files for image library.
 * @return void.
 */ 
void ProcessJobFile(char* jobPath, char* imagesPath, char* outputPath, char* execPath);