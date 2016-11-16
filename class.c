#include <stdlib.h>
#include <stdio.h>

typedef struct node {
	int data;
	struct node *next;
} NODE;

typedef struct {
	//you can ilitailize things within a structure
	NODE *head;
	NODE *tail;
} LIST;

void traverse(NODE *head);
NODE *addHead(NODE **ppcur, int newData);

int main() {
	LIST mylist = {NULL, NULL};
	addHead(&head, 27);
	addHead(&head, 43);
	addHead(&head, 67);
	//NODE *a = malloc(sizeof(NODE));
	//NODE *b = malloc(sizeof(NODE));
	//NODE *c = malloc(sizeof(NODE));
	//a -> data = 67;
	//b -> data = 43;
	//c -> data = 27;
	//a -> next = b;
	//b -> next = c;
	//head = a;
	//a = NULL; //still works because head still points to the same list, the list is still there in the heap
	//b = NULL;
	//c = NULL;
	traverse(head);
}

void traverse(NODE *head) {
	while (head != NULL) {
		printf("Node address %p", head);
		printf("Data %d\n", head->data);
		printf("Next address %p\n", head->next);
		head = head->next;
	}
}

void addHead(LIST *list, int newData) {
	NODE *temp = malloc(sizeof(NODE));
	if (temp == NULL) {
		printf("Yikes at line %d in %s!\n", __LINE__, __FILE__;
			exit(99);
	}
	temp->data = newData;
	if (list->head == NULL) {
		list->tail = temp;
	}
	temp->next = list->head;
	*ppcur = temp;
}

int deleteHead(NODE **ppcur) {
	if (*ppcur == NULL) {
		printf("Deleting from empty list...\n");
		exit(98);
	}
	int retval = (*ppcur) -> data;
	NODE *temp = *ppcur;
	*ppcur = (*ppcur)->next;
	free(temp);
}