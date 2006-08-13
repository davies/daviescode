#include <stdio.h>
#include "log.h"
#include "playerStruct.h"

void writeMap(int **pMap,PLAYER_STRUCT *ps)
{
	FILE *fp = fopen("map.txt","a");
	fprintf(fp,"***********************%d**************************\n",ps->info[0]->CurTime);
	fprintf(fp,"  ");
	for(int i=0;i<ps->Width;i++)
		fprintf(fp,"%4d",i);
	fprintf(fp,"\n");
	for(i=0;i<ps->Height;i++)
	{
		fprintf(fp,"%2d",i);
		for(int j=0;j<ps->Width;j++)
			if( pMap[i][j] == WALL )
				fprintf(fp,"¨€¨€¨€¨€",pMap[i][j]);
			else
				fprintf(fp,"%4d",pMap[i][j]);
		fprintf(fp,"%2d\n",i);
	}
	fprintf(fp,"  ");
	for(i=0;i<ps->Width;i++)
		fprintf(fp,"%4d",i);
	fprintf(fp,"\n\n\n\n");
	fclose(fp);
}

void Log(MapStatus &map,PLAYER_STRUCT *ps)
{
	FILE *fp = ps->logFile;
	for(int i=0;i<2;i++)
	{
		fprintf(fp,"ID%d:",i);
		fprintf(fp,"-%d-",map.move[i]);
		fprintf(fp,"(%d,%d,%4d)",map.status[i].dead,map.status[i].safe,map.status[i].living);
		fprintf(fp,"   (%3d)",map.score);
		fprintf(fp,"   (%d,%4d,%4d)\n",map.status[i].canEatFruit,map.status[i].distToFruit,map.status[i].controlEara);
	}
}

void Log(char* string,PLAYER_STRUCT *ps)
{
	FILE *fp = ps->logFile ;
	fprintf(fp,string);
	fprintf(fp,"\n");
}

void Log(int num,PLAYER_STRUCT *ps)
{
	FILE *fp = ps->logFile;
	fprintf(fp,"%d\n",num);
}
