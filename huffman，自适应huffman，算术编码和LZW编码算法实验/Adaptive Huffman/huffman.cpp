#include<iostream>
#include <stack>
#include<deque>
#include<queue>
#include<string>
#include "huffman.h"
using namespace std;
int HuffmanTree::sum = 1;


//��������Ա����ʵ��
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
//�����ڵ�
bool BinaryTree::swap(Node * p_nodeA, Node * p_nodeB)
{
	if (p_nodeA == nullptr || p_nodeB == nullptr || p_nodeA == p_nodeB)
		return false;
	Node *pTemp;
	//�ж������ӻ����Һ���
	//AΪ����
	if (getBrotherState(p_nodeA) == LeftChild)
	{      //�����Ϊ��ֱ�ӻ�
		if (getBrotherState(p_nodeB) == LeftChild) {
			pTemp = p_nodeA->p_parent->p_left;
			p_nodeA->p_parent->p_left = p_nodeB->p_parent->p_left;
			p_nodeB->p_parent->p_left = pTemp;
		}
		//���һ��һ��ֱ�ӽ���
		else {
			pTemp = p_nodeA->p_parent->p_left;
			p_nodeA->p_parent->p_left = p_nodeB->p_parent->p_right;
			p_nodeB->p_parent->p_right = pTemp;
		}
	}
	//AΪ�Һ���
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



//����������Ա����ʵ��
HuffmanTree::HuffmanTree() :tree(0, 0)
{
}

HuffmanTree::~HuffmanTree()
{
	
}



//��ȡ�ڵ�Ĺ���������
string HuffmanTree::getHuffmanCode(Node *p_n)
{
	string huffmanCode = "";
	stack<Node *> stack;
	deque<char> code;

	//��p_n�ڵ�Ϊ����ʱ����и����������������Ϊ�����Һ���
	if (p_n == tree.getRoot())
		p_n = p_n->p_right;
	//������ƣ���Ϊ���ӵ�ʱ������0����Ϊ�Һ��ӵ�ʱ������1��
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

//�ҵ����ڿ������ڵ��ŵĽڵ�
Node * HuffmanTree::findLarge(Node *p_node)
{
	std::stack<Node *> stack;
	Node *p = tree.getRoot();
	Node *large = p;
	while (p || !stack.empty()) {
		if (p != nullptr) {
			stack.push(p);
			if (p->weight == p_node->weight) {
				//���large����ͬȨ���£�����largeΪp
				if (large->weight != p->weight) {
					large = p;
				}
				//ͬȨ���µ�large��pС������largeΪp
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
	//large�������Ǹ��ڵ㣬��largeΪ���ڵ�ʱ����ԭ�ڵ�
	if (large == tree.getRoot()) {
		return p_node;
	}
	return large;
}


//���뺯��
string HuffmanTree::encode(string s)
{
	string finalCode[100];
	int i=0;
	string::iterator it;
	//��ȡ�ַ�������nyt�ڵ�Ϊ���ڵ�
	char cbuffer;
	Node *nyt = tree.getRoot();
	for(it=s.begin();it!=s.end();it++)
	{
		cbuffer = *it;
		bool exist = false;
		string code;
		auto existNode = buffers.begin();	//buffers��һ������������cbuffer���������е�ʱ��existNodeָ��ýڵ�
		for (existNode; existNode != buffers.end(); existNode++) 
		{
			if (existNode->key == cbuffer)
			{
				code = existNode->value;
				exist = true;
				finalCode[i] = code;
				cout << cbuffer << "�� " <<code << endl;
				break;
			}
		}

		if (exist) {
			Node *root = existNode->p;
			weightAdd(root);
		}
		else {
			//���ַ�����������ʱ�����½������������ԭnyt�ڵ�
			Node *c = new Node(nullptr, nullptr, nyt);
			c->num = sum++;
			c->weight = 1;

			Node *NYT = new Node(nullptr, nullptr, nyt);
			NYT->num = sum++;
			NYT->weight = 0;

			tree.addNode(nyt, NYT, BinaryTree::LeftChild);
			tree.addNode(nyt, c, BinaryTree::RightChild);
			nyt->weight = 1;

			//������ֵ��Ϊnyt+ԭ�ַ�ֵ
			code = getHuffmanCode(nyt);
			finalCode[i] = code;
			cout << cbuffer << "��" << code << endl;

			//���µ��ַ��Ž�buffers��
			charMap* new_cm = new charMap();
			new_cm->key = cbuffer;
			new_cm->p = nyt->p_right;
			new_cm->value = getHuffmanCode(nyt->p_right);
			buffers.push_back(*new_cm);

			//��������Ȩ��
			Node *root = nyt->p_parent;
			weightAdd(root);

			//�����µ�nyt�ڵ�Ϊԭnyt�ڵ������
			nyt = nyt->p_left;
		}
		i++;//�ַ�����
		
	}
	string output=finalCode[0];
	for (int j = 1; j < i; j++)
		output += finalCode[j];

	return output;

}
//�ӵ�ǰ�ڵ���������Ȩ��ֵ��һ�����Ǽ�һǰ���ж��Ƿ�����ֵ�����
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

