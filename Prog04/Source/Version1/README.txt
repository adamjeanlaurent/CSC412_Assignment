Plan:

arguments to main: numberOfProcesses, path to data directory

Steps:

1. Find out how many files there are in the file path -- CHECK
2. Store all files names -- CHECK
3. "Assign" each function call a certain number of files to check
    - This can probably be done with a function call for each "processs", where the function call checks those indexes in array of filename
4. While checking, the function will append the file name appropriate list of filenames
5. call "data processing" function from main n times, this will take in a list of files
6. Data processing function will sort fragments of list, and return as a long string
7. Main function will concat all long strings
8. main function will write long string to file


TODO:

- handle paths ending in / and those that don't (can change this in file path)
- correct number of files handled correctly