# GameOfLife
Cellular Automata visualilzed using the OpenGL library

## ISSUES
FIX THE FREAKING BUG FOR INDEX BUFFERING
## 2d Dynamic arrays in thihs game
- using a block of allocated 1d array and indexing using the index function which takes in (x,y) value


## OPENGL GAME ENGINE
- vertex shaders and buffers to store vertices to draw
- usage of simple animations such as color shaders 
- ability to easily draw triangles and squares to the window.


## understanding the drawings
- ELEMENTS? -> 
- VERTICES?
- layouts?
- buffers?
- array buffers?


## Game break down
- make board from drawing quads on the screen.
- values in board correspond to grafix colour
- positions of the grid are divided by the size of the screen.
- pixels are simply two triangles that are pretty small
- **BE CAREFUL OF DYNAMIC BUFFER ALLOCATIONS!!!!**
