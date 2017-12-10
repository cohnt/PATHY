# Standalone A* Implementation

Compile:
```
g++ -Wall -Werror -std=c++11 AStar.cpp -o AStar.exe
```

The format for a maze is


```
startRow,startCol
endRow,endCol
maze00,maze01,...,maze0N
maze10,maze11,...,maze1N
   .     .          .
   .        .       .
   .           .    .
mazeM0,mazeM1,...,MazeMN
```


Then set the contents of whichMaze.txt to the filename (with extension).