#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
 
struct list_node_s
{
    int data;
    struct list_node_s* next;
};
 
 
int n;
int m;
float mMember;
float mInsert;
float mDelete;
 
 int member( int value, struct  list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;
     
    while( curr_p != NULL && curr_p->data < value )
    {
        curr_p = curr_p->next;
    }
 
    if(curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
 
int insert(int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;          
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p = NULL;
 
    while(curr_p !=NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
     
    if(curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));        
        temp_p->data = value;
        temp_p->next = curr_p;
         
        if(pred_p == NULL)
        {
            *head_pp = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }
        return 1;
  
    }
    else
    {
        return 0;
    }
}   
 
 
int delete (int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
     
    while(curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }   
     
    if(curr_p != NULL && curr_p -> data < value)
    {
        if(pred_p == NULL){
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
         
    }
    else
    {
        return 0;
    }
 
}   
 

void Get_args(int argc, char* argv[]) {
    if (argc != 6)
    {
        Usage(argv[0]);
    }
    n = (int) strtol(argv[1], (char **)NULL, 10);
    m = (int) strtol(argv[2], (char **)NULL, 10);
     
    mMember = (float) atof(argv[3]);
    mInsert = (float) atof(argv[4]);
    mDelete = (float) atof(argv[5]);
     
   if (n <= 0 || m <= 0 || mMember + mInsert + mDelete!=1.0) Usage(argv[0]);
    
}  
 
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <n> <m> <member> <insert> <delete>\n", prog_name);
   fprintf(stderr,"n es la cantidad de elementos inicial en la lista.\n");
   fprintf(stderr,"m es la cantidad de operaciones a realizar entre los tres tipos.\n");
   fprintf(stderr,"member es la fraccion de operaciones member.\n");
   fprintf(stderr,"insert es la fraccion de operaciones insert.\n");
   fprintf(stderr,"delete es la fraccion de operaciones delete.\n");
              
   exit(0);
} 
int printList( struct  list_node_s* head_p ) 
{
    struct list_node_s* curr_p = head_p;
     
    while(curr_p != NULL)
    {
        printf("%d ",curr_p->data);
        curr_p = curr_p->next;
    }
    printf("\n");
}   


int main(int argc, char* argv[])
{
    struct list_node_s* head = NULL;    
    int i=0;
    double start, finish, elapsed;
     
    Get_args(argc, argv); 
      
    for(;i<n;i++)
    {   
        int r = rand()%65536;
        if(!insert(r,&head))
        {
            i--;
        }
    }
     
    int count_member=0;
    int count_insert=0;
    int count_delete=0;
         
    start = clock();    
    for(i=0;i<m;i++)
    {
     
        float prob = (rand()%10000/10000.0);
         
         
        int r = rand()%65536;
        if(prob<mMember)
        {
            member(r,head);
            count_member++;
        }
        else if(prob < mMember + mInsert )
        {
            insert(r,&head);
            count_insert++;
        }
        else
        {           
            delete(r,&head);
            count_delete++;
        }
         
    }
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;
     
    printf("Tiempo(lista enlazada serial) = %.10f seconds\n", elapsed);

    return 0;
} 
 
