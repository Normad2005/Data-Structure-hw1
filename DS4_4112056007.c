#include <stdio.h>
#include <string.h>
#include <limits.h>

int nextInt(FILE *fptr);
int max(int a, int b);
int solution(int n, int num, int *arr);

int main() {
    FILE *fptr = fopen("testcase4.txt", "r");
    FILE *out = fopen("output4.txt", "w");  // 開啟檔案輸出
    int testcase;
    if (fptr == NULL || out == NULL) {
        printf("IOerror\n");
        return 1;
    }
    testcase = nextInt(fptr);
    while(testcase--){
        int sz = nextInt(fptr);
        int num = nextInt(fptr);
        int ans = 0;
        int min = INT_MAX;
        int arr[sz+1];
        arr[0] = 0;
        for(int i=1;i<sz+1;i++){
            arr[i] = nextInt(fptr);
            min = (arr[i] < min) ? arr[i] : min;
        }
        if(min <= 0){
            for(int i=1;i<sz+1;i++){
                arr[i] += ((-1)*min + 1);
            }
        }
        if(num > sz/2){
            fprintf(out, "Error!\n");  // 使用 fprintf 輸出錯誤訊息
            continue;
        }
        ans = solution(sz, num, arr);
        ans -= num*((-1)*min + 1);
        fprintf(out, "%d\n", ans);  // 輸出結果到檔案
    }

    fclose(fptr);
    fclose(out);
    return 0;
}

// arr有n+1個, arr[0]為0, arr[1~n]就是第n個位置的價值
int solution(int n, int num, int *arr){
    //index範圍 arr[1~n], dp[1~num][1~n]
    int res1 = INT_MIN;
    int res2 = INT_MIN;

    int dp[num+1][n+1];
    for(int i=0;i<=num;i++){
        for(int j=0;j<=n;j++){
            dp[i][j] = 0;
        }
    }

    //1, 3, 5, 7, ..., n
    //種一棵
    dp[1][1] = arr[1];
    for(int i=2;i<=n-1;i++){
        int take = arr[i];
        int no_take = dp[1][i-1];
        dp[1][i] = max(take, no_take);
    }
    //最多種n顆
    for(int i=2;i<=num;i++){
        for(int j=2*i-1;j<=n-1;j++){
            int take = dp[i-1][j-2] + arr[j];
            int no_take = dp[i-1][j-1];
            dp[i][j] = max(take, no_take);
        }
    }

    for(int i=0;i<=n;i++){
        if(dp[num][i] > res1)
            res1 = dp[num][i];
    }

    //reset dp
    for(int i=0;i<=num;i++){
        for(int j=0;j<=n;j++){
            dp[i][j] = 0;
        }
    }

    //2, 4, 6, ..., n
    dp[1][2] = arr[1];
    for(int i=3;i<=n;i++){
        int take = arr[i];
        int no_take = dp[1][i-1];
        dp[1][i] = max(take, no_take);
    }
    //最多種n顆
    for(int i=2;i<=num;i++){
        for(int j=2*i;j<=n;j++){
            int take = dp[i-1][j-2] + arr[j];
            int no_take = dp[i-1][j-1];
            dp[i][j] = max(take, no_take);
        }
    }

    for(int i=0;i<=n;i++){
        if(dp[num][i] > res2)
            res2 = dp[num][i];
    }
    return max(res1, res2);
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

int max(int a, int b){
    return (a >= b) ? a:b;
}
