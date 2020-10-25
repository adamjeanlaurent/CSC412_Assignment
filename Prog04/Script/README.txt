Hello!

Included here are 3 shell scripts.

build.sh: 
- builds executables called v1 and v2, and store it in Source/Version1, and Source/Version2
- You actually don't need to run this before using script01 and script02 , as they call this script themselves.

script01.sh
- finds largest index and runs version1.
- usage: ./script01.sh <path_to_data_files> <output_file_path>
- Relative and full paths as parameters work just fine, as well as with and without slashes.
- Example Usage: { ./script01.sh /Users/ajean-laurent/Desktop/CSC_412_Work/Prog04/Data-Sets/Data-Set-1/ /Users/ajean-laurent/Desktop/CSC_412_Work/Prog04/Source/Version1/outputfile.c } 
- Example Usage: { ./script01.sh ../Data-Sets/Large-Data-Sets/DataSet-9--12-procs/ ../Source/Version1/output.c }

script02.sh
- finds largest index and runs version2.
- usage: ./script02.sh <path_to_data_files> <output_file_path>
- Relative and full paths as parameters work just fine, as well as with and without slashes.
- Example Usage: { ./script02.sh /Users/ajean-laurent/Desktop/CSC_412_Work/Prog04/Data-Sets/Data-Set-1/ /Users/ajean-laurent/Desktop/CSC_412_Work/Prog04/Source/Version2/outputfile.c } 
- Example Usage: { ./script02.sh ../Data-Sets/Large-Data-Sets/DataSet-9--12-procs/ ../Source/Version2/output.c }