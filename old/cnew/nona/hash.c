/*-----------------------------------------------------------------
    SDCChast.c - contains support routines for hashtables/sets 
                
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

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "hash.h"



extern int operandKey;
/*-----------------------------------------------------------------*/
/* newBucket - allocates & returns a new bucket 		   */
/*-----------------------------------------------------------------*/
bucket	 *newBucket ()
{
	bucket *bp ;
	      
	ALLOC(bp,sizeof(bucket));
	
	return bp ;
}

/*-----------------------------------------------------------------*/
/* newHashtItem - creates a new hashtable Item                     */
/*-----------------------------------------------------------------*/
 hashtItem *newHashtItem (int key, void *item)
{
    hashtItem *htip;

    ALLOC(htip,sizeof(hashtItem));
    
    htip->key = key ;
    htip->item = item;
    htip->next = NULL ;
    return htip;
}

/*-----------------------------------------------------------------*/
/* newHashTable - allocates a new hashtable of size                */
/*-----------------------------------------------------------------*/
hTab *newHashTable (int size)
{
    hTab *htab;    
    
    ALLOC(htab,sizeof(hTab));  

    if (!(htab->table = GC_malloc((size +1)* sizeof(hashtItem *)))) {
	werror(E_OUT_OF_MEM);
	exit(0);
    }    
    htab->minKey = htab->size = size ;    
    return htab;
}


/*-----------------------------------------------------------------*/
/* hTabAddItem - adds an item to the hash table                    */
/*-----------------------------------------------------------------*/
void hTabAddItem (hTab **htab, int key, void *item )
{   
    hashtItem *htip ;
    hashtItem *last ;

    if (!(*htab) )
	*htab = newHashTable ( operandKey + 1 );
    
    if (key > (*htab)->size ) {	
	int i;       
	(*htab)->table = GC_realloc ((*htab)->table, (key*2 + 2)*sizeof(hashtItem *));
	for ( i = (*htab)->size ; i <= (key*2 + 1); i++ )
	    (*htab)->table[i] = NULL ;
	(*htab)->size = key*2 + 1;
    }
  
    /* update the key */
    if ((*htab)->maxKey < key )
	(*htab)->maxKey = key ;

    if ((*htab)->minKey > key )
	(*htab)->minKey = key ;

    /* create the item */
    htip = newHashtItem (key,item);

    /* if there is a clash then goto end of chain */
    if ((last = (*htab)->table[key])) {
	while (last->next)
	    last = last->next ;
	last->next = htip ;
    } else
	/* else just add it */
	(*htab)->table[key] = htip ;
    (*htab)->nItems++ ;
}

/*-----------------------------------------------------------------*/
/* hTabDeleteItem - either delete an item                          */
/*-----------------------------------------------------------------*/
void hTabDeleteItem (hTab **htab, int key ,
		     void *item, int action ,
		     int (*compareFunc)(void *,void *))
{
    hashtItem *htip, *htipp ;
    
    if (!(*htab))
	return ;

    /* first check if anything exists in the slot */
    if (! (*htab)->table[key] )
	return ;
    
    /* if delete chain */
    if ( action == DELETE_CHAIN ) 	   
	(*htab)->table[key] = NULL ;
    else { 

	/* delete specific item */
	/* if a compare function is given then use the compare */
	/* function to find the item , else just compare the items */

	for (htipp = htip = (*htab)->table[key]; htip; htip = htip->next) {
	    if (compareFunc && (*compareFunc)(item,htip->item))
		break;
	    else
		if (htip->item == item)
		    break;
	}
	
	/* if we found a deletetable item */
	if (htip) {
	    /* if this is the first item */
	    if (htipp == htip)
		(*htab)->table[key] = htip->next ;
	    else
		htipp->next = htip->next;	    	   
	}
    }
    
    (*htab)->nItems-- ;    
    if (!(*htab)->nItems) {
	*htab = NULL;
    } 
}

/*-----------------------------------------------------------------*/
/* hTabIsInTable - will determine if an Item is in the hasht       */
/*-----------------------------------------------------------------*/
int hTabIsInTable (hTab *htab, int key, 
			void *item , int (*compareFunc)(void *,void *))
{
    hashtItem *htip ;

    for (htip = htab->table[key] ; htip ; htip = htip->next ) {
	/* if a compare function is given use it */
	if (compareFunc && (*compareFunc)(item,htip->item))
	    break ;
	else
	    if (item == htip->item)
		break;
    }

    if ( htip)
	return 1;
    return 0;
}

/*-----------------------------------------------------------------*/
/* hTabFirstItem - returns the first Item in the hTab              */
/*-----------------------------------------------------------------*/
void *hTabFirstItem (hTab *htab, int *k)
{   
    int key ;

    if (!htab)
	return NULL ;

    for ( key = htab->minKey ; key <= htab->maxKey ; key++ ) {
	if (htab->table[key]) {
	    htab->currItem = htab->table[key];
	    htab->currKey  = key ;
	    *k = key ;
	    return htab->table[key]->item;
	}
    }
    return NULL ;
}

/*-----------------------------------------------------------------*/
/* hTabNextItem - returns the next item in the hTab                */
/*-----------------------------------------------------------------*/
void *hTabNextItem (hTab *htab, int *k)
{  
    int key ;

    if (!htab)
	return NULL;

    /* if this chain not ended then */
    if (htab->currItem->next) {
	*k = htab->currItem->key ;
	return (htab->currItem = htab->currItem->next)->item ;
    }

    /* find the next chain which has something */
    for ( key = htab->currKey + 1; key <= htab->maxKey ; key++ ) {
	if (htab->table[key]) {
	    htab->currItem = htab->table[key];
	    *k = htab->currKey  = key ;
	    return htab->table[key]->item;
	}
    }

    return NULL ;
}

/*-----------------------------------------------------------------*/
/* hTabFromTable - hash Table from a hash table                    */
/*-----------------------------------------------------------------*/
hTab *hTabFromTable (hTab *htab)
{
    hTab *nhtab ;
    hashtItem *htip;
    int key ;

    if (!htab)
	return NULL ;

    nhtab = newHashTable(htab->size);

    for (key = htab->minKey; key <= htab->maxKey ; key++ ) {
	
	for (htip = htab->table[key] ; htip ; htip = htip->next)
	    hTabAddItem (&nhtab, htip->key, htip->item);
    }

    return nhtab ;
}

/*-----------------------------------------------------------------*/
/* isHtabsEqual - returns 1 if all items in htab1 is found in htab2*/
/*-----------------------------------------------------------------*/
int isHtabsEqual (hTab *htab1, hTab *htab2, 
		  int (*compareFunc)(void *,void *))
{
    void *item;
    int key;    
 
    if ( htab1 == htab2 )
	return 1;
    
    if (htab1 == NULL || htab2 == NULL )
	return 0;

    /* if they are different sizes then */
    if ( htab1->nItems != htab2->nItems)
	return 0;

    /* now do an item by item check */
    for ( item = hTabFirstItem (htab1,&key) ;item;
	  item = hTabNextItem (htab1,&key))
	if (!hTabIsInTable (htab2, key, item, compareFunc))
	    return 0;

    return 1;
}


/*-----------------------------------------------------------------*/
/* hTabSearch - returns the first Item with the specified key      */
/*-----------------------------------------------------------------*/
hashtItem *hTabSearch (hTab *htab, int key )
{
    if (!htab)
	return NULL ;

    if (!htab->table[key])
	return NULL ;

    return htab->table[key] ;
}

/*-----------------------------------------------------------------*/
/* hTabItemWithKey - returns the first item with the gievn key     */
/*-----------------------------------------------------------------*/
void *hTabItemWithKey (hTab *htab, int key )
{
    hashtItem *htip;

    if (!(htip = hTabSearch(htab,key)))
	return NULL;

    return htip->item;
}

/*-----------------------------------------------------------------*/
/*hTabAddItemIfNotP - adds an item with nothing found with key     */
/*-----------------------------------------------------------------*/
void hTabAddItemIfNotP (hTab **htab, int key, void *item)
{
    if (!*htab) {
	hTabAddItem (htab,key,item);
	return;
    }

    if (hTabItemWithKey(*htab,key))
	return ;

    hTabAddItem(htab,key,item);
}

/*-----------------------------------------------------------------*/
/* hashKey - computes the hashkey given a symbol name              */
/*-----------------------------------------------------------------*/
int hashKey (char *s)
{
    unsigned long key = 0;

    while (*s)
	key += *s++ ;
    return key % 256 ;
}

/*-----------------------------------------------------------------*/
/* addSym - adds a symbol to the hash Table                        */
/*-----------------------------------------------------------------*/
void  addSym ( bucket **stab , 
	       void *sym     , 
	       char *sname   , 
	       int level     , 
	       int block)
{
    int i ;	  /* index into the hash Table */
    bucket   *bp ; /* temp bucket    *	       */
    
    /* the symbols are always added at the head of the list  */
    i = hashKey(sname) ;      
    /* get a free entry */
    ALLOC(bp,sizeof(bucket));
    
    bp->sym = sym ;	/* update the symbol pointer  */
    bp->level = level;   /* update the nest level      */   
    bp->block = block;
    strcpy(bp->name,sname);	/* copy the name into place	*/
    
    /* if this is the first entry */
    if (stab[i] == NULL) {
	bp->prev = bp->next = (void *) NULL ;  /* point to nothing */
	stab[i] = bp ;
    }
    /* not first entry then add @ head of list */
    else {       
	bp->prev      = NULL ;
	stab[i]->prev = bp ;
	bp->next      = stab[i] ;
	stab[i]	      = bp ;
    }
}

/*-----------------------------------------------------------------*/
/* deleteSym - deletes a symbol from the hash Table  entry	   */
/*-----------------------------------------------------------------*/
void  deleteSym ( bucket **stab, void *sym, char *sname)
{
    int i = 0 ;
    bucket *bp ;
    
    i = hashKey(sname) ;
    
    bp = stab[i] ;
    /* find the symbol */
    while (bp) {
	if (bp->sym == sym)  /* found it then break out */
	    break ;	   /* of the loop	      */
	bp = bp->next ;
    }
    
    if (!bp)   /* did not find it */
	return ;
    /* if this is the first one in the chain */
    if ( ! bp->prev ) {
	stab[i] = bp->next ;
	if ( stab[i] ) /* if chain ! empty */
	    stab[i]->prev = (void *) NULL ;
    }
    /* middle || end of chain */
    else {
	if ( bp->next ) /* if not end of chain */
	    bp->next->prev = bp->prev ;
	
	bp->prev->next = bp->next ;
    }
    
}

/*-----------------------------------------------------------------*/
/* findSym - finds a symbol in a table				   */
/*-----------------------------------------------------------------*/
void  *findSym ( bucket **stab, void *sym, char *sname)
{
   bucket *bp ;

   bp = stab[hashKey(sname)] ;
   while (bp)
   {
      if ( bp->sym == sym || strcmp (bp->name,sname) == 0 )
	      break ;
      bp = bp->next ;
   }

   return ( bp ? bp->sym : (void *) NULL ) ;
}

/*-----------------------------------------------------------------*/
/* findSymWithLevel - finds a symbol with a name & level           */
/*-----------------------------------------------------------------*/
void *findSymWithLevel ( bucket **stab, symbol *sym)
{
  bucket *bp ;

  bp = stab[hashKey(sym->name)];

  /**
   **  do the search from the head of the list since the 
   **  elements are added at the head it is ensured that
   ** we will find the deeper definitions before we find
   ** the global ones. we need to check for symbols with 
   ** level <= to the level given, if levels match then block
   ** numbers need to match as well
   **/
  while (bp) {

    if ( strcmp(bp->name,sym->name) == 0 && bp->level <= sym->level) {
      /* if this is parameter then nothing else need to be checked */
      if (((symbol *)(bp->sym))->_isparm)
	return (bp->sym) ;
      /* if levels match then block numbers hsould also match */
      if (bp->level && bp->level == sym->level && bp->block == sym->block )
	      return ( bp->sym );
      /* if levels don't match then we are okay */
      if (bp->level && bp->level != sym->level)
	      return ( bp->sym );
      /* if this is a global variable then we are ok too */
      if (bp->level == 0 )
	      return (bp->sym);
    }
    
    bp = bp->next; 
  }

  return (void *) NULL ;
}

