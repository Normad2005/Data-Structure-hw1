#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 100  // 初始容量
#define MAX_STRING_LENGTH 10000  // 最大字串長度

// 定義鍵值對 (string -> int)
typedef struct {
    char key[MAX_STRING_LENGTH];
    int value;
} MapEntry;

// 定義 Map 結構
typedef struct {
    MapEntry *data;
    int size;  // 當前大小
    int capacity;  // 總容量
} StringMap;

void init_map(StringMap *map) {
    map->size = 0;
    map->capacity = INITIAL_SIZE;
    map->data = (MapEntry *)malloc(map->capacity * sizeof(MapEntry));
    if (!map->data) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
}

void free_map(StringMap *map) {
    free(map->data);
}

// 按 key 升冪排序
int compare_entries(const void *a, const void *b) {
    return strcmp(((MapEntry *)a)->key, ((MapEntry *)b)->key);
}

void insert(StringMap *map, const char *key, int value) {
    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->data[i].key, key) == 0) {
            map->data[i].value = value;  // 更新值
            return;
        }
    }
    if (map->size == map->capacity) {
        map->capacity *= 2;
        map->data = (MapEntry *)realloc(map->data, map->capacity * sizeof(MapEntry));
        if (!map->data) {
            fprintf(stderr, "Memory reallocation failed!\n");
            exit(1);
        }
    }
    strcpy(map->data[map->size].key, key);
    map->data[map->size].value = value;
    map->size++;
    qsort(map->data, map->size, sizeof(MapEntry), compare_entries);
}

// 回傳指標，若不存在則回傳 NULL
int *find(StringMap *map, const char *key) {
    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->data[i].key, key) == 0) {
            return &map->data[i].value;
        }
    }
    return NULL;
}

