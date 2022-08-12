# Instructions
### Sample data loading
- Download sample data (NRRD header and raw data file)
http://klacansky.com/open-scivis-datasets/

- Enter isovalue

- Load NHDR first, load data next to generate isosurface

- Enter new isovalue and click regenerate to render at different levels

### Math function implicit surface generation
- Select buttons 'sphere', 'bumps', 'torus' or 'bowl'
- Enter new isovalue and click regenerate to render at different levels
- Select show grid to disable/enable grid
- Select show faces to disable/enable wireframe mode

### Vertex debugging
- Use the matrix of checkboxes to enable/disable vertices for the debug cells

### Other
- Use scale slider to scale the isosurface up and down
- Use the place light button to change the light position to the current camera position
- Use output tris button to export current isosurface to OBJ file

## Controls
Movement - WASD

Orient - Mouse

Exit - ESC

# Installation
```
git clone https://github.com/ranaxdev/marching-cubes
cd marching-cubes
mkdir build
cd build
cmake ../
make
./Marching-Cubes
```