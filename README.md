# Subdivision-of-Surfaces
Loop Subdivision of surfaces

This is applicable to a triangle mesh. The basic idea is - given a coarse triangle mesh input, subdivide each triangle into four new 
triangles and compute positions for the old and the new vertices usign some rules of neighorhood that would result into a smooth surface
after a few subdivsion. An appropriate data structure that keeps adjacency information in an efficient manner is used so that operation 
of subdivision and later vertex modifcations are done efficiently. For this half edge data structure or winged data structure is used.

a) The input mesh is read through a file, which contains the initial model generated in a modeler or imported.

b) The level of subdivision is specified as a parameter.

c) Implemented constraints to generate crease (sharp edges).

d) The rendering and viewing is done using OpenGL.

e) A scene has been generated with multiple copies at different level of subdivision and rendered using a strategy such that near objects
are rendered with higher levels of subdivsion and farther objects are rendered with lower levels.

f) The deformation capability of the approach is also shown  by moving one or more vertices of the initial model and applying the 
subdivision.

g) Texture mapping has been added by correspondences between the texture coordinates and the vertices of the coarse description of the 
model. The subdivision has been performed considering both the geometry and texture coordinates (in 5D).
