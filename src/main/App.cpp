#include <SDL.h>        
#include <SDL_image.h>
#include <SDL_render.h>

#include "StartUp.hpp"

#include "file/FileUtil.hpp"

#include "nanosvg.h"
#include "nanosvgrast.h"

using namespace moduru;

NSVGimage* nsvgParseFromFile1(const char* filename, const char* units, float dpi)
{
	FILE* fp = NULL;
	size_t size;
	char* data = NULL;
	NSVGimage* image = NULL;

	fp = fopen(filename, "rb");
	if (!fp) goto error;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	data = (char*)malloc(size + 1);
	if (data == NULL) goto error;
	if (fread(data, 1, size, fp) != size) goto error;
	data[size] = '\0';	// Must be null terminated.
	fclose(fp);
	image = nsvgParse(data, units, dpi);
	free(data);

	return image;

error:
	if (fp) fclose(fp);
	if (data) free(data);
	if (image) nsvgDelete(image);
	return NULL;
}

int main(int argc, char ** argv)
{
	const auto sep = file::FileUtil::getSeparator();
	const auto resPath = mpc::StartUp::resPath;
	const auto imgPath = resPath + sep + "img" + sep;
	const auto bgPath = imgPath + "mpc-px.svg";


	NSVGimage* mpcSvg = NULL;
	NSVGrasterizer *rast = NULL;
	unsigned char* img = NULL;
	int w, h;

	printf("parsing %s\n", bgPath.c_str());
	mpcSvg = nsvgParseFromFile1(bgPath.c_str(), "px", 96);
	if (mpcSvg == NULL) {
		printf("Could not open SVG image.\n");
		return 0;
	}
	w = (int)mpcSvg->width;
	h = (int)mpcSvg->height;
	NSVGshape* shape;
	NSVGpath* path;

	for (shape = mpcSvg->shapes; shape != NULL; shape = shape->next) {	
		//shape->opacity = 0.5f;
		for (path = shape->paths; path != NULL; path = path->next) {
			// do something with paths if necessary
			for (int p = 0; p < path->npts; p++) {
				path->pts[p];
			}
		}
	}

	rast = nsvgCreateRasterizer();
	if (rast == NULL) {
		printf("Could not init rasterizer.\n");
		return 0;
	}

	img = (unsigned char*) malloc(w*h * 4);
	if (img == NULL) {
		printf("Could not alloc image buffer.\n");
		return 0;
	}

	printf("rasterizing image %d x %d\n", w, h);
	nsvgRasterize(rast, mpcSvg, 0, 0, 1, img, w, h, w * 4);

	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);

	const int width = 940;
	const int height = 686;

	SDL_Window * window = SDL_CreateWindow("vMPC2000XL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int shift = (req_format == STBI_rgb) ? 8 : 0;
	rmask = 0xff000000 >> shift;
	gmask = 0x00ff0000 >> shift;
	bmask = 0x0000ff00 >> shift;
	amask = 0x000000ff >> shift;
#else // little endian, like x86
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000; // should maybe be 0, see https://wiki.libsdl.org/SDL_CreateRGBSurfaceFrom#Remarks
#endif

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Surface* imgSurface = SDL_CreateRGBSurfaceFrom(img, width, height, 32, 4 * width, rmask, gmask, bmask, amask);
	SDL_Surface * image = IMG_Load(bgPath.c_str());
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
	
	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
