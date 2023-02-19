#include "malloc_structs/malloc_structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define N_ENTRIES 10000

#ifndef N_ENTRIES
#define N_ENTRIES MDS_TABLE_SIZE
#endif

#define N_TESTS N_ENTRIES

// #define GRAPHVIZ_DUMP
#define USE_BST
#define BST_PERF
#define AVL_PERF
#define NAIVE_PERF
// #define DEBUG
#ifdef BST_PERF
#define USE_BST
#endif

mds_entry address_tab[N_ENTRIES], naive_tab[N_ENTRIES], bst_tab[N_ENTRIES];

// #ifdef BST_PERF
// // custom register for separate bst and avl trees
// mds_entry *mds_register(__ptr_t address, size_t size)
// {
// 	mds_entry *entry = &bst_tab[bst_tab_index];
// #ifdef DEBUG
// 	fprintf(stderr,"Registering %ld of size %ld using index %d\n", (long int)address, size, _mds_table_index);
// #endif
// 	bst_tab_index++;
// 	entry->address = address;
// 	entry->size = size;
// 	mds_insert(entry);
// 	return entry;
// }
// #endif

// #ifdef AVL_PERF
// mds_entry *mds_avl_register(__ptr_t address, size_t size)
// {
// 	mds_entry *entry = &avl_tab[avl_tab_index];
// #ifdef DEBUG
// 	fprintf(stderr,"Registering %ld of size %ld using index %d\n", (long int)address, size, _mds_table_index);
// #endif
// 	avl_tab_index++;
// 	entry->address = address;
// 	entry->size = size;
// 	mds_avl_insert(entry);
// 	return entry;
// }
// #endif

// A function to generate a random permutation of arr[]
void randomize(mds_entry arr[], int n, int n_permutations)
{
	for (int j = 0; j < n_permutations; j++)
	{
		for (int i = n - 1; i > 0; i--)
		{
			int j = rand() % (i + 1);
			mds_entry temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}
}

#ifdef NAIVE_PERF
mds_entry *naive_tab_search(__ptr_t address)
{
	for (int i = 0; i < N_ENTRIES; i++)
	{
		if (naive_tab[i].address == address)
			return &naive_tab[i];
	}

	return NULL;
}
#endif

int main()
{
	srand(time(NULL));

	fprintf(stderr, "Sizes :\tint %ld, long %ld, long long %ld, void* %ld, size_t %ld\n",
			sizeof(int), sizeof(long), sizeof(long long), sizeof(void *), sizeof(size_t));
	fprintf(stderr, "\tmds_entry %ld (on %ldb int), mds_entry %ld (on %ldb int)\n",
			sizeof(mds_entry), sizeof(int) * 8, (sizeof(mds_entry)) / 2, (sizeof(int) * 8) / 2);

	for (int i = 0; i < N_ENTRIES; i++)
	{
		__ptr_t addr = (__ptr_t)((long)(rand() + 0x8000 + i) % 0xFFFFFFFF);
		address_tab[i].address = addr;
		address_tab[i].size = i;

#ifdef NAIVE_PERF
		naive_tab[i].address = addr;
		naive_tab[i].size = i;
#endif

#ifdef USE_BST
		bst_tab[i].address = addr;
		bst_tab[i].size = i;
#endif
	}
#ifdef USE_BST
	randomize(bst_tab, N_ENTRIES, 3);

	for (int j = 0; j < N_ENTRIES; j++)
	{
		mds_entry *entry = mds_get(bst_tab[j].address);
		if (entry == NULL)
		{
			entry = mds_register(bst_tab[j].address, bst_tab[j].size);
		}
#ifdef DEBUG
		else
		{
			fprintf(stderr, "Already registered %ld of size %d, cannot register %ld of size %d\n", (long int)entry->address, entry->size, (long int)bst_tab[j].address, bst_tab[j].size);
			exit(1);
		}

		if (entry->address != bst_tab[j].address || entry->size != bst_tab[j].size)
		{
			fprintf(stderr, "Error : %ld != %ld or %d != %d\n", (long int)entry->address, (long int)bst_tab[j].address, entry->size, bst_tab[j].size);
			exit(1);
		}
		fprintf(stderr, "Registered %ld of size %d\n", (long int)entry->address, entry->size);
#endif
	}
#endif

	// shuffle
	randomize(address_tab, N_ENTRIES, 3);
#ifdef NAIVE_PERF
	randomize(naive_tab, N_ENTRIES, 3);
#endif

#ifdef GRAPHVIZ_DUMP
	char buffer[128 * N_ENTRIES];
	mds_dump(buffer, "G1");
	printf("\n%s\n", buffer);
#endif

	if (mds_check(mds_root()) != 0)
		fprintf(stderr, "MDS consistance check passed with %d/%d nodes\n", mds_count(mds_root()), N_ENTRIES);
	else
	{
		fprintf(stderr, "MDS consistance check failed with %d/%d nodes\n", mds_count(mds_root()), N_ENTRIES);
		exit(1);
	}
	if (mds_count(mds_root()) != N_ENTRIES)
	{
		fprintf(stderr, "MDS count failed with %d nodes\n", mds_count(mds_root()));
		exit(1);
	}

#if defined(BST_PERF) || defined(AVL_PERF) || defined(NAIVE_PERF)
	clock_t start, end;
#endif
#ifdef BST_PERF
	double time_bst = 0;
	start = clock();
	for (int i = 0; i < N_TESTS; i++)
	{
		mds_entry *entry = mds_get(address_tab[i].address);
	}
	end = clock();
	time_bst = ((double)(end - start)) / CLOCKS_PER_SEC;
	fprintf(stderr, "Performance of bst implementation : %lf\n", (time_bst * 1000));

	// time_bst = 0;
	// start = clock();
	// for (int i = 0; i < N_TESTS; i++)
	// {
	// 	mds_entry *entry = mds_get_rec(address_tab[i].address);
	// }
	// end = clock();
	// time_bst = ((double)(end - start)) / CLOCKS_PER_SEC;
	// fprintf(stderr, "Performance of bst get rec implementation : %lf\n", (time_bst * 1000));
#endif
#ifdef NAIVE_PERF

	double time_naive = 0;
	start = clock();
	for (int i = 0; i < N_TESTS; i++)
	{
		mds_entry *entry = naive_tab_search(address_tab[i].address);
	}
	end = clock();
	time_naive = ((double)(end - start)) / CLOCKS_PER_SEC;
	fprintf(stderr, "Performance of naive implementation : %lf\n", (time_naive * 1000));
#endif

#if defined(BST_PERF) && defined(NAIVE_PERF)
	fprintf(stderr, "bst is %.2f%% faster for %d entries and %d tests\n", (float)(time_naive / time_bst) * 100 - 100, N_ENTRIES, N_TESTS);
#endif
	return 0;
}