struct Node_{ 
	char data;
    struct Node_* ptr;
};
typedef struct Node_ Node;

Node* InsertAtHead(Node* head, char x);
Node* Reverse(Node* head);

Node* InsertAtTail(Node* head, char x);
Node* Delete(Node* head, int n);
void DeleteAll(Node* p);
Node* InsertNode(Node* head, int n, char x);
