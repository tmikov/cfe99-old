/*-----------------------------------------------------------------
    SDCChast.h - contains support routines for hashtables/sets .

    Written By - Sandeep Dutta . sandeep.dutta@usa.net (1998)

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2, or (at your option) any
    later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
    
    In other words, you are welcome to use, share and improve this program.
    You are forbidden to forbid anyone else to use, share and improve
    what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/

#ifndef SDCCHASHT_H
#define SDCCHASHT_H

#define SDCC_NAME_MAX 32
#define HASHTAB_SIZE 256

/* hash table bucket */
typedef struct bucket
{
    void     	  *sym	;	                 /* pointer to the object   */
    char	   name[SDCC_NAME_MAX+1];	 /* name of this symbol		 */
    int            level ;                       /* nest level for this symbol   */         
    int            block ;                       /* belongs to which block */
    struct bucket  *prev ;	                 /* ptr 2 previous bucket   */
    struct bucket  *next ;	                 /* ptr 2 next bucket	    */
} bucket ;


/* hashtable item */
typedef struct hashtItem
{
    int key ;
    void *item ;
    struct hashtItem *next ;
} hashtItem ;

/* hashtable */
typedef struct hTab
{
    int size  ;             /* max number of items */
    int minKey;             /* minimum key value   */
    int maxKey ;            /* maximum key value */
    hashtItem **table ;     /* the actual table  */
    int currKey  ;          /* used for iteration */
    hashtItem *currItem ;   /* current item within the list */
    int nItems ;
} hTab ;



enum {
    DELETE_CHAIN = 1,
    DELETE_ITEM };


/*-----------------------------------------------------------------*/
/*	     Forward   definition    for   functions		   */
/*-----------------------------------------------------------------*/
void	       initHashT	    (				   );
bucket	      *newBucket	    (				   );
void	       addSym		    ( bucket ** , void	 *, char  *, int, int);
void	       deleteSym	    ( bucket ** , void	 *, char  *);
void	      *findSym		    ( bucket ** , void	 *, char  *);
void          *findSymWithLevel     ( bucket ** , struct symbol *  );


/* hashtable related functions */
hTab   *newHashTable      (int);
void         hTabAddItem (hTab **, int , void * );
void         hTabDeleteItem (hTab **, int  ,
				  void *, int  ,
				  int (*compareFunc)(void *,void *));
int          hTabIsInTable (hTab *, int , void * , 
				 int (*compareFunc)(void *,void *));
void         *hTabFirstItem (hTab *, int *);
void         *hTabNextItem (hTab *, int *);
hTab         *hTabFromTable (hTab *);
int          isHtabsEqual (hTab *,hTab *, int (*compareFunc)(void *,void *));
hashtItem    *hTabSearch  (hTab *, int );
void         *hTabItemWithKey(hTab *,int );
void         hTabAddItemIfNotP(hTab **,int, void *);
#endif
