#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>

using namespace std;

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
    string imageFilename;
};

vector<Job> GetJobList(string jobFilepath);