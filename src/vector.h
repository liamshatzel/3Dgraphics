#ifndef VECTOR_H
#define VECTOR_H

typedef struct{
	float x, y; 
}vec2_t;

typedef struct{
	float x, y, z;
}vec3_t;

////////////////////////////////////////
//Vector 2D Functions 
////////////////////////////////////////
float vec2_length(vec2_t v);

////////////////////////////////////////
//Vector 3D Functions 
////////////////////////////////////////
float vec3_length(vec3_t v);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

#endif
