#ifndef LOOP
#define LOOP

#include "halfedge.h"

// step (1) generate edge points
void loop_generate_edge_points(Mesh *mesh, Mesh *previous);

// step (2) generate new vertices
void loop_generate_new_vertices(Mesh *mesh, Mesh *previous);

// step (3) connect
void loop_connect_new_mesh(Mesh *mesh, Mesh *previous);

#endif
#pragma once
