#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char *filename;
	char *section = malloc(100 * sizeof(char));
	char *key = malloc(100 * sizeof(char));
	if (argc == 3)
	{
		// getting the filename
		filename = argv[1];
		// getting the section and the key
        char sectionKey[100];
        strcpy(sectionKey, argv[2]);
		char *token = strtok(sectionKey, ".");
		int counter = 0;
		while (token != NULL) {
			if (counter == 0)
			{
				strcpy(section, token);
				token = strtok(NULL, ".");
                counter++;
			}
			else
			{
				strcpy(key, token);
                token = NULL;
			}
		}

	}
	else
	{
		printf("Usage: ./name filename.ini section.key\n");
		return 0;
	}

	if (access(filename, F_OK) != 0)
	{
		printf("Unable to open %s\n", filename);
		return 0;
	}

	char *contents = 0;

	FILE *file = fopen(filename, "r");

	// moving the pointer to the end of the file in order to get it's size
	fseek(file, 0L, SEEK_END);

	int length = ftell(file);

	// moving the pointer back to the start of the file
	fseek(file, 0L, SEEK_SET);

	contents = malloc(length);

	// getting the contents
	if (contents)
	{
		fread(contents, 1, length, file);
	}

	fclose(file);

	char *contentsPointer = malloc((length + 1) * sizeof(char) * 2);
	memcpy(contentsPointer, contents, length);

	char *saveptr;
	char *tk = strtok_r(contentsPointer, "\n", &saveptr);

	int found = 0;
	// splitting the contents into lines
	while (tk != NULL)
	{
        if (!strcmp(tk, section))
        {
            found = 1;
        }
        else if (found)
        {
	        char *words[] = {};
	        int counter = 0;
            char *word = strtok(tk, " = ");
            while (word != NULL) {
	            words[counter++] = word;
	            word = strtok(NULL, " = ");
            }
	        if (!strcmp(words[0], key))
            {
				printf("The value is %s\n", words[1]);
	            free(contentsPointer);
	            return 1;
			}
        }
		tk = strtok_r(NULL, "\n", &saveptr);
	}

	printf("Not found\n");

	free(contentsPointer);

	return 1;
}
