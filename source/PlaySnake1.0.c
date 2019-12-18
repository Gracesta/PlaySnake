#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#define N 10
#define MAX 25
#define MAXQUEUE 300

//#define clear() printf("\033[H\033[J")

char map[MAX+1][MAX+1];
int food_num = 1;

struct zahyo{ int x, y; };

typedef struct zahyo elementtype;

struct queue{
    int front;
    int rear;
    elementtype contents[MAXQUEUE];
};

void init_snake(struct queue *s);
void draw_snake(struct queue *s);
void creating_map();
void print_map();                                           /* create a map where the playsnake moves */
void Snake_Grow(struct queue *p);                           /* when the snake eat a food, it grows */
void initqueue(struct queue *p);
int queueempty(struct queue *p);
elementtype getq(struct queue *p);
void putq(struct queue *p, elementtype e);
void printfqueue(struct queue *p);
void snake_move(struct queue *p, char cm);
void creating_food();

int main(){
    struct queue *s;
    char cm, buf;
    s = (struct queue *)malloc(sizeof(struct queue));
    int i, j;
    creating_map();
    init_snake(s);
    draw_snake(s);
    map[18][21] = '#';
    while(1){
        print_map();
        fflush(stdin);
        printf("Give your command(W,S,A,D) to move:");
        cm = 'X';
        cm = getchar();
        //printf("cm = %c", cm);
        snake_move(s, cm);
        draw_snake(s);
    }
    print_map();
    return 0;
}

void creating_food(){               /*creating three food in different place*/
    srand(time(NULL));
    int x, y;
    for(int i=0;i<3;i++){
        while(1){
            x = rand() % MAX;
            y = rand() % MAX;
            if(map[y][x] == ' '){
                map[y][x] = '#';
                food_num++;
                break;
            }
        }
    }
}
void snake_move(struct queue *p, char cm){
    struct zahyo head = p->contents[p->rear-1];
    //printf("head = %d, %d\n", head.x, head.y);
    switch(cm){                     /*return head forbidden*/
        case 'W':head.y--;break;
        case 'S':head.y++;break;
        case 'A':head.x--;break;
        case 'D':head.x++;break;
        default: return;
    }
    //printf("new head = %d %d\n", head.x, head.y);
    if(map[head.y][head.x] == '-' || map[head.y][head.x] == '*'){
        printf("----------\n");
        printf("|YOU LOST|\n");
        printf("----------\n");
        exit(0);
    }
    putq(p, head);                          /*put the moved coordinate as snake'head into QUEUE*/                        
    if(map[head.y][head.x] != '#'){
        struct zahyo tail = getq(p);        /*move tail forward if snake didn't eat a food*/
        map[tail.y][tail.x] = ' ';   
    }else{
        food_num--;
        if(food_num == 0){                  /*if food run out, refill 3 foods in map*/
            creating_food();
        }
    }
}

void draw_snake(struct queue *p){
    int pos = p->front;
    int y, x;
    while(1){
        if(pos == p->rear){
            break;
        }else{
            y = p->contents[pos].y;
            x = p->contents[pos].x;
            map[y][x] = '*';
            pos++;
            if(pos == MAXQUEUE) pos = 0;
        }
    }
    printf("\n");
}

void init_snake(struct queue *s){
    struct zahyo t;
    t.x = 20, t.y = 20;
    putq(s, t);
    t.y = 19;
    putq(s, t);
    t.y = 18;
    putq(s, t);
}
void creating_map(){
    for(int i=0;i<MAX;i++){
        map[i][0] = map[i][MAX] = '-';
        map[0][i] = map[MAX][i] = '-';
    }
    for(int i=1;i<MAX;i++){
        for(int j=1;j<MAX;j++){
            map[i][j] = ' ';
        }
    }
}

void print_map(){
    for(int i=0;i<=MAX;i++){
        for(int j=0;j<=MAX;j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

void initqueue(struct queue *p){
    p->front = p->rear = 0;
}
int queueempty(struct queue *p){
    if(p->front == p->rear){
        return 1;
    }else{
        return 0;
    }
}
elementtype getq(struct queue *p){
    if( queueempty(p) ){
        printf("Underflow\n");
        exit(1);
    }
    if(p->front >= MAXQUEUE-1) {
        p->front = 0;
        return p->contents[MAXQUEUE];
    }
    return p->contents[p->front++];
}
void putq(struct queue *p, elementtype e){
    if(p->rear == p->front-1 || p->rear == MAXQUEUE-1 && p->front == 0){
        printf("Overflow\n");
        exit(1);
    }else{
        p->contents[p->rear++] = e;
        if(p->rear == MAXQUEUE){ p->rear = 0; }
    }
}

void printfqueue(struct queue *p){
    int pos = p->front;
    while(1){
        if(pos == p->rear){
            break;
        }else{
            printf("[%d]", p->contents[pos++]);
            if(pos == MAXQUEUE) pos = 0;
        }
    }
    printf("\n");
}
