#include<stdio.h>
#include "avl.h"

struct int_avl{
	struct avl avl;
	int value;
};

int cmpint(void* a,void* b){
	return ((struct int_avl*)a)->value - ((struct int_avl*)b)->value;
}

struct avl_tree ints;

struct int_avl myint[20];

void listree(struct avl* a,int m){
	int n=m;
	if(a==0) return;
	if(a->right) listree(a->right,m+1);
	while(n--) printf("   ");
	printf("%d (%d)\n",((struct int_avl*)a)->value,a->balance);
	if(a->left) listree(a->left,m+1);
}

int main(int argc,char* argv[]){
	int i;
	for(i=0;i<20;i++)
		myint[i].value=(i*9)%20;
	ints.compar=cmpint;
	ints.root=0;
	for(i=0;i<20;i++){
		printf("-------------\n");
		avl_insert(&ints,(struct avl*)&myint[i]);
		listree(ints.root,0);
	}
	for(i=0;i<20;i++){
		printf("++++++++++++++\n");
		avl_remove(&ints,(struct avl*)&myint[i]);

		listree(ints.root,0);
	}
	return 0;
}
