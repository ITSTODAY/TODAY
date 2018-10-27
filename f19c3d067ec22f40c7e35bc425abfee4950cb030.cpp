#include "stdio.h"

//建树时，如果数值为0，向左转，否则右转

struct number{//如果一个节点有多个元素，应该记录其元素数值
    int next = -1;
    int num = 0;
};
//-1代表空指针
struct node{//树中的普通节点
    int left = -1;//左儿子默认为-1；
    int right = -1;//右儿子默认为-1；
};

struct end_node{//叶子结点，记录指向数值数组的位置
    int num = -1;
};

int root = 0;//根节点数值为零
node tree[25000000];//普通节点库
end_node end_range[500005];//叶子节点库
number num_rec[5000005];//数值库
int free = 1;//节点库中一定为空闲的节点
int free_end = 1;//叶子节点库中一定为空闲的节点
int free_number =1;//叶子结点库中一定空闲的节点
int n;//有多少个电
int k;//最大间隔
unsigned long long numbers[500000];//记录所有数字

//这里默认了遍历使用的临时变量名为recent
#define left tree[recent].left//定义左节点的宏
#define right tree[recent].right//定义右节点的宏
#define ending end_range[recent]//定义叶子节点的宏

void creatPoint(int i){//建立节点
    int recent = 0;//遍历临时变量
    char bin_number[65];//存储二进制数
    unsigned long long tempt = numbers[i];
    for(int j=63;j>=0;j--,tempt/=2) bin_number[j] = (char)(tempt%2+(int)'0');//转化为二进制数组
    for(int j=0;j<=63;j++) {
        if (j != 63) {//如果没有到达末尾
            if (bin_number[j] == '0') {//这一位为0
                if (left == -1) {//如果左儿子不存在，建立左儿子
                    left = free++;
                    recent = left;
                } else recent = left;//否则直接进入左儿子
            } else {
                if (right == -1) {//这一位为1时同理
                    right = free++;
                    recent = right;
                } else recent = right;
            }
        } else {//如果到达末尾
            if (bin_number[j] == '0') {//末尾为0
                if (left == -1) {//如果左儿子不存在
                    left = free_end++;//建立左叶子节点
                    recent = left;
                    ending.num = free_number++;
                    num_rec[ending.num].num = i;//记录当前节点数值
                } else {//否则
                    recent = left;
                    int temp_num = ending.num;
                    while (num_rec[temp_num].next != -1) temp_num = num_rec[temp_num].next;//遍历到数值链表的最后一位
                    num_rec[temp_num].next = free_number++;//添加新节点
                    num_rec[num_rec[temp_num].next].num = i;//赋值
                }
            } else {//末尾为1，则一切对左儿子的操作转化为对右儿子的操作
                if (right == -1) {
                    right = free_end++;
                    recent = right;
                    ending.num = free_number++;
                    num_rec[ending.num].num = i;
                } else {
                    recent = right;
                    int temp_num = ending.num;
                    while (num_rec[temp_num].next != -1) temp_num = num_rec[temp_num].next;
                    num_rec[temp_num].next = free_number++;
                    num_rec[num_rec[temp_num].next].num = i;
                }
            }
        }
    }
}

void deletePoint(int i){//删除一个节点
    unsigned long long tempt = numbers[i];
    char bin_number[65];
    for(int j=63;j>=0;j--,tempt/=2) bin_number[j] = (char)(tempt%2+(int)'0');
    int recent = 0;
    int last_mark = -1;//记录最晚的一个二叉分支
    int record;//记录最晚的二叉分枝对应的数字位数
    for(int j=0;j<=63;j++){
        if(left!=-1&&right!=-1){//如果节点有两个儿子，更新last mark
            last_mark = recent;
            record =j;//记录当前位
        }
        if(j!=63){//不断向下找
            if(bin_number[j]=='0') recent = left;
            else recent = right;
        }else{//当找到最后一位的时候
            if(bin_number[j]=='0'){//如果当前位为0
                recent = left;
                int temp_num = ending.num;//删除最早的一个数值
                temp_num = num_rec[temp_num].next;
                ending.num = temp_num;
                if(temp_num== -1&&last_mark!=-1){//如果数值链表为空，将这个节点的前序二叉节点的一支删除
                    recent = last_mark;
                    if(bin_number[record]=='0') left = -1;//删除节点
                    else right = -1;
                }
            }else{//如果当前位为-1，处理方法一致，对左儿子的处理变为对右儿子
                recent = right;
                int temp_num = ending.num;
                temp_num = num_rec[temp_num].next;
                ending.num = temp_num;
                if(temp_num== -1&&last_mark!=-1){
                    recent = last_mark;
                    if(bin_number[record]=='0') left = -1;
                    else right = -1;
                }
            }
        }
    }
}

void createTree(){//建造初始的树
    int s = 0;
    if(k>=n) s = n-1;
    else s = k;//s的范围不大于k，只需要建立这些树，保证0号数据所需要的数据被完全建造在树中。
    for(int i=0;i<=s;i++) creatPoint(i);//建立节点
}

void rebuldTree(int place){
    if(place-k-2>=0) deletePoint(place-k-2);//如果范围超出，删除节点
    if(place+k+1<n) creatPoint(place+k+1);//增加一个需要节点
}

void searchTree(int place){//搜索
    char bin_number[65];
    unsigned long long tempt = numbers[place];
    for(int j=63;j>=0;j--,tempt/=2) bin_number[j] = (char)(tempt%2+(int)'0');//将数字转化为二进制
    int recent = 0;//访问临时变量
    for(int j=0;j<=63;j++){
        if(bin_number[j]=='0'&&right!=-1) recent = right;//如果当前位为0，右儿子存在，右转
        else if(bin_number[j]=='0'&&right==-1) recent = left;
        else if(bin_number[j]=='1'&&left!=-1) recent = left;//如果当前位为1，左儿子存在，左转
        else if(bin_number[j]=='1'&&left==-1) recent = right;
        if(j==63) printf("%d\n",num_rec[ending.num].num);
    }
}

int main(){
    scanf("%d",&n);
    scanf("%d",&k);

    for(int i=0;i<n;i++) scanf("%llu",&numbers[i]);

    createTree();//建树
    for(int place=0;place<n;place++){
        rebuldTree(place);//每次更新树
        searchTree(place);//查找输出结果
    }
    return 0;
}