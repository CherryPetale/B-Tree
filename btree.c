#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN    (10)

struct __TREE_NODE{
    struct __TREE_NODE *parent;
    struct __TREE_NODE *rightnode;
    struct __TREE_NODE *leftnode;
    int index;
    char data[DATA_LEN];
};
typedef struct __TREE_NODE TREE_NODE;

TREE_NODE* rootNode = NULL;

// ノートを作成する
TREE_NODE* createNode(){
    TREE_NODE *target = (TREE_NODE*)malloc(sizeof(TREE_NODE));
    if(target == NULL){
        fprintf(stderr, "malloc error.");
        exit(EXIT_FAILURE);
    }
    target->parent = NULL;
    target->rightnode = NULL;
    target->leftnode = NULL;
    return target;
}

// ノードを解放する
void freeNode(TREE_NODE *current){
    if(current != NULL && current->leftnode != NULL) freeNode(current->leftnode);
    if(current != NULL && current->rightnode != NULL) freeNode(current->rightnode);
    if(current != NULL) free(current);
}

/*
 * 現在のノード状態を表示する
 */
void drawNodes(TREE_NODE *current){
    if(current != NULL && current->leftnode != NULL) drawNodes(current->leftnode);
    if(current != NULL && current->rightnode != NULL) drawNodes(current->rightnode);
    if(current != NULL) printf("%d -> %s\n", current->index, current->data);
}

/*
 * ノードを追加する
 */
void addNode(int key, char value[DATA_LEN]){

    // First Add
    if(rootNode == NULL){
        rootNode = createNode();
        rootNode->index = key;
        strncpy(rootNode->data, value, DATA_LEN);
        return;
    }

    TREE_NODE *current = rootNode;
    TREE_NODE *parent = rootNode;
    int isLeft = 0;

    // 挿入場所まで移動させる
    while(1){
        parent = current;

        if(current->index > key){ // Left
            current = current->leftnode;
            isLeft = 1;
        }else{ // right
            current = current->rightnode;
            isLeft = 0;
        }
        
        if(current == NULL){
            break;
        }
    }

    current = createNode();

    if(isLeft == 1) 
        parent->leftnode = current;
    else
        parent->rightnode = current;

    current->parent = parent;
    current->index = key;
    strncpy(current->data, value, DATA_LEN);
}



/*
 * メイン関数！！
 */
int main( void ){

    int key;
    char val[DATA_LEN];

    while(1){

        // key
        printf("key(number)->");
        scanf("%d", &key);
        fflush(stdin);

        if(key < 0) break;

        //value
        printf("value(string[10])->");
        scanf("%9s", val);
        fflush(stdin);

        addNode(key, val);
    }

    drawNodes(rootNode);

    freeNode(rootNode);

    return 0;
}