#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"

//global variables

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = {.x =0, .y = 0, .z = -5 };
float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;

void setup(void){
	//allocate required memory in bytes to hold color buffer
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height); //get size of uint * window size * window height
		
	//creting an SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
	
	//loads the cube values in the mesh data structure
	//load_cube_mesh_data();
	load_obj_file_data("./assets/cube.obj");
}

void process_input(void){
	SDL_Event event;
	SDL_PollEvent(&event); //pass the reference to the event we created above
	switch(event.type){
		case SDL_QUIT:
			is_running = false;	
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE){
				is_running = false;
			}
			break;
	}
}


/////////////////////////////////////////////////////////////////////
// Function that recieves a 3D vec and returns a projected 2D point
/////////////////////////////////////////////////////////////////////

vec2_t project(vec3_t point){
	vec2_t projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};
	return projected_point;
}

void update(void){

	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
		SDL_Delay(time_to_wait);
	}

	previous_frame_time = SDL_GetTicks(); //How many secs passed since start

	//Initialize the array of triangles to render
	triangles_to_render = NULL;
		
	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	//loop all triangle faces of cube mesh
	int num_face = array_length(mesh.faces);
	for(int i =0; i < num_face; i++){
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];
		
		triangle_t projected_triangle;

		//Loop all three vertices of the current face and apply transform
		for(int j = 0; j < 3; j++){
			vec3_t transformed_vertex = face_vertices[j];
	
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			//translate vertices away from camera in z
			transformed_vertex.z -= camera_position.z;
				
			vec2_t projected_point = project(transformed_vertex);
			
			//scale and translate projected point to the middle of the screen
			projected_point.x += (window_width / 2);
			projected_point.y += (window_height /2);

			projected_triangle.points[j] = projected_point; 
		}
		
		//save the projected triangle in the array of triangles to render
		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
	}

}

void render(void){
	//draw_grid();	
	
	
	//Loop all projected triangles and render them
	int num_triangles = array_length(triangles_to_render);

	for(int i = 0; i < num_triangles; i++){
		triangle_t triangle = triangles_to_render[i];	
		
		//draw vertex pixels
		draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFF00FF00); //vertex A
		draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFF00FF00); //vertex B
		draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFF00FF00); //vertex C

		//draw unfilled triangle
		draw_triangle(
			triangle.points[0].x, triangle.points[0].y, //vertex A
			triangle.points[1].x,triangle.points[1].y, //vertex B
			triangle.points[2].x, triangle.points[2].y, //vertex C
			0xFF00FF00
		);
	}

	//clear the array of triangles to render
	array_free(triangles_to_render);

	render_color_buffer();	
	clear_color_buffer(0xFF000000);		
	
	SDL_RenderPresent(renderer);
}
////////////////////////////////////////////////////////////
//Free the memory that was allocated
////////////////////////////////////////////////////////////
void free_resources(void){
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int main(void) {
	
	/*Create an SDL window*/
	
	is_running = initialize_window();	
	
	setup();

	//game loop	
	//we need a consistent rendering -> while loop is processor dependent
	while(is_running){
		process_input();
		update();
		render();
	}
	
	destroy_window();
	free_resources();	

	return 0;

}
