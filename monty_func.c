#define _POSIX_C_SOURCE 200809L 
#include "monty.h"
#include <stdio.h>

void read_file(char *filename, stack_t **stack)
{
    char *line = NULL;
    size_t i = 0;
    int line_count = 1;
    instruct_func s;
    int check;
    ssize_t read;

    var_global.file = fopen(filename, "r");

    if (var_global.file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&var_global.buffer, &i, var_global.file)) != -1)
    {
        line = parse_line(var_global.buffer, stack, line_count);
        if (line == NULL || line[0] == '#')
        {
            line_count++;
            continue;
        }
        s = get_op_func(line);
        if (s == NULL)
        {
            fprintf(stderr, "L%d: unknown instruction %s\n", line_count, line);
            exit(EXIT_FAILURE);
        }
        s(stack, line_count);
        line_count++;
    }
    free(var_global.buffer);
    check = fclose(var_global.file);
    if (check == -1)
        exit(-1);
}

instruct_func get_op_func(char *str)
{
    int i;

    instruction_t instruct[] = {
        {"push", _push},
        {"pall", _pall},
        {"pint", _pint},
        {"pop", _pop},
        {"swap", _swap},
        {"add", _add},
        {"nop", _nop},
        {"sub", _sub},
        {"mul", _mul},
        {"div", _div},
        {"mod", _mod},
        {"pchar", _pchar},
        {"pstr", _pstr},
        {"rotl", _rotl},
        {"rotr", _rotr},
        {NULL, NULL},
    };

    i = 0;
    while (instruct[i].f != NULL && strcmp(instruct[i].opcode, str) != 0)
    {
        i++;
    }

    return (instruct[i].f);
}

int isnumber(char *str)
{
    unsigned int i;

    if (str == NULL)
        return (0);
    i = 0;
    while (str[i])
    {
        if (str[0] == '-')
        {
            i++;
            continue;
        }
        if (!isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

char *parse_line(char *line, stack_t **stack, unsigned int line_number)
{
    char *op_code, *push, *arg;
    (void)stack;

    push = "push";
    op_code = strtok(line, "\n ");
    if (op_code == NULL)
        return (NULL);

    if (strcmp(op_code, push) == 0)
    {
        arg = strtok(NULL, "\n ");
        if (isnumber(arg) == 1 && arg != NULL)
        {
            var_global.push_arg = atoi(arg);
        }
        else
        {
            fprintf(stderr, "L%d: usage: push integer\n", line_number);
            exit(EXIT_FAILURE);
        }
    }
    return (op_code);
}
