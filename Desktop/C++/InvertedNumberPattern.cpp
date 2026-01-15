#include<iostream>
using namespace std;


int main(){

       /*  Read input as specified in the question.
	* Print output as specified in the question.
	*/
  	int N;
    cin>> N;
    int i=1;
    while(i<=N){
       int  j = 1;
        while (j<=N-i+1){
            cout<<N-i+1;
            j++;
        }
        cout <<endl;i++;
    }
    return 0;
    
}