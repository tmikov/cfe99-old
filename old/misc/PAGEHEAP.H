/*
 $Header: /Datapark/GenLib/PAGEHEAP.H 1     1/23/99 10:09p Ceco $

 $Log: /Datapark/GenLib/PAGEHEAP.H $
 
 1     1/23/99 10:09p Ceco
 
 1     12/11/98 3:43p Ceco
 
 4     11/15/98 4:53p Ceco
 Added setting of default libraries.
*/
#ifndef PAGEHEAP_H
#define PAGEHEAP_H

#pragma once           // speed up using of this header

#include "clist.h"

typedef struct TPagedHeap
{
  unsigned   blockSize;
  unsigned   pageLen;
  unsigned   maxPages;    // maximum number of pages
  unsigned   curPages;    // currenr number of pages
  TListRoot  freeBlocks;
  TListRoot  orphanBlocks; 
  TListRoot  virginPages;
  TListRoot  whores;
} TPagedHeap;

void InitPagedHeap ( TPagedHeap * pHeap, unsigned blkSize, unsigned numInPage,
                     unsigned maxPages );
int  PreallocatePagedHeap ( TPagedHeap * pHeap, unsigned numPages );
void DonePagedHeap ( TPagedHeap * pHeap );
void * AllocPagedBlock ( TPagedHeap * pHeap );
void * CAllocPagedBlock ( TPagedHeap * pHeap );
void FreePagedBlock ( TPagedHeap * pHeap, void * usedBlock );
void CompactPagedHeap ( TPagedHeap * pHeap );

#endif


