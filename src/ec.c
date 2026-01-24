/*
 * EC Language Interpreter v1.2.0 (Solid Core)
 * A next-generation minimalist programming language
 * 
 * Update: Added robust error detection and stack tracing.
 * 
 * Copyright (c) 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

#ifdef _WIN32
    #include <windows.h>
    #define popen _popen
    #define pclose _pclose
#else
    #include <unistd.h>
#endif

#define MAX_VARS 1024
#define MAX_FUNCS 256
#define MAX_CLASSES 64
#define MAX_STACK 256
#define MAX_LINE 4096
#define MAX_NAME 128
#define MAX_ARRAYS 256

// ============ Type Definitions ============

typedef enum {
    TYPE_NULL,
    TYPE_NUMBER,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_OBJECT
} ECType;

typedef struct {
    char name[MAX_NAME];
    ECType type;
    double num_val;
    char str_val[MAX_LINE];
    int arr_id;
    int obj_class_id;
} ECVar;

typedef struct {
    double* num_data;
    char** str_data;
    ECType elem_type;
    int size;
    int capacity;
} ECArray;

typedef struct {
    char name[MAX_NAME];
    int start_line;
    int end_line;
    char params[16][MAX_NAME];
    int param_count;
} ECFunc;

typedef struct {
    char name[MAX_NAME];
    int start_line;
    int end_line;
    char members[32][MAX_NAME];
    int member_count;
    char methods[32][MAX_NAME];
    int method_count;
} ECClass;

typedef struct {
    int line_num;
    char func_name[MAX_NAME];
} StackFrame;

// ============ Global State ============

ECVar vars[MAX_VARS];
int var_count = 0;

ECArray arrays[MAX_ARRAYS];
int array_count = 0;

ECFunc funcs[MAX_FUNCS];
int func_count = 0;

ECClass classes[MAX_CLASSES];
int class_count = 0;

char** lines = NULL;
int line_count = 0;
int current_line = 0;

int call_stack[MAX_STACK];
StackFrame debug_stack[MAX_STACK]; // For error reporting
int call_stack_top = 0;

int loop_start[MAX_STACK];
int loop_end[MAX_STACK];
int loop_depth = 0;

int running = 1;
int in_function = 0;
double return_value = 0;
int has_return = 0;

char last_exec_output[MAX_LINE * 4] = "";

// ============ Utility Functions ============

void trim(char* str) {
    char* start = str;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) str[--len] = '\0';
}

// Error Reporting
void runtime_error(const char* format, ...) {
    fprintf(stderr, "\n\033[1;31m[RUNTIME ERROR]\033[0m at line %d:\n", current_line + 1);
    
    // Print the line content
    if (current_line < line_count) {
        char temp[MAX_LINE];
        strcpy(temp, lines[current_line]);
        trim(temp);
        fprintf(stderr, ">> %s\n", temp);
    }
    
    va_list args;
    va_start(args, format);
    fprintf(stderr, "Details: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);

    // Call Stack Trace
    if (call_stack_top > 0) {
        fprintf(stderr, "\nStack Trace:\n");
        for (int i = call_stack_top - 1; i >= 0; i--) {
            fprintf(stderr, "  at line %d (in %s)\n", debug_stack[i].line_num + 1, debug_stack[i].func_name);
        }
    }
    
    // Clean exit
    exit(1);
}

int is_number(const char* str) {
    if (!str || !*str) return 0;
    char* end;
    strtod(str, &end);
    return *end == '\0';
}

int find_var(const char* name) {
    for (int i = var_count - 1; i >= 0; i--) {
        if (strcmp(vars[i].name, name) == 0) return i;
    }
    return -1;
}

int find_func(const char* name) {
    for (int i = 0; i < func_count; i++) {
        if (strcmp(funcs[i].name, name) == 0) return i;
    }
    return -1;
}

int find_class(const char* name) {
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].name, name) == 0) return i;
    }
    return -1;
}

ECVar* get_or_create_var(const char* name) {
    int idx = find_var(name);
    if (idx >= 0) return &vars[idx];
    
    if (var_count >= MAX_VARS) {
        runtime_error("Stack Overflow: Too many variables declared (Limit: %d).", MAX_VARS);
    }
    
    ECVar* v = &vars[var_count++];
    strncpy(v->name, name, MAX_NAME - 1);
    v->type = TYPE_NULL;
    v->num_val = 0;
    v->str_val[0] = '\0';
    v->arr_id = -1;
    v->obj_class_id = -1;
    return v;
}

ECVar* get_var_checked(const char* name) {
    int idx = find_var(name);
    if (idx < 0) {
        runtime_error("Undefined variable '%s'. Please declare it with 'EC' first.", name);
    }
    return &vars[idx];
}

// ============ Expression Parser ============

double evaluate_expr(const char* expr);

double parse_value(const char* token) {
    char tok[MAX_LINE];
    strncpy(tok, token, MAX_LINE - 1);
    tok[MAX_LINE - 1] = '\0';
    trim(tok);
    
    if (is_number(tok)) return strtod(tok, NULL);
    
    char* bracket = strchr(tok, '[');
    if (bracket) {
        char arr_name[MAX_NAME];
        int name_len = bracket - tok;
        strncpy(arr_name, tok, name_len);
        arr_name[name_len] = '\0';
        
        char* end_bracket = strchr(bracket, ']');
        if (end_bracket) {
            char idx_str[MAX_NAME];
            int idx_len = end_bracket - bracket - 1;
            strncpy(idx_str, bracket + 1, idx_len);
            idx_str[idx_len] = '\0';
            
            int arr_idx = (int)evaluate_expr(idx_str);
            int var_idx = find_var(arr_name);
            
            if (var_idx < 0) runtime_error("Undefined array '%s'.", arr_name);
            if (vars[var_idx].arr_id < 0) runtime_error("Variable '%s' is not an array.", arr_name);
            
            ECArray* arr = &arrays[vars[var_idx].arr_id];
            
            if (arr_idx < 0 || arr_idx >= arr->size) {
                runtime_error("Array Index Out of Bounds: Index %d, Size %d.", arr_idx, arr->size);
            }
            return arr->num_data[arr_idx];
        } else {
             runtime_error("Missing closing bracket ']' in array access.");
        }
    }

    ECVar* v = get_var_checked(tok);
    if (v->type == TYPE_STRING) return atof(v->str_val); // Try convert string to num
    return v->num_val;
}

int get_precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': case '%': return 2;
        default: return 0;
    }
}

double apply_op(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) runtime_error("Division by zero.");
            return a / b;
        case '%': 
            if (b == 0) runtime_error("Modulo by zero.");
            return fmod(a, b);
        default: return 0;
    }
}

double evaluate_expr(const char* expr) {
    char buf[MAX_LINE];
    strncpy(buf, expr, MAX_LINE - 1);
    buf[MAX_LINE - 1] = '\0';
    trim(buf);
    
    if (strlen(buf) == 0) return 0;
    
    double values[64];
    char ops[64];
    int val_top = 0, op_top = 0;
    
    char token[MAX_LINE];
    int token_pos = 0;
    int paren_depth = 0;
    
    for (int i = 0; buf[i]; i++) {
        char c = buf[i];
        
        if (c == '(') { paren_depth++; token[token_pos++] = c; }
        else if (c == ')') { paren_depth--; token[token_pos++] = c; }
        else if (paren_depth == 0 && (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')) {
            if (c == '-' && token_pos == 0 && val_top == 0) {
                token[token_pos++] = c;
                continue;
            }
            
            token[token_pos] = '\0';
            trim(token);
            if (token_pos > 0) {
                if (token[0] == '(') {
                    token[strlen(token) - 1] = '\0';
                    values[val_top++] = evaluate_expr(token + 1);
                } else {
                    values[val_top++] = parse_value(token);
                }
            }
            token_pos = 0;
            
            while (op_top > 0 && get_precedence(ops[op_top - 1]) >= get_precedence(c)) {
                double b = values[--val_top];
                double a = values[--val_top];
                values[val_top++] = apply_op(a, b, ops[--op_top]);
            }
            ops[op_top++] = c;
        } else {
            token[token_pos++] = c;
        }
    }
    
    token[token_pos] = '\0';
    trim(token);
    if (token_pos > 0) {
        if (token[0] == '(') {
            token[strlen(token) - 1] = '\0';
            values[val_top++] = evaluate_expr(token + 1);
        } else {
            values[val_top++] = parse_value(token);
        }
    }
    
    while (op_top > 0) {
        if (val_top < 2) runtime_error("Invalid expression syntax: '%s'", expr);
        double b = values[--val_top];
        double a = values[--val_top];
        values[val_top++] = apply_op(a, b, ops[--op_top]);
    }
    
    if (val_top != 1) return 0; // Should ideally error 
    return values[0];
}

char* get_string_value(const char* expr, char* result) {
    char buf[MAX_LINE];
    strncpy(buf, expr, MAX_LINE - 1);
    trim(buf);
    
    if (buf[0] == '"' && buf[strlen(buf) - 1] == '"') {
        strncpy(result, buf + 1, strlen(buf) - 2);
        result[strlen(buf) - 2] = '\0';
        return result;
    }
    
    int idx = find_var(buf);
    if (idx >= 0 && vars[idx].type == TYPE_STRING) {
        strcpy(result, vars[idx].str_val);
        return result;
    }
    
    double val = evaluate_expr(buf);
    if (val == (int)val) sprintf(result, "%d", (int)val);
    else sprintf(result, "%g", val);
    return result;
}

int evaluate_condition(const char* cond) {
    char buf[MAX_LINE];
    strncpy(buf, cond, MAX_LINE - 1);
    trim(buf);
    
    char* ops[] = {"==", "!=", ">=", "<=", ">", "<"}; // Longest first
    int op_lens[] = {2, 2, 2, 2, 1, 1};
    
    for (int i = 0; i < 6; i++) {
        char* pos = strstr(buf, ops[i]);
        if (pos) {
            char left[MAX_LINE], right[MAX_LINE];
            int left_len = pos - buf;
            strncpy(left, buf, left_len);
            left[left_len] = '\0';
            strcpy(right, pos + op_lens[i]);
            
            trim(left); trim(right);
            double lval = evaluate_expr(left);
            double rval = evaluate_expr(right);
            
            switch (i) {
                case 0: return lval == rval;
                case 1: return lval != rval;
                case 2: return lval >= rval;
                case 3: return lval <= rval;
                case 4: return lval > rval;
                case 5: return lval < rval;
            }
        }
    }
    return evaluate_expr(buf) != 0;
}

// ============ Static Analysis ============

void validate_syntax(void) {
    int if_depth = 0;
    int loop_depth_check = 0;
    int fn_depth = 0;
    int class_depth = 0;

    for (int i = 0; i < line_count; i++) {
        char temp[MAX_LINE];
        strcpy(temp, lines[i]);
        trim(temp);
        if (strlen(temp) == 0 || temp[0] == '#') continue;

        char cmd[MAX_NAME];
        sscanf(temp, "%127s", cmd);

        if (strcasecmp(cmd, "IF") == 0) if_depth++;
        else if (strcasecmp(cmd, "ENDIF") == 0) if_depth--;
        else if (strcasecmp(cmd, "LOOP") == 0) loop_depth_check++;
        else if (strcasecmp(cmd, "ENDLOOP") == 0) loop_depth_check--;
        else if (strcasecmp(cmd, "FN") == 0) fn_depth++;
        else if (strcasecmp(cmd, "ENDFN") == 0) fn_depth--;
        else if (strcasecmp(cmd, "CLASS") == 0) class_depth++;
        else if (strcasecmp(cmd, "ENDCLASS") == 0) class_depth--;

        if (if_depth < 0) { fprintf(stderr, "Syntax Error: Unexpected ENDIF at line %d\n", i+1); exit(1); }
        if (loop_depth_check < 0) { fprintf(stderr, "Syntax Error: Unexpected ENDLOOP at line %d\n", i+1); exit(1); }
        if (fn_depth < 0) { fprintf(stderr, "Syntax Error: Unexpected ENDFN at line %d\n", i+1); exit(1); }
        if (class_depth < 0) { fprintf(stderr, "Syntax Error: Unexpected ENDCLASS at line %d\n", i+1); exit(1); }
    }

    if (if_depth > 0) { fprintf(stderr, "Syntax Error: Missing ENDIF detected\n"); exit(1); }
    if (loop_depth_check > 0) { fprintf(stderr, "Syntax Error: Missing ENDLOOP detected\n"); exit(1); }
    if (fn_depth > 0) { fprintf(stderr, "Syntax Error: Missing ENDFN detected\n"); exit(1); }
    if (class_depth > 0) { fprintf(stderr, "Syntax Error: Missing ENDCLASS detected\n"); exit(1); }
}

// ============ Command Handlers ============

void skip_to_endif(void);
void skip_to_else_or_elif_or_endif(void);
void execute_line(const char* line);

void cmd_ec(const char* args) {
    char name[MAX_NAME], rest[MAX_LINE] = "";
    if (sscanf(args, "%127s %[^\n]", name, rest) < 1) {
        runtime_error("EC requires variable name");
    }
    
    ECVar* v = get_or_create_var(name);
    if (strlen(rest) > 0) {
        trim(rest);
        if (rest[0] == '"') {
            v->type = TYPE_STRING;
            char* end = strrchr(rest, '"');
            if (end && end != rest) {
                int len = end - rest - 1;
                strncpy(v->str_val, rest + 1, len);
                v->str_val[len] = '\0';
            }
        } else {
            v->type = TYPE_NUMBER;
            v->num_val = evaluate_expr(rest);
        }
    }
}

void cmd_set(const char* args) {
    char name[MAX_NAME], rest[MAX_LINE];
    if (sscanf(args, "%127s %[^\n]", name, rest) < 2) {
        runtime_error("SET requires variable and value");
    }
    
    char* bracket = strchr(name, '[');
    if (bracket) {
        char arr_name[MAX_NAME];
        int name_len = bracket - name;
        strncpy(arr_name, name, name_len);
        arr_name[name_len] = '\0';
        
        char* end_bracket = strchr(bracket, ']');
        if (end_bracket) {
            char idx_str[MAX_NAME];
            int idx_len = end_bracket - bracket - 1;
            strncpy(idx_str, bracket + 1, idx_len);
            idx_str[idx_len] = '\0';
            
            int arr_idx = (int)evaluate_expr(idx_str);
            int var_idx = find_var(arr_name);
            
            if (var_idx < 0) runtime_error("Undefined array '%s'", arr_name);
            if (vars[var_idx].arr_id < 0) runtime_error("Variable '%s' is not an array", arr_name);
            
            ECArray* arr = &arrays[vars[var_idx].arr_id];
            if (arr_idx < 0 || arr_idx >= arr->size) {
                 runtime_error("Array assignment index out of bounds: %d", arr_idx);
            }
            arr->num_data[arr_idx] = evaluate_expr(rest);
        }
        return;
    }
    
    ECVar* v = get_var_checked(name); // Must exist
    trim(rest);
    if (rest[0] == '"') {
        v->type = TYPE_STRING;
        char* end = strrchr(rest, '"');
        if (end && end != rest) {
            int len = end - rest - 1;
            strncpy(v->str_val, rest + 1, len);
            v->str_val[len] = '\0';
        }
    } else {
        v->type = TYPE_NUMBER;
        v->num_val = evaluate_expr(rest);
    }
}

void cmd_arr(const char* args) {
    char name[MAX_NAME]; int size;
    if (sscanf(args, "%127s %d", name, &size) < 2) {
        runtime_error("ARR requires name and size");
    }
    if (size <= 0) runtime_error("Array size must be positive");
    if (array_count >= MAX_ARRAYS) runtime_error("Too many arrays");
    
    ECArray* arr = &arrays[array_count];
    arr->num_data = (double*)calloc(size, sizeof(double));
    arr->str_data = NULL;
    arr->size = size;
    arr->capacity = size;
    arr->elem_type = TYPE_NUMBER;
    
    ECVar* v = get_or_create_var(name);
    v->type = TYPE_ARRAY;
    v->arr_id = array_count++;
}

void cmd_out(const char* args) {
    char buf[MAX_LINE];
    strncpy(buf, args, MAX_LINE - 1);
    trim(buf);
    
    char output[MAX_LINE * 4] = "";
    char* token = buf;
    char* plus;
    
    while ((plus = strstr(token, " + ")) != NULL) {
        char part[MAX_LINE];
        int len = plus - token;
        strncpy(part, token, len);
        part[len] = '\0';
        trim(part);
        
        char val[MAX_LINE];
        get_string_value(part, val);
        strcat(output, val);
        
        token = plus + 3;
    }
    
    char val[MAX_LINE];
    get_string_value(token, val);
    strcat(output, val);
    
    printf("%s\n", output);
}

void cmd_in(const char* args) {
    char name[MAX_NAME], prompt[MAX_LINE] = "";
    if (sscanf(args, "%127s %[^\n]", name, prompt) < 1) {
        runtime_error("IN requires variable name");
    }
    
    if (strlen(prompt) > 0) {
        trim(prompt);
        if (prompt[0] == '"') {
            prompt[strlen(prompt) - 1] = '\0';
            printf("%s", prompt + 1);
        }
    }
    
    char input[MAX_LINE];
    if (fgets(input, MAX_LINE, stdin)) {
        input[strcspn(input, "\n")] = '\0';
        ECVar* v = get_or_create_var(name);
        if (is_number(input)) { v->type = TYPE_NUMBER; v->num_val = atof(input); }
        else { v->type = TYPE_STRING; strcpy(v->str_val, input); }
    }
}

void cmd_if(const char* args) { if (!evaluate_condition(args)) skip_to_else_or_elif_or_endif(); }
void cmd_elif(const char* args) { skip_to_endif(); }
void cmd_else(const char* args) { skip_to_endif(); }

void cmd_loop(const char* args) {
    loop_start[loop_depth] = current_line;
    
    int depth = 1, end = current_line + 1;
    while (end < line_count && depth > 0) {
        char temp[MAX_LINE]; strcpy(temp, lines[end]); trim(temp);
        char cmd[MAX_NAME];
        if (sscanf(temp, "%127s", cmd) == 1) {
            if (strcasecmp(cmd, "LOOP") == 0) depth++;
            else if (strcasecmp(cmd, "ENDLOOP") == 0) depth--;
        }
        end++;
    }
    if (depth > 0) runtime_error("Missing ENDLOOP for LOOP at line %d", current_line+1);
    loop_end[loop_depth] = end - 1;
    
    if (strlen(args) > 0 && !evaluate_condition(args)) {
        current_line = loop_end[loop_depth];
        return;
    }
    loop_depth++;
}

void cmd_endloop(const char* args) {
    if (loop_depth > 0) { loop_depth--; current_line = loop_start[loop_depth] - 1; }
}

void cmd_break(const char* args) {
    if (loop_depth > 0) { loop_depth--; current_line = loop_end[loop_depth]; }
}

void cmd_continue(const char* args) {
    if (loop_depth > 0) current_line = loop_start[loop_depth - 1] - 1;
}

void cmd_fn(const char* args) {
    char name[MAX_NAME], params[MAX_LINE] = "";
    char* paren = strchr(args, '(');
    if (paren) {
        int name_len = paren - args;
        strncpy(name, args, name_len);
        name[name_len] = '\0'; trim(name);
        char* end_paren = strchr(paren, ')');
        if (end_paren) {
            int param_len = end_paren - paren - 1;
            strncpy(params, paren + 1, param_len);
            params[param_len] = '\0';
        }
    } else sscanf(args, "%127s", name);
    
    ECFunc* fn = &funcs[func_count];
    strncpy(fn->name, name, MAX_NAME - 1);
    fn->start_line = current_line;
    fn->param_count = 0;
    
    if (strlen(params) > 0) {
        char* tok = strtok(params, ",");
        while (tok && fn->param_count < 16) {
            trim(tok);
            strncpy(fn->params[fn->param_count++], tok, MAX_NAME - 1);
            tok = strtok(NULL, ",");
        }
    }
    
    int depth = 1, end = current_line + 1;
    while (end < line_count && depth > 0) {
        char temp[MAX_LINE]; strcpy(temp, lines[end]); trim(temp);
        char cmd[MAX_NAME];
        if (sscanf(temp, "%127s", cmd) == 1) {
            if (strcasecmp(cmd, "FN") == 0) depth++;
            else if (strcasecmp(cmd, "ENDFN") == 0) depth--;
        }
        end++;
    }
    if (depth > 0) runtime_error("Missing ENDFN for FN %s", name);
    fn->end_line = end - 1;
    func_count++;
    current_line = fn->end_line;
}

void cmd_call(const char* args) {
    char name[MAX_NAME], params[MAX_LINE] = "";
    char* paren = strchr(args, '(');
    if (paren) {
        int name_len = paren - args;
        strncpy(name, args, name_len);
        name[name_len] = '\0'; trim(name);
        char* end_paren = strchr(paren, ')');
        if (end_paren) {
            int param_len = end_paren - paren - 1;
            strncpy(params, paren + 1, param_len);
            params[param_len] = '\0';
        }
    } else sscanf(args, "%127s %[^\n]", name, params);
    
    int fn_idx = find_func(name);
    if (fn_idx < 0) {
        runtime_error("Function '%s' not found", name);
    }
    
    ECFunc* fn = &funcs[fn_idx];
    if (strlen(params) > 0 && fn->param_count > 0) {
        char params_copy[MAX_LINE]; strcpy(params_copy, params);
        char* tok = strtok(params_copy, ",");
        int i = 0;
        while (tok && i < fn->param_count) {
            trim(tok);
            ECVar* v = get_or_create_var(fn->params[i]);
            if (tok[0] == '"') {
                v->type = TYPE_STRING;
                tok[strlen(tok) - 1] = '\0';
                strcpy(v->str_val, tok + 1);
            } else {
                v->type = TYPE_NUMBER;
                v->num_val = evaluate_expr(tok);
            }
            tok = strtok(NULL, ",");
            i++;
        }
    }
    
    call_stack[call_stack_top] = current_line;
    
    // Debug stack info
    debug_stack[call_stack_top].line_num = current_line;
    strncpy(debug_stack[call_stack_top].func_name, "Global/Previous", MAX_NAME);
    
    call_stack_top++;
    in_function++;
    has_return = 0;
    current_line = fn->start_line;
}

void cmd_ret(const char* args) {
    if (strlen(args) > 0) { return_value = evaluate_expr(args); has_return = 1; }
    if (call_stack_top > 0) { current_line = call_stack[--call_stack_top]; in_function--; }
}

void cmd_class(const char* args) {
    char name[MAX_NAME];
    sscanf(args, "%127s", name);
    
    ECClass* cls = &classes[class_count];
    strncpy(cls->name, name, MAX_NAME - 1);
    cls->start_line = current_line;
    cls->member_count = 0;
    cls->method_count = 0;
    
    int depth = 1, end = current_line + 1;
    while (end < line_count && depth > 0) {
        char temp[MAX_LINE]; strcpy(temp, lines[end]); trim(temp);
        char cmd[MAX_NAME];
        if (sscanf(temp, "%127s", cmd) == 1) {
            if (strcasecmp(cmd, "CLASS") == 0) depth++;
            else if (strcasecmp(cmd, "ENDCLASS") == 0) depth--;
        }
        end++;
    }
    if (depth > 0) runtime_error("Missing ENDCLASS for CLASS %s", name);
    cls->end_line = end - 1;
    class_count++;
    current_line = cls->end_line;
}

void cmd_new(const char* args) {
    char var_name[MAX_NAME], class_name[MAX_NAME];
    if (sscanf(args, "%127s %127s", var_name, class_name) < 2) {
        runtime_error("NEW requires variable and class name");
    }
    int cls_idx = find_class(class_name);
    if (cls_idx < 0) {
        runtime_error("Class '%s' not found", class_name);
    }
    ECVar* v = get_or_create_var(var_name);
    v->type = TYPE_OBJECT;
    v->obj_class_id = cls_idx;
}

void cmd_add(const char* args) {
    char dest[MAX_NAME], rest[MAX_LINE];
    if (sscanf(args, "%127s %[^\n]", dest, rest) < 2) runtime_error("ADD requires variable and value");
    ECVar* v = get_var_checked(dest);
    v->num_val += evaluate_expr(rest);
    v->type = TYPE_NUMBER;
}

void cmd_sub(const char* args) {
    char dest[MAX_NAME], rest[MAX_LINE];
    if (sscanf(args, "%127s %[^\n]", dest, rest) < 2) runtime_error("SUB requires variable and value");
    ECVar* v = get_var_checked(dest);
    v->num_val -= evaluate_expr(rest);
    v->type = TYPE_NUMBER;
}

void cmd_mul(const char* args) {
    char dest[MAX_NAME], rest[MAX_LINE];
    if (sscanf(args, "%127s %[^\n]", dest, rest) < 2) runtime_error("MUL requires variable and value");
    ECVar* v = get_var_checked(dest);
    v->num_val *= evaluate_expr(rest);
    v->type = TYPE_NUMBER;
}

void cmd_div(const char* args) {
    char dest[MAX_NAME], rest[MAX_LINE];
    if (sscanf(args, "%127s %[^\n]", dest, rest) < 2) runtime_error("DIV requires variable and value");
    double divisor = evaluate_expr(rest);
    if (divisor == 0) runtime_error("Division by zero");
    ECVar* v = get_var_checked(dest);
    v->num_val /= divisor;
    v->type = TYPE_NUMBER;
}

void cmd_mod(const char* args) {
    char dest[MAX_NAME], rest[MAX_LINE];
    if (sscanf(args, "%127s %[^\n]", dest, rest) < 2) runtime_error("MOD requires variable and value");
    ECVar* v = get_var_checked(dest);
    v->num_val = fmod(v->num_val, evaluate_expr(rest));
    v->type = TYPE_NUMBER;
}

// ============ External Execution ============

void cmd_exec(const char* args) {
    // EXEC "command" [result_var]
    char command[MAX_LINE] = "", result_var[MAX_NAME] = "";
    
    if (args[0] == '"') {
        char* end = strchr(args + 1, '"');
        if (end) {
            int len = end - args - 1;
            strncpy(command, args + 1, len);
            command[len] = '\0';
            
            char* rest = end + 1;
            while (*rest && isspace(*rest)) rest++;
            if (*rest) sscanf(rest, "%127s", result_var);
        }
    } else {
        sscanf(args, "%[^\n]", command);
    }
    
    last_exec_output[0] = '\0';
    FILE* fp = popen(command, "r");
    if (fp) {
        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, fp)) {
            if (strlen(last_exec_output) + strlen(line) < sizeof(last_exec_output) - 1)
                strcat(last_exec_output, line);
        }
        pclose(fp);
        
        // Remove trailing newline
        int len = strlen(last_exec_output);
        if (len > 0 && last_exec_output[len-1] == '\n') last_exec_output[len-1] = '\0';
    }
    
    if (strlen(result_var) > 0) {
        ECVar* v = get_or_create_var(result_var);
        v->type = TYPE_STRING;
        strcpy(v->str_val, last_exec_output);
    }
}

void cmd_pyrun(const char* args) {
    // PYRUN "script.py" [func_name] [args...] [result_var]
    char script[MAX_LINE] = "", func[MAX_NAME] = "", py_args[MAX_LINE] = "", result_var[MAX_NAME] = "";
    
    char* ptr = (char*)args;
    while (*ptr && isspace(*ptr)) ptr++;
    
    if (*ptr == '"') {
        char* end = strchr(ptr + 1, '"');
        if (end) {
            int len = end - ptr - 1;
            strncpy(script, ptr + 1, len);
            script[len] = '\0';
            ptr = end + 1;
        }
    }
    
    while (*ptr && isspace(*ptr)) ptr++;
    
    // Parse remaining: func(args) result_var  OR just result_var
    char* paren = strchr(ptr, '(');
    if (paren) {
        int len = paren - ptr;
        strncpy(func, ptr, len);
        func[len] = '\0';
        trim(func);
        
        char* end_paren = strchr(paren, ')');
        if (end_paren) {
            int arg_len = end_paren - paren - 1;
            strncpy(py_args, paren + 1, arg_len);
            py_args[arg_len] = '\0';
            
            ptr = end_paren + 1;
            while (*ptr && isspace(*ptr)) ptr++;
            if (*ptr) sscanf(ptr, "%127s", result_var);
        }
    } else if (*ptr) {
        sscanf(ptr, "%127s", result_var);
    }
    
    char command[MAX_LINE * 2];
    if (strlen(func) > 0) {
        sprintf(command, "python -c \"import sys; sys.path.insert(0, '.'); from %.*s import %s; print(%s(%s))\"",
                (int)(strrchr(script, '.') ? strrchr(script, '.') - script : strlen(script)),
                script, func, func, py_args);
    } else {
        sprintf(command, "python \"%s\"", script);
    }
    
    last_exec_output[0] = '\0';
    FILE* fp = popen(command, "r");
    if (fp) {
        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, fp)) {
            if (strlen(last_exec_output) + strlen(line) < sizeof(last_exec_output) - 1)
                strcat(last_exec_output, line);
        }
        pclose(fp);
        int len = strlen(last_exec_output);
        if (len > 0 && last_exec_output[len-1] == '\n') last_exec_output[len-1] = '\0';
    }
    
    if (strlen(result_var) > 0) {
        ECVar* v = get_or_create_var(result_var);
        if (is_number(last_exec_output)) {
            v->type = TYPE_NUMBER;
            v->num_val = atof(last_exec_output);
        } else {
            v->type = TYPE_STRING;
            strcpy(v->str_val, last_exec_output);
        }
    }
}

void cmd_crun(const char* args) {
    // CRUN "source.c" [result_var]
    // Compiles and runs C source, captures output
    char source[MAX_LINE] = "", result_var[MAX_NAME] = "";
    
    char* ptr = (char*)args;
    while (*ptr && isspace(*ptr)) ptr++;
    
    if (*ptr == '"') {
        char* end = strchr(ptr + 1, '"');
        if (end) {
            int len = end - ptr - 1;
            strncpy(source, ptr + 1, len);
            source[len] = '\0';
            ptr = end + 1;
        }
    }
    
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr) sscanf(ptr, "%127s", result_var);
    
    char command[MAX_LINE * 2];
    #ifdef _WIN32
        char temp_exe[] = "__ec_temp.exe";
        sprintf(command, "gcc -o %s \"%s\" -lm 2>&1 && %s", temp_exe, source, temp_exe);
    #else
        char temp_exe[] = "./__ec_temp";
        sprintf(command, "gcc -o %s \"%s\" -lm 2>&1 && %s", temp_exe, source, temp_exe);
    #endif
    
    last_exec_output[0] = '\0';
    FILE* fp = popen(command, "r");
    if (fp) {
        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, fp)) {
            if (strlen(last_exec_output) + strlen(line) < sizeof(last_exec_output) - 1)
                strcat(last_exec_output, line);
        }
        pclose(fp);
        int len = strlen(last_exec_output);
        if (len > 0 && last_exec_output[len-1] == '\n') last_exec_output[len-1] = '\0';
    }
    
    // Cleanup temp file
    #ifdef _WIN32
        remove("__ec_temp.exe");
    #else
        remove("./__ec_temp");
    #endif
    
    if (strlen(result_var) > 0) {
        ECVar* v = get_or_create_var(result_var);
        if (is_number(last_exec_output)) {
            v->type = TYPE_NUMBER;
            v->num_val = atof(last_exec_output);
        } else {
            v->type = TYPE_STRING;
            strcpy(v->str_val, last_exec_output);
        }
    }
}

// ============ Control Flow Helpers ============

void skip_to_endif(void) {
    int depth = 1;
    while (current_line < line_count - 1 && depth > 0) {
        current_line++;
        char temp[MAX_LINE]; strcpy(temp, lines[current_line]); trim(temp);
        char cmd[MAX_NAME];
        if (sscanf(temp, "%127s", cmd) == 1) {
            if (strcasecmp(cmd, "IF") == 0) depth++;
            else if (strcasecmp(cmd, "ENDIF") == 0) depth--;
        }
    }
}

void skip_to_else_or_elif_or_endif(void) {
    int depth = 1;
    while (current_line < line_count - 1 && depth > 0) {
        current_line++;
        char temp[MAX_LINE]; strcpy(temp, lines[current_line]); trim(temp);
        char cmd[MAX_NAME], rest[MAX_LINE] = "";
        if (sscanf(temp, "%127s %[^\n]", cmd, rest) >= 1) {
            if (strcasecmp(cmd, "IF") == 0) depth++;
            else if (strcasecmp(cmd, "ENDIF") == 0) depth--;
            else if (depth == 1) {
                if (strcasecmp(cmd, "ELSE") == 0) return;
                else if (strcasecmp(cmd, "ELIF") == 0) {
                    if (evaluate_condition(rest)) return;
                }
            }
        }
    }
}

// ============ Main Execution ============

void execute_line(const char* line) {
    char buf[MAX_LINE];
    strncpy(buf, line, MAX_LINE - 1);
    buf[MAX_LINE - 1] = '\0';
    trim(buf);
    
    if (strlen(buf) == 0 || buf[0] == '#' || (buf[0] == '/' && buf[1] == '/')) return;
    
    char cmd[MAX_NAME], args[MAX_LINE] = "";
    sscanf(buf, "%127s %[^\n]", cmd, args);
    trim(args);
    
    if (strcasecmp(cmd, "EC") == 0) cmd_ec(args);
    else if (strcasecmp(cmd, "SET") == 0) cmd_set(args);
    else if (strcasecmp(cmd, "ARR") == 0) cmd_arr(args);
    else if (strcasecmp(cmd, "OUT") == 0) cmd_out(args);
    else if (strcasecmp(cmd, "IN") == 0) cmd_in(args);
    else if (strcasecmp(cmd, "IF") == 0) cmd_if(args);
    else if (strcasecmp(cmd, "ELIF") == 0) cmd_elif(args);
    else if (strcasecmp(cmd, "ELSE") == 0) cmd_else(args);
    else if (strcasecmp(cmd, "ENDIF") == 0) { }
    else if (strcasecmp(cmd, "LOOP") == 0) cmd_loop(args);
    else if (strcasecmp(cmd, "ENDLOOP") == 0) cmd_endloop(args);
    else if (strcasecmp(cmd, "BREAK") == 0) cmd_break(args);
    else if (strcasecmp(cmd, "CONTINUE") == 0) cmd_continue(args);
    else if (strcasecmp(cmd, "FN") == 0) cmd_fn(args);
    else if (strcasecmp(cmd, "ENDFN") == 0) {
        if (call_stack_top > 0) { current_line = call_stack[--call_stack_top]; in_function--; }
    }
    else if (strcasecmp(cmd, "CALL") == 0) cmd_call(args);
    else if (strcasecmp(cmd, "RET") == 0) cmd_ret(args);
    else if (strcasecmp(cmd, "CLASS") == 0) cmd_class(args);
    else if (strcasecmp(cmd, "ENDCLASS") == 0) { }
    else if (strcasecmp(cmd, "NEW") == 0) cmd_new(args);
    else if (strcasecmp(cmd, "ADD") == 0) cmd_add(args);
    else if (strcasecmp(cmd, "SUB") == 0) cmd_sub(args);
    else if (strcasecmp(cmd, "MUL") == 0) cmd_mul(args);
    else if (strcasecmp(cmd, "DIV") == 0) cmd_div(args);
    else if (strcasecmp(cmd, "MOD") == 0) cmd_mod(args);
    else if (strcasecmp(cmd, "EXEC") == 0) cmd_exec(args);
    else if (strcasecmp(cmd, "PYRUN") == 0) cmd_pyrun(args);
    else if (strcasecmp(cmd, "CRUN") == 0) cmd_crun(args);
    else if (strcasecmp(cmd, "END") == 0) running = 0;
    else runtime_error("Unknown command '%s'", cmd);
}

int load_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) { fprintf(stderr, "Error: Cannot open file '%s'\n", filename); return 0; }
    
    char buf[MAX_LINE];
    int count = 0;
    while (fgets(buf, MAX_LINE, f)) count++;
    rewind(f);
    
    lines = (char**)malloc(count * sizeof(char*));
    line_count = 0;
    
    while (fgets(buf, MAX_LINE, f)) {
        buf[strcspn(buf, "\r\n")] = '\0';
        lines[line_count] = (char*)malloc(strlen(buf) + 1);
        strcpy(lines[line_count], buf);
        line_count++;
    }
    fclose(f);
    return 1;
}

void cleanup(void) {
    for (int i = 0; i < line_count; i++) free(lines[i]);
    free(lines);
    for (int i = 0; i < array_count; i++) {
        free(arrays[i].num_data);
        if (arrays[i].str_data) {
            for (int j = 0; j < arrays[i].size; j++) free(arrays[i].str_data[j]);
            free(arrays[i].str_data);
        }
    }
}

void print_help(void) {
    printf("EC Language Interpreter v1.2.0\n");
    printf("Usage: EC <filename.ec>\n\n");
    printf("Options:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -v, --version  Show version information\n");
}

void print_version(void) {
    printf("EC Language Interpreter\n");
    printf("Version: 1.2.0 (Solid Core)\n");
    printf("Build Date: 2026-01-25\n");
    printf("Features: OOP, Arrays, External Exec, Syntax Validation, Stack Trace\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) { print_help(); return 1; }
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) { print_help(); return 0; }
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) { print_version(); return 0; }
    
    if (!load_file(argv[1])) return 1;

    // Phase 0: Static Syntax Analysis
    validate_syntax();
    
    // Phase 1: Register functions and classes
    for (current_line = 0; current_line < line_count && running; current_line++) {
        char buf[MAX_LINE]; strcpy(buf, lines[current_line]); trim(buf);
        char cmd[MAX_NAME];
        if (sscanf(buf, "%127s", cmd) == 1) {
            if (strcasecmp(cmd, "FN") == 0 || strcasecmp(cmd, "CLASS") == 0)
                execute_line(lines[current_line]);
        }
    }
    
    running = 1;
    
    // Phase 2: Execute
    for (current_line = 0; current_line < line_count && running; current_line++) {
        char buf[MAX_LINE]; strcpy(buf, lines[current_line]); trim(buf);
        char cmd[MAX_NAME];
        if (sscanf(buf, "%127s", cmd) == 1) {
            if (strcasecmp(cmd, "FN") == 0) {
                // Skip function definition manually
                int depth = 1;
                int end = current_line + 1;
                while (end < line_count && depth > 0) {
                    char temp[MAX_LINE]; strcpy(temp, lines[end]); trim(temp);
                    char c[MAX_NAME];
                    if (sscanf(temp, "%127s", c) == 1) {
                        if (strcasecmp(c, "FN") == 0) depth++;
                        else if (strcasecmp(c, "ENDFN") == 0) depth--;
                    }
                    end++;
                }
                current_line = end - 1;
            } 
            else if (strcasecmp(cmd, "CLASS") == 0) {
                // Skip class definition manually
                int depth = 1;
                int end = current_line + 1;
                while (end < line_count && depth > 0) {
                    char temp[MAX_LINE]; strcpy(temp, lines[end]); trim(temp);
                    char c[MAX_NAME];
                    if (sscanf(temp, "%127s", c) == 1) {
                        if (strcasecmp(c, "CLASS") == 0) depth++;
                        else if (strcasecmp(c, "ENDCLASS") == 0) depth--;
                    }
                    end++;
                }
                current_line = end - 1;
            } 
            else {
                execute_line(lines[current_line]);
            }
        }
    }
    
    cleanup();
    return 0;
}
