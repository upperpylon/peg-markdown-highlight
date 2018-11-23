#include <stdio.h>
#include <stdlib.h>

#include "../pmh_parser.h"
#include "../pmh_definitions.h"

char* readFile(const char *fileName)
{
    char *buffer = NULL;
    int strSize, readSize;
    FILE *handler = fopen(fileName, "r");

   if (handler)
   {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);

       // Offset from the first to the last byte, or in other words, filesize
       strSize = ftell(handler);

       // go back to the start of the file
       rewind(handler);

       // Allocate a string that can hold it all
       buffer = (char*) malloc(sizeof(char) * (strSize + 1) );

       // Read it all in one operation
       readSize = fread(buffer, sizeof(char), strSize, handler);

       // fread doesn't set it so put a \0 in the last position
       // and buffer is now officially a string
       buffer[strSize] = '\0';

       if (strSize != readSize)
       {
           // Something went wrong, throw away the memory and set
           // the buffer to NULL
           free(buffer);
           buffer = NULL;
       }

       // Always remember to close the file.
       fclose(handler);
   }

   return buffer;
}

void print_result_debug_info(pmh_element *elem[])
{
    printf("\n");

    for (int i = 0; i < pmh_NUM_TYPES - 1; i++)
    {
        pmh_element *cursor = elem[i];
        while (cursor != NULL)
        {
            printf("[%ld-%ld] %u 0x%lx: %s\n",
                   cursor->pos,
                   cursor->end,
                   cursor->type,
                   (unsigned long)cursor,
                   pmh_element_name_from_type(cursor->type));

            if (cursor->label != NULL)
                printf("  label: '%s'\n", cursor->label);
            if (cursor->address != NULL)
                printf("  address: '%s'\n", cursor->address);

            cursor = cursor->next;
        }
    }
}

void test(char *content)
{
    pmh_element **result;
    int exts = pmh_EXT_MATH | pmh_EXT_MATH_RAW | pmh_EXT_STRIKE | pmh_EXT_TABLE;
    pmh_markdown_to_elements(content, exts, &result);

    print_result_debug_info(result);

    pmh_free_elements(result);
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "Please provide a Markdown file to parse for testing\n");
        return 1;
    }

    const char *mdFilePath = argv[argc - 1];

    printf("Welcome to test PEG Markdown Highlight: %s\n", mdFilePath);

    char *content = readFile(mdFilePath);
    if (!content) {
        fprintf(stderr, "fail to read %s\n", mdFilePath);
        return 1;
    }

    test(content);

    free(content);

    return 0;
}
