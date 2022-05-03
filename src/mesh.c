#include <stdio.h>
#include "mesh.h"
#include "array.h"
#include <string.h>

mesh_t mesh = {
	.vertices = NULL,
	.faces = NULL,
	.rotation = { 0, 0, 0 }
};
vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{ .x = -1, .y = -1, .z = -1 },  //1
	{ .x = -1, .y =  1, .z = -1 },  //2
	{ .x =  1, .y =  1, .z = -1 },  //3
	{ .x =  1, .y = -1, .z = -1 },  //4
	{ .x =  1, .y =  1, .z =  1 },  //5
	{ .x =  1, .y = -1, .z =  1 },  //6
	{ .x = -1, .y =  1, .z =  1 },  //7
	{ .x = -1, .y = -1, .z =  1 }  	//8
};

face_t cube_faces[N_CUBE_FACES] = {
	//front
	{ .a = 1, .b = 2, .c = 3 }, 
	{ .a = 1, .b = 3, .c = 4 }, 
	//right
	{ .a = 4, .b = 3, .c = 5 }, 
	{ .a = 4, .b = 5, .c = 6 }, 
	//back
	{ .a = 6, .b = 5, .c = 7 }, 
	{ .a = 6, .b = 7, .c = 7 }, 
	//left
	{ .a = 8, .b = 7, .c = 2 }, 
	{ .a = 8, .b = 2, .c = 1 }, 
	//top
	{ .a = 2, .b = 7, .c = 5 }, 
	{ .a = 2, .b = 5, .c = 3 }, 
	//bottom
	{ .a = 6, .b = 8, .c = 1 }, 
	{ .a = 6, .b = 1, .c = 4 }
};

void load_cube_mesh_data(void){
	for(int i = 0; i < N_CUBE_VERTICES; i++){
		vec3_t cube_vertex = cube_vertices[i];
		array_push(mesh.vertices, cube_vertex);
	}
	for(int i = 0; i < N_CUBE_FACES; i++){
		face_t cube_face = cube_faces[i];
		array_push(mesh.faces, cube_face);
	}
};


void load_obj_file_data(char* filename){

	//TODO:
	//Read contents of the OBJ file
	//and load the vertices and faces in our mesh.vertices and mesh.faces
	//see above
	//to research: how to read from files and parse strings
	//need to read in file as a whole, look for every line beginning with v
	//push lines beginning with v into 
	FILE* fp; //file pointer
	char str[60];
	fp = fopen(filename, "r");
	if(fp == NULL){
		perror("File failed to open.");
	}
	while(fgets(str, sizeof str, fp) != NULL){
		char letter = str[0];
		if(letter == 'v' && str[1] == ' '){
			for(int i = 0; i < strlen(str); i++){
				if(str[i] == ' ' && ( i + 1) < strlen(str)){
					char to_add = -1;
					if(str[i+1] == '-'){
						printf("%c%c \n", str[i+1], str[i+2]);
					}else{
						printf("%c \n", str[i + 1]);
					}

				//	vec3_t file_vertex = { .a = str[2], .b = str[10], .c = 	
				}
			}
			
		}
	 if (feof(fp))
       		puts("End of file reached");
	}
	fclose(fp);
};
