#include<stdio.h>
#include "avl.h"

struct int_avl:public avl{
	int value;
};

int cmpint(void* a,void* b){
	return ((int_avl*)a)->value - ((int_avl*)b)->value;
}

avl_tree ints;

int_avl myint[20];

void listree(int_avl* a,int m){
	int n=m;
	if(a==0) return;
	if(a->right) listree((int_avl*)a->right,m+1);
	while(n--) printf("   ");
	printf("%d (%d)\n",a->value,a->balance);
	if(a->left) listree((int_avl*)a->left,m+1);
}

int main(int argc,char* argv[]){
	int i;
	for(i=0;i<20;i++)
		myint[i].value=(i*9)%20;
	ints.compar=cmpint;
	ints.root=0;
	for(i=0;i<20;i++){
		printf("-------------\n");
		avl_insert(&ints,(avl*)&myint[i]);
		listree((int_avl*)ints.root,0);
	}
	for(i=0;i<20;i++){
		printf("++++++++++++++\n");
		avl_remove(&ints,(avl*)&myint[i]);

		listree((int_avl*)ints.root,0);
	}
	return 0;
}
