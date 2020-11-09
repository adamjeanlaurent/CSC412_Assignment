#include "job.h"
#include "utility.h"

std::vector<Job> GetJobList(std::string jobFilePath)
{
    std::cout << "path: " << jobFilePath << std::endl;
    FILE *fp = fopen(jobFilePath.c_str(), "r");

    std::vector<Job> jobList;
    
    if (fp != NULL)
    {
        char command[20];
        char rotation[10];
        char filename[500];
        std::cout << "Fp not null" << std::endl;
        // loop through enture job file
        while (fscanf(fp, "%s", command) == 1)
        {
            std::cout << "command found: " << command << std::endl;
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

            // if end found stop reading
            if(job.task == end)
            {
                break;
            }
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
    if(jobList.size() == 0)
    {
        std::cout << "Job List Is Empty! :(" << std::endl;
    }

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

// what needs to be piped
// full path to tga file
// full path to output
// other are just numbers and rotations and stuff
std::string JobToString(Job job, std::string outputPath, std::string dataFolderPath)
{
    std::string str = "";

    switch(job.task)
    {
        case flipH:
            str += CombinePathWithFile(outputPath, job.imageFilename) ;
            str += " " + outputPath;
            break;
        case flipV:
            str += CombinePathWithFile(outputPath, job.imageFilename);
            str += " " + outputPath;
            break;
         case gray:
            str += CombinePathWithFile(outputPath, job.imageFilename);
            str += " " + outputPath;
            break;
         case crop:
            str += CombinePathWithFile(outputPath, job.imageFilename);
            str += " " + outputPath;
            str += " " + std::to_string(job.x);
            str += " " + std::to_string(job.y);
            str += " " + std::to_string(job.w);
            str += " " + std::to_string(job.h);
            break;
         case rotate:
            str += CombinePathWithFile(outputPath, job.imageFilename);
            str += " " + job.rotation;
            str += " " + outputPath;
            break;
         case end:
            str += "end";
            break;
    }
}