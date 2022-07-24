# TO-DO

## Implementation

- Debug cube with clickable vertices (construct triangles from there)
- Buffer size passing to rendering functions
- Live isovalue edit (reconstruct triangle buffers live)
- Matching debug vertices (black/white) with isovalue
- Vertex normal calculation
- Different buffer sizes enum ONE_MB, FIVE_MB through param etc..

## Writing
- Write about optimization (grid only bands near isosurface)
- All the cases generated w/ pics
- Wrong case in MC tables w/ fixed papers (reflection in impl)
- in implementation show implicit then with data
- nrrd format, write about raw data used
- debugging stuff







    if(edgeTable[cube_index] & 1){
        t_v[0] = vertex_lerp(vertices[0], vertices[1], samples[0], samples[1], isovalue);
    }
    if(edgeTable[cube_index] & 2){
        t_v[1] = vertex_lerp(vertices[1], vertices[2], samples[1], samples[2], isovalue);
    }
    if(edgeTable[cube_index] & 4){
        t_v[2] = vertex_lerp(vertices[2], vertices[3], samples[2], samples[3], isovalue);
    }
    if(edgeTable[cube_index] & 8){
        t_v[3] = vertex_lerp(vertices[3], vertices[0], samples[3], samples[0], isovalue);
    }
    if(edgeTable[cube_index] & 16){
        t_v[4] = vertex_lerp(vertices[4], vertices[5], samples[4], samples[5], isovalue);
    }
    if(edgeTable[cube_index] & 32){
        t_v[5] = vertex_lerp(vertices[5], vertices[6], samples[5], samples[6], isovalue);
    }
    if(edgeTable[cube_index] & 64){
        t_v[6] = vertex_lerp(vertices[6], vertices[7], samples[6], samples[7], isovalue);
    }
    if(edgeTable[cube_index] & 128){
        t_v[7] = vertex_lerp(vertices[7], vertices[4], samples[7], samples[4], isovalue);
    }
    if(edgeTable[cube_index] & 256){
        t_v[8] = vertex_lerp(vertices[0], vertices[4], samples[0], samples[4], isovalue);
    }
    if(edgeTable[cube_index] & 512){
        t_v[9] = vertex_lerp(vertices[1], vertices[5], samples[1], samples[5], isovalue);
    }
    if(edgeTable[cube_index] & 1024){
        t_v[10] = vertex_lerp(vertices[2], vertices[6], samples[2], samples[6], isovalue);
    }
    if(edgeTable[cube_index] & 2048){
        t_v[11] = vertex_lerp(vertices[3], vertices[7], samples[3], samples[7], isovalue);
    }








    if(edgeTable[cube_index] & 1){
        t_v[0] = vertex_lerp(vertices[0], vertices[4], samples[0], samples[4], isovalue);
    }
    if(edgeTable[cube_index] & 2){
        t_v[1] = vertex_lerp(vertices[4], vertices[5], samples[4], samples[5], isovalue);
    }
    if(edgeTable[cube_index] & 4){
        t_v[2] = vertex_lerp(vertices[5], vertices[1], samples[5], samples[1], isovalue);
    }
    if(edgeTable[cube_index] & 8){
        t_v[3] = vertex_lerp(vertices[1], vertices[0], samples[1], samples[0], isovalue);
    }
    if(edgeTable[cube_index] & 16){
        t_v[4] = vertex_lerp(vertices[2], vertices[6], samples[2], samples[6], isovalue);
    }
    if(edgeTable[cube_index] & 32){
        t_v[5] = vertex_lerp(vertices[6], vertices[7], samples[6], samples[7], isovalue);
    }
    if(edgeTable[cube_index] & 64){
        t_v[6] = vertex_lerp(vertices[7], vertices[3], samples[7], samples[3], isovalue);
    }
    if(edgeTable[cube_index] & 128){
        t_v[7] = vertex_lerp(vertices[3], vertices[2], samples[3], samples[2], isovalue);
    }
    if(edgeTable[cube_index] & 256){
        t_v[8] = vertex_lerp(vertices[0], vertices[2], samples[0], samples[2], isovalue);
    }
    if(edgeTable[cube_index] & 512){
        t_v[9] = vertex_lerp(vertices[4], vertices[6], samples[4], samples[6], isovalue);
    }
    if(edgeTable[cube_index] & 1024){
        t_v[10] = vertex_lerp(vertices[5], vertices[7], samples[5], samples[7], isovalue);
    }
    if(edgeTable[cube_index] & 2048){
        t_v[11] = vertex_lerp(vertices[1], vertices[3], samples[1], samples[3], isovalue);
    }
