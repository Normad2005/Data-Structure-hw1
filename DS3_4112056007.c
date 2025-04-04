#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_QUEUE_SIZE 1000000

typedef struct Position{
    int x;
    int y;
}Position;

Position parent[1005][1005]; // 記錄前驅節點
Position queue[MAX_QUEUE_SIZE];

char arr[1005][1005];
char visited[1005][1005];
Position flash[1005][1005];
int N, M;
int Sx, Sy, Gx, Gy;
int front = 0, rear = 0;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

int inside(int x, int y);
void block(int x, int y, FILE *out);
void unblock(int x, int y, FILE *out);
void print_maze(FILE *out);
void portal(int x1, int y1, int x2, int y2, FILE *out);
void bfs(FILE *out);

int main(){
    FILE *fptr = fopen("testcase3.txt", "r");
    FILE *out = fopen("output3.txt", "w");
    if (!fptr || !out) {
        printf("IO error\n");
        return 1;
    }

    memset(visited, 0, sizeof(visited));
    fscanf(fptr, "%d %d", &N, &M);
    fgetc(fptr);
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            arr[i][j] = fgetc(fptr);
            if(arr[i][j] == 'S'){
                Sx = i;
                Sy = j;
            }
            if(arr[i][j] == 'G'){
                Gx = i;
                Gy = j;
            }
        }fgetc(fptr);
    }

    int n;
    fscanf(fptr, "%d", &n);

    for(int i=0;i<n;i++){
        char input[15];
        fscanf(fptr, "%s", &input);

        if(!strcmp(input, "block")){
            int x, y;
            fscanf(fptr, "%d%d", &x, &y);
            block(x, y, out);  // Pass the output file pointer to block
        }

        if(!strcmp(input, "unblock")){
            int x, y;
            fscanf(fptr, "%d%d", &x, &y);
            unblock(x, y, out);  // Pass the output file pointer to unblock
        }

        if(!strcmp(input, "portal")){
            int x1, y1, x2, y2;
            fscanf(fptr, "%d%d%d%d", &x1, &y1, &x2, &y2);
            portal(x1, y1, x2, y2, out);  // Pass the output file pointer to portal
        }

        if(!strcmp(input, "path")){
            bfs(out);  // Pass the output file pointer to bfs
        }

        if(!strcmp(input, "print")){
            print_maze(out);  // Pass the output file pointer to print_maze
        }   
    }

    fclose(fptr);
    fclose(out);
    return 0;
}

int inside(int x, int y){
    return (x < N && y < M) && (x >= 0 && y >= 0);
}

void block(int x, int y, FILE *out){
    if(!inside(x, y)){
        fprintf(out, "INVALID OPERATION\n");
        return;
    }

    if(arr[x][y] == 'G' || arr[x][y] == 'P' || arr[x][y] == 'S'){
        fprintf(out, "INVALID OPERATION\n");
    }
    else
        arr[x][y] = '#';
}

void unblock(int x, int y, FILE *out){
    if(!inside(x, y)){
        fprintf(out, "INVALID OPERATION\n");
        return;
    }
    if(arr[x][y] == 'G' || arr[x][y] == 'P' || arr[x][y] == 'S'){
        fprintf(out, "INVALID OPERATION\n");
    }
    else
        arr[x][y] = '.';
}

void portal(int x1, int y1, int x2, int y2, FILE *out) {
    if (!inside(x1, y1) || !inside(x2, y2)) {
        fprintf(out, "INVALID OPERATION\n");
        return;
    }
    flash[x1][y1].x = x2;
    flash[x1][y1].y = y2;
    flash[x2][y2].x = x1;
    flash[x2][y2].y = y1;
    arr[x1][y1] = 'P';
    arr[x2][y2] = 'P';
}

void print_maze(FILE *out){
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            fprintf(out, "%c", arr[i][j]);
        }
        fprintf(out, "\n");
    }
}

void bfs(FILE *out) {
    int length = 0;
    front = 0;
    rear = 0;
    memset(visited, 0, sizeof(visited));
    
    queue[rear++] = (Position){Sx, Sy};
    visited[Sx][Sy] = 1;
    parent[Sx][Sy] = (Position){-1, -1}; 

    int found = 0;
    while (front < rear) {
        Position cur = queue[front++];

        if (cur.x == Gx && cur.y == Gy) {
            found = 1;
            break;
        }
        
        if (arr[cur.x][cur.y] == 'P') {
            Position teleport = flash[cur.x][cur.y];
            int nx = teleport.x;
            int ny = teleport.y;
            if (inside(nx, ny) && !visited[nx][ny] && arr[nx][ny] != '#') {
                visited[nx][ny] = 1;
                queue[rear++] = (Position){nx, ny};
                parent[nx][ny] = parent[cur.x][cur.y];
            }
        }

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (inside(nx, ny) && !visited[nx][ny] && arr[nx][ny] != '#') {
                visited[nx][ny] = 1;
                queue[rear++] = (Position){nx, ny};
                parent[nx][ny] = cur;
            }
        }
    }

    if (!found) {
        fprintf(out, "NO PATH\n");
        return;
    }

    // 回溯路徑並標記 '*', portal標記'+'
    Position p = (Position){Gx, Gy};
    while (p.x != -1 && p.y != -1) {
        if (arr[p.x][p.y] != 'S' && arr[p.x][p.y] != 'G'){
            if(arr[p.x][p.y] == 'P'){
                arr[p.x][p.y] = '+';
                Position a = flash[p.x][p.y];
                arr[a.x][a.y] = '+';
                length += 2;
            }
            else{
                arr[p.x][p.y] = '*';
                length ++;
            }
        }
        p = parent[p.x][p.y]; // 回溯
    }

    // output
    length += 2; // 包含S跟G
    fprintf(out, "%d, ", length);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if(arr[i][j] == '*' ||arr[i][j] == '+' || arr[i][j] == 'S' || arr[i][j] == 'G'){
                if(--length){
                    fprintf(out, "(%d %d) -> ", i, j);
                }
                else{
                    fprintf(out, "(%d %d)", i, j);
                }
            }
        }
    }
    fprintf(out, "\n");

    // recover
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if(arr[i][j] == '*'){
                arr[i][j] = '.';
            }
            else if(arr[i][j] == '+')
                arr[i][j] = 'P';
        }
    }
}
