#include <stdio.h>
#include <stdlib.h>

#define qsize 15000

typedef struct Process{
	int id, arr, time, pior;
}Process;

Process q[qsize];
int cur, f, r;

int cmp(const void *a, const void *b){
	Process pa = *(Process *)a;
	Process pb = *(Process *)b;
	if(pa.pior == pb.pior) return pa.id - pb.id;
	return -(pa.pior - pb.pior);
}

void deq(){
	Process p = q[f];
	f = (f+1)%qsize;
	cur += p.time;
	printf("%d %d\n", p.id, cur);
}

void enq(Process p){
	if(f == r){
		q[r] = p;
		r = (r+1)%qsize;
		cur = p.arr;
		return;
	}
	int remain = p.arr - cur;
	while(remain > 0 && f != r){
		if(q[f].time > remain){
			q[f].time -= remain;
			remain = 0;
		}
		else{
			remain -= q[f].time;
			deq();
		}
	}
	
	q[r] = p;
	r = (r+1) % qsize;
	cur = p.arr;
	
	if(f != r)
		qsort(q + f, r - f, sizeof(Process), cmp);
}

FILE *fin, *fout;

int main(){
	fin = fopen("testcase73.txt", "r");
	fout = fopen("eeoo4.txt", "w");
	int t, n;
	fscanf(fin, "%d", &t);
	while(t--){
		fscanf(fin, "%d", &n);
		for(int i=0;i<n;i++){
			Process p;
			fscanf(fin, "%d%d%d%d", &p.id, &p.arr, &p.time, &p.pior);
			enq(p);
		}
		while(f != r){
			deq();
		}
		printf("\n");
	}
	return 0;
}
