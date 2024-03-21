clang -g -o game game.c subgame.c
use flag

maybe do
    set worldarray.collision and or worldarray.istree null so you don't need to remake the array.
    use memset(array, {set as(do 0)}, size);

to do
    check portal logic
    add levels - done
bug 
    -in tree
        -tree spawns too close to  wall
    -in getinput
        -first input is ignore
