#include <stdio.h>
#include <stdlib.h>

#define MAX_ENTRIES 30

typedef struct {
    float number;
    float label;  // 0 for even, 1 for odd
} Entry;

Entry* load_data() {
    FILE *fp;
    Entry *entries = malloc(MAX_ENTRIES * sizeof(Entry));
    if (entries == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    int count = 0;

    fp = fopen("data.txt", "r");
    if (fp == NULL) {
        perror("Failed to open file");
        free(entries);
        exit(1);
    }

    while (fscanf(fp, "%f %f", &entries[count].number, &entries[count].label) != EOF && count < MAX_ENTRIES) {
        count++;
    }
    fclose(fp);

    return entries;
}
