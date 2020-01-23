
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ObjectType {
    EMPTY = 0,
    OBSTACLE,
    BOTANIST,
    FLOWER
};

enum EndType {
    NOT_FINISHED = 0,
    FINISHED_FOUND,
    FINISHED_NOT_FOUND,
    
};

/* The structure about board*/
typedef struct _Forest {
    int** Map;
    int Width;
    int Height;
    int Flower_X;
    int Flower_Y;
} *Forest;

/* The structure about botanist */
typedef struct _Botanist {
    int Coord_X;
    int Coord_Y;
    int Water_Bottle_Size;
} *Botanist;

void init_game(Forest forest, Botanist botanist)/* The funtion about initialize of board*/
{
    int i, j;
    
    FILE *fp;
    char line[256];
    
    int bottle_size;
    int height, width;
    
    fp = fopen("init.csv" , "r");
    
    if (fp == NULL) {
        perror("Error opening init.csv file");
        exit(-1);
    }
    
    fscanf(fp, "%d\n", &bottle_size);
    fscanf(fp, "%d,%d\n", &height, &width);
    
    botanist->Water_Bottle_Size = bottle_size;
    
    forest->Map = (int **) calloc(height, sizeof(int *));
    
    for (i = 0; i < height; ++i)
        forest->Map[i] = (int *) calloc(width, sizeof(int));
    
    forest->Width = width;
    forest->Height = height;
    
    i = 0;
    
    while (fgets (line, sizeof(line), fp) != NULL) {
        char *token = strtok(line, ",\r\n");
        
        for (j = 0; j < forest->Width; ++j) {
            switch(token[0]) {
                case '#':
                    forest->Map[i][j] = OBSTACLE;
                    break;
                case 'B':
                    forest->Map[i][j] = BOTANIST;
                    botanist->Coord_X = i;
                    botanist->Coord_Y = j;
                    break;
                case 'F':
                    forest->Map[i][j] = FLOWER;
                    forest->Flower_X = i;
                    forest->Flower_Y = j;
                    break;
                case ' ':
                default:
                    forest->Map[i][j] = EMPTY;
                    break;
            }
            
            token = strtok(NULL, ",\r\n");
        }
        
        ++i;
    }
    
    fclose(fp);
}

void print_map(Forest forest)
{
    int i, j;
    
    for (i = 0; i < forest->Height; ++i) {
        for (j = 0; j < forest->Width; ++j) {
            switch(forest->Map[i][j]) {
                case EMPTY:
                    printf("  ");
                    break;
                case OBSTACLE:
                    printf("# ");
                    break;
                case BOTANIST:
                    printf("B ");
                    break;
                case FLOWER:
                    printf("F ");
                    break;
            }
        }
        
        printf("\n");
    }
    
    printf("\n");
}

/*Recursive function that makes Botanist find flower*/
void search(Forest forest, Botanist botanist, int i, int j, int** visitMap, int *isFinished, int *isVisited)
{
    int d;
    
    int directions[4][2] = {
        {  0,  1 },
        {  1,  0 },
        {  0, -1 },
        { -1,  0 }
    };
    
    *isVisited = 0;
    
    if (i < 0 || i >= forest->Height || j < 0 || j >= forest->Width || visitMap[i][j])
        return;
    
    if (forest->Map[i][j] == OBSTACLE)
        return;
    
    if (*isFinished)
        return;
    
    *isVisited = 1;
    visitMap[i][j] = 1;
    
    botanist->Water_Bottle_Size--;
     
    botanist->Coord_X = i;
    botanist->Coord_Y = j;
    
    if (forest->Map[i][j] == FLOWER) {
        *isFinished = FINISHED_FOUND;
        print_map(forest);
        printf("I have found it on (%d,%d)!\n\n", i, j);
        return;
    }
    
    if (botanist->Water_Bottle_Size <= 0) {
        *isFinished = FINISHED_NOT_FOUND;
        forest->Map[i][j] = BOTANIST;
        print_map(forest);
        printf("Help I am on (%d,%d)\n\n", i, j);
        return;
    }
    
    for (d = 0; d < sizeof(directions) / sizeof(directions[0]); ++d) {
        int visited = 0;
        
        int ni = i + directions[d][0];
        int nj = j + directions[d][1];
        
        search(forest, botanist, ni, nj, visitMap, isFinished, &visited);
        
        if (*isFinished == 0 && visited) {
            botanist->Water_Bottle_Size--;
     
            botanist->Coord_X = i;
            botanist->Coord_Y = j;
            
            if (botanist->Water_Bottle_Size <= 0) {
                *isFinished = FINISHED_NOT_FOUND;
                forest->Map[i][j] = BOTANIST;
                print_map(forest);
                printf("Help I am on (%d,%d)\n\n", i, j);
                return;
            }

        }
    }
}

int main()
{
    int i, j;
    
    int visited;
    int isFinished;
    int **visitMap;
    
    Forest forest = calloc(1, sizeof(*forest));
    Botanist botanist = calloc(1, sizeof(*botanist));
    
    init_game(forest, botanist);
    print_map(forest);
    
    printf("Searching...\n\n");
    
    forest->Map[botanist->Coord_X][botanist->Coord_Y] = EMPTY;
    
    visitMap = (int **) calloc(forest->Height, sizeof(int *));
    
    for (i = 0; i < forest->Height; ++i) {
        visitMap[i] = (int *) calloc(forest->Width, sizeof(int));
        
        for (j = 0; j < forest->Width; ++j) {
            switch(forest->Map[i][j]) {
                case EMPTY:
                    visitMap[i][j] = 0;
                    break;
                case OBSTACLE:
                    visitMap[i][j] = 1;
                    break;
                case BOTANIST:
                    visitMap[i][j] = 0;
                    break;
                case FLOWER:
                    visitMap[i][j] = 0;
                    break;
            }
        }
    }
    
    isFinished = NOT_FINISHED;
    search(forest, botanist, botanist->Coord_X, botanist->Coord_Y, visitMap, &isFinished, &visited);
    
    return 0;
}

