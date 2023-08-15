#include <stdio.h>
#include <stdlib.h>









struct Node 
{
    int data;
    struct Node* next;
};
struct Node* newNode(int data,struct Node* nextNode)
{
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = data;
    temp->next = nextNode;
    
    return temp;
}




//init test
void hello()
{
    printf("Hello world!\n");
}


int scm_fibonacci (int num2) {


    if (num2 == 0 || num2 ==1) {
        return num2;
    }
    return (scm_fibonacci(num2-1) + scm_fibonacci(num2-2));
}



int scm_factorial(int num){
    switch (num) {
    case 1:
        return 1;
        break;

    default:
        return scm_factorial(num-1)*num;

    }
}

//working with strings


const char* myName(){
    return "This a test with long test";
}


//working wuth structs

struct Pearson {
  int age;
  int amount;
} ;



struct Pearson struct_getter() {
    struct Pearson e1 = {100, 200};
    return (e1);
}


struct CharPearson {

    char* username;
    char*  email;

};

struct CharPearson details_getters(){
    struct CharPearson e2;
    e2.username = "alexanderkbua";
    e2.email = "alexanderkabua@gmail.com";
    return (e2);
}

//implementing struct and ffi c 


/*

int main()
{
    int values[] = {100,200,300,400,500};
    int n = sizeof(values)/sizeof(values[0]);
    struct Node* head = NULL;
    struct Node* ptr;
    
    for(int i=n-1;i>=0;i--)
    {
        ptr    = newNode(values[i],ptr);
        head = ptr;
    }
    
    //printing LinkedList
    while(ptr->next != NULL)
    {
        printf("%d ",ptr->data);
        ptr = ptr->next;
    }
}
*/