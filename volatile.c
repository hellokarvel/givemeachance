#include <stdio.h>


int    square(volatile  int  *ptr) { 
   int  a; 
  a   =  *ptr;  							//��ȷ 
  return   a  *  a; 
   } 

 //int  square(volatile  int  *ptr) {  
 //int  a,b; 
 // a   =  *ptr;                 ���� 
 //  b   =  *ptr; 
  //  return   a  *  b;  }  


int main()
{int a=6;
	int answer = 	int  square(*a) ;
	printf("%d",answer);
}