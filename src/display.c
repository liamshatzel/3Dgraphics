#include "display.h"

//global variables

SDL_Window* window = NULL; 
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL; //pointer to the first position of the 32bit int array
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;


bool initialize_window(void){
	SDL_Init(SDL_INIT_EVERYTHING);
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		fprintf(stderr, "Error initializing SDL. \n");
		return false; //if something goes wrong when initializing we return false
	}	
	//use SDL to query what the fullscreen max width and height are
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;	
	window_height = display_mode.h;



	//Create an SDL window
	window = SDL_CreateWindow(	
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL window \n");
		return false;
	}

	//create an SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);	
	if (!renderer){
		fprintf(stderr, "Error creating SDL renderer. \n");
		return false;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void draw_pixel(int x, int y, uint32_t color){
	if(x >= 0 && x < window_width && y >= 0 && y < window_height){
		color_buffer[(window_width * y) + x] = color;	
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t color){
	for(int y_pos = y; y_pos < (height + y); y_pos++){
		for(int x_pos = x; x_pos < (width + x); x_pos++){
			draw_pixel(x_pos, y_pos, color);
		}

	}	
}

void draw_grid(void){
	for(int y = 0; y < window_width; y++){
		for(int x = 0; x < window_height; x++){
			if(x % 10 == 0 || y % 10 == 0){
				color_buffer[(window_width * y) + x] = 0xFFFFFFFF;
			}
		}
	}
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color){
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);
	
	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);
	
	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;
	
	float current_x = x0;	
	float current_y = y0;
	
	for(int i = 0; i < side_length; i++){
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color){
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x0, y0, x2, y2, color);
}

void render_color_buffer(){
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t))
	);
	SDL_RenderCopy(
		renderer,
		color_buffer_texture,
		NULL,
		NULL
	);
}

void clear_color_buffer(uint32_t color){
	for(int y = 0; y < window_height; y++){
		for(int x = 0; x < window_width; x++){
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void destroy_window(void){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
