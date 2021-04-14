#include<iostream>
#include <stack>
#include<deque>
#include<queue>
#include<string>
#include "huffman.h"
using namespace std;
int HuffmanTree::sum = 1;


//二叉树成员函数实现
BinaryTree::BinaryTree(int num, int weight)
{
	p_root = new Node(nullptr, nullptr, nullptr);
	p_root->num = num;
	p_root->weight = weight;
}

BinaryTree::~BinaryTree()
{
	deleteNode(p_root);
}
//交换节点
bool BinaryTree::swap(Node * p_nodeA, Node * p_nodeB)
{
	if (p_nodeA == nullptr || p_nodeB == nullptr || p_nodeA == p_nodeB)
		return false;
	Node *pTemp;
	//判断是左孩子还是右孩子
	//A为左孩子
	if (getBrotherState(p_nodeA) == LeftChild)
	{      //如果都为左直接换
		if (getBrotherState(p_nodeB) == LeftChild) {
			pTemp = p_nodeA->p_parent->p_left;
			p_nodeA->p_parent->p_left = p_nodeB->p_parent->p_left;
			p_nodeB->p_parent->p_left = pTemp;
		}
		//如果一左一右直接交换
		else {
			pTemp = p_nodeA->p_parent->p_left;
			p_nodeA->p_parent->p_left = p_nodeB->p_parent->p_right;
			p_nodeB->p_parent->p_right = pTemp;
		}
	}
	//A为右孩子
	else {
		if (getBrotherState(p_nodeB) == LeftChild) {
			pTemp = p_nodeA->p_parent->p_right;
			p_nodeA->p_parent->p_right = p_nodeB->p_parent->p_left;
			p_nodeB->p_parent->p_left = pTemp;
		}
		else {
			pTemp = p_nodeA->p_parent->p_right;
			p_nodeA->p_parent->p_right = p_nodeB->p_parent->p_right;
			p_nodeB->p_parent->p_right = pTemp;

		}
	}
	pTemp = p_nodeA->p_parent;
	p_nodeA->p_parent = p_nodeB->p_parent;
	p_nodeB->p_parent = pTemp;
	return true;

}

bool BinaryTree::addNode(Node * p_parent, Node * p_child, Brother brotherState)
{
	if (p_parent == nullptr || p_child == nullptr)
		return false;
	if (brotherState == LeftChild) {
		if (p_parent->p_left != nullptr) {
			cout << "error:left child exist!" << endl;
			return false;
		}
		p_parent->p_left = p_child;
	}
	else if (brotherState == RightChild) {
		if (p_parent->p_right != nullptr) {
			cout << "error:right child exist!" << endl;
			return false;
		}
		p_parent->p_right = p_child;
	}
	else {
		cout << "error:brotherState is wrong!" << endl;
		return false;
	}
	p_child->p_parent = p_parent;
	return true;
}

Node * BinaryTree::findNode(Node *p)
{
	Node *p_node = p_root;
	std::queue<Node *> queue;
	queue.push(p_node);
	while (!queue.empty()) {
		p_node = queue.front();
		if (p_node == p) {
			return p_node;
		}
		queue.pop();
		if (p_node->p_left != nullptr) {
			queue.push(p_node->p_left);
		}
		if (p_node->p_right != nullptr) {
			queue.push(p_node->p_right);
		}
	}
	return nullptr;
}

bool BinaryTree::setNodeNum(Node* p_node, int num)
{
	if (p_node == nullptr)
		return false;
	else {
		p_node->num = num;
		return true;
	}
}

bool BinaryTree::isAncestor(Node * p_nodeChild, Node * p_nodeAncestor)
{
	while (p_nodeChild != p_root) {
		if (p_nodeChild == p_nodeAncestor) {
			return true;
		}
		else {
			p_nodeChild = p_nodeChild->p_parent;
		}
	}
	return false;
}

void BinaryTree::deleteNode(Node *p_node)
{
	if (p_node->p_left != nullptr) {
		deleteNode(p_node->p_left);
	}
	if (p_node->p_right != nullptr) {
		deleteNode(p_node->p_right);
	}
	delete p_node;
}

BinaryTree::Brother BinaryTree::getBrotherState(Node *p_node)
{
	if (p_node->p_parent->p_left == p_node) {
		return LeftChild;
	}
	else {
		return RightChild;
	}
}



//哈夫曼树成员函数实现
HuffmanTree::HuffmanTree() :tree(0, 0)
{
}

HuffmanTree::~HuffmanTree()
{
	
}



//获取节点的哈夫曼编码
string HuffmanTree::getHuffmanCode(Node *p_n)
{
	string huffmanCode = "";
	stack<Node *> stack;
	deque<char> code;

	//当p_n节点为根的时候进行更新树操作：将其改为根的右孩子
	if (p_n == tree.getRoot())
		p_n = p_n->p_right;
	//逆向后推，当为左孩子的时候则置0，当为右孩子的时候则置1。
	while (p_n != tree.getRoot()) {
		if (tree.getBrotherState(p_n) == tree.LeftChild) {
			code.push_back('0');
		}
		else {
			code.push_back('1');
		}
		p_n = p_n->p_parent;
	}

	while (!code.empty()) {
		huffmanCode += code.back();
		code.pop_back();
	}
	return huffmanCode;
}

//找到所在块中最大节点编号的节点
Node * HuffmanTree::findLarge(Node *p_node)
{
	std::stack<Node *> stack;
	Node *p = tree.getRoot();
	Node *large = p;
	while (p || !stack.empty()) {
		if (p != nullptr) {
			stack.push(p);
			if (p->weight == p_node->weight) {
				//如果large不在同权重下，则置large为p
				if (large->weight != p->weight) {
					large = p;
				}
				//同权重下的large比p小，则置large为p
				else if (large->num > p->num) {
					large = p;
				}
			}
			p = p->p_left;
		}
		else {
			p = stack.top();
			stack.pop();
			p = p->p_right;
		}
	}
	//large不可能是根节点，当large为根节点时返回原节点
	if (large == tree.getRoot()) {
		return p_node;
	}
	return large;
}


//编码函数
string HuffmanTree::encode(string s)
{
	string finalCode[100];
	int i=0;
	string::iterator it;
	//读取字符，设置nyt节点为根节点
	char cbuffer;
	Node *nyt = tree.getRoot();
	for(it=s.begin();it!=s.end();it++)
	{
		cbuffer = *it;
		bool exist = false;
		string code;
		auto existNode = buffers.begin();	//buffers的一个迭代器，当cbuffer存在于树中的时候，existNode指向该节点
		for (existNode; existNode != buffers.end(); existNode++) 
		{
			if (existNode->key == cbuffer)
			{
				code = existNode->value;
				exist = true;
				finalCode[i] = code;
				cout << cbuffer << "： " <<code << endl;
				break;
			}
		}

		if (exist) {
			Node *root = existNode->p;
			weightAdd(root);
		}
		else {
			//当字符不存在树中时，则新建子树，并替代原nyt节点
			Node *c = new Node(nullptr, nullptr, nyt);
			c->num = sum++;
			c->weight = 1;

			Node *NYT = new Node(nullptr, nullptr, nyt);
			NYT->num = sum++;
			NYT->weight = 0;

			tree.addNode(nyt, NYT, BinaryTree::LeftChild);
			tree.addNode(nyt, c, BinaryTree::RightChild);
			nyt->weight = 1;

			//将编码值设为nyt+原字符值
			code = getHuffmanCode(nyt);
			finalCode[i] = code;
			cout << cbuffer << "：" << code << endl;

			//将新的字符放进buffers中
			charMap* new_cm = new charMap();
			new_cm->key = cbuffer;
			new_cm->p = nyt->p_right;
			new_cm->value = getHuffmanCode(nyt->p_right);
			buffers.push_back(*new_cm);

			//依次增加权重
			Node *root = nyt->p_parent;
			weightAdd(root);

			//设置新的nyt节点为原nyt节点的左孩子
			nyt = nyt->p_left;
		}
		i++;//字符个数
		
	}
	string output=finalCode[0];
	for (int j = 1; j < i; j++)
		output += finalCode[j];

	return output;

}
//从当前节点往上依次权重值加一，但是加一前先判断是否符合兄弟属性
void HuffmanTree::weightAdd(Node * p_node)
{
	while (p_node != nullptr) {
		Node* large = findLarge(p_node);
		if (large != p_node && !tree.isAncestor(p_node, large)) {
			tree.swap(large, p_node);
			int temp;
			temp = large->num;
			large->num = p_node->num;
			p_node->num = temp;
			for (auto iterator = buffers.begin(); iterator != buffers.end(); iterator++) {
				iterator->value = getHuffmanCode(iterator->p);
			}
		}
		p_node->weight++;
		p_node = p_node->p_parent;
	}
}

