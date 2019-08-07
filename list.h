#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>

struct list_head {
  struct list_head *next, *prev;
};

#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)
#define LIST_HEAD_INIT(name) \
  { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list) {
  list->next = list;
  list->prev = list;
}

// container_of - cast a member of a structure out to the containing structure
// @ptr:        the pointer to the member.
// @type:       the type of the container struct this is embedded in.
// @member:     the name of the member within the struct.
#define container_of(ptr, type, member)                \
  ({                                                   \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); \
  })

// list_entry - get the struct for this entry
// @ptr:    the &struct list_head pointer.
// @type:   the type of the struct this is embedded in. * @member: the name of
// the list_head within the struct.
#define list_entry(ptr, type, member) \
  container_of(ptr, type, member)

// list_first_entry - get the first element from a list
// @ptr:    the list head to take the element from.
// @type:   the type of the struct this is embedded in.
// @member: the name of the list_head within the struct.
//
// Note, that list is expected to be not empty.
#define list_first_entry(ptr, type, member) \
  list_entry((ptr)->next, type, member)

// list_last_entry - get the last element from a list
// @ptr:    the list head to take the element from.
// @type:   the type of the struct this is embedded in.
// @member: the name of the list_head within the struct.
//
// Note, that list is expected to be not empty.
#define list_last_entry(ptr, type, member) \
  list_entry((ptr)->prev, type, member)

// list_next_entry - get the next element in list
// @pos:    the type * to cursor
// @member: the name of the list_head within the struct.
#define list_next_entry(pos, member) \
  list_entry((pos)->member.next, typeof(*(pos)), member)

// list_prev_entry - get the prev element in list
// @pos:    the type * to cursor
// @member: the name of the list_head within the struct.
#define list_prev_entry(pos, member) \
  list_entry((pos)->member.prev, typeof(*(pos)), member)

// list_for_each_entry - iterate over list of given type
// @pos:    the type * to use as a loop cursor.
// @head:   the head for your list.
// @member: the name of the list_head within the struct.
#define list_for_each_entry(pos, head, member)             \
  for (pos = list_first_entry(head, typeof(*pos), member); \
       &pos->member != (head); pos = list_next_entry(pos, member))

// list_for_each_entry_reverse - iterate backwards over list of given type.
// @pos:    the type * to use as a loop cursor.
// @head:   the head for your list.
// @member: the name of the list_head within the struct.
#define list_for_each_entry_reverse(pos, head, member)    \
  for (pos = list_last_entry(head, typeof(*pos), member); \
       &pos->member != (head); pos = list_prev_entry(pos, member))

// list_for_each_entry_safe - iterate over list of given type safe against
// removal of list entry
// @pos:    the type * to use as a loop cursor.
// @n:      another type * to use as temporary storage
// @head:   the head for your list.
// @member: the name of the list_head within the struct.
#define list_for_each_entry_safe(pos, n, head, member)     \
  for (pos = list_first_entry(head, typeof(*pos), member), \
      n = list_next_entry(pos, member);                    \
      &pos->member != (head); pos = n, n = list_next_entry(n, member))

// list_for_each_entry_safe_reverse - iterate backwards over list safe against
// removal
// @pos:    the type * to use as a loop cursor.
// @n:      another type * to use as temporary storage
// @head:   the head for your list.
// @member: the name of the list_head within the struct.
//
// Iterate backwards over list of given type, safe against removal
// of list entry.
#define list_for_each_entry_safe_reverse(pos, n, head, member) \
  for (pos = list_last_entry(head, typeof(*pos), member),      \
      n = list_prev_entry(pos, member);                        \
      &pos->member != (head); pos = n, n = list_prev_entry(n, member))

// Insert a new entry between two known consecutive entries.
//
// This is only for internal list manipulation where we know
// the prev/next entries already!
static inline void __list_add(struct list_head *new_node,
                              struct list_head *prev,
                              struct list_head *next) {
  next->prev = new_node;
  new_node->next = next;
  new_node->prev = prev;
  prev->next = new_node;
}

// list_add - add a new entry
// @new: new entry to be added
// @head: list head to add it after
//
// Insert a new entry after the specified head.
// This is good for implementing stacks.
static inline void list_add(struct list_head *new_node,
                            struct list_head *head) {
  __list_add(new_node, head, head->next);
}

// list_add_tail - add a new entry
// @new: new entry to be added
// @head: list head to add it before
//
// Insert a new entry before the specified head.
// This is useful for implementing queues.
static inline void list_add_tail(struct list_head *new_node,
                                 struct list_head *head) {
  __list_add(new_node, head->prev, head);
}

// Delete a list entry by making the prev/next entries
// point to each other.
//
// This is only for internal list manipulation where we know
// the prev/next entries already!
static inline void __list_del(struct list_head *prev,
                              struct list_head *next) {
  next->prev = prev;
  prev->next = next;
}

static inline void list_del(struct list_head *entry) {
  __list_del(entry->prev, entry->next);
  entry->next = NULL;
  entry->prev = NULL;
}

// list_del_init - deletes entry from list and reinitialize it.
// @entry: the element to delete from the list.
static inline void list_del_init(struct list_head *entry) {
  __list_del(entry->prev, entry->next);
  INIT_LIST_HEAD(entry);
}

// list_move - delete from one list and add as another's head
// @list: the entry to move
// @head: the head that will precede our entry
static inline void list_move(struct list_head *list, struct list_head *head) {
  __list_del(list->prev, list->next);
  list_add(list, head);
}

// list_move_tail - delete from one list and add as another's tail
// @list: the entry to move
// @head: the head that will follow our entry
static inline void list_move_tail(struct list_head *list,
                                  struct list_head *head) {
  __list_del(list->prev, list->next);
  list_add_tail(list, head);
}

// list_empty - tests whether a list is empty
// @head: the list to test.
static inline int list_empty(const struct list_head *head) {
  return head->next == head;
}

static inline void __list_splice(const struct list_head *list,
                                 struct list_head *prev,
                                 struct list_head *next) {
  struct list_head *first = list->next;
  struct list_head *last = list->prev;

  first->prev = prev;
  prev->next = first;

  last->next = next;
  next->prev = last;
}

// list_splice - join two lists, this is designed for stacks
// @list: the new list to add.
// @head: the place to add it in the first list.
static inline void list_splice(const struct list_head *list,
                               struct list_head *head) {
  if (!list_empty(list)) __list_splice(list, head, head->next);
}

// list_splice_tail - join two lists, each list being a queue
// @list: the new list to add.
// @head: the place to add it in the first list.
static inline void list_splice_tail(struct list_head *list,
                                    struct list_head *head) {
  if (!list_empty(list)) __list_splice(list, head->prev, head);
}

// list_splice_init - join two lists and reinitialise the emptied list.
// @list: the new list to add.
// @head: the place to add it in the first list.
//
// The list at @list is reinitialised
static inline void list_splice_init(struct list_head *list,
                                    struct list_head *head) {
  if (!list_empty(list)) {
    __list_splice(list, head, head->next);
    INIT_LIST_HEAD(list);
  }
}

// list_splice_tail_init - join two lists and reinitialise the emptied list
// @list: the new list to add.
// @head: the place to add it in the first list.
//
// Each of the lists is a queue.
// The list at @list is reinitialised
static inline void list_splice_tail_init(struct list_head *list,
                                         struct list_head *head) {
  if (!list_empty(list)) {
    __list_splice(list, head->prev, head);
    INIT_LIST_HEAD(list);
  }
}

#endif /* _LIST_H */
