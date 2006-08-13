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
void saveMap(PLAYER_STRUCT* ps, PLAYER_INFO* pi);
void loadMap(PLAYER_STRUCT* ps, MAP_INFO *info);


#endif