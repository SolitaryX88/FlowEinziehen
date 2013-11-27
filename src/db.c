/*
 ============================================================================
 Name        : db.c
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Database functionalities
 ============================================================================
 */

int addOne(int num){

	return (num+1);
}

int addTwo(int num){

	return (num+2);
}

int addThree(int num){

	return (num+3);
}

typedef int (*FunctionPtr)(int);


typedef struct _add_t   {

	FunctionPtr add2;
	//OR int (*FunctionPtr)(int) = &TheFunction;

	FunctionPtr add1;
	FunctionPtr add3;

}add_t;

add_t f;

//add_t f;


void setupF(){

	f.add2 = &addTwo;

	f.add3 = &addThree;

	f.add1 = &addOne;
}


int mainfunction() {

	setupF();

	printf("This is the res: %d", f.add2(5) );

	return(0);

}


/*#include<stdio.h>

int func (int a, int b)
{
    printf("\n a = %d\n",a);
    printf("\n b = %d\n",b);

    return 0;
}

int main(void)
{
    int(*fptr)(int,int); // Function pointer

    fptr = func; // Assign address to function pointer

    func(2,3);
    fptr(2,3);

    return 0;
}*/
