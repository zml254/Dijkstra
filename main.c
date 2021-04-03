#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#define START_CITY 1
#define FILE_NAME "/Users/meaning/CLionProjects/Dijkstra/cities.txt"
typedef struct {
    int state;
    int value;
    char *path;
}Distance;

int getCityCount() {
    FILE *file;
    int cityCount;
    char string[20];
    if ((file = fopen(FILE_NAME, "r")) == NULL) {
        return -1;
    }
    fgets(string, 19, file);
    cityCount = (int) strtol(string, NULL, 10);
    fclose(file);
    return cityCount;
}

void getCitiesData(char **citiesName, int **adjacency, int cityCount) {
    FILE *file;
    int i = 0, temp;
    char string[20];
    if ((file = fopen(FILE_NAME, "r")) == NULL) {
        printf("文件打开错误!");
        return;
    }
    if (fgets(string, 19, file) == NULL) {
        printf("数据发生改变!");
        return;
    }
    while (i < cityCount) {
        char *str = malloc(sizeof(char) * 20);
        if (fscanf(file, "%s", str) == -1) {
            printf("数据读取异常!\n");
            return;
        }
        citiesName[i] = str;
        i++;
    }
    if (fgets(string, 19, file) == NULL) {
        printf("数据发生改变!");
        return;
    }
    i = 0;
    while (i < cityCount * cityCount) {
        if (fgets(string, 8, file) == NULL) {
            printf("数据读取异常!\n");
        }
        if ((temp = (int) strtol(string, NULL, 10)) == 0) {
            adjacency[i / cityCount][i % cityCount] = INT_MAX;
        } else{
            adjacency[i / cityCount][i % cityCount] = temp;
        }
        i++;
    }
    fclose(file);
}

void Dijkstra(Distance *distances, int **adjacency, char **citiesName, int cityCount, int begin) {
    int i, minIndex = 0, count = 0, min;
    for (i = 0; i < cityCount; i++) {
        char *str = malloc(sizeof(char) * 60);
        sprintf(str,"%s->%s",citiesName[begin-1],citiesName[i]);
        distances[i].value = adjacency[begin - 1][i];
        distances[i].path = str;
    }
    distances[begin - 1].value = 0;
    distances[begin - 1].state = 1;
    count++;
    while (count < cityCount) {
        min = INT_MAX;
        for (i = 0; i < cityCount; i++) {
            if (!distances[i].state && distances[i].value < min) {
                min = distances[i].value;
                minIndex = i;
            }
        }
        distances[minIndex].state = 1;
        count++;
        for (i = 0; i < cityCount; i++) {
            if (!distances[i].state && adjacency[minIndex][i] != INT_MAX &&
                (distances[minIndex].value + adjacency[minIndex][i]) < distances[i].value) {
                char *str = malloc(sizeof(char) * 60);
                distances[i].value = distances[minIndex].value + adjacency[minIndex][i];
                sprintf(str,"%s->%s",distances[minIndex].path,citiesName[i]);
                distances[i].path = str;
            }
        }
    }
}

int main() {
    int i, cityCount, **adjacency;
    char **citiesName;
    Distance *distances;
    cityCount = getCityCount();
    citiesName = malloc(sizeof(char *) * cityCount);
    adjacency = malloc(sizeof(int *) * cityCount);
    for (i = 0; i < cityCount; i++) {
        adjacency[i] = malloc(sizeof(int) * cityCount);
    }
    distances = malloc(sizeof(Distance) * cityCount);
    getCitiesData(citiesName, adjacency, cityCount);
    Dijkstra(distances, adjacency, citiesName, cityCount, START_CITY);
    printf("城市有以下：\n");
    for (i = 0; i < cityCount; i++) {
        printf("%s\t", citiesName[i]);
    }
    printf("\n邻接矩阵：\n");
    for (i = 0; i < cityCount * cityCount; i++) {
        if (adjacency[i / cityCount][i % cityCount] == INT_MAX) {
            printf("%6d", 0);
        } else {
            printf("%6d", adjacency[i / cityCount][i % cityCount]);
        }
        if ((i + 1) % cityCount == 0) {
            printf("\n");
        }
    }
    printf("\n城市数量%d\n", cityCount);
    printf("出发城市到各城市到最短路径：\n");
    for (i = 0; i < cityCount; i++) {
        printf("%s,%d\n", distances[i].path, distances[i].value);
    }
    free(adjacency);
    free(citiesName);
    return 0;
}
