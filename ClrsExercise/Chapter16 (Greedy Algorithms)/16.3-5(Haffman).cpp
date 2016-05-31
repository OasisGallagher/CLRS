/*
 * 在进行Huffman编码后, 编码后的信息被写入. 为了读取, 在写入的信息中, 必须
 * 加入编码的树的信息, 否则, 无法根据信息来解码.
 * 本题要求证明的, 实际上是存储这棵树所需的空间, 为:
 * 2n - 1 + n * ceil(log(n)).
 * 由上式可以推断:
 * 2n-1是一颗叶子数为n个满二叉树(每个节点有0或2个节点)的所有节点的数目.
 * ceil(log(n))为表示[0, n-1]这n个数, 所需要的二进制位个数.
 *
 * 题意中的需要编码的字符为0~n-1, 恰好需要ceil(log(n))位, 那么, n个这样的
 * 字符就需要n*ceil(log(n))位.
 *
 * 参考:
 * http://stackoverflow.com/questions/759707/efficient-way-of-storing-huffman-tree?answertab=votes#tab-top
 * 对一棵Huffman树, 将叶子节点记做1, 内节点记做0, 对树进行先序遍历.
 * 遇到叶子节点, 写入1, 并写入n*ceil(log(n))位的该字符的值.
 * 遇到内节点, 写入0, 并继续进行遍历.
 * 这样, 每个节点只会被访问一次, 并写入0或1到文件中.
 * 所以, 对于一棵有2n-1个节点的满二叉树, 写入它以及对应的原始码所需的空间
 * 恰为2n - 1 + n * ceil(log(n)).
 * 例如: 对于0, 1, 2, 3:
 *        6
 *     3    3
 *   1   2
 * 0   1
 * 写入树:
 * 000100 101 110 111
 * 000100: 先序遍历, 6->3->1->0, 写入顺序为0001, 再写入0, 为00.
 * 101: 继续遍历, 到1, 写入1, 再写入1, 为01.
 * 110: 继续遍历, 到2, 写入1, 再写入2, 为10.
 * 111: 继续遍历, 到3, 写入1, 再写入3, 为11.
 * 共15bits = 2 * 4 - 1 + 4 * ceil(log(4)).
 * 读取树:
 * 读1 bit,
 * 如果是0, 那么, 递归调用该过程, 返回值作为该节点的左孩子.
 * 再调用该过程, 返回值作为该节点的右孩子.
 * 如果1是1, 那么, 读取2bits的原始码, 返回一个叶子节点, 值为该原始码.
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

