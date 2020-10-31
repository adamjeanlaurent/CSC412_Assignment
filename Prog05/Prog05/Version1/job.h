#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>

enum Rotation
{
    l,
    r,
    ll,
    rr
};

enum Task
{
    flipH,
    flipV,
    gray,
    crop,
    rotate,
    end
};

struct Job 
{
    int x; // only used for crop task
    int y; // only used for crop task
    int w; // only used for crop task
    int h; // only used for crop task
    Rotation rotation; // only used for rotate task
    Task task;
    std::string imageFilename;
};

std::vector<Job> GetJobList(std::string jobFilePath);

Rotation RotationFromString(char* rotation);

void PrintJobList(std::vector<Job> jobList);
