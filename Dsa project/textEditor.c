#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure to represent a line in the text editor
typedef struct Line
{
    char *text;
    struct Line *next;
} Line;

// Define a structure to represent the entire document
typedef struct Document
{
    Line *first_line;
} Document;

// Function to create a new document
Document *createDocument()
{
    Document *doc = (Document *)malloc(sizeof(Document));
    doc->first_line = NULL;
    return doc;
}

// Function to add a line to the document
void addLine(Document *doc, const char *text)
{
    Line *newLine = (Line *)malloc(sizeof(Line));
    newLine->text = strdup(text);
    newLine->next = NULL;

    if (doc->first_line == NULL)
    {
        doc->first_line = newLine;
    }
    else
    {
        Line *currentLine = doc->first_line;
        while (currentLine->next != NULL)
        {
            currentLine = currentLine->next;
        }
        currentLine->next = newLine;
    }
}

// Function to delete a line from the document
void deleteLine(Document *doc, int line_number)
{
    if (line_number == 0)
    {
        Line *temp = doc->first_line;
        doc->first_line = doc->first_line->next;
        free(temp->text); // Free the text first
        free(temp);
    }
    else
    {
        Line *currentLine = doc->first_line;
        for (int i = 0; i < line_number - 1 && currentLine != NULL; i++)
        {
            currentLine = currentLine->next;
        }

        if (currentLine != NULL && currentLine->next != NULL)
        {
            Line *temp = currentLine->next;
            currentLine->next = temp->next;
            free(temp->text); // Free the text first
            free(temp);
        }
    }
}

// Function to delete a word from a specific line
void deleteWord(Document *doc, int line_number, const char *word)
{
    Line *currentLine = doc->first_line;
    int current_line_number = 0;

    while (currentLine != NULL && current_line_number < line_number)
    {
        currentLine = currentLine->next;
        current_line_number++;
    }

    if (currentLine == NULL || current_line_number != line_number)
    {
        printf("Line %d does not exist in the document.\n", line_number);
        return;
    }

    char *found = strstr(currentLine->text, word);
    if (found == NULL)
    {
        printf("The word \"%s\" is not in line %d: %s\n", word, line_number, currentLine->text);
    }
    else
    {
        // Calculate the position of the word in the line
        int position = found - currentLine->text;

        // Remove the word from the line
        memmove(found, found + strlen(word), strlen(found + strlen(word)) + 1);
    }
}

// Function to delete a word from a specific line in a specific file
void deleteWordFromSpecificFile(const char *filename, Document *doc, int line_number)
{
    FILE *file = fopen(filename, "r"); // Open the file for reading

    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Read the lines from the file and update the document
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Remove the newline character at the end
        buffer[strcspn(buffer, "\n")] = '\0';
        addLine(doc, buffer);
    }

    fclose(file); // Close the file after reading

    // Prompt the user for the word to delete
    char wordToDelete[256];
    printf("Enter the word to delete: ");
    scanf("%255s", wordToDelete);

    // Now perform the word deletion in the document
    deleteWord(doc, line_number, wordToDelete);
}

// Function to save the document to a file

// existing or non -existing file edit problem
// void saveDocument(Document *doc, const char *filename)
// {
//     FILE *file = fopen(filename, "w");
//     if (file == NULL)
//     {
//         printf("Error opening file for writing.\n");
//         return;
//     }

//     Line *currentLine = doc->first_line;
//     while (currentLine != NULL)
//     {
//         fprintf(file, "%s\n", currentLine->text);
//         currentLine = currentLine->next;
//     }

//     fclose(file); // Close the file after writing to it
// }

// fixed virson:
/*
 NOW WE CAN ETHER CREAT A NEW TEXT FILE
                    OR
 EDIT AN EXISTING FILE IN THE SAME DIRECTORY
*/

void saveDocument(Document *doc, const char *filename)
{
    FILE *file;

    // Prompt user for file name if filename is not provided
    if (filename[0] == '\0')
    {
        char new_filename[256];
        printf("Enter filename to save: ");
        scanf("%255s", new_filename);
        file = fopen(new_filename, "w"); // Use "w" mode for write
    }
    else
    {
        // Open the specified file for writing (truncating existing content)
        file = fopen(filename, "w"); // Use "w" mode for write
    }

    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    Line *currentLine = doc->first_line;
    while (currentLine != NULL)
    {
        fprintf(file, "%s\n", currentLine->text);
        currentLine = currentLine->next;
    }

    fclose(file); // Close the file after writing to it
}



// Function to add a word in a specific line at a specific position

// 1. {space problem}
// void addWord(Document *doc, int line_number, const char *word, const char *target_word, int before)
// {
//     Line *currentLine = doc->first_line;
//     int current_line_number = 0;

//     while (currentLine != NULL && current_line_number < line_number)
//     {
//         currentLine = currentLine->next;
//         current_line_number++;
//     }

//     if (currentLine == NULL || current_line_number != line_number)
//     {
//         printf("Line %d does not exist in the document.\n", line_number);
//         return;
//     }

//     char *found = strstr(currentLine->text, target_word);
//     if (found == NULL)
//     {
//         printf("The target word \"%s\" does not exist in line %d: %s\n", target_word, line_number, currentLine->text);
//     }
//     else
//     {
//         int position = found - currentLine->text;

//         // Calculate the new word length
//         int new_word_length = strlen(word);

//         // Calculate the new line length
//         int new_line_length = strlen(currentLine->text) + new_word_length + 1;

//         // Create a new line with enough space for the new word
//         char *new_line = (char *)malloc(new_line_length);

//         if (before)
//         {
//             // Copy text before the target word
//             strncpy(new_line, currentLine->text, position);
//             new_line[position] = '\0';

//             // Append the new word
//             strcat(new_line, word);

//             // Append the text after the target word
//             strcat(new_line, found);
//         }
//         else
//         {
//             // Copy text before the target word
//             strncpy(new_line, currentLine->text, position + strlen(target_word));

//             // Append the new word
//             strcat(new_line, word);

//             // Append the text after the target word
//             strcat(new_line, found + strlen(target_word));
//         }

//         // Update the line's text and free the old line
//         free(currentLine->text);
//         currentLine->text = new_line;
//     }
// }

// enhanced version:
// void addWord(Document *doc, int line_number, const char *word, const char *target_word, int before)
// {
//     Line *currentLine = doc->first_line;
//     int current_line_number = 0;

//     while (currentLine != NULL && current_line_number < line_number)
//     {
//         currentLine = currentLine->next;
//         current_line_number++;
//     }

//     if (currentLine == NULL || current_line_number != line_number)
//     {
//         printf("Line %d does not exist in the document.\n", line_number);
//         return;
//     }

//     char *found = strstr(currentLine->text, target_word);
//     if (found == NULL)
//     {
//         printf("The target word \"%s\" does not exist in line %d: %s\n", target_word, line_number, currentLine->text);
//     }
//     else
//     {
//         int position = found - currentLine->text;

//         // Calculate the new word length
//         int new_word_length = strlen(word);

//         // Calculate the new line length
//         int new_line_length = strlen(currentLine->text) + new_word_length + 3; // 3 for two spaces and null terminator

//         // Create a new line with enough space for the new word and spaces
//         char *new_line = (char *)malloc(new_line_length);

//         if (before)
//         {
//             // Copy text before the target word
//             strncpy(new_line, currentLine->text, position);
//             new_line[position] = ' '; // Add a space before the new word
//             new_line[position + 1] = '\0';

//             // Append the new word
//             strcat(new_line, word);

//             // Append the space and the text after the target word
//             strcat(new_line, found);
//             strcat(new_line, " "); // Add a space after the new word
//         }
//         else
//         {
//             // Copy text before the target word
//             strncpy(new_line, currentLine->text, position + strlen(target_word));

//             // Append the space before the new word
//             new_line[position + strlen(target_word)] = ' ';
//             new_line[position + strlen(target_word) + 1] = '\0';

//             // Append the new word
//             strcat(new_line, word);

//             // Append the space and the text after the target word
//             strcat(new_line, found + strlen(target_word));
//             strcat(new_line, " "); // Add a space after the new word
//         }

//         // Update the line's text and free the old line
//         free(currentLine->text);
//         currentLine->text = new_line;
//     }
// }



// enhanced virsion (let the user choose the filename)
void addWord(Document *doc, int line_number, const char *word, const char *target_word, int before)
{
    Line *currentLine = doc->first_line;
    int current_line_number = 0;

    // Locate the specified line
    while (currentLine != NULL && current_line_number < line_number - 1)
    {
        currentLine = currentLine->next;
        current_line_number++;
    }

    // Check if the line exists
    if (line_number <= 0 || currentLine == NULL || current_line_number != line_number)
    {
        printf("Line %d does not exist in the document.\n", line_number);
        return;
    }

    char *found = strstr(currentLine->text, target_word);
    if (found == NULL)
    {
        printf("The target word \"%s\" does not exist in line %d: %s\n", target_word, line_number, currentLine->text);
    }
    else
    {
        int position = found - currentLine->text;

        // Calculate the new word length
        int new_word_length = strlen(word);

        // Calculate the new line length
        int new_line_length = strlen(currentLine->text) + new_word_length + 3; // 3 for two spaces and null terminator

        // Create a new line with enough space for the new word and spaces
        char *new_line = (char *)malloc(new_line_length);

        if (before)
        {
            // Copy text before the target word
            strncpy(new_line, currentLine->text, position);
            new_line[position] = ' '; // Add a space before the new word
            new_line[position + 1] = '\0';

            // Append the new word
            strcat(new_line, word);

            // Append the space and the text after the target word
            strcat(new_line, found);
        }
        else
        {
            // Copy text before the target word
            strncpy(new_line, currentLine->text, position + strlen(target_word));

            // Append the space before the new word
            new_line[position + strlen(target_word)] = ' ';
            new_line[position + strlen(target_word) + 1] = '\0';

            // Append the new word
            strcat(new_line, word);

            // Append the space and the text after the target word
            strcat(new_line, found + strlen(target_word));
        }

        // Update the line's text and free the old line
        free(currentLine->text);
        currentLine->text = new_line;
    }
}

// Function to search for a word in the document
void searchWord(Document *doc, const char *word)
{
    Line *currentLine = doc->first_line;
    int line_number = 0;
    while (currentLine != NULL)
    {
        if (strstr(currentLine->text, word) != NULL)
        {
            printf("Line %d: %s\n", line_number, currentLine->text);
        }
        currentLine = currentLine->next;
        line_number++;
    }
}

// Function to print the document
void printDocument(Document *doc)
{
    Line *currentLine = doc->first_line;
    int line_number = 0;
    while (currentLine != NULL)
    {
        printf("Line %d: %s\n", line_number, currentLine->text);
        currentLine = currentLine->next;
        line_number++;
    }
}

int main()
{
    Document *doc = createDocument();

    while (1)
    {
        printf("\nText Editor Menu:\n");
        printf("1. Add a line\n");
        printf("2. Delete a line\n");
        printf("3. Save the document\n");
        printf("4. Search for a word\n");
        printf("5. DISPLAY the document\n");
        printf("6. Delete a word from a specific line\n");
        printf("7. Add a word in a specific position in a specific line \n");
        printf("8. Quit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            char buffer[500];
            printf("Enter text: ");
            getchar(); // Clear the newline character from the input buffer
            fgets(buffer, sizeof(buffer), stdin);
            addLine(doc, buffer);
        }
        else if (choice == 2)
        {
            int line_number;
            printf("Enter line number to delete: ");
            scanf("%d", &line_number);
            deleteLine(doc, line_number);
        }
        else if (choice == 3)
        {
            char filename[500];
            printf("Enter filename to save: ");
            scanf("%s", filename);
            saveDocument(doc, filename);
        }
        else if (choice == 4)
        {
            char word[500];
            printf("Enter word to search: ");
            scanf("%s", word);
            searchWord(doc, word);
        }
        else if (choice == 5)
        {
            printDocument(doc);
        }
        else if (choice == 6)
        {
            int line_number;
            printf("Enter line number: ");
            scanf("%d", &line_number);

            char filename[256];
            printf("Enter the filename: ");
            scanf("%255s", filename);

            deleteWordFromSpecificFile(filename, doc, line_number);
        }
        else if (choice == 7)
        {
            int line_number;
            printf("Enter line number: ");
            scanf("%d", &line_number);

            char filename[256];
            printf("Enter the filename: ");
            scanf("%255s", filename);

            char word[500];
            printf("Enter word to add: ");
            scanf("%499s", word); // Limit the input size to avoid buffer overflow

            char target_word[500];
            printf("Enter the target word: ");
            scanf("%499s", target_word); // Limit the input size to avoid buffer overflow

            int before;
            printf("Add word before (1) or after (0) the target word: ");
            scanf("%d", &before);

            addWord(doc, line_number, word, target_word, before);
        }
        else if (choice == 8)
        {
            break;
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }
    }

    // Clean up and free memory
    Line *currentLine = doc->first_line;
    while (currentLine != NULL)
    {
        Line *temp = currentLine;
        currentLine = currentLine->next;
        free(temp->text);
        free(temp);
    }
    free(doc);

    return 0;

}
