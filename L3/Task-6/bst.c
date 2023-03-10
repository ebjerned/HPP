#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tree_node
{
   int              ID;
   char             *name;
   struct tree_node *left;
   struct tree_node *right;
} node_t;


void print_bst(node_t *node)
{
   if(node == NULL) {printf("Tree is empty!\n"); return;}

   if (node != NULL) printf("%d %s: \t", node->ID, node->name);
   if (node->left != NULL) printf("L%d,", node->left->ID);
   if (node->right != NULL) printf("R%d", node->right->ID);

   printf("\n");



   if (node->left != NULL)
      print_bst(node->left);
   if (node->right != NULL)
      print_bst(node->right);
}


void delete_tree(node_t **node)
{
	

	if((*node) == NULL) return;
	
	delete_tree(&((*node)->left));
	delete_tree(&((*node)->right));
	
	printf("Deleting %s\n", (*node)->name);
	// Deleteing nodes by freeing memory
	free((*node)->name);
	free(*node);
	//Setting final root pointer to exist
	*node = NULL;
	
	//printf("ERROR: Function delete_tree is not implemented\n");*/
}

void insert(node_t **node, int ID, char *name)
{
  if (*node==NULL){
//	printf("Creating node... \n");
	char* n = strdup(name);
	
	node_t *new_node;
	new_node = malloc(sizeof(node_t));


	new_node->ID = ID;
	new_node->name = n;
	new_node->left = NULL;
	new_node->right = NULL;
	*node = new_node;
  } else if (ID > (*node)->ID)
	insert(&(*node)->right, ID, name);
    else if (ID < (*node)->ID)
	insert(&(*node)->left, ID, name);
  	//printf("ERROR: Function insert is not implemented\n");
}


void search(node_t *node, int ID)
{
	if(node == NULL)  return(void) printf("Plant with ID %i does not exist!\n", ID);
	else if (node->ID == ID) return	(void) printf("Plant with ID %i has name %s\n", node->ID, node->name);
	else if(node->ID > ID) return search(node->left, ID);
	else if(node->ID < ID) return search(node->right,ID);
}


int main(int argc, char const *argv[])
{
   node_t *root = NULL;  // empty tree
   printf("Inserting nodes to the binary tree.\n");

   insert(&root, 445, "sequoia");
   

   insert(&root, 162, "fir");
   insert(&root, 612, "baobab");
   insert(&root, 845, "spruce");
   insert(&root, 862, "rose");
   insert(&root, 168, "banana");
   insert(&root, 225, "orchid");
   insert(&root, 582, "chamomile"); 

   printf("Printing nodes of the tree.\n");
   print_bst(root);


   search(root, 168);
   search(root, 467);

   printf("Deleting tree.\n");
   delete_tree(&root);

   print_bst(root);
   return 0;
}
