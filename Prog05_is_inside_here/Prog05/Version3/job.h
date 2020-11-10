#pragma once

#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>

/** Enum for different tasks that a job file contains. */
enum Task
{
    flipH,
    flipV,
    gray,
    crop,
    rotate,
    end
};

/** Struct for information about any one job. */
struct Job 
{
     /** only used for crop task */
    int x;
    /** only used for crop task */
    int y; 
    /** only used for crop task */
    int w; 
    /** only used for crop task */
    int h; 
    /** only used for rotate task */
    std::string rotation; 
    /** Task to perform. */
    Task task; 
    /** filename to peform job on. */
    std::string imageFilename; 
};

/**
 * Summary: Reads a job file line by line, creating a Job object for each line (stops when finding end command anywhere in file), and storing them in a vector.
 * @param jobFilePath: Path to job file to read. 
 * @return Vector of Job objects, where each task in the job file corresponds to a single job object.
 */ 
std::vector<Job> GetJobList(std::string jobFilePath);

/**
 * Summary: Prints an std::vector<Job> to stdout, purely for debugging purposes.
 * @param jobList: vector of Job objects.
 * @return void. 
 */ 
void PrintJobList(std::vector<Job> jobList);

/**
 * Summary: Helper method to return a string value of a task Enum. ex. input: flipH => output: flipH"
 * @param task: Task enum to get string value of.
 * @return sting value of passed enum.
 */ 
std::string TaskEnumToString(Task task);

/**
 * Summary: Deconstructs a Job object into a string, which will be sent as a message to a resident dispatcher.
 * @param job: Job object to deconstruct.
 * @param outputPath: Output path for images.
 * @param dataFolderPath: Path to data folder.
 * @return Job as string.
 */ 
std::string JobToString(Job job, std::string outputPath, std::string dataFolderPath);