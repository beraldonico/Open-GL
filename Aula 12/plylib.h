#ifndef _PLYLIB_H_
#define _PLYLIB_H_

typedef struct {
  float x, y, z;
} vec3f;

typedef struct {
  int a, b, c;
  vec3f normal;
} triangle;

typedef struct {
  vec3f *vertices;
  int nVertices;

  triangle *triangles;
  int nTriangles;
} ply_model;

ply_model* load_ply_model(const char* f);
void   delete_ply_model(ply_model *m);

#endif

