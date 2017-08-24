#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "huffman.h"

long int compare(const long int a,const long int b)
{
	
     if((a)<(b)) return -1;
    else if((a)>(b))return 1;
	else return 0;
}

long int compare1(const unsigned char a,const unsigned char b)
{       if((a)<(b)) return 1;
	else if((a)>(b))return -1; 
	else return 0;
}

lnode *lnode_construct(const long int new_object, const char new_object1)
{
	lnode *ret_node = (lnode *)malloc(sizeof(lnode));
	if (ret_node == NULL) {
		// fprintf(stderr, "malloc fail!!!!\n");
		return NULL;
	}
	ret_node->ptr = (long int)new_object;
	ret_node->ch = new_object1;
	ret_node->next = NULL;
	ret_node->left = NULL;
	ret_node->right = NULL;
	return ret_node;
}

lnode *PQ_enqueue(lnode **pq, const long int new_object, const char new_object1, 
		long int (*cmp_fn)(const long int,  const long int), long int (*cmp_fn1)(const unsigned char, const unsigned char))
{
	// if (new_object == NULL) {
	// return NULL;
	// }
	lnode *new_node = lnode_construct(new_object, new_object1);
	if (new_node == NULL) {
		// fprintf(stderr, "malloc fail!!!!\n");
		return NULL;
	}
	lnode dummy;
	dummy.next = *pq;
	lnode *prev = &dummy;
	lnode *curr = *pq;
	while (curr != NULL) {
		if ((cmp_fn(curr->ptr, new_object) > 0)  && (cmp_fn1(curr->ch, new_object1) >=0)) {
			//{// Hello
			break;}
		// else { prev = curr; curr = curr->next;}
		// } 
		else {
			prev = curr;
			curr = curr->next;
		}
	}
	prev->next = new_node;
	new_node->next = curr;
	*pq = dummy.next;
	return new_node;
}



void lnode_destroy(lnode *root)
{
	//  lnode *temp = list;
	//lnode *next; 

	if (root == NULL) {
		return;
	}
	lnode_destroy(root->left);
	lnode_destroy(root->right);
	free(root);
	//  free(temp);
	//  free(list);
	//temp = next;

}


long int functiontwo(char *readfile, char *treefile, char *charfile, char *bitfile)
{
	FILE *fptr;// *dptr1;
	//char str[100];
	fptr = fopen(readfile,"r");
	if(fptr == NULL)
		return EXIT_FAILURE;
 
        fseek(fptr,0,SEEK_END);
        if(ftell(fptr) == 0) 
           	{fclose(fptr); return EXIT_FAILURE;}
        else
                fseek(fptr,0,SEEK_SET);
	lnode *stack;
	stack = NULL;
	/* long long int d = 5;
	   PQ_enqueue(&stack,&d,compare);
	   lnode_print(stack,printit);*/

	long int i = 0;
	long int ch = 0;
	static long int number[256] = {0};
	//char *str;
	while((ch = fgetc(fptr)) != EOF)
	{
		number[ch]++;
	}

	for(i = 0;i<256;i++)
	{
		if(number[i] != 0)
		{ unsigned char c = i;
			PQ_enqueue(&stack,number[i],c,&compare, &compare1);
			//  lnode_print(stack,printit, printit1);
		}

	}

	stack = functionthree(stack, treefile);

	lnode *temp2; //= lnode_construct(0,'0');
	temp2 = stack;

	stack = functionchar(stack, charfile, bitfile);
	// stack = functionlast(stack, chararray, bitfile);
	lnode_destroy(stack);
	fclose(fptr);
	return 0;
}

lnode *PQ_enqueue1(lnode **pq, const long int new_object,
		long int (*cmp_fn)(const long int,  const long int),  lnode *left, lnode *right)
{

	lnode *new_node = lnode_construct(new_object, '0');
	if (new_node == NULL) {
		// fprintf(stderr, "malloc fail!!!!\n");
		return NULL;
	}
	lnode dummy;
	dummy.next = *pq;
	lnode *prev = &dummy;
	lnode *curr = *pq;
	while (curr != NULL) {
		if ((cmp_fn(curr->ptr, new_object) > 0)) {
			//{// Hello
			break;}
		// else { prev = curr; curr = curr->next;}
		// } 
		else {
			prev = curr;
			curr = curr->next;
		}
	}
	prev->next = new_node;
	new_node->next = curr;
	*pq = dummy.next;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

long int preorder_collect(long int i, long int *array, lnode *node, FILE *fptr) 
{
	if (node->left == NULL && node->right == NULL) {
		long int j;
		fputc(node->ch,fptr);
		fputc(':',fptr);
		for(j = 0;j<i;j++)
		{ fputc(array[j]+'0',fptr);}
		fputc('\n',fptr);
		return i;
	}
	array[i] = 0;


	preorder_collect(i+1, array, node->left, fptr);
	array[i] = 1;
	preorder_collect(i+1, array, node->right,fptr);

	return i;
}

lnode *functionthree(lnode *stack, char *treefile)
{


	while(stack->next != NULL)
	{
		PQ_enqueue1(&stack, stack->ptr + stack->next->ptr, compare, stack, stack->next);
		stack = stack->next->next;}

	FILE *fptr;
	fptr = fopen(treefile,"w");
	// long int j = stack->ptr;
	long int array[stack->ptr];
	// printfunction(0, array, stack, stack, fptr);
	lnode *temp = stack;
	preorder_collect(0, array, temp, fptr) ;

	fclose(fptr);

	return stack;
}
long int postordercount(lnode *root, long int *i)
{

	if (root == NULL) {
		*i = *i + 1;// = i+1;    
		return *i;
	}
	if(root->left == NULL && root->right == NULL)
	{  *i = *i + 1; }

	postordercount(root->left, i);
	postordercount(root->right, i);
	//fputc(root->ch, fptr);
	// *i = *i + 1;
	return *i;
}

long int postorder(lnode *root, FILE *fptr, long int *i)
{

	if (root == NULL) {
		*i = *i + 1;// = i+1;    
		return 0;
	}
	if(root->left == NULL && root->right == NULL)
	{  fputc('1',fptr); }

	postorder(root->left, fptr, i);
	postorder(root->right, fptr,i);
	fputc(root->ch, fptr);
	// *i = *i + 1;
	return 0;
}

void print_char_in_bits(char number)
{
	long int i;
	unsigned char mask = 1 << (8 - 1);
	for (i = 0; i < 8; i++) {
		// need to form a mask
		// mask = 1;
		// mask <<= (CHAR_SIZE - i - 1);
		long int result = mask & number;
		if (result == 0) {
		//	     printf("0");
		} else {
		//	  printf("1");
		}
		mask >>= 1;
	}
}


lnode *functionchar(lnode *stack, char *charfile, char *bitfile)
{
	lnode *temp = stack;
	FILE *fptr;
	//  long int arrayofchar[
	fptr = fopen(charfile, "w");
	long int count = 0;

	count = postordercount(temp, &count);
	//   unsigned long int arrayofchar[count];
	// printf("TOTAL NODES : %d\n", count);
	temp = stack; 
	long int i = 0;
	postorder(temp, fptr, &i);
	fputc('0',fptr);  



	fclose(fptr);

	FILE *dptr;
	dptr = fopen(charfile, "r");
	fseek(dptr, 0, SEEK_SET);



	count = 0;
	FILE *gptr;
	gptr = fopen(bitfile,"w");
	char check = 0xff;

	long int ch;
        int checkin = 0;
	unsigned char finalchar = 0x00;
	unsigned char leftover = 0x00; 
	long int check2 = 0;
//	unsigned char temporary = 0x00;
        unsigned char z = 0x00;
	//unsigned char ch1 = 0x00;
	// unsigned char ch2 = 0x00;
	while((ch = fgetc(dptr)) != EOF)
	{ 
if(checkin == 0){
		if(check != 0x00)
		{
			finalchar = finalchar | leftover;
			// print_char_in_bits(finalchar);
			//   printf("\n");  
			//  printf("\n in leftover \n");
			if(count == 8){fputc(finalchar,gptr); //print_char_in_bits(finalchar); 
				finalchar = 0x00; count = 0;}
		}
		if(ch == '1')
		{  
			unsigned char x  = 1<<(8-count-1);
			finalchar = finalchar | x;  count++;  check = 0x00; 
                        checkin = 1;
			//printf("\n");  
			//   printf("\n in 1 \n");
			if(count == 8){fputc(finalchar,gptr); //print_char_in_bits(finalchar);
				finalchar = 0x00; count = 0; check2++;}
		}
		else if(ch == '0')
		{
			unsigned char y = 0xff<<(8-count-1);
			finalchar = finalchar & y; count++; check = 0x00;
			// print_char_in_bits(finalchar);
			//printf("\n");  
			//  printf("\n in 0 \n");
			if(count == 8){fputc(finalchar,gptr); //print_char_in_bits(finalchar);
				finalchar = 0x00; count = 0; check2++;}

		}
           }
	 else if(checkin == 1)
		{


			z = ch>>(count);
			//   printf("Hello %d",count);
			//if(count == 8) {count = 0;}
//                                fprintf(stderr,"%c\n",leftover);
			leftover = ch<<(8-count); 

			check = 0xff;
			//   if(count == 8) {count++;}
			finalchar = finalchar | z;
			// print_char_in_bits(finalchar);
			// printf("\n");  
			// printf("\n final print %d \n",count);
			fputc(finalchar,gptr);//print_char_in_bits(finalchar); 
			finalchar = 0x00;
                        checkin = 0;
			// if(count == 3) break;
			// count = 0;
			// 
			//  printf("\n assigning a char %d \n",count);
		}

		//temporary = ch;
		/*
		   if(count == 8)
		   {


		   }*/
	}

	//  if(leftover != 0x00){ printf("Hello");fputc(finalchar,gptr);}
      //  fprintf(stderr,"%c\n",z);
      //  finalchar = leftover | z;
    //    fprintf(stderr,"%c\n",leftover);
     //   fprintf(stderr,"%c\n",z);
      //  fprintf(stderr,"%c\n",finalchar);
    //    fprintf(stderr,"%c\n",finalchar);
     //  print_char_in_bits(leftover);printf("\n");
     //  print_char_in_bits(z);printf("\n");
     //  print_char_in_bits(finalchar);printf("\n");
     //   if(leftover >0x20)
     //  {
     //   fputc(finalchar,gptr);}
    //   printf("%ld\n",count);
        if(count >0) {fputc(finalchar,gptr);}
	fclose(dptr);
	fclose(gptr);

	return stack;
}
 int main(int argc, char *argv[])
{
	if(argc != 5)
		return EXIT_FAILURE;

	else {
    
		functiontwo(argv[1], argv[2], argv[3], argv[4]);
              

	} //functionthree();
	return EXIT_SUCCESS;
}

