#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height);

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();


int main(int argc, char *argv[])
{

        int x = 0;
        int frameNumber = -1;
        unsigned int width = -1;
        unsigned int height = -1;
        int aging = -1;
        int hflip = -1;
        int edge = -1;
        int cropStart = -1;
        int cropEnd = -1;
        int fast = -1;
        int reverse = -1;
        int spotlight = -1;
        int zoom = -1;
        char *watermark = NULL;
        char *fin = NULL;
        char *fout = NULL;
        char *ptr;


        /* the while loop to check options in the command line */
        while (x < argc) {
                /* the input file name */
                if (strcmp(argv[x], "-i") == 0) {
                        if (x < argc - 1) {
                                fin = argv[x + 1];
                        } /*fi*/
                        else {
                                printf("Missing argument for the input file name!\n");
                                return 5;
                        } /*esle*/
                        x += 2;
                        continue;
                } /*fi*/

                /* the output file name */
                if (strcmp(argv[x], "-o") == 0) {
                        if (x < argc - 1) {
                                fout = argv[x + 1];
                        } /*fi*/
                        else {
                                printf("Missing argument for the output file name!\n");
                                return 5;
                        } /*esle*/
                        x += 2;
                        continue;
                } /*fi*/

                /* the help option */
                if (strcmp(argv[x], "-h") == 0) {
                        PrintUsage();
                        return 0;
                } /*fi*/
                /* frame Number arguement */
                if(strcmp(argv[x], "-f") == 0) {
                        if (x < argc - 1) {
                                frameNumber = strtol(argv[x + 1],&ptr,10);
                        } /*fi*/
                        else {
                                printf("Missing argument for the Frame Number!\n");
                                return 5;
                        } /*esle*/
                        x += 2;
                        continue;
                } /*fi*/
                if(strcmp(argv[x], "-s") == 0) {
                        if (x < argc - 1) {
                                if (sscanf(argv[x+1], "%ux%u", &width, &height) == 2) {
                                } else {
                                        printf("format error for resolution of video");
                                }
                        } /*fi*/
                        else {
                                printf("Missing argument for the video size!\n");
                                return 5;
                        } /*esle*/
                        x += 2;
                        continue;
                } /*fi*/
                if(strcmp(argv[x], "-aging") == 0) {
                        aging = 1;
                        x += 1;
                        continue;
                } /*fi*/
                if(strcmp(argv[x], "-hflip") == 0) {
                        hflip = 1;
                        x += 1;
                        continue;
                } /*fi*/
                if(strcmp(argv[x], "-edge") == 0) {
                        edge = 1;
                        x += 1;
                        continue;
                } /*fi*/
                if(strcmp(argv[x], "-crop") == 0) {
                        if (x < argc - 1) {
                                if (sscanf(argv[x+1], "%u-%u", &cropStart, &cropEnd) == 2) {
                                } else {
                                        printf("format error for resolution of video");
                                }
                        } /*fi*/
                        else {
                                printf("Missing argument for crop!\n");
                                return 5;
                        } /*esle*/
                        x += 2;
                        continue;

                } /*fi*/
                 if(strcmp(argv[x], "-fast") == 0) {
                        if (x < argc - 1) {
                                fast = strtol(argv[x + 1],&ptr,10);
                        } /*fi*/
                        else {
                                printf("Missing argument for the fast operation!\n");
                                return 5;
                        } /*esle*/
                        x += 2;
                        continue;
                } /*fi*/

                if(strcmp(argv[x], "-reverse") == 0) {
                        reverse = 1;
                        x += 1;
                        continue;
                } /*fi*/
                if(strcmp(argv[x], "-watermark") == 0) {
                        if (x < argc - 1) {
                                watermark = argv[x + 1];
                        } /*fi*/
                        else {
                                printf("Missing argument for the Watermark!\n");
                                return 5;
                        } /*esle*/
                        x += 2;
                        continue;
                } /*fi*/
                if(strcmp(argv[x], "-spotlight") == 0) {
                        spotlight = 1;
                        x += 1;
                        continue;
                } /*fi*/
                if(strcmp(argv[x], "-zoom") == 0) {
                        zoom = 50;
                        x += 1;
                        continue;
                } /*fi*/

                x++;
        } /*elihw*/

        if (fin == NULL) {
                printf("Missing required argument for the input file name!\n");
                PrintUsage();
                return 5;
        }

        if (fout == NULL) {
                printf("Missing required argument for the output file name!\n");
                PrintUsage();
                return 5;
        }
        if (width == -1){
                printf("Missing required argument for size!\n");
                PrintUsage();
                return 5;
        }
        if (frameNumber == -1){
                printf("Missing required argument for frame Number!\n");
                PrintUsage();
                return 5;
        }


        MOVIE *movie = LoadMovie(fin,frameNumber,width,height);
        printf("The movie file %s has been read successfully!\n",fin);
        IENTRY *i = movie->Frames->First;

        YUV2RGBMovie(movie);
        if(aging != -1 || hflip != -1 || edge != -1){
                while(i){
                        if(aging != -1){
                                i->RGBImage = Aging(i->RGBImage);
                        }
                        if(hflip != -1){
                                i->RGBImage = HFlip(i->RGBImage);
                        }
                        if(edge != -1){
                                i->RGBImage = Edge(i->RGBImage);
                        }
                        i = i->Next;
                }
                if(aging != -1){
                        printf("Operation Aging is done!\n");
                }
                if(hflip != -1){
                        printf("Operation Hflip is done!\n");
                }
                if(edge != -1){
                        printf("Operation Edge is done!\n");
                }
        }
        i = movie->Frames->First;
        if(cropStart != -1 && cropEnd != -1 ){
                CropImageList(i->List,cropStart,cropEnd);
                printf("Operation Crop is done!\n");
        }

        if(fast != -1){
                FastImageList(movie->Frames,fast);
                printf("Operation Fast Forward is done!\n");
        }

        if(reverse != -1){
                ReverseImageList(movie->Frames);
                printf("Operation Reverse is done!\n");
        }

        i = movie->Frames->First;
        if(watermark != NULL){
                while(i){
                        IMAGE *image = LoadImage(watermark);
                        Watermark(i->RGBImage,image,0,0);
                        DeleteImage(image);
                        i = i->Next;
                }
                printf("Operation Watermark is done!\n");
        }

        i = movie->Frames->First;
        if(spotlight != -1){
                int frame = 0;
                int H = i->RGBImage->H/2;
                int W = i->RGBImage->W;
                int test;
                double X;
                X = H*H + W*W;
                int maxRadius = sqrt(X);
                test  = ((double)(H*H + W*W));

                if(i->List->Length <60){
                        printf("Could not perform Spotlight for movie less than 60 frames!");
                }else{
                        while(frame < 20){
                                Spotlight(i->RGBImage,width/2,height/2,frame * maxRadius/39);
                                frame ++;
                        }
                        while(frame < i->List->Length - 20){
                                frame ++;
                                i = i->Next;
                        }
                        while(i != NULL && i->List->Length){
                                Spotlight(i->RGBImage,width/2,height/2,((i->List->Length - 1)-frame) * maxRadius/19);
                                i = i->Next;
                                frame ++;
                        }
                        printf("Operation Spotlight is done!\n");
                }
        }
        i = movie->Frames->First;
        if(zoom != -1){
                int frame = 0;

                        while(frame < 20){
                                Zoom(i->RGBImage,(100*frame)/20);
                                frame ++;
                                i = i->Next;
                        }
                        while(frame < i->List->Length - 20){
                                frame ++;
                                i = i->Next;
                        }
                        while(i != NULL && i->List->Length){
                                Zoom(i->RGBImage,((i->List->Length-frame)*100)/20);
                                i = i->Next;
                                frame ++;
                        }
        printf("Operation zoom is done!\n");
        }

        RGB2YUVMovie(movie);
        SaveMovie(fout,movie);
        DeleteMovie(movie);
        fin = NULL;
        fout = NULL;

        return 0;
}

void PrintUsage()
{
        printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> -s <WIDTHxHEIGHT> [options]\n"
               "Options:\n"
               "-aging                Activate the aging filter on every movie frame\n"
               "-hflip                Activate horizontal flip on every movie frame\n"
               "-edge                 Activate the edge filter on every movie frame\n"
               "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
               "-fast <factor>        Fast forward the movie by <factor>\n"
               "-reverse              Reverse the frame order of the input movie\n"
               "-watermark <file>     Add a watermark from <file> to every movie frame\n"
               "-spotlight            Spotlight the fade in and fade out\n"
               "-zoom                 Zoom in and out the input movie\n"
               "-h                    Display this usage information\n"
              );
}

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height)
{
        FILE *file;
        unsigned int x, y;
        unsigned char c;
        YUVIMAGE* YUVimage;

        /* Check errors */
        assert(fname);
        assert(n >= 0);

        YUVimage = CreateYUVImage(width, height);
        if (YUVimage == NULL) {
                return NULL;
        }

        /* Open the input file */
        file = fopen(fname, "r");
        if (file == NULL) {
                DeleteYUVImage(YUVimage);
                return NULL;
        }

        /* Find the desired frame */
        fseek(file, 1.5 * n * width * height, SEEK_SET);

        for (y = 0; y < height; y++) {
                for (x = 0; x < width; x++) {
                        c = fgetc(file);
                        SetPixelY(YUVimage, x, y, c);
                } /*rof*/
        }

        for (y = 0; y < height; y += 2) {
                for (x = 0; x < width; x += 2) {
                        c = fgetc(file);
                        SetPixelU(YUVimage, x, y, c);
                        SetPixelU(YUVimage, x + 1, y, c);
                        SetPixelU(YUVimage, x, y + 1, c);
                        SetPixelU(YUVimage, x + 1, y + 1, c);
                }
        }

        for (y = 0; y < height; y += 2) {
                for (x = 0; x < width; x += 2) {
                        c = fgetc(file);
                        SetPixelV(YUVimage, x, y, c);
                        SetPixelV(YUVimage, x + 1, y, c);
                        SetPixelV(YUVimage, x, y + 1, c);
                        SetPixelV(YUVimage, x + 1, y + 1, c);
                }
        }

        /* Check errors */
        assert(ferror(file) == 0);

        /* Close the input file and return */
        fclose(file);
        file = NULL;
        return YUVimage;
}

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height)
{

  MOVIE *movie = CreateMovie();
  int i;
  for(i = 0; i < frameNum; i++){
     AppendYUVImage(movie->Frames,LoadOneFrame(fname, i, width, height));
  }
  return movie;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie)
{
        int count;
        FILE *file;
        IENTRY *curr;

        /* Open the output file */
        file = fopen(fname, "w");
        if (file == NULL) {
                return 1;
        }

        count = 0;
#ifdef DEBUG
        printf("number of frames in imageList: %d\n",movie->Frames->Length);
#endif
        curr = movie->Frames->First;
        while (curr != NULL) {

                SaveOneFrame(curr->YUVImage, fname, file);
                curr = curr->Next;
                count++;
        }

        fclose(file);
        file = NULL;

        printf("The movie file %s has been written successfully!\n", fname);
        printf("%d frames are written to the file %s in total.\n", count, fname);
        return 0;
}


/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file)
{
        int x, y;
        for (y = 0; y < image->H; y++) {
                for (x = 0; x < image->W; x++) {
                        fputc(GetPixelY(image, x, y), file);
                }
        }

        for (y = 0; y < image->H; y += 2) {
                for (x = 0; x < image->W; x += 2) {
                        fputc(GetPixelU(image, x, y), file);
                }
        }

        for (y = 0; y < image->H; y += 2) {
                for (x = 0; x < image->W; x += 2) {
                        fputc(GetPixelV(image, x, y), file);
                }
        }
}
