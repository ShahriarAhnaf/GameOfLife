# GameOfLife
Cellular Automata visualilzed using the OpenGL library

## 2d Dynamic arrays in thihs game
- using a block of allocated 1d array and indexing using the index function which takes in (x,y) value

## OPENGL GAME ENGINE
- vertex shaders and buffers to store vertices to draw
- usage of simple animations such as color shaders 
- ability to easily draw triangles and squares to the window.


## understanding the drawings
- ELEMENTS? -> a collection of vertices to make a quad represented by 4 vertices
- VERTICES? - > a collection of xy coordinate represented by 2 floats
- layouts? -> how to read elements in specific order using the buffer. made up of type, count and a normalization bool.
- buffers? -> an array of data to store to call for drawing later
- array buffers? -> an array of buffers to choose which one to draw later. 
**THE TWO METHODS OF ARRAY BUFFERS**

USING ONE VERTEX ARRAY BUFFER WITH MULTIPLE LAYOUTS TO BIND 

OR 

USING MULTIPLE VERTEX ARRAY BUFFERS FOR EACH OBJECT. 


## Game break down
- make board from drawing quads on the screen.
- values in board correspond to grafix colour
- positions of the grid are divided by the size of the screen.
- pixels are simply two triangles that are pretty small
- **BE CAREFUL OF DYNAMIC BUFFER ALLOCATIONS!!!!**
