#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum { False, True } Boolean;
int  i = 1;

typedef struct Stack {
	struct StackNode *head;
	int capacity;
}Stack, * Pstack;

typedef struct StackNode {
	struct Node *node;
	struct StackNode *next;
}StackNode, *PstackNode;

typedef struct Node {
	int index;
	struct Edge** conected;
	struct node* whereComeFrom;
	Boolean visited;
	int distanceFromSource;
	int numOfConnection;
}Node, *Pnode;

typedef struct Egde {
	Pnode dest;
	Pnode source;
	int weight;
}Edge, * Pedge;

typedef struct Graph {
	Pnode* nodes;
	int numOfNodes;
}Graph, *Pgraph;

//Graph
void insertIntoGraph(Pgraph graph, Pnode node);
void Dijkstra(Pgraph graph, Pnode startNode,Pnode endNode);
void createEdge(Pnode *src, Pnode *dest, int weight);
void createConnection(Pnode *node, Pedge *edge);
Pnode createAndInitNode();

//Stack
Boolean isEmpty(Pstack stck);
Pnode pop(Pstack stck);
void insertToStack(Pstack stck, Pnode node);

int main() {
	Graph  graph2;
	Pnode A, B, C, D, E, F, G;

	graph2.nodes = NULL;
	graph2.numOfNodes = 0;

	// create the node of the graph
	A = createAndInitNode();
	B = createAndInitNode();
	C = createAndInitNode();
	D = createAndInitNode();
	E = createAndInitNode();
	F = createAndInitNode();
	G = createAndInitNode();
	

	//create edges between the nodes
	createEdge(&A, &B, 4);
	createEdge(&A, &E, 7);
	createEdge(&A, &C, 3);

	createEdge(&B, &C, 6);
	createEdge(&B, &D, 5);

	createEdge(&C, &D, 11);
	createEdge(&C, &E, 8);

	createEdge(&E, &D, 2);
	createEdge(&E, &G, 5);

	createEdge(&D, &G, 10);
	createEdge(&D, &F, 2);

	createEdge(&G, &F, 3);


	//insert the nodes into the graph
	insertIntoGraph(&graph2, A);
	insertIntoGraph(&graph2, B);
	insertIntoGraph(&graph2, C);
	insertIntoGraph(&graph2, D);
	insertIntoGraph(&graph2, E);
	insertIntoGraph(&graph2, F);
	insertIntoGraph(&graph2, G);

	//call Dijkstra
	//A -  the staring node | F - the end node
	Dijkstra(&graph2, A,F);
	printf("\n");
}

//insert nodes into the stack by priority of the distance
void insertToStack(Pstack stck, Pnode node) {
	PstackNode temp;
	PstackNode tmp = stck->head, curr;
	Boolean first = True;

	temp = (StackNode*)malloc(sizeof(StackNode));
	if (temp == NULL) {
		printf("allocation faild.\nExit program\n");
		exit(1);
	}
	temp->node = node;
	temp->next = NULL;

	if (tmp != NULL) {
		curr = tmp;
		while (tmp != NULL) {
			if (tmp->node->distanceFromSource < node->distanceFromSource)
				tmp = tmp->next;
			else
				break;

			if (first == True)
				first = False;
			else
				curr = curr->next;
		}
		if (curr == tmp) {
			temp->next = curr;
			stck->head = temp;
		}
		else {
			curr->next = temp;
			temp->next = tmp;
		}
	}
	else
		stck->head = temp;
	stck->capacity++;
}

//pull out the Node in the head of the stack and return it
Pnode pop(Pstack stck) {
	Pnode temp;
	PstackNode tmp3;

	tmp3 = stck->head;
	temp = tmp3->node;
	stck->head = stck->head->next;
	stck->capacity--;
	free(tmp3);
	return temp;
}

//check if the stack is empty
Boolean isEmpty(Pstack stck) {
	if (stck->capacity == 0)
		return True;
	return False;
}

// reorgenize the stack by the distance
void reOrgenizStack(Pstack stck) {
	Pnode tmpNode;
	Stack returnStack;
	returnStack.capacity = 0;
	returnStack.head = NULL;
	while (isEmpty(stck) != True) {
		tmpNode = pop(stck);
		insertToStack(&returnStack, tmpNode);
	}
	*stck = returnStack;
}


void Dijkstra(Pgraph graph, Pnode startNode, Pnode endNode) {
	Stack stck, visited;
	Pnode tmpNode;
	Pnode printNode;
	Pnode dest, src;
	Pedge tmpEdge;
	int i,j, weight;
	stck.capacity = 0;
	stck.head = NULL;
	startNode->distanceFromSource = 0;
	startNode->visited = True;

	visited.capacity = 0;
	visited.head = NULL;
	//insert into stack
	for (i = 0; i < graph->numOfNodes; i++)
		insertToStack(&stck, graph->nodes[i]);
	
	while (isEmpty(&stck) != True) {
		tmpNode = pop(&stck);
		for (j = 0; j < tmpNode->numOfConnection; j++) {
			tmpEdge = tmpNode->conected[j];
			weight = tmpEdge->weight + tmpNode->distanceFromSource;
			if (tmpNode == tmpEdge->dest) {
				dest = tmpEdge->source;
				src = tmpEdge->dest;
			}
			else {
				dest = tmpEdge->dest;
				src = tmpEdge->source;
			}
			if (dest->distanceFromSource > weight&& dest->visited == False) {
				dest->distanceFromSource = weight;
				dest->whereComeFrom = tmpNode;
			}
		}
		reOrgenizStack(&stck);
		tmpNode->visited = True;
		insertToStack(&visited, tmpNode);
	}
	while (endNode != startNode) {
		printf("%d-> ", endNode->index);
		endNode = endNode->whereComeFrom;
	} 
	printf("%d ", endNode->index);
}

//insert node into the graph
//implementation with array
void insertIntoGraph(Pgraph graph, Pnode node) {
	Pnode *temp=NULL;
	if (graph->nodes == NULL)
		graph->nodes = (Node**)malloc(sizeof(Node*));
	else {
		temp = (Node**)realloc(graph->nodes, (graph->numOfNodes + 1) * sizeof(Node*));
		if (temp != NULL)
			graph->nodes = temp;
	}
	if(graph->nodes !=NULL) 
		graph->nodes[ graph->numOfNodes++ ] = node;
}

//create new node and initialize it by Dijkstra algorithm
Pnode createAndInitNode() {
	Pnode node = (Node*)malloc(sizeof(Node));
	if (node != NULL) {
		// i represent the index of the node
		node->index = i++;
		node->distanceFromSource = INT_MAX;
		node->visited = False;
		node->numOfConnection = 0;
		node->conected = NULL;
		node->whereComeFrom = NULL;
		return node;
	}
	return NULL;
}

//create edge between two nodes and set the weight of the edge
void createEdge(Pnode *src, Pnode *dest, int weight) {
	Pedge edge = (Edge*)malloc(sizeof(Edge));
	if (edge != NULL) {
		edge->dest = *dest;
		edge->source = *src;
		edge->weight = weight;

		//update the connection to each node
		createConnection(src, &edge);
		createConnection(dest, &edge);
	}
}

// to each node set the edges that connecting to it
void createConnection(Pnode *node, Pedge *edge) {
	Pedge *temp = NULL;
	//if nothing connected to node
	if ((*node)->conected == NULL) 
		(*node)->conected = (Edge**)malloc(sizeof(Edge*));
	//resize the array of the edges to indicate which nodes connected to the current node 
	else {
		temp = (Edge**)realloc((*node)->conected, ((*node)->numOfConnection + 1) * sizeof(Edge*));
		if (temp != NULL) 
			(*node)->conected = temp;
	}
	
	//save the edge how connected to the node
	//edge consist two nodes (source and dest)
	if ((*node)->conected != NULL) 
		(*node)->conected[ (*node)->numOfConnection++ ] = *edge;
}