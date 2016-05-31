/*
 * �ڽ���Huffman�����, ��������Ϣ��д��. Ϊ�˶�ȡ, ��д�����Ϣ��, ����
 * ��������������Ϣ, ����, �޷�������Ϣ������.
 * ����Ҫ��֤����, ʵ�����Ǵ洢���������Ŀռ�, Ϊ:
 * 2n - 1 + n * ceil(log(n)).
 * ����ʽ�����ƶ�:
 * 2n-1��һ��Ҷ����Ϊn����������(ÿ���ڵ���0��2���ڵ�)�����нڵ����Ŀ.
 * ceil(log(n))Ϊ��ʾ[0, n-1]��n����, ����Ҫ�Ķ�����λ����.
 *
 * �����е���Ҫ������ַ�Ϊ0~n-1, ǡ����Ҫceil(log(n))λ, ��ô, n��������
 * �ַ�����Ҫn*ceil(log(n))λ.
 *
 * �ο�:
 * http://stackoverflow.com/questions/759707/efficient-way-of-storing-huffman-tree?answertab=votes#tab-top
 * ��һ��Huffman��, ��Ҷ�ӽڵ����1, �ڽڵ����0, ���������������.
 * ����Ҷ�ӽڵ�, д��1, ��д��n*ceil(log(n))λ�ĸ��ַ���ֵ.
 * �����ڽڵ�, д��0, ���������б���.
 * ����, ÿ���ڵ�ֻ�ᱻ����һ��, ��д��0��1���ļ���.
 * ����, ����һ����2n-1���ڵ����������, д�����Լ���Ӧ��ԭʼ������Ŀռ�
 * ǡΪ2n - 1 + n * ceil(log(n)).
 * ����: ����0, 1, 2, 3:
 *        6
 *     3    3
 *   1   2
 * 0   1
 * д����:
 * 000100 101 110 111
 * 000100: �������, 6->3->1->0, д��˳��Ϊ0001, ��д��0, Ϊ00.
 * 101: ��������, ��1, д��1, ��д��1, Ϊ01.
 * 110: ��������, ��2, д��1, ��д��2, Ϊ10.
 * 111: ��������, ��3, д��1, ��д��3, Ϊ11.
 * ��15bits = 2 * 4 - 1 + 4 * ceil(log(4)).
 * ��ȡ��:
 * ��1 bit,
 * �����0, ��ô, �ݹ���øù���, ����ֵ��Ϊ�ýڵ������.
 * �ٵ��øù���, ����ֵ��Ϊ�ýڵ���Һ���.
 * ���1��1, ��ô, ��ȡ2bits��ԭʼ��, ����һ��Ҷ�ӽڵ�, ֵΪ��ԭʼ��.
 */
#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>

struct code {
	std::string str;
	std::string::size_type pos;
	std::string::value_type read() {
		return str[pos++];
	}
};

const unsigned char Bit0 = '0';
const unsigned char Bit1 = '1';

class node {
	int freq;
	unsigned char ch;
	node* lchild;
	node* rchild;
public:
	friend void write_tree(node*, std::string&);
	friend node* read_tree(code&);
	friend void destroy_tree(node* root);
	node(int Freq, unsigned char Ch = 0) : freq(Freq), ch(Ch), lchild(NULL), rchild(NULL) { }
	node(node* lc, node* rc) {
		freq = lc->freq + rc->freq;
		lchild = lc, rchild = rc;
		ch = 0;
	}
	bool operator > (const node& other) const {
		return freq > other.freq;
	}
	bool leaf() const {
		return lchild == NULL && rchild == NULL;
	}

	// DEBUG.
	void* operator new(size_t size) {
		++Count;
		return ::operator new(size);
	}
	void operator delete(void* ptr) {
		--Count;
		::operator delete(ptr);
	}
	static int Count;
};

int node::Count = 0;

typedef node* nodeptr;
typedef std::vector<nodeptr> container;

void write_tree(nodeptr root, std::string& code);
nodeptr read_tree(const code&);
void chkmem();
void huffman(const container& cont);
void destroy_tree(nodeptr root);

int main() {
	const int freq[] = { 6, 1, 6, 2, 5 };
	const char arr[] = { 'A', 'B', 'C', 'D', 'E' };
	container cont;
	for(size_t i = 0; i < _countof(freq); ++i)
		cont.push_back(new node(freq[i], arr[i]));

	huffman(cont);

	atexit(chkmem);
}

void chkmem() {
	std::cout << "\nDEBUG: " << node::Count << " node(s) left\n";
}

void huffman(const container& cont) {
	std::priority_queue<container::value_type, std::vector<container::value_type>, std::greater<container::value_type> > 
		q(cont.begin(), cont.end());
	for(; q.size() > 1; ) {
		container::value_type n1 = q.top(); q.pop();
		container::value_type n2 = q.top(); q.pop();
		nodeptr n3 = new node(n1, n2);
		q.push(n3);
	}

	std::string c;
	write_tree(q.top(), c);
	std::cout << std::endl;

	code cd;
	cd.str = c, cd.pos = 0;
	nodeptr r = read_tree(cd);
	std::string c2;
	write_tree(r, c2);

	assert(c == c2);

	destroy_tree(q.top());
	destroy_tree(r);
}

void write_tree(nodeptr root, std::string& c) {
	if(root->leaf()) {
		c += Bit1;
		c += root->ch;
		std::cout << Bit1;
		std::cout << root->ch;
	}
	else {
		c += Bit0;
		std::cout << Bit0;
		write_tree(root->lchild, c);
		write_tree(root->rchild, c);
	}
}

nodeptr read_tree(code& c) {
	if(c.pos >= c.str.size()) return NULL;

	if(c.read() == Bit1) {
		return new node(-1, c.read());
	}

	nodeptr lc = read_tree(c);
	nodeptr rc = read_tree(c);

	nodeptr r = new node(-1);
	r->lchild = lc;
	r->rchild = rc;
	return r;
}

void destroy_tree(nodeptr root) {
	if(root != NULL) {
		destroy_tree(root->lchild);
		destroy_tree(root->rchild);
		delete root;
	}
}

