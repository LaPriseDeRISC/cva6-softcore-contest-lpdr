#include <stdlib.h>

typedef struct bst_node_s
{
	__ptr_t address;
	size_t size;
	struct bst_node_s *left;
	struct bst_node_s *right;
} bst_node_t;

typedef bst_node_t mds_entry;
// typedef mds_entry *mds_entry_ptr;

void mds_insert(mds_entry *entry);
mds_entry *mds_register(__ptr_t address, size_t size);

void mds_remove(mds_entry *entry);

int mds_check_all();
int mds_check(mds_entry *entry);
void mds_unregister(__ptr_t address);
void mds_dump();
mds_entry *mds_get(__ptr_t address);
