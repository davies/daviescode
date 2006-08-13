#ifndef LOG_H
#define LOG_H

#include "playerstruct.h"
#include "analyze.h"
#include "search.h"

//#define LOG

void writeMap(int **pMap,PLAYER_STRUCT *ps);
void Log(MapStatus &map,PLAYER_STRUCT *ps);
void Log(char* string,PLAYER_STRUCT *ps);
void Log(int num,PLAYER_STRUCT *ps);

#endif