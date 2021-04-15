#define MaxLineLen 1000

//PatNode to store count occurrence of pattern in a line
typedef struct PatNode {
    int patNum; //count of pattern within line
    struct PatNode *next;
}PatNode;

typedef PatNode *Patptr;

//DLL Used as it gives Insertion and Deletuon Using Tail and Head Respectively in O(1) time
//Node to store line having the pattern, line number - Doubly Linked list
typedef struct Node{
    char line[MaxLineLen]; //store line where pattern is present
    int lineNum; //line number where pattern is present
    PatNode *PatHead; //Head Part of Doubly Linked list Node
    PatNode *PatTail; //Tail Part of Doubly Linked list Node
    struct Node *next;
}Node;

typedef Node* Nodeptr;

//Doubly Linked List Functions
void init_ll(Nodeptr l);
Nodeptr appendAtTail_ll(Nodeptr tail, char *line, int lineNum, int patNum);
Nodeptr removeFromHead_ll(Nodeptr head, char *line, int *lineNum, Patptr *patNum);
void print_ll(Nodeptr head);
void delete_ll(Nodeptr head);


//Grep command functions
void print_usage();
void printLines(Node *head);
int strcicmp(char const *a, char const *b);
int isFile(char* filename);
void d_grep(char *filename, char *pattern);
void longest_line_pattern(char *filename, char *pattern);
void c_grep(char *filename, char *pattern);
void h_grep(char *filename, char *pattern);
void v_grep(char *filename, char *pattern);
void i_grep(char *filename, char *pattern);
int l_grep(char *filename, char *pattern);
void w_grep(char *filename, char *pattern);
void o_grep(char *filename, char *pattern);
void n_grep(char *filename, char *pattern);
void x_grep(char *filename, char *pattern);

