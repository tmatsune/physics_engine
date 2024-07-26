#include "utils.h"


void array_init(dynamic_array *array){
  array->count = 0;
  array->max_count = 5;
  array->items = malloc(array->max_count * sizeof(void *));
  ASSERT(array->items == NULL, "FAILED TO ALLOCATE INIT MEMORY TO MEMEORY");
};

void array_append(dynamic_array *array, const void *item, size_t item_size){
  if(array->count >= array->max_count){
    array->max_count *= 2;
    array->items = realloc(array->items, array->max_count * item_size);
    //ASSERT(array->items == NULL, "ERROR REALLOC MEMORY, APPENDING");
    if (array->items == NULL) {
        perror("ERROR REALLOC MEMORY, APPENDING");
        exit(EXIT_FAILURE);
    }
}
  void *new_item = malloc(item_size);

  new_item = memcpy(new_item, item, item_size);
  array->items[array->count] = new_item;
  array->count++;
}

void *array_pop(dynamic_array *array){
  ASSERT(array->count == 0, "CAN POP FROM EMPTY ARRAY");
  void *item = array->items[array->count - 1];
  array->count--;
  return item;
}

void *array_get(dynamic_array *array, size_t index){
  ASSERT(index < 0 || (int)index >= array->max_count, "INDEX OUT OF RANGE");
  return array->items[index];
}

void array_free(dynamic_array *array){
  for(int i = 0; i < array->count; i++){
    free(array->items[i]);
  }
  free(array->items);
  array->count = 0;
  array->max_count = 0;
}
 



void print(char *str){ printf("%s\n", str); }
void print_int(int num){ printf("%d\n", num); }

void vec_print(const vec2 v){ printf("X: %f, Y: %f", v.x, v.y); }

