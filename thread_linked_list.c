#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>/* for thread functions */
#include <unistd.h>
#include <string.h>

typedef enum Bool
{
	true=1, false=0
}bool;

typedef struct Node
{
  	void* data;
  	struct Node* next;
}node;

typedef struct linkedlist
{
  	node* head;
  	int count;
  	pthread_mutex_t lock;
  	int (*compareFn)(void*, void*);
  	void (*printFn)(void*);
}linked;

linked* createlist(int (*compare)(void*, void*), void (*print)(void*));
void push(linked*, void*);
void print(linked*);
bool presentinlist(linked*, void*);
bool delnode(linked*, void*);

linked *list;

linked* createlist(int (*compare)(void*, void*), void (*print)(void*))
{
  	linked* list = (linked*)calloc(1, sizeof(linked));
  	list->head = NULL;
 	list->count = 0;
  	pthread_mutex_init(&(list->lock), NULL);
  	list->compareFn = compare;
  	list->printFn = print;
  	return list;
}

void push(linked* list, void* data)
{
  	pthread_mutex_lock(&(list->lock));
  	node* new = (node*)calloc(1, sizeof(node));
  	new->data = calloc(sizeof(data), 1);
  	memcpy(new->data, data, sizeof(data));
  	new->next = NULL;
  	if(list->count==0)
  	{
    		list->head = new;
    		++list->count;
    		pthread_mutex_unlock(&(list->lock));
    		return;
  	}
  	node* curr = list->head;
  	
    while(curr->next!=NULL)
    		curr=curr->next;

  	curr->next = new;
  	++list->count;
  	pthread_mutex_unlock(&(list->lock));
}

void print(linked* list)
{
  	pthread_mutex_lock(&(list->lock));
  	node* curr = list->head;
  	while(curr!=NULL)
  	{
    		list->printFn(curr->data);
    		printf("\n");
    	curr = curr->next;
  	}
  	pthread_mutex_unlock(&(list->lock));
}

bool presentinlist(linked* list, void* data)
{
  	pthread_mutex_lock(&(list->lock));
  	node* ptr = list->head;
	while(ptr!=NULL)
	{
		if((list->compareFn(ptr->data, data))==0)
		{
      			pthread_mutex_unlock(&(list->lock));
			return true;
		}
		else
		{
			ptr = ptr->next;
		}
	}
  	pthread_mutex_unlock(&(list->lock));
	return false;
}

bool delnode(linked* list, void* data)
{
  	pthread_mutex_lock(&(list->lock));
  	if(list->head==NULL)
  	{
    		pthread_mutex_unlock(&(list->lock));
    		return false;
  	}
  	node *curr = list->head, *prev = NULL, *next = NULL;
  	
	while(curr!=NULL)
	{
		if(list->compareFn(curr->data, data)==0)
		{
			next = curr->next;
			--list->count;
			break;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	if(curr==NULL)
	{
    		pthread_mutex_unlock(&(list->lock));
		return false;
	}
	else if(curr!=NULL && prev==NULL)
	{
		list->head = list->head->next;
	}
	else
	{
		prev->next = next;
	}
  	pthread_mutex_unlock(&(list->lock));
	return true;
}

void printint(void* a)
{
  	printf("%d", *((int*)a));
}

int compareint(void* a, void* b)
{
  	int* x = (int*)a;
  	int* y = (int*)b;
  	if(*x>*y)
    		return 1;
  	if(*x==*y)
    		return 0;
  	return -1;
}

void printfloat(void* a)
{
  	printf("%.2f", *((float*)a));
}

int comparefloat(void* a, void* b)
{
  	float* x = (float*)a;
  	float* y = (float*)b;
  	if(*x>*y)
    		return 1;
  	if(*x==*y)
    		return 0;
  	return -1;
}

void* myThread(void* p)
{
  	printf("%d\n", *((int*)p));
  	if(presentinlist(list, p))
  	{
    		delnode(list, p);
    		printf(" is Present\n");
  	}
  	return NULL;
}

int main()
{
  	pthread_t tid[2];
  	
  	list = createlist(compareint, printint);
  	
  	int i, x=5, y=8;
  	for(i=1; i<15; i++)
    		push(list, (void*)&i);
    		
    	printf("list has 15 elements\n");
    
  	pthread_create(&tid[0], NULL, myThread, (void*)&x);
  	pthread_create(&tid[1], NULL, myThread, (void*)&y);
  	
  	pthread_join(tid[0], NULL);
  	pthread_join(tid[1], NULL);
  	
  	printf("\nThese aren't passed \n");
 
  	print(list);

	return 0;
}
