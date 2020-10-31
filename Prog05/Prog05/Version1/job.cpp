#include "job.h"

std::vector<Job> GetJobList(std::string jobFilePath)
{
    FILE *fp = fopen(jobFilePath.c_str(), "r");

    std::vector<Job> jobList;
    
    if (fp != NULL)
    {
        char command[20];
        char rotation[10];
        char filename[500];
    
        // loop through enture job file
        while (fscanf(fp, "%s", command) == 1)
        {
            Job job;

            // flipH command
            if (strcmp(command, "flipH") == 0)
            {
                job.task = flipH;
                fscanf(fp, "%s ", filename); // get file name
            }
            // flipV command
            else if (strcmp(command, "flipV") == 0)
            {
                job.task = flipV;
                fscanf(fp, "%s ", filename); // get file name
            }
            // gray command
            else if (strcmp(command, "gray") == 0)
            {
                job.task = gray;
                fscanf(fp, "%s ", filename); // get file name
            }
            // crop command
            else if (strcmp(command, "crop") == 0)
            {
                job.task = crop;

                fscanf(fp, "%s %d %d %d %d ", filename, &job.x, &job.y, &job.w, &job.h); // get x y w h and file name
            }
            // rotate command
            else if (strcmp(command, "rotate") == 0)
            {
                job.task = rotate;

                fscanf(fp, "%s %s ", rotation, filename); // get rotation and image file name
                
                job.rotation = std::string(rotation);
            }
            // end command
            else if (strcmp(command, "end") == 0)
            {
                job.task = end;
            }

            job.imageFilename = std::string(filename); // convert char* file name to std::string

            jobList.push_back(job);
        }
        fclose(fp);
    }

    else
    {
        std::cout << "could not open file!" << std::endl;
    }
    return jobList;
}

std::string TaskEnumToString(Task task)
{
    const char* TaskEnumStrings[] = { "flipH", "flipV", "gray", "crop", "rotate", "end" };
    return std::string(TaskEnumStrings[task]);
}

void PrintJobList(std::vector<Job> jobList)
{
    for (Job job : jobList)
    {
        switch (job.task)
        {
        case flipH:
            std::cout << "Command: flipH "
                      << "fileName: " << job.imageFilename << std::endl;
            break;
        case flipV:
            std::cout << "Command: flipV "
                      << "fileName: " << job.imageFilename << std::endl;
            break;
        case gray:
            std::cout << "Command: gray "
                      << "fileName: " << job.imageFilename << std::endl;
            break;
        case crop:
            std::cout << "Command: crop "
                      << "fileName: " << job.imageFilename << " x: " << job.x << " y: " << job.y << " w: " << job.w << " h: " << job.h << std::endl;
            break;
        case rotate:
            std::cout << "Command: rotate "
                      << "fileName: " << job.imageFilename << " rotation: " << job.rotation << std::endl;
            break;
        case end:
                std::cout << "Command: End" << std::endl;
            break;
        default:
            std::cout << "Command: Default" << std::endl;
            break;
        }
    }
}