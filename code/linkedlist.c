#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>


Node* InsertAtHead(Node* head, char x){
	Node* tmp = (Node*)malloc(sizeof(Node));
	tmp -> data = x;
	tmp -> ptr = NULL;
	if(head != NULL) tmp -> ptr = head;
	head = tmp;
	
	return head;
}

Node* Reverse(Node* head){
	Node *prev, *cur, *next;
    prev = NULL;
    cur = head;
    while(cur != NULL){
    	next = cur -> ptr;
        cur -> ptr = prev;
        prev = cur;
        cur = next;
    }
    head = prev;
    return head;
}


Node* InsertNode(Node* head, int n, char x){
	Node* new = (Node*)malloc(sizeof(char));
	new -> data = x;
	new -> ptr = NULL;

	if(head == NULL){//create new node
		head = new;
	}else if(n == 0){ //insert at head
		Node* tmp = head;
		head = new;
		new -> ptr = tmp;
	}else{
		Node* tmp = head;
		for(int i = 0; i < n-1; i++) tmp = tmp -> ptr;
		Node* tmp1 = tmp -> ptr; 
		tmp -> ptr  = new;
		new -> ptr = tmp1;
	}
	return head;
}

Node* Delete(Node* head, int n){
	if(head == NULL) return head;

	Node* tmp = head;
	if(n == 1){
		head = tmp -> ptr;
		free(tmp);
		return head;
	}
	for(int i = 0; i < n-2; i++) tmp = tmp -> ptr;
	Node* tmp1 = tmp -> ptr;
	tmp -> ptr = tmp1 -> ptr;
	free(tmp1);

	return head;
}

void DeleteAll(Node* head){
	Node* tmp = head;
	while(tmp != NULL){
		head = tmp -> ptr;
		free(tmp);
		tmp = head;
	}
}