#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t head_mutex;
struct list_node_s
{
    int data;
    struct list_node_s* next;
    pthread_mutex_t mutex;
};
 
const int MAX_THREADS = 1024;
 
long thread_count;
struct list_node_s* head = NULL;    
 
int n;
int m;
float mMember;
float mInsert;
float mDelete;
int count_member=0;
int count_insert=0;
int count_delete=0;
 
void* thread_oparation(void* rank) 
{
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_m = m/thread_count;
    for( i=0; i < my_m; i++ )
    {
 
        float prob = (rand()%10000/10000.0);
  
     
     
        int r = rand()%65536;
        if(prob<mMember)
        {
            pthread_mutex_lock(&mutex);
            member(r,head);
            count_member++;
            pthread_mutex_unlock(&mutex);
        }
        else if(prob < mMember + mInsert )
        {
            pthread_mutex_lock(&mutex);
            insert(r,&head);
            count_insert++;
            pthread_mutex_unlock(&mutex);
        }
        else
        {           
            pthread_mutex_lock(&mutex);
            delete(r,&head);
            count_delete++;
            pthread_mutex_unlock(&mutex);
        }   
    }  
 
   return NULL;
} 
 
int  member(int value) {
   struct list_node_s* temp;

   pthread_mutex_lock(&head_mutex);
   temp = head;
   while (temp != NULL && temp->data < value) {
      if (temp->next != NULL) 
         pthread_mutex_lock(&(temp->next->mutex));
      if (temp == head)
         pthread_mutex_unlock(&head_mutex);
      pthread_mutex_unlock(&(temp->mutex));
      temp = temp->next;
   }

   if (temp == NULL || temp->data > value) {
#     ifdef DEBUG
      printf("%d no esta en la lista\n", value);
#     endif
      if (temp == head)
         pthread_mutex_unlock(&head_mutex);
      if (temp != NULL) 
         pthread_mutex_unlock(&(temp->mutex));
      return 0;
   } else {
#     ifdef DEBUG
      printf("%d esta en la lista\n", value);
#     endif
      if (temp == head)
         pthread_mutex_unlock(&head_mutex);
      pthread_mutex_unlock(&(temp->mutex));
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
    if (argc != 7)
    {
        Usage(argv[0]);
    }
    thread_count = strtol(argv[1], NULL, 10);  
    if (thread_count <= 0 || thread_count > MAX_THREADS)
    {
        Usage(argv[0]);
    }
    
    n = (int) strtol(argv[2], (char **)NULL, 10);
    m = (int) strtol(argv[3], (char **)NULL, 10);
     
    mMember = (float) atof(argv[4]);
    mInsert = (float) atof(argv[5]);
    mDelete = (float) atof(argv[6]);
     
   if (n <= 0 || m <= 0 || mMember + mInsert + mDelete!=1.0) Usage(argv[0]);
    
}  
 

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <N_hilos> <n> <m> <member> <insert> <delete>\n", prog_name);
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
    int i=0;
    long       thread;  
    pthread_t* thread_handles;
    double start, finish, elapsed,efficency;
     
    Get_args(argc, argv); 
      
    for(;i<n;i++)
    {   
        int r = rand()%65536;
        if(!insert(r,&head))
        {
            i--;
        }
    }
        
    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));  
     
    start = clock();
    pthread_mutex_init(&mutex, NULL);
     
    for (thread = 0; thread < thread_count; thread++)  
    {
        pthread_create(&thread_handles[thread], NULL,thread_oparation , (void*)thread);  
    }
     
    for (thread = 0; thread < thread_count; thread++) 
    {
        pthread_join(thread_handles[thread], NULL); 
    }
     
    pthread_mutex_destroy(&mutex);
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;
    efficency = (2.3437500000)/(elapsed*thread_count);
    printf("Hilos usados = %ld \n",thread_count);
    printf("Tiempo(Mutex per node) = %e segundos\n", elapsed);
    printf("Eficencia(Mutex per node) = %e \n", efficency);
     
    return 0;
}
 
  
