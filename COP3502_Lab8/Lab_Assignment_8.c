#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;


void maxHeap(int arr[], int size, int index) {
	int largest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if (left < size && arr[left] > arr[largest])
		largest = left;
	if (right < size && arr[right] > arr[largest])
		largest = right;

	if (largest != index) {
		int swap = arr[index];
		arr[index] = arr[largest];
		arr[largest] = swap;

		maxHeap(arr, size, largest);
	}
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n){
	//Create Max Heap
	for (int i = n / 2 - 1; i >= 0; i--)
		maxHeap(arr, n, i);

	//Heap Sort
	for (int j = n - 1; j > 0; j--) {
		int swap = arr[0];
		arr[0] = arr[j];
		arr[j] = swap;

		maxHeap(arr, j, 0);
	}

}

void merge(int pData[], int l, int m, int r) {
	int i, j, k;
	int leftSize = m - l + 1;
	int rightSize = r - m;

	int* left = (int*)malloc(sizeof(int) * leftSize);
	extraMemoryAllocated += sizeof(left);
	int* right = (int*)malloc(sizeof(int) * rightSize);
	extraMemoryAllocated += sizeof(right);


	for (i = 0; i < leftSize; i++)
		left[i] = pData[l + i];
	for (j = 0; j < rightSize; j++)
		right[j] = pData[m + 1 + j];

	i = j = 0;
	k = l;
	while (i < leftSize && j < rightSize) {
		if (left[i] <= right[j]) {
			pData[k] = left[i];
			i++;
		}
		else {
			pData[k] = right[j];
			j++;
		}
		k++;
	}

	while (i < leftSize)
		pData[k++] = left[i++];
	while (j < rightSize)
		pData[k++] = right[j++];

	free(left);
	free(right);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r){
	if (l < r) {
		int mid = (l + r) / 2;

		mergeSort(pData, l, mid);
		mergeSort(pData, mid + 1, r);

		merge(pData, l, mid, r);
	}

}

// parses input file to an integer array
int parseData(char* inputFileName, int** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n, * data;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (int*)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d ", &n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i = 0; i < 100; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\t");

	for (i = sz; i < dataSz; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

	for (i = 0; i < 4; ++i)
	{
		int* pDataSrc, * pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int*)malloc(sizeof(int) * dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n", dataSz);
		printf("---------------------------\n");

		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.3lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.3lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		free(pDataCopy);
		free(pDataSrc);
	}

}