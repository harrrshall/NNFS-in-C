#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// STEPS:->>

// Make Dataset -> Bigrams -> itos/stoi -> change it into one hot vector embedding(calculating log, logprob and likelyhood)
// Train it on Gradient Descent -> forward bias and backward bias
// Use it to predict the words -> using loop to predict next words

// Open  file
char *open_file()
{
    FILE *file_ptr;
    char filename[] = "words.txt";

    file_ptr = fopen(filename, "r");

    if (file_ptr == NULL)
    {
        printf("Unable to open the file\n");
        return NULL; // Return NULL to indicate failure
    }

    // Determine the size of the file
    fseek(file_ptr, 0, SEEK_END);
    long file_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    // Allocate memory for the buffer
    char *buffer = (char *)malloc(file_size + 1);

    if (buffer == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(file_ptr);
        return NULL; // Return NULL to indicate failure
    }

    // Read the entire file into the buffer
    fread(buffer, 1, file_size, file_ptr);
    buffer[file_size] = '\0'; // Null-terminate the string

    // Close the file
    fclose(file_ptr);

    // Now, 'buffer' contains the entire contents of the file
    return buffer;
}

// Making the dataset
void make_dataset()
{
    char *words = open_file();
    if(words == NULL) {
        return 0;
    }
    char* arr[] =  words;
    for (int i = 0; i < sizeof(arr); i++)
    {
        
    }
    
}

int main()
{
    make_dataset();
    return 0;
}