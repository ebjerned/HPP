#include <stdio.h>
#include <stdlib.h>


typedef struct temprange {
	struct temprange* next_node;
	int index;
	float min, max;
} temprange_node;

int add_to_database(int index, float min, float max, temprange_node** root);
int remove_from_database(int index, temprange_node** root);
int remove_database(temprange_node** root);
void print_database(temprange_node* db);



int main(){
	char comm;
	int index;
	float min, max;
	int running = 1;
	temprange_node* root = NULL;

	// Main loop
	while(running){
		printf("Enter command: ");
		scanf(" %c",&comm);
		// Calls correct function according to given command
		switch(comm){
			case 'A':
				scanf(" %i %f %f", &index, &min, &max); 
				add_to_database(index, min, max, &root);
				break;
			case 'D':
				scanf(" %i", &index);
				remove_from_database(index, &root);
				break;
			case 'P':
				print_database(root);
				break;
			case 'Q':
				running = 0;
				break;
			default:
				printf("Command not defined\n");
		}

	}
	remove_database(&root);
	return 0;

}

int add_to_database(int index, float min, float max, temprange_node** root){
	// Inserts given min and max at index into database.
	// Returns 1 if successfully inserted into databas, else 0

	// Limiting the possible indicies
	if (index > 31 || index < 1) return 0;

	// Allocating and assigning values to the new node
	temprange_node* new_entry = (temprange_node*)malloc(sizeof(temprange_node));
	new_entry->index = index;
	new_entry->min = min;
	new_entry->max = max;
	new_entry->next_node = NULL;

	// Find where the new node should be inserted

	// Base case
	if(*root == NULL){
		*root = new_entry;
		return 1;
	}
	temprange_node* curr = *root;

	// Smaller than root
	if(curr->index > index){
		new_entry->next_node = *root;
		*root = new_entry;
		return 1;
	}

	// Equal to root
	if(curr->index == index){
		if(curr->next_node != NULL) new_entry->next_node = curr->next_node;
		free(curr);
		*root = new_entry;
		return 1;
	}

	// Not related to root, loop until to see where new should be inserted
	while(curr->next_node != NULL && curr->next_node->index < index){
		curr = curr->next_node;
	}

	// Appending entered node
	if(curr->next_node == NULL){
		curr->next_node = new_entry;
		return 1;
	}else if (curr->next_node->index > index){
		// New node should be entered inbetween two existing nodes
		new_entry->next_node = curr->next_node;
		curr->next_node = new_entry;
		return 1;
	}else if (curr->next_node->index == index){
		// New node should replace an existing node
		new_entry->next_node = curr->next_node->next_node;
		free(curr->next_node);
		curr->next_node = new_entry;
		return 1;
	}

	return 0;
}

void print_database(temprange_node* root){ 
	// Prints database with origin in root
	if(root == NULL) return;
	printf("day\tmin\t\tmax\n");

	// Loops through database and prints each nodes sequentally
	temprange_node* curr = root;
	while(curr != NULL){
		printf("%i\t%f\t%f\n", curr->index, curr->min, curr->max);
		curr = (temprange_node*)(curr->next_node);
	}
	return;
}

int remove_from_database(int index, temprange_node** root){
	// Returns 1 if index is succesfully removed else 0
	if (*root == NULL) return 0;

	temprange_node* curr = *root;
	// Checks if root is affected by removal
	if ((*root)->index == index){
		// Root is only node
		if((*root)->next_node == NULL) {
			free(*root);
			*root = NULL;
			return 1;
		} else  {
			// Reassigning root node to next node
			temprange_node* next = (*root)->next_node;
			free(*root);
			*root = next;
			return 1;
		}
	}

	// Loops through database, breaks at given index found or next node does not exist
	while(curr->next_node != NULL && curr->next_node->index != index){
		curr = curr->next_node;
	}

	// Index does not exist in database
	if(curr->next_node == NULL) return 0;

	// Index does exist
	if(curr->next_node->index == index){
		// Found node is not last node, reconnect the nodes that becomes disconnected because of removal
		if(curr->next_node->next_node != NULL){
			temprange_node* next = curr->next_node->next_node;
			free(curr->next_node);
			curr->next_node = next;
			return 1;
		}else {
			// Found node is last node
			free(curr->next_node);
			curr->next_node = NULL;
			return 1;
		}
	}
	return 0;
}

int remove_database(temprange_node** root){
	// Returns 1 if database is removed succesfully else 0

	// Loops through database and removes elements sequentially
	temprange_node* curr = *root;
	while(curr != NULL){
		if(curr->next_node == NULL){
			free(curr);
			return 1;
		}
		temprange_node* next = (temprange_node*)(curr->next_node);
		free(curr);
		curr = next;
	}
	return 0;
}

