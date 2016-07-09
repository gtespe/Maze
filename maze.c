#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MAZE_ROWS (51)
#define MAZE_COLS (111)

typedef struct {
    int row;
    int col;
} Posn;

void solveMaze(int** maze, Posn start, Posn finish);
int contains(Posn* list,int length, Posn aPosn);
int containsAlt(Posn* list, int length, int row, int col);
int** allocMaze(int rows, int cols);
void randPrim(int** maze, int rows, int cols);
void printMaze(int** maze, int rows, int cols);
void removeIndex(Posn* list, int length, int index);
void removePosn(Posn* list, int length, Posn aPosn);
int addNearbyWalls(int** maze, int rows, int cols, Posn* list, int length, Posn aPosn);
void encloseMaze(int** maze, int rows, int cols);

int main(){

   srand(time(NULL));
   int** maze = allocMaze(MAZE_ROWS,MAZE_COLS); 
   
   randPrim(maze,MAZE_ROWS,MAZE_COLS);
   encloseMaze(maze, MAZE_ROWS,MAZE_COLS);

   Posn start; start.row = 1; start.col = 1;
   Posn finish; finish.row = MAZE_ROWS - 2; finish.col = MAZE_COLS - 2;

   solveMaze(maze, start, finish);
 
   printMaze(maze, MAZE_ROWS, MAZE_COLS);

}

//Searchces through the maze for empty cells, clockwise starting up, iteratively,
void solveMaze(int** maze, Posn start, Posn finish){
    if(maze[start.row][start.col] != 0 || maze[finish.row][finish.col] != 0){
        //Invalid starting position
        return;
    }

    //For tracking where you are in the maze
    Posn currentCell; currentCell.row = start.row; currentCell.col = start.col;

    int isTrapped = 0;
    
    //change this
    Posn* addedWalls = malloc(sizeof(Posn) * MAZE_ROWS * MAZE_COLS);
    int wIndex = 0;

    while(currentCell.row != finish.row || currentCell.col != finish.col){
        // if the space up is empty, move there and leave a mark behind
        // weighted clockwise, starting up
        if(maze[currentCell.row -1][currentCell.col] == 0){
            maze[currentCell.row][currentCell.col] = 2;
            currentCell.row -= 1;
            isTrapped = 0;
        }
        else if(maze[currentCell.row][currentCell.col+1] == 0){
            maze[currentCell.row][currentCell.col] = 2;
            currentCell.col +=1;
            isTrapped = 0;
        }
        else if(maze[currentCell.row + 1][currentCell.col] == 0){
            maze[currentCell.row][currentCell.col] = 2;
            currentCell.row += 1;
            isTrapped = 0;
        }
        else if(maze[currentCell.row][currentCell.col-1] == 0){
            maze[currentCell.row][currentCell.col] = 2;
            currentCell.col -=1;
            isTrapped = 0;
        }

        isTrapped++;

        //if it is in a dead end, backtrack until you reach a new branch       
        //Add temporary walls to the wall list.
        if(isTrapped == 2){

            isTrapped = 1;

            if(maze[currentCell.row -1][currentCell.col] == 2){
                maze[currentCell.row][currentCell.col] = 1;
                addedWalls[wIndex] = currentCell;
                wIndex++;

                currentCell.row -= 1;
            }
            else if(maze[currentCell.row][currentCell.col+1] == 2){
                maze[currentCell.row][currentCell.col] = 1;
                addedWalls[wIndex] = currentCell;
                wIndex++;

                currentCell.col +=1;
            }
            else if(maze[currentCell.row + 1][currentCell.col] == 2){
                maze[currentCell.row][currentCell.col] = 1;
                addedWalls[wIndex] = currentCell;
                wIndex++;

                currentCell.row += 1;
            }
            else if(maze[currentCell.row][currentCell.col-1] == 2){
                maze[currentCell.row][currentCell.col] = 1;
                addedWalls[wIndex] = currentCell;
                wIndex++;

                currentCell.col -=1;
            }
        }
    }

    //Add the final square to the route
    maze[currentCell.row][currentCell.col] = 2;

    //Now delete all the temporary walls
    for(int k = 0; k < wIndex; k++){
        maze[addedWalls[k].row][addedWalls[k].col] = 0;    
    }

}

//Print the maze, x's for walls
void printMaze(int** maze, int rows, int cols){
   for(int i = 0; i < rows; i++){  
        for(int k = 0; k < cols; k++){  
            switch(maze[i][k]){
                case 2:
                    printf("o");
                    break;
                case 1:
                    printf("X");
                    break;
                case 0:
                     printf(" ");
                   break;
             }
         }
        printf("\n");
    }
    printf("\n\n");
}

//Allocate a 2d array of ints, with a grid of walls
//works best with odd rows/cols
int** allocMaze(int rows, int cols){
     int x, y; // Column and row coordinates.
 
     // First, allocate the array of pointers to the columns
     int** array = (int**) (calloc(rows, sizeof(int*)));
 
     for (x = 0; x < rows; x++) {
         // Allocate each column array.
         array[x] = (int*) (calloc(cols, sizeof(int)));
 
         for(y = 0; y < cols; y++){
             if(y%2 != 0 && x%2 != 0)
                 array[x][y] = 0;
             else
                 array[x][y] = 1;
         }
     }
      
     return array;
}

//return 1 if the posn list contains the posn, else 0
int contains(Posn* list,int length, Posn aPosn){
    for(int k = 0; k < length; k++){
        if(list[k].row == aPosn.row && list[k].col == aPosn.col)
            return 1;
    }
    return 0;
}
int containsAlt(Posn* list, int length, int row, int col){
    Posn theCell;
    theCell.row = row;
    theCell.col = col;
    
    return contains(list, length, theCell);
}

//Remove the posn at the specified index and shift late elements left
void removeIndex(Posn* list, int length, int index){
    for(int k = index+1; k < length; k++){
        list[k-1] = list[k];
    }
}
void removePosn(Posn* list, int length, Posn aPosn){
    //Find the index of the posn then use other function
    for(int k = 0; k < length; k++){
        if(list[k].row == aPosn.row && list[k].col == aPosn.col)
            return removeIndex(list, length, k);
    }
    printf("\n\n -- No such Posn %d %d Unvisited  \n\n", aPosn.row, aPosn.col);
}

//Make all the borders walls
void encloseMaze(int** maze, int rows, int cols){
    for(int i = 0; i < cols; i++){
        maze[0][i] = 1;
        maze[rows-1][i] = 1;
    }

    for(int k = 0; k < rows; k++){
        maze[k][0] = 1;
        maze[k][cols-1] = 1;
    }
}

//Adds nearby walls to the list of posns Given, from the maze at the specified posn
//Return the num of nearby walls added
int addNearbyWalls(int** maze, int rows, int cols, Posn* list, int length, Posn aPosn){

    int originalLength = length;
    if(aPosn.row -1  >= 0){
        Posn thisWall;
        thisWall.row = aPosn.row -1;
        thisWall.col = aPosn.col;
       //If it is a wall, and is not already in the list, add it to the list 
        if(maze[thisWall.row][thisWall.col] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if(aPosn.row + 1 < rows){
        Posn thisWall;
        thisWall.row = aPosn.row +1;
        thisWall.col = aPosn.col;

        if(maze[thisWall.row][thisWall.col] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if(aPosn.col -1 >= 0){
        Posn thisWall;
        thisWall.row = aPosn.row;
        thisWall.col = aPosn.col-1;

        if(maze[thisWall.row][thisWall.col] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    if(aPosn.col +1 < cols){
        Posn thisWall;
        thisWall.row = aPosn.row;
        thisWall.col = aPosn.col+1;
        
        if(maze[thisWall.row][thisWall.col] && !contains(list, length, thisWall)){
            list[length] = thisWall;
            length++;
        }
    }

    //return the number of elements added 
    return length - originalLength;
}


void randPrim(int** maze, int rows, int cols){
Posn* wallList = (Posn*)malloc(sizeof(Posn) * rows * cols);
int wallListLength = 0;

    Posn* unvisited = (Posn*)malloc(sizeof(Posn)* rows * cols);

    int uIndex = 0; // index of the unvisited array;
    
    //Add all the unvisted cells to the list
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if(maze[i][k] == 0){
                Posn nextPosn;
                nextPosn.row = i;
                nextPosn.col = k;
                unvisited[uIndex] = nextPosn;
                uIndex++;
            }
        }
    }


    while(uIndex > 0){
        while(wallListLength > 0){
            int nextWallIndex = rand() % wallListLength;
            Posn nextWall = wallList[nextWallIndex];
            removeIndex(wallList, wallListLength, nextWallIndex);
            wallListLength--;
            
            int numNearUnvisited = 0;
            Posn nextMove; 
            //If the next position is valid and unvisited
            if(nextWall.row -1 >= 0 && containsAlt(unvisited, uIndex, nextWall.row-1, nextWall.col)){
               //set it to the next move, increment numNearUnvisited
               nextMove.row = nextWall.row -1;
               nextMove.col = nextWall.col;
               numNearUnvisited++;
            }

            if(nextWall.row +1 < rows && containsAlt(unvisited, uIndex, nextWall.row+1, nextWall.col)){
                nextMove.row = nextWall.row+1;
                nextMove.col = nextWall.col;
                numNearUnvisited++;
            }

            if(nextWall.col -1 >= 0 && containsAlt(unvisited, uIndex, nextWall.row, nextWall.col-1)){
                nextMove.row = nextWall.row;
                nextMove.col = nextWall.col-1;
                numNearUnvisited++;
            }

            if(nextWall.col +1 < cols && containsAlt(unvisited, uIndex, nextWall.row, nextWall.col+1)){
                nextMove.row = nextWall.row;
                nextMove.col = nextWall.col+1;
                numNearUnvisited++;
            }
            
            //If there is only one unvisited cell by the wall, make a passage
            //Then add the neighboring walls of that cell to the wall list
            if(numNearUnvisited == 1){
                maze[nextWall.row][nextWall.col] = 0;
                removePosn(unvisited, uIndex, nextMove);
                uIndex -= 1;
                wallListLength += addNearbyWalls(maze, rows, cols, wallList, wallListLength, nextMove);
            }
            printMaze(maze,rows,cols);
        }

        if(uIndex == 0)
            break;

        int nextIndex = rand() % uIndex;
        Posn nextCell = unvisited[nextIndex];

        removeIndex(unvisited, uIndex, nextIndex);
        uIndex -= 1;
        
        //add those walls to nthe list
        wallListLength += addNearbyWalls(maze,rows,cols,wallList,wallListLength,nextCell);
        printMaze(maze,rows,cols);
    }

}
