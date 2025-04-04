/*
department: CSIE
student number: 4112056007
author: 楊珽鈞
*/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#define MAX_SIZE 15005

typedef struct Process{
    int processID;
    int Arrival_time;
    int Execution_time;
    int Priority;
}Process;

typedef struct Pair{
    int ProcessID;
    int time;
}Pair;

typedef struct Node{
    int left;
    int right;
    struct Node *next;
}Node;

int compare1(const void *a, const void *b);
int compare2(const void *a, const void *b);
int nextInt(FILE *fptr);
Node *new_node(int left, int right);
void insert(Node **head, int left, int right);
void removeNode(Node *head, int L, int R);

int main(){
    FILE *fptr = fopen("testcase2.txt", "r");
    FILE *out = fopen("output2.txt", "w");
    if (!fptr || !out) {
        printf("IO error\n");
        return 1;
    }

    int testcase = nextInt(fptr);

    while(testcase--){
        Node *head = new_node(-1, 0);//head跟last都是dummy node
        Node *last = new_node(INT_MAX, INT_MAX);
        head->next = last;
        last->next = NULL;
        
        Process arr[MAX_SIZE];
        Pair ans[MAX_SIZE];
        int n = nextInt(fptr);
        for(int i=0;i<n;i++){
            Process P;
            P.processID = nextInt(fptr);
            P.Arrival_time = nextInt(fptr);
            P.Execution_time = nextInt(fptr);
            P.Priority = nextInt(fptr);
            arr[i] = P;
        }

        qsort(arr, n, sizeof(Process), compare1);

        for(int i=0;i<n;i++){
            int time = arr[i].Arrival_time;
            int end;
            int todo = arr[i].Execution_time;
            Node *cur = head-> next;

            while(todo > 0 && cur != NULL){
                if(time >= cur->left && time <= cur->right){
                    time = cur->right;
                    cur = cur->next;
                    continue;
                }
                
                int gap = cur->left - time;
                
                if(gap > 0){
                    if(gap >= todo){
                        time += todo;
                        break;
                    }
                    todo -= gap;
                    time = cur->right;
                }
                cur = cur->next;
            }
            removeNode(head, arr[i].Arrival_time, time);
            insert(&head, arr[i].Arrival_time, time);

            ans[i].ProcessID = arr[i].processID;
            ans[i].time = time;
        }

        qsort(ans, n, sizeof(Pair), compare2);

        for(int i=0;i<n;i++){
            fprintf(out, "%d %d\n", ans[i].ProcessID, ans[i].time);
        }
        fprintf(out, "\n");
    }

    fclose(fptr);
    fclose(out);
    return 0;
}

int compare1(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    
    if (p1->Priority != p2->Priority) {
        return p2->Priority - p1->Priority;  // 優先權由大到小排序
    }
    return p1->Arrival_time - p2->Arrival_time;  // 到達時間由小到大排序
}

int compare2(const void *a, const void *b){
    Pair *p1 = (Pair *)a;
    Pair *p2 = (Pair *)b;
    return p1->time - p2->time;
}

int nextInt(FILE *fptr) {
    int ch, num = 0, sign = 1;
    int result;
    int found = 0;
    while ((ch = fgetc(fptr)) != EOF && !((ch>='0' && ch<='9') || (ch == '+' || ch =='-')));

    if (ch == '-') {
        sign = -1;
        ch = fgetc(fptr);
    } else if (ch == '+') {
        ch = fgetc(fptr);
    }
    while (ch != EOF && (ch>='0' && ch<='9')) {
        found = 1;
        num = num * 10 + (ch - '0');
        ch = fgetc(fptr);
    }
    if(!found)
        printf("Integer not found\n");
    return num*sign;
}

Node *new_node(int left, int right){
    Node *new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }
    new_node->left = left;
    new_node->right = right;
    new_node->next = NULL;
    return new_node;
}

void insert(Node **head, int left, int right) { //以left為比較基準對把node插到適當位置
    Node *newNode = new_node(left, right);
    if (newNode == NULL) return;

    if (*head == NULL || (*head)->left >= left) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    Node *tmp = *head;
    while (tmp->next != NULL && tmp->next->left < left) {
        tmp = tmp->next;
    }

    newNode->next = tmp->next;
    tmp->next = newNode;
}

void removeNode(Node *head, int L, int R) {
    Node *tmp = head;

    while (tmp->next != NULL) {
        if (tmp->next->left >= L && tmp->next->right <= R) {
            Node *toDelete = tmp->next;
            tmp->next = tmp->next->next;  
            free(toDelete);  
        } else {
            tmp = tmp->next;
        }
    }
}
