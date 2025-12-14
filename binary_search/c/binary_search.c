#include<stdio.h>
#include"tests.h"

int binary_search(const int* list, int len, int target){
	int a=0;
	int b=len-1;
	
	while(a<=b){
		int idx=(a+(b-a)/2);
		
		if (list[idx]==target){
			return idx;
		}
		else if (list[idx]<target){
			a=idx+1;
		}
		else{
			b=idx-1;
		}
	}
	return -1;
}


int main(void){
	int len= sizeof(TESTS)/sizeof(TESTS[0]);
	int passed=0;
	for (int i=0;i<len;i++){
		TestCase test=TESTS[i];
		int result=binary_search(test.arr,test.len,test.target);
		if(result==test.expected){
			printf("test %d passed\n",i);
			passed++;
		}
		else if (test.expected==-2){
			printf("test %d has multiple occurrencies. result= %d \n",i,result); 
			passed++;
		}
		else{
			printf("test %d not passed \n",i);
		}
	}
	printf("tests passed %d / %d\n",passed,len);	
	return 0;
}
