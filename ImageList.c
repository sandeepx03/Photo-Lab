#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "ImageList.h"
#include "Image.h"

/* Create a new image list */
ILIST *CreateImageList(void)
{
  ILIST *list = malloc(sizeof(ILIST));
  if (list == NULL) {
                return NULL;
        }
  list->Length = 0;
  list->First = NULL;
  list->Last = NULL;
  return list;
}

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{
  assert(list);
  IENTRY *i,*temp;
  i = list->First;
  if(i->YUVImage == NULL){
    while(i){
        temp = i->Next;
        assert(i->RGBImage);
        DeleteImage(i->RGBImage);
        free(i);
        i = temp;
    }
  }else{
        while(i){
                temp = i->Next;
                assert(i->YUVImage);
                DeleteYUVImage(i->YUVImage);
                free(i);
                i = temp;
        }
  }
  free(list);
}

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, IMAGE *RGBimage)
{
  IENTRY *imgEntry = malloc(sizeof(IENTRY));
  if (imgEntry == NULL) {
                exit(10);
        }
  imgEntry->List = list;
  imgEntry->Next = NULL;
  imgEntry->Prev = list->Last;
  imgEntry->RGBImage = RGBimage;
  imgEntry->YUVImage = NULL;

  if(list->First == NULL){
    list->First = imgEntry;
  }else{
    list->Last->Next = imgEntry;
  }
  list->Last = imgEntry;
  list->Length ++;
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVIMAGE *YUVimage)
{
  IENTRY *imgEntry = malloc(sizeof(IENTRY));
  if (imgEntry == NULL) {
                exit(10);
        }
  imgEntry->List = list;
  imgEntry->Next = NULL;
  imgEntry->Prev = list->Last;
  imgEntry->RGBImage = NULL;
  imgEntry->YUVImage = YUVimage;

  if(list->First == NULL){
    list->First = imgEntry;
  }else{
    list->Last->Next = imgEntry;
  }
  list->Last = imgEntry;
  list->Length ++;
}

/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{
  assert(list);
  IENTRY *i,*temp;
  int counter = 0;
  i = list->First;
  while(i && counter < start){
    temp = i->Next;
    DeleteImage(i->RGBImage);
    if(temp){
        list->First = temp;
        temp->Prev = NULL;
    }
    free(i);
    i = temp;
    list->Length --;
    counter++;
  }
  while(counter < end){
    i = i->Next;
    counter++;
  }
  list->Last = i;
  i = i->Next;
  i->Prev->Next = NULL;
  while(i){
    temp = i->Next;
    DeleteImage(i->RGBImage);
    free(i);
    i = temp;
    list->Length --;
  }
}

void FastImageList(ILIST *list, unsigned int factor)
{
  assert(list);
  assert(list->First->YUVImage == NULL);
  IENTRY *i,*temp;
  int counter = 0;
  i = list->First;
  while(i){
    temp = i->Next;
    if(counter % factor != 0){
      if(temp){
         temp->Prev = i->Prev;
      }
      i->Prev->Next = temp;
      DeleteImage(i->RGBImage);
      free(i);
      list->Length --;
    }
    counter ++;
    i = temp;
  }
  i = list->First;
  while(i){
        list->Last = i;
        i = i->Next;
  }
}

/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
  assert(list->First->YUVImage == NULL);
  assert(list);
  IENTRY *i,*temp;
  i = list->First;
  while(i){
    temp = i->Next;
    i->Next = i->Prev;
    i->Prev = temp;
    i = i->Prev;
  }
  temp = list->First;
  list->First = list->Last;
  list->Last = list->First;
}

