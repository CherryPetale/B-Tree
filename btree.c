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
void freeNodes(TREE_NODE *current){
    if(current != NULL && current->leftnode != NULL) freeNodes(current->leftnode);
    if(current != NULL && current->rightnode != NULL) freeNodes(current->rightnode);
    if(current != NULL) free(current);
}

/*
 * 親から見て片側ノードの数を返す
 * （isLeftNotRight= 1:左　1以外:右
 */
int getNodesLength(TREE_NODE *current, int depth, int isLeftNotRight){
    int length = depth == 0 ? 0 : 1;
    if(current != NULL && current->leftnode != NULL && !(isLeftNotRight != 1 && depth == 0)){
        length += getNodesLength(current->leftnode, depth+1, isLeftNotRight);
    }
    if(current != NULL && current->rightnode != NULL && !(isLeftNotRight == 1 && depth == 0)){
        length += getNodesLength(current->rightnode, depth+1, isLeftNotRight);
    }
    return length;
}

/*
 * ノードの不均等を修正
 * 再構築後の親を返す
 */
TREE_NODE* recreateNodes(TREE_NODE *current){
    int leftNlen = getNodesLength(current, 0, 1);
    int rightNlen = getNodesLength(current, 0, 0);
    int sub = leftNlen > rightNlen ? leftNlen - rightNlen : rightNlen - leftNlen;
    int i;

    // There's Bug;
    for(i = 0; i < (int)(sub / 2); i++){
        if(leftNlen > rightNlen){
            current->parent = current->leftnode;
            current->leftnode->rightnode = current;
            current->leftnode->parent = NULL;
            current = current->leftnode;
            current->rightnode->leftnode = NULL;
        }else{
            current->parent = current->rightnode;
            current->rightnode->leftnode = current;
            current->rightnode->parent = NULL;
            current = current->rightnode;
            current->leftnode->rightnode = NULL;
        }
    }
    return current;
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
 * ノードを検索
 */
TREE_NODE* findNode(TREE_NODE *current, int key){
    if(current->index == key){
        return current;
    }else if(current->index > key && current->leftnode != NULL){ // left
        return findNode(current->leftnode, key);
    }else if(current->index < key && current->rightnode != NULL){ // right
        return findNode(current->rightnode, key);
    }else{
        return NULL;
    }
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
        }else if(current->index < key){ // right
            current = current->rightnode;
            isLeft = 0;
        }else if(current->index == key){ // 同じなら上書き
            strncpy(current->data, value, DATA_LEN);
            return;
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
 * ノードを削除する
 */
void removeNode(int key){
    TREE_NODE *target = findNode(rootNode, key);
    TREE_NODE *childTop = NULL;
    
    if(target == NULL) return;

    if(target->parent == NULL){ // 自分がトップ
        if(target->leftnode != NULL && target->rightnode != NULL){
            target->rightnode->parent = target->leftnode;
            target->leftnode->parent = NULL;
        }else{
            if(target->leftnode != NULL) target->leftnode->parent = NULL;
            if(target->rightnode != NULL) target->rightnode->parent = NULL;
        }
    }else{ // 親がいる
        if(target->leftnode != NULL && target->rightnode != NULL){
            target->leftnode->parent = target->parent;
            target->rightnode->parent = target->leftnode;
            childTop = target->leftnode;
        }else{
            if(target->leftnode != NULL){
                target->leftnode->parent = target->parent;
                childTop = target->leftnode;
            }
            if(target->rightnode != NULL){
                target->rightnode->parent = target->parent;
                childTop = target->rightnode;
            } 
        }
    }

    if(target->parent->leftnode == target)
        target->parent->leftnode = childTop;
    else if(target->parent->rightnode == target)
        target->parent->rightnode = childTop;
    
    free(target);
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

    drawNodes(rootNode); //表示

    printf("Left Nodes Length: %d\n", getNodesLength(rootNode, 0, 1));
    printf("Right Nodes Length: %d\n", getNodesLength(rootNode, 0, 0));

    rootNode = recreateNodes(rootNode); // 不均等修正

    // removeNode(7); // 削除
    // printf("Delete No.7\n");

    // drawNodes(rootNode); //表示

    printf("Left Nodes Length: %d\n", getNodesLength(rootNode, 0, 1));
    printf("Right Nodes Length: %d\n", getNodesLength(rootNode, 0, 0));

    // メモリ解放
    freeNodes(rootNode);

    return 0;
}