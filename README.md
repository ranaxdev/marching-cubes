# Instructions
### Sample data loading
- Download sample data (NRRD header and raw data file)
http://klacansky.com/open-scivis-datasets/

- Run program with header file path and raw data file path

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
## Linux
git clone https://github.com/ranaxdev/marching-cubes
cd marching-cubes
mkdir build
cd build
cmake ../
make
./Marching-Cubes [header file path] [raw data file path]
```

## Windows
Recommend using windows powershell
```
git clone https://github.com/ranaxdev/marching-cubes
cd marching-cubes
mkdir build
cd build
cmake ../
Open MarchingCubes.sln with visual studio
Right click MarchingCubes project and build
cp .\Debug\* ./
./Marching-Cubes [header file path] [raw data file path]
```

# NOTE
File dialog window system was removed, now you have to pass the NHDR header file and raw data file paths into the program like

`./Marching-Cubes [header file path] [raw data file path]`
