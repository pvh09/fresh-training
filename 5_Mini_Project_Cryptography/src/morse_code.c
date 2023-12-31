#include "morse_code.h"
#include "binary_tree.h"
#include "file_handling.h"

tree_node_t *g_root = NULL;

char *map_morse_code(char character)
{
    switch (character)
    {
    case 'A':
        return ".-";
    case 'B':
        return "-...";
    case 'C':
        return "-.-.";
    case 'D':
        return "-..";
    case 'E':
        return ".";
    case 'F':
        return "..-.";
    case 'G':
        return "--.";
    case 'H':
        return "....";
    case 'I':
        return "..";
    case 'J':
        return ".---";
    case 'K':
        return "-.-";
    case 'L':
        return ".-..";
    case 'M':
        return "--";
    case 'N':
        return "-.";
    case 'O':
        return "---";
    case 'P':
        return ".--.";
    case 'Q':
        return "--.-";
    case 'R':
        return ".-.";
    case 'S':
        return "...";
    case 'T':
        return "-";
    case 'U':
        return "..-";
    case 'V':
        return "...-";
    case 'W':
        return ".--";
    case 'X':
        return "-..-";
    case 'Y':
        return "-.--";
    case 'Z':
        return "--..";
    case ' ':
        return "/";
    default:
        return ""; // Return an empty string for characters not in the Morse code mapping
    }
}

tree_node_t *build_morse_code_tree()
{
    g_root = create_node('\0');
    char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";
    // Loop through the characters array and insert into the tree
    for (int i = 0; characters[i] != '\0'; i++)
    {
        insert(g_root, characters[i], map_morse_code(characters[i]));
    }

    return g_root;
}

void init_tree(const char *input_file_name, const char *output_file_name)
{
    g_root = build_morse_code_tree();
}

void encode_character(char character, FILE *output)
{
    char *morse_code = map_morse_code(character);
    fputs(morse_code, output);
}

// Function to encode Morse code for a given character
void encode_morse(tree_node_t *root, char c, FILE *output)
{
    if (root == NULL)
        return;

    if (root->data == c)
    {
        // Perform the necessary action to output/record the Morse code
        // In this example, we write the Morse code to the output file
        encode_character(c, output);
    }

    encode_morse(root->p_left, c, output);
    encode_morse(root->p_right, c, output);
}

// Function to handle file I/O and encoding process
int encode_file(const char *input_file_name, const char *output_file_name)
{
    char buffer[MAX_FILE] = {0}; // Adjust the buffer size as needed

    // Open the input file
    FILE *input = fopen(input_file_name, "r");
    FILE *output = fopen(output_file_name, "w");

    if (input == NULL)
    {
        perror("Error opening input file");
        return 1;
    }
    // Open the output file
    if (output == NULL)
    {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        // Remove the newline character at the end of the line
        buffer[strcspn(buffer, "\n")] = '\0';

        // Encode the line character by character
        for (int i = 0; i < strlen(buffer); i++)
        {
            char c = buffer[i];
            if (c == ' ')
            {
                // Use the special character for space
                encode_morse(g_root, ' ', output);
            }
            else
            {
                encode_morse(g_root, c, output);
            }

            // Add a space after each encoded character (except for the last one)
            if (i != strlen(buffer) - 1)
                fputs(" ", output);
        }

        // Write a newline character to the output file for the next line
        fputs("\n", output);
    }

    // Close the input and output files
    fclose(input);
    fclose(output);

    return 0;
}

// Function to free the memory of the Morse code tree
void free_morse_code_tree()
{
    if (g_root == NULL)
    {
        return;
    }

    free_morse_code_tree(g_root->p_left);
    free_morse_code_tree(g_root->p_right);
    free(g_root);
    g_root = NULL;
}

// Function to replace '/' with a space in the output file
void handle_slash(FILE *output)
{
    fprintf(output, " ");
}

// Function to decode the Morse code and write the corresponding character to the output file
void decode_morse_code(const char *morse_code, FILE *output)
{
    tree_node_t *current_node = g_root;
    for (int i = 0; morse_code[i] != '\0'; i++)
    {
        if (morse_code[i] == '.')
        {
            current_node = current_node->p_left;
        }
        else if (morse_code[i] == '-')
        {
            current_node = current_node->p_right;
        }
    }
    fprintf(output, "%c", current_node->data);
}

// Function to handle individual tokens (Morse code or '/')
void handle_token(const char *token, FILE *output)
{
    char morse_code[MAX_FILE] = {0};
    if (strcmp(token, "/") == 0)
    {
        handle_slash(output);
    }
    else
    {
        memset(morse_code, 0, MAX_FILE);
        for (int i = 0; token[i] != '\0'; i++)
        {
            if (token[i] == '.')
            {
                strcat(morse_code, ".");
            }
            else if (token[i] == '-')
            {
                strcat(morse_code, "-");
            }
        }
        decode_morse_code(morse_code, output);
    }
}

// Function to decode Morse code from an input file and write the decoded text to an output file
void decode_file(const char *input_file, const char *output_file)
{
    char line[MAX_FILE] = {0}; // Adjust the buffer size as needed
    FILE *input = fopen(input_file, "r");
    FILE *output = fopen(output_file, "w");

    if (input == NULL)
    {
        printf("Unable to open input file.\n");
        return;
    }
    if (output == NULL)
    {
        printf("Unable to open output file.\n");
        fclose(input);
        return;
    }
    while (fgets(line, sizeof(line), input) != NULL)
    {
        char *token = strtok(line, " \n"); // Tokenize based on space and newline
        while (token != NULL)
        {
            handle_token(token, output);
            token = strtok(NULL, " \n"); // Move to the next token
        }
        fprintf(output, "\n"); // Add a space after each word
    }

    fclose(input);
    fclose(output);
}

// Function to decode Morse code from user input and write the decoded text to an output file
void decode_keyboard(const char *output_file)
{
    char input[MAX_FILE] = {0}; // Adjust the buffer size as needed
    char *token = NULL;
    
    FILE *output = fopen(output_file, "w");
    if (output == NULL)
    {
        printf("Unable to open output file.\n");
        return;
    }
    __fpurge(stdin);
    printf("Enter Morse code (separated by spaces): ");
    
    fgets(input, sizeof(input), stdin);
    token = strtok(input, " \n"); // Tokenize based on space and newline
    while (token != NULL)
    {
        handle_token(token, output);
        token = strtok(NULL, " \n"); // Move to the next token
    }

    fclose(output);
    printf("Decoding complete. Output saved to %s\n", output_file);
}

int encode_keyboard(const char *output_filename)
{
    int c = 0;
    int rs = 1;
    // Open the output file
    FILE *output = fopen(output_filename, "w");
    if (output == NULL)
    {
        perror("Error opening output file");
        return 1;
    }
    __fpurge(stdin);
    printf("Enter text: ");
    while ((c = getchar()) != EOF && c != '\n')
    {
        if (c == ' ')
        {
            // Use the special character for space
            encode_morse(g_root, ' ', output);
        }
        else
        {
            encode_morse(g_root, (char)c, output);
        }

        // Add a space after each encoded character (except for the last one)
        fputs(" ", output);
        rs = 1;
    }

    if (rs != 1)
    {
        printf("\n>> Error encoding file\n");
    }
    else
    {
        printf("\n>> File encoded successfully. Please check folder\n");
    }
    fclose(output);
    return 0;
}