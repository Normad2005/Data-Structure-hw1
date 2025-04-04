#include <stdio.h>
#include <string.h>
#include "Mymap.h"

char s[100000];
int idx = 0;

int read(FILE *fptr) {
    if (fgets(s, sizeof(s), fptr) == NULL) {
        return EOF;
    }
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
    strcat(s, ")");
    return 1;
}

void cal(StringMap *map, int cur) {
    while (s[idx] == ')') idx++;
    if (idx >= strlen(s)) return;

    while (idx < strlen(s) && s[idx] != ')') {
        if (s[idx] >= 'A' && s[idx] <= 'Z') {
            char atom[100] = {'\0'};
            int id = 0, num = 0;
            atom[id++] = s[idx++];

            while (s[idx] >= 'a' && s[idx] <= 'z') {
                atom[id++] = s[idx++];
            }

            while (s[idx] >= '0' && s[idx] <= '9') {
                num = num * 10 + (s[idx] - '0');
                idx++;
            }
            if (num == 0) num = 1;

            int *c = find(map, atom);
            if (c == NULL) {
                insert(map, atom, num * cur);
            } else {
                insert(map, atom, *c + num * cur);
            }
        } else if (s[idx] == '(') {
            idx++;
            int start = idx;
            int stk = 1;
            while (stk > 0) {
                if (s[idx] == '(') stk++;
                else if (s[idx] == ')') stk--;
                idx++;
            }
            int end = idx - 1;

            int mul = 0;
            while (s[idx] >= '0' && s[idx] <= '9') {
                mul = mul * 10 + (s[idx] - '0');
                idx++;
            }
            if (mul == 0) mul = 1;

            int saved_idx = idx;
            idx = start;
            cal(map, cur * mul);
            idx = saved_idx;
        } else {
            break;
        }
    }
}

int main() {
    FILE *fptr = fopen("testcase1.txt", "r");
    if (!fptr) {
        printf("IO error\n");
        return 1;
    }

    FILE *out = fopen("output1.txt", "w");
    if (!out) {
        printf("Failed to open output file\n");
        fclose(fptr);
        return 1;
    }

    while (read(fptr) != EOF) {
        StringMap m;
        init_map(&m);

        idx = 0;
        cal(&m, 1);

        int mapSize = m.size;
        for (int i = 0; i < mapSize; i++) {
            fprintf(out, "%s", m.data[i].key);
            if (m.data[i].value > 1) {
                fprintf(out, "%d", m.data[i].value);
            }
        }
        fprintf(out, "\n");
    }

    fclose(fptr);
    fclose(out);
    return 0;
}
