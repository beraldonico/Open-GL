#include "plylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

vec3f minus(vec3f a, vec3f b) {
  vec3f ret;

  ret.x = a.x-b.x;
  ret.y = a.y-b.y;
  ret.z = a.z-b.z;

  return ret;
}

vec3f cross(vec3f u, vec3f v) {
  vec3f ret;

  ret.x = u.y*v.z-u.z*v.y;
  ret.y = u.z*v.x-u.x*v.z;
  ret.z = u.x*v.y-u.y*v.x;
  
  return ret;
}

float length(vec3f a) {
  return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}

vec3f normalize(vec3f a) {
  float l = length(a);
  vec3f ret;

  ret.x = a.x/l;
  ret.y = a.y/l;
  ret.z = a.z/l;

  return ret;
}

ply_model* load_ply_model(const char* f) {

  FILE *file = fopen(f,"r");
  if (file == NULL) return NULL;

  #define BUFFER_SIZE 1024
  char line[BUFFER_SIZE];

  // header
  char format[128];
  int version[2];
  if (fscanf(file,"ply\nformat %s %d.%d\n", format, version, version+1) != 3) {
    printf("Failed[%d]\n",__LINE__);
    fclose(file);
    return NULL;
  }

  while (fgets(line, BUFFER_SIZE, file) != NULL) {
    if (strncmp(line,"comment",7) == 0) {
      //printf("comment: %s",&line[8]);
    } else break;
  }

  int nVertices = 0;
  if (sscanf(line,"element vertex %d",&nVertices) != 1) {
    printf("Failed[%d]\n",__LINE__);
    fclose(file);
    return NULL;
  }

  while (fgets(line, BUFFER_SIZE, file) != NULL) {
    if (strncmp(line,"property",7) == 0) {
      //printf("property: %s",&line[8]);
    } else break;
  }

  int nTriangles = 0;
  if (sscanf(line,"element face %d",&nTriangles) != 1) {
    printf("Failed[%d]\n",__LINE__);
    fclose(file);
    return NULL;
  }

  while (fgets(line, BUFFER_SIZE, file) != NULL) {
    if (strncmp(line,"property",7) == 0) {
      //printf("property: %s",&line[8]);
    } else break;
  }

  if (strncmp(line,"end_header",10) != 0) {
    printf("Failed[%d]\n",__LINE__);
    fclose(file);
    return NULL;
  }

  ply_model *m = (ply_model*)malloc(sizeof(ply_model));
  if (m == NULL) {
    fclose(file);
    return NULL;
  }
  m->nVertices = nVertices;
  m->nTriangles = nTriangles;

  m->vertices = (vec3f*)malloc(nVertices*sizeof(vec3f));
  if (m->vertices == NULL) {
    fclose(file);
    free(m);
    return NULL;
  }

  m->triangles = (triangle*)malloc(nTriangles*sizeof(triangle));
  if (m->triangles == NULL) {
    fclose(file);
    free(m->vertices);
    free(m);
    return NULL;
  }

  int i;
  for (i=0;i<nVertices && fgets(line, BUFFER_SIZE,file) != NULL;i++) {
    float x,y,z;
    sscanf(line,"%f %f %f", &x, &y, &z);
    m->vertices[i].x = x;
    m->vertices[i].y = y;
    m->vertices[i].z = z;
  }

  for (i=0;i<nTriangles && fgets(line, BUFFER_SIZE,file) != NULL;i++) {
    int sides;
    int a,b,c;
    sscanf(line,"%d %d %d %d", &sides, &a, &b, &c);
    m->triangles[i].a = a;
    m->triangles[i].b = b;
    m->triangles[i].c = c;

    vec3f u = minus(m->vertices[a],m->vertices[b]);
    vec3f v = minus(m->vertices[a],m->vertices[c]);;

    m->triangles[i].normal = normalize(cross(u,v));
  }

  return m;
}

void   delete_ply_model(ply_model *m) {
  if (m == NULL) return;

  free(m->triangles);
  free(m->vertices);
  free(m);
}

