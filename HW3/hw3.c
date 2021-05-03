#include <stdio.h>
#include <stdlib.h>     // malloc

/*
    불일치로 생기는 손해: 1
    틈으로 생기는 손해: 2
*/
int misMatch = 2;
int gap = 3;

void Sequence(int x_size, char* xsq, int y_size, char* ysq);
void opt_cost(int **mtrx, int x_size, int y_size, int i, int j, char *xsq, char *ysq);
void opt_align(int i, int j, int **mtrx, char *xalign, char *yalign, char *xsq, char *ysq, int x_size, int y_size, int *cost, int *num);
int min(int a, int b, int c);

int main()
{
    int size, x_size, y_size, i;
    char xsq[100], ysq[100];

    // input.txt 파일 오픈
    FILE *fp = fopen("/Users/joohyunlee/VSCodejh/HW3/input.txt", "r");     // input.txt 파일을 읽기 모드로 열기, 파일 포인터 반환
    if (fp == NULL) {
        printf("파일열기 실패\n");
        return 1;
    }

    // 데이터의 개수: size
    fscanf(fp, "%d", &size);
    printf("size: %d\n", size);

    /* 
    데이터의 개수만큼 X, Y 길이와 시퀀스 입력받고 optimal alignment와 배열 M 출력
    */ 
    for(i=0; i<size; i++){
        fscanf(fp, "%d", &x_size);      // X의 길이
        printf("%d) x_size: %d\n", i+1, x_size);
        fscanf(fp, "%s", xsq);      // X의 시퀀스
        printf("--> xsq: %s\n", xsq);

        fscanf(fp, "%d", &y_size);      // Y의 길이
        printf("%d) y_size: %d\n", i+1, y_size);
        fscanf(fp, "%s", ysq);      // Y의 시퀀스
        printf("--> ysq: %s\n", ysq);

        Sequence(x_size, xsq, y_size, ysq);
    }

    fclose(fp);     // 파일 포인터 닫기

    return 0;
}

void Sequence(int x_size, char* xsq, int y_size, char* ysq)
{
    int i, j;
    int **mtrx;
    char *xalign1, *yalign1, *xalign2, *yalign2, *xalign3, *yalign3;
    int cost1=0, cost2=gap, cost3=gap;
    int num1=1, num2=1, num3=1;
    int mincost;

    // x_size, y_size 만큼 이차원 배열 동적할당
    mtrx = (int **) malloc ( sizeof(int*) * (x_size+1) );
    for(i=0; i<(x_size+1); i++) {
        mtrx[i] = (int*) malloc ( sizeof(int) * (y_size+1) );
        for(j=0; j<(y_size+1); j++) mtrx[i][j] = -1;    // -1 으로 초기화
    }
    xalign1 = (char *) malloc ( sizeof(char) * (x_size+y_size));
    yalign1 = (char *) malloc ( sizeof(char) * (x_size+y_size));
    xalign2 = (char *) malloc ( sizeof(char) * (x_size+y_size));
    yalign2 = (char *) malloc ( sizeof(char) * (x_size+y_size));
    xalign3 = (char *) malloc ( sizeof(char) * (x_size+y_size));
    yalign3 = (char *) malloc ( sizeof(char) * (x_size+y_size));

    // 최적 정렬 비용 구하기
    mtrx[x_size][y_size] = 0;
    opt_cost(mtrx, x_size, y_size, 0, 0, xsq, ysq);

    // 최적 정렬 구하기 1
    xalign1[0] = xsq[0];
    yalign1[0] = ysq[0];
    if (xsq[0]!=ysq[0]) cost1 = misMatch;
    opt_align(1, 1, mtrx, xalign1, yalign1, xsq, ysq, x_size, y_size, &cost1, &num1);
    printf("num1: %d\ncost1: %d\n", num1, cost1);

    // 최적 정렬 구하기 2
    xalign2[0] = '-';
    yalign2[0] = ysq[0];
    opt_align(0, 1, mtrx, xalign2, yalign2, xsq, ysq, x_size, y_size, &cost2, &num2);
    printf("num2: %d\ncost2: %d\n", num2, cost2);

    // 최적 정렬 구하기 3
    xalign3[0] = xsq[0];
    yalign3[0] = '-';
    opt_align(1, 0, mtrx, xalign3, yalign3, xsq, ysq, x_size, y_size, &cost3, &num3);
    printf("num3: %d\ncost3: %d\n", num3, cost3);

    mincost = min(cost1, cost2, cost3);
    if(mincost == cost1) {
        printf("<<< cost1: %d >>>\n", cost1);
        for(i=0; i<num1; i++) printf("%c", xalign1[i]);
        printf("\n");
        for(i=0; i<num1; i++) printf("%c", yalign1[i]);
    }
    else if(mincost == cost2) {
        printf("<<< cost2: %d >>>\n", cost2);
        for(i=0; i<num2; i++) printf("%c", xalign2[i]);
        printf("\n");
        for(i=0; i<num2; i++) printf("%c", yalign2[i]);
    }
    else {
        printf("<<< cost3: %d >>>\n", cost3);
        for(i=0; i<num3; i++) printf("%c", xalign3[i]);
        printf("\n");
        for(i=0; i<num3; i++) printf("%c", yalign3[i]);
    }
    
    // 최적 정렬 비용 출력
    printf("\n최적 정렬 비용 출력\n");
    for(i=0; i<(x_size+1); i++)
    {
        for(j=0; j<(y_size+1); j++) printf("%d      ", mtrx[i][j]);
        printf("\n");
    }

    // 죄적 정렬 구하기

}

/*
최적 정렬 비용을 구하는 함수
*/
void opt_cost(int **mtrx, int x_size, int y_size, int i, int j, char *xsq, char *ysq)
{
    int penalty = 0;
    if (mtrx[i][j] != -1) return;
    if (i == x_size) {
        mtrx[i][j] = 2 * (y_size-j);
        //printf("[%d][%d] >> %d\n", i, j, mtrx[i][j]);
    }
    else if(j == y_size) {
        mtrx[i][j] = 2 * (x_size-i);
        //printf("[%d][%d] >> %d\n", i, j, mtrx[i][j]);
    }
    else {
        if(xsq[i] != ysq[j]) penalty = misMatch;
        if (mtrx[i+1][j+1] == -1) opt_cost(mtrx, x_size, y_size, i+1, j+1, xsq, ysq);
        if (mtrx[i+1][j] == -1) opt_cost(mtrx, x_size, y_size, i+1, j, xsq, ysq);
        if (mtrx[i][j+1] == -1) opt_cost(mtrx, x_size, y_size, i, j+1, xsq, ysq);
        mtrx[i][j] = min(mtrx[i+1][j+1]+penalty, mtrx[i+1][j]+gap, mtrx[i][j+1]+gap);
       // printf("[%d][%d] >> %d\n", i, j, mtrx[i][j]);
    }
    return;
}

void opt_align(int i, int j, int **mtrx, char *xalign, char *yalign, char *xsq, char *ysq, int x_size, int y_size, int *c, int *n){
    int penalty;
    int num = *n;
    int cost = *c;
    //printf("xalign[0] = %c, yalign[0] = %c, cost = %d\n", xalign[0], yalign[0], cost );
    while(i != x_size || j != y_size){
        if (xsq[i] == ysq[j]) penalty = 0;
        else penalty = misMatch;
        
        if (i == x_size) {
            xalign[num] = '-';
            yalign[num++] = ysq[j++];
            cost += 2;
            //printf("xalign[%d] = %c, yalign[%d] = %c, cost = %d\n", num-1, xalign[num-1], num-1, yalign[num-1], cost );
            continue;
        }
        if (j == y_size) {
            xalign[num] = xsq[i++];
            yalign[num++] = '-';
            cost += 2;
            //printf("xalign[%d] = %c, yalign[%d] = %c, cost = %d\n", num-1, xalign[num-1], num-1, yalign[num-1], cost );
            continue;
        }

        if (mtrx[i][j] == mtrx[i+1][j+1] + penalty){
            xalign[num] = xsq[i++];
            yalign[num++] = ysq[j++];
            cost += penalty;
           // printf("xalign[%d] = %c, yalign[%d] = %c, cost = %d\n", num-1, xalign[num-1], num-1, yalign[num-1], cost );
        }
        else if (mtrx[i][j] == mtrx[i+1][j] + 2){
            xalign[num] = xsq[i++];
            yalign[num++] = '-';
            cost += gap;
           // printf("xalign[%d] = %c, yalign[%d] = %c, cost = %d\n", num-1, xalign[num-1], num-1, yalign[num-1], cost );
        }
        else {
            xalign[num] = '-';
            yalign[num++] = ysq[j++];
            cost += gap;
          //  printf("xalign[%d] = %c, yalign[%d] = %c, cost = %d\n", num-1, xalign[num-1], num-1, yalign[num-1], cost );
        }
    }
    *c = cost;
    *n = num;
    return;
}

int min(int a, int b, int c){
    int min = a;
    if (min > b) min = b;
    if (min > c) min = c;
    return min;
}