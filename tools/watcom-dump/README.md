# Dethrace Watcom dump tools

### parse_dump.py

Takes an (exedump)[https://github.com/jeff-1amstudios/open-watcom-v2/tree/master/bld/exedump] output file and generates _almost-compilable_ skeleton "c" project files.

Used to generate (src)[../../src]

Example output of a single function:
```c
// Offset: 4876
// Size: 1321
// EAX: pWorld
// EDX: pTrack_spec
// EBX: pCamera
// ECX: pCamera_to_world
void RenderTrack(br_actor *pWorld, tTrack_spec *pTrack_spec, br_actor *pCamera, br_matrix34 *pCamera_to_world, int pRender_blends) {
  tU8 column_x;
  tU8 column_z;
  tU8 min_x;
  tU8 max_x;
  tU8 min_z;
  tU8 max_z;
  br_vector3 edge_before;
  br_vector3 edge_after;
  br_camera *camera;
  br_scalar tan_fov_ish;
  br_actor *result;
}
```

### split-dump.sh
Takes an (exedump)[https://github.com/jeff-1amstudios/open-watcom-v2/tree/master/bld/exedump] output file and splits it into a single file per module for easier debugging.

