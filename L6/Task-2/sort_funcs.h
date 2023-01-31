typedef char intType;
// Smaller types gives faster performance, memory is closer to the core with smaller types, cache and so on.
// There is no big advantage in using other types for the other variables.
// The type should be big enough to contain all the data, ie N should not 
// be a char since the size is often quite large than 128 or 256 if unsigned.
// Using types as long long for N is not efficient since that amount of capacity
// is not needed.

void bubble_sort(intType* list, int N);
void merge_sort (intType* list, int N);
