#include <stdio.h>
#include <stdlib.h>


typedef struct temprange {
	int* next_node;
	int index;
	float min, max;
} temprange_node;

int add_to_database(int index, float min, float max, temprange_node** root);
int remove_from_database(int index, temprange_node** root);
int remove_database(temprange_node** root);
void print_database(temprange_node* db);



int main(){
	char a;
	int index;
	float min, max;
	int running = 1;
	temprange_node* root = NULL;


	while(running){
		printf("Enter command: ");
		scanf(" %c",&a);
		switch(a){
			case 'A':
				scanf(" %i %f %f",&index, &min, &max); 
				printf("Index: %i%f%f \n", index,min,max);
				add_to_database(index, min, max, &root);
				printf("%p\n", root);
				break;
			case 'D':
				scanf(" %i", &index);
				printf("Deleted: %i \n", index);
				break;
			case 'P':
				print_database(root);
				break;
			case 'Q':
				running = 0;
				break;
			default:
				continue;
		}
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
	if(*root == NULL){
		*root = new_entry;
		printf("Created rootnode\n");
		printf("%p\n", root);
		return 1;
	}
	temprange_node* curr = *root;
	while(curr != NULL){
		printf("Looping");
		if (curr->index > index){
			new_entry->next_node = (int*)(*root);
			*root = new_entry;
			return 1;
		}
		temprange_node* next = (temprange_node*)(curr->next_node);
		if(next != NULL){
			if(next->index > index && curr->index < index){
				new_entry->next_node = curr->next_node;
				curr->next_node = (int*)new_entry;
				return 1;
			} 
			if(next->index == index){
				new_entry->next_node = next->next_node;
				curr->next_node = (int*)new_entry;
				free(next);
				return 1;	
			}

		} else {
			curr->next_node = (int*)new_entry;
			return 1;
		}
//		printf("%p", curr);
		curr = next;
//		printf("%p", curr);
//		break;

	}

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

}
