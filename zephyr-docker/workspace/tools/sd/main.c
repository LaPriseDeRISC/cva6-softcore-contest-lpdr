#include "malloc_structs/malloc_structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N_ENTRIES 100
#define N_TESTS N_ENTRIES / 2
// #define GRAPHVIZ_DUMP

mds_entry naive_tab[N_ENTRIES], address_table[N_ENTRIES];

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

mds_entry *naive_tab_search(__ptr_t address)
{
	for (int i = 0; i < N_ENTRIES; i++)
	{
		if (naive_tab[i].address == address)
			return &naive_tab[i];
	}

	return NULL;
}

int main()
{
	srand(time(NULL));
	printf("Sizes :\tint %ld, long %ld, long long %ld, void* %ld, size_t %ld, mds_entry %ld, mds_entry %ld\n",
		   sizeof(int), sizeof(long), sizeof(long long), sizeof(void *), sizeof(size_t));
	printf("\tmds_entry %ld (on %ldb int), mds_entry %ld (on %ldb int)",
		   sizeof(mds_entry), sizeof(int) * 8, (sizeof(mds_entry)) / 2, (sizeof(int) * 8) / 2);

	for (int i = 0; i < N_ENTRIES; i++)
	{
		__ptr_t addr = (__ptr_t)((long)(rand() + 0x8000) % 0xFFFF);
		naive_tab[i].address = addr;
		naive_tab[i].size = i;
		address_table[i].address = addr;
		address_table[i].size = i;

		mds_entry *entry = mds_register(addr, i);
		// printf("Registered %p of size %d\n", entry->address, entry->size);
	}
	randomize(naive_tab, N_ENTRIES, 3);
	randomize(address_table, N_ENTRIES, 3);

	if (mds_check_all() != 0)
		printf("MDS consistance check passed\n");
	else
		printf("MDS consistance check failed\n");

#ifdef GRAPHVIZ_DUMP
	char buffer[4096];
	mds_dump(buffer);
	printf("\n%s\n", buffer);
#endif

	clock_t start, end;
	double time_bst = 0;

	start = clock();
	for (int i = 0; i < N_TESTS; i++)
	{
		mds_entry *entry = mds_get(address_table[i].address);
	}
	end = clock();
	time_bst = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Performance of bst implementation : %lf\n", (time_bst * 1000));

	double time_naive = 0;
	start = clock();
	for (int i = 0; i < N_TESTS; i++)
	{
		mds_entry *entry = naive_tab_search(address_table[i].address);
	}
	end = clock();
	time_naive = ((double)(end - start)) / CLOCKS_PER_SEC;

	printf("Performance of naive implementation : %lf\n", (time_naive * 1000));
	printf("bst is %.2f%% faster for %d entries and %d tests\n", (float)(time_naive / time_bst) * 100 - 100, N_ENTRIES, N_TESTS);
	return 0;
}