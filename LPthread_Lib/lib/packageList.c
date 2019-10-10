
#include <malloc.h>

typedef struct paquete {
	int id;
	int tipo;				//0 normal, 1 urgente, 2 radioactivo
	int masa;				//Se definio que es un numero entre 1 y 10 kg
	int lado;  				//0 derecha, 1 izquierda
	int estado;				//0 si aun no ha pasado al lado opuesto, 1 si ya el paquete esta listo
    int pos;
} paquete;

typedef struct node {
   paquete data;
   struct node *next;
} node;
paquete getPaqueteNode(struct node *temp){
   return temp->data;
}
//display the list
void printList(struct node *head) {
   struct node *ptr = head;
   printf("\n[ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%d,%d) ",ptr->data.id,ptr->data.tipo);
      ptr = ptr->next;
   }
	
   printf(" ]");
}

//insert link at the first location
void insertFirst(int id, paquete data, struct node *head) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->data.id = id;
   link->data = data;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

//delete first item
struct node* deleteFirst(struct node *head) {

   //save reference to first link
   struct node *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	
   //return the deleted link
   return tempLink;
}

//is list empty
int isEmpty(struct node *head) {
   return head == NULL;
}

int length(struct node *head) {
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//find a link with given key
struct node* find(int id, struct node *head) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->data.id != id) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//delete a link with given key
struct node* deleteN(int id, struct node *head) {

   //start from the first link
   struct node* current = head;
   struct node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->data.id != id) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}


struct node* getAt(int pos, struct node *head){
   //start from the first link
   struct node* current = head;
   struct node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   for(int i = 0; i<= pos; i++){

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }
   return current;

}