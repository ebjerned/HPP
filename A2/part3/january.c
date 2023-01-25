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


	while(running){
		printf("Enter command: ");
		scanf(" %c",&comm);
		switch(comm){
			case 'A':
				scanf(" %i %f %f", &index, &min, &max); 
				add_to_database(index, min, max, &root);
				break;
			case 'D':
				scanf(" %i", &index);
				printf("Deleted: %i \n", index);
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
		printf("\n");
	}
	remove_database(&root);
	return 0;

}

int add_to_database(int index, float min, float max, temprange_node** root){
	if (index > 31 || index < 1) return 0;
	temprange_node* new_entry = (temprange_node*)malloc(sizeof(temprange_node));

	new_entry->index = index;
	new_entry->min = min;
	new_entry->max = max;
	new_entry->next_node = NULL;

	if(*root == NULL){
		printf("Null root");
		*root = new_entry;
		return 1;
	}
	temprange_node* curr = *root;
	if(curr->index > index){
		printf("Smaller than root");
		new_entry->next_node = *root;
		*root = new_entry;
		return 1;
	}
	if(curr->index == index){
		if(curr->next_node != NULL) new_entry->next_node = curr->next_node;
		free(curr);
		*root = new_entry;
		return 1;
	}
	while(curr->next_node != NULL && curr->next_node->index < index){

		curr = curr->next_node;

	}

	if(curr->next_node == NULL){
		curr->next_node = new_entry;
		return 1;
	}else if (curr->next_node->index > index){
		new_entry->next_node = curr->next_node;
		curr->next_node = new_entry;
		return 1;
	}else if (curr->next_node->index == index){
		new_entry->next_node = curr->next_node->next_node;
		free(curr->next_node);
		curr->next_node = new_entry;
		return 1;
	}
	
	return 0;
}

void print_database(temprange_node* root){ 
	printf("%p\n", root);
	if(root == NULL){
		printf("Database is empty\n");
		return;
	}
	printf("day\tmin\tmax\n");

	temprange_node* curr = root;
	while(1){
		printf("%i\t%f%f\n", curr->index, curr->min, curr->max);
		if (curr->next_node == NULL) break;
		curr = (temprange_node*)(curr->next_node);
	}
	return;
}

int remove_from_database(int index, temprange_node** root){
	if (*root == NULL) return 0;

	temprange_node* curr = *root;
	if ((*root)->index == index){
		if((*root)->next_node == NULL) {
			free(*root);
			*root = NULL;
			return 1;
		} else  {
			temprange_node* next = (*root)->next_node;
			free(*root);
			*root = next;
			return 1;
		}
		return 1;
	}

	while(curr->next_node != NULL && curr->next_node->index != index){
		curr = curr->next_node;
	}
	if(curr->next_node == NULL) return 0;

	if(curr->next_node->index == index && curr->next_node != NULL){
		if(curr->next_node->next_node != NULL){
			temprange_node* next = curr->next_node->next_node;
			free(curr->next_node);
			curr->next_node = next;
		}else {
			free(curr->next_node);
			curr->next_node = NULL;

		}
	} else {
		return 0;
	}
	return 0;
}

int remove_database(temprange_node** root){
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
	return 1;
}
