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
		fprintf(fp,"(%d,%d,%4d,%4d)",map.status[i].dead,map.status[i].safe,map.status[i].space,map.status[i].living);
		fprintf(fp,"   (%3d)",map.score);
		fprintf(fp,"   (%d,%4d)\n",map.status[i].canEatFruit,map.status[i].distToFruit);
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

void saveMap(PLAYER_STRUCT* ps, PLAYER_INFO* pi)
{
	FILE *fp = fopen("save.txt","a");
	fprintf(fp,"\n%d\n",pi->CurTime);
	fprintf(fp,"%d %d %d %d\n",pi->Fruit.ExistTime,pi->Fruit.Pos.Row,pi->Fruit.Pos.Col,pi->Fruit.Value );
	for(int i=0;i<ps->ii->SnakeCount;i++){
		int length = pi->SnakeArr[i]->Length;
		fprintf(fp,"%4d%4d\n",length,pi->SnakeArr[i]->Direction);
		for( int j=0;j<length;j++)
		{
			fprintf(fp,"%3d%3d",pi->SnakeArr[i]->Pos[j].Row,pi->SnakeArr[i]->Pos[j].Col);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void loadMap(PLAYER_STRUCT* ps, MAP_INFO *info)
{
	FILE *fp = fopen("step.txt","r");
	if( !fp ) return ;
	fscanf(fp,"%d",&info->CurTime);
	fscanf(fp,"%d%d%d%d",&info->Fruit.ExistTime,&info->Fruit.Pos.Row,&info->Fruit.Pos.Col,&info->Fruit.Value);
	for(int i=0;i<ps->ii->SnakeCount;i++){
		int length ;
		fscanf(fp,"%d",&length);
		fscanf(fp,"%d",&info->SnakeArr[i].Direction);
		info->SnakeArr[i].Length = length;
		info->SnakeArr[i].HeadPos = length-1;
		for( int j=length-1;j>=0;j--)
		{
			fscanf(fp,"%d%d",&info->SnakeArr[i].Pos[j].Row,&info->SnakeArr[i].Pos[j].Col);
		}
	}
	fclose(fp);	
}

