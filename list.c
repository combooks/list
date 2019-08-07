#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct list_node {
  int value;
  struct list_head list;
};

static LIST_HEAD(list_header);

static void print(void) {
  struct list_node *node;
  if (!list_empty(&list_header)) {
    list_for_each_entry(node, &list_header, list) {
      printf("%d -> ", node->value);
    }
  }
  printf("NULL\n");
}

static void insert(int val) {
  struct list_node *node;
  node = (struct list_node *)malloc(sizeof(struct list_node));
  node->value = val;
  list_add(&node->list, &list_header);
}

static void del(int val) {
  struct list_node *node, *tmp;
  if (!list_empty(&list_header)) {
    list_for_each_entry_safe(node, tmp, &list_header, list) {
      if (node->value == val) list_del(&node->list);
    }
  }
}

int main(void) {
  int i;

  for (i=0; i<10; i++)
    insert(i);

  print();

  del(3);
  del(6);
  del(9);

  print();
  return 0;
}
