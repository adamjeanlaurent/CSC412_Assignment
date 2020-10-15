#ifndef PATTERNMATCHOUTPUT_H
#define PATTERNMATCHOUTPUT_H

typedef struct PatternMatchInfo{
    int numberOfTimesFound;
    int placesFound[10000];
    int size;
}PatternMatchInfo;

#endif
