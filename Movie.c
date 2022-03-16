#include <stdlib.h>
#include <assert.h>
#include "Movie.h"
#include "ImageList.h"

int clip(int x);

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
MOVIE *CreateMovie(void)
{
        MOVIE *newMovie = malloc(sizeof(MOVIE));
        newMovie->Frames = CreateImageList();
        return newMovie;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(MOVIE *movie)
{
        assert(movie);
        DeleteImageList(movie->Frames);
        free(movie);
}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(MOVIE *movie)
{
  int E,C,D;
  ILIST *list = movie->Frames;
  assert(list->First->YUVImage);
  IENTRY *i;
  i  = list->First;
  while(i){
    i->RGBImage = CreateImage(i->YUVImage->W,i->YUVImage->H);
    for(int x = 0; x < i->YUVImage->W;x++){
      for(int y=0; y < i->YUVImage->H;y++){
        C = GetPixelY(i->YUVImage,x,y) - 16;
        D = GetPixelU(i->YUVImage,x,y) - 128;
        E = GetPixelV(i->YUVImage,x,y) - 128;
        SetPixelR(i->RGBImage,x,y,clip(( 298 * C + 409 * E + 128) >> 8));
        SetPixelG(i->RGBImage,x,y,clip(( 298 * C - 100 * D - 208 * E + 128) >> 8));
        SetPixelB(i->RGBImage,x,y,clip(( 298 * C + 516 * D + 128) >> 8));
      }
    }
    DeleteYUVImage(i->YUVImage);
    i->YUVImage = NULL;
    i = i->Next;
  }
}

/* Convert a RGB movie to a YUV movie */

int clip(int x){
  return x > 255? 255 : x < 0 ? 0 : x;
}

void RGB2YUVMovie(MOVIE *movie)
{
  ILIST *list = movie->Frames;
  assert(list->First->RGBImage);
  int R,G,B;
  IENTRY *i;
  i  = list->First;
  while(i){
    i->YUVImage = CreateYUVImage(i->RGBImage->W,i->RGBImage->H);
    for(int x = 0; x < i->YUVImage->W;x++){
      for(int y=0; y < i->YUVImage->H;y++){
        R = GetPixelR(i->RGBImage,x,y);
        G = GetPixelG(i->RGBImage,x,y);
        B = GetPixelB(i->RGBImage,x,y);
        SetPixelY(i->YUVImage,x,y,clip( ( ( 66 * R + 129 * G + 25 * B + 128) >> 8) + 16 ));
        SetPixelU(i->YUVImage,x,y, clip( ( ( -38 * R - 74 * G + 112 * B + 128) >> 8) + 128 ));
        SetPixelV(i->YUVImage,x,y,clip( ( ( 112 * R - 94 * G - 18 * B + 128) >> 8) + 128 ));
      }
    }
    DeleteImage(i->RGBImage);
    i->RGBImage = NULL;
    i = i->Next;
  }
}
