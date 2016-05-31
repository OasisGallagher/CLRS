/**
a) 	该问题中, 所谓的该数组中, 所有的数字个数是N, 表示的是, 该数组中, 所有的数字, 所有的
	位数加起来是N. 比如数组[12, 345], N = 5.这与[67, 897]一样, N = 5.
	首先按照数字的长度将数组中的数字分到不同bucket里, 即长度为i的数字, 放到(i - 1)号bucket中.
	然后对每个bucket进行radix sort.
	时间分析:
		首先, 通过log10(x)就可以知道x的长度, 因此可以在常数时间内将数组中的数字分到bucket中.
		然后, 对bucket进行radixsort, 它的复杂度为
			该bucket中的数字的长度 * (数字的进制(这里是10) + 数字的个数).
		展开:
			该bucket中的数字的长度 * 数字的进制(这里是10) + 该bucket中的数字的长度 * 数字的个数.
		即:
			该bucket中的数字的长度 * 数字的进制(这里是10) + 这个bucket中所有的数字所含数字的个数(即123含3个, 45含2个).
		排序所有的bucket, 并忽略常数, 即得到
			CONSTANT + SUM(该bucket中的数字的长度 * (数字的进制(这里是10) + 数字的个数))
		即: CONSTANT + N = O(N)
*/


/**
b)	对字符串数组进行排序, 这些字符串数组所含的总的字母数是N(这个数是总的字母的个数, 而不是相同的2个字母算两个字母).
	有一种方法就是把短的字符串后面填上0, 使得它与长的字符串长度相同, 然后从后向前进行countsort.
	遍历每个字符串, 然后尾部填0, 使得复杂度不可能是O(N).
	因此需要从前向后调用countsort. 当然, 直接调用是不可以的.比如:
	b a
	a c
	a b
	countsort第一位:
	a c
	a b
	b a
	countsort第二位:
	b a
	a b
	a c
	这样显然是错误的.
	排序完第一位之后, 需要的是, 排序ac和ab的第二位, 以及ba的第二位(两次countsort).
	也就是将排序完第一位的字符串数组中, 第一位相同的几个字符串按照第二位进行排序.
*/

// 字符串数组中, 最长的字符串的长度.
size_t MaxLengthOfStringArray(const String* input, int from, int to);
const size_t maxlength = MaxLengthOfStringArray(input, 0, CountOfArrayElements);

// 按照字符串的第p个位置, 计数排序input[from, to).
void CountSort(String& input, int from, int to, int p);

size_t GetFirstStringWithCharAtPos(const String* input, int from, int to, int ch, int p);
size_t GetLastStringWithCharAtPos(const String* input, int from, int to, int ch, int p);

typedef std::string String;
// input为需要排序的字符串数组.
// from为排序这个数组的[from, to)索引内的字符串.
// p表示按照字符串的第p个位置进行countsort.如果p超出字符串的长度, 那么短的字符串排到后面.
void SortStrArray(String* input, int from, int to, int p) {
	if(p >= maxlength) return;
	CountSort(input, from, to, p);
	
	// 下面的过程的目的, 就是找到上次CountSort排序过的p位置的字符相同的数组的起始索引.
	// 这个位置, 在CountSort中即可以给出, 不必再进行计算.
	// 因为countsort统计了, p位置为a的字符串的个数(ca), 为b的个数(cb) ...
	// 所以, 下述的first, last就是0, ca - 1, 下一轮的first, last就是ca, cb - 1 ...
	// 所以这个时间是计算在CountSort的O(26 + (from - to))里的.
	// 下面的计算方法只是为了说明.
	for(int c = 'a'; c <= 'z'; ++c) {
		// 取得第一个p位置是c的字符串的索引.
		size_t first = GetFirstStringWithCharAtPos(input, from, to, c, p);
		size_t last = GetLastStringWithCharAtPos(input, from, to, c, p);
		if(first < last) {
			SortStrArray(input, first, last, p + 1);
		}
	}
}
/**
	复杂度分析: 获取最大字符串长度的过程, 即遍历和每个字符串中的每个字符, 即O(N).
	SortStrArray也是遍历字符串中的每个字符, 没有重复遍历, 所以也是O(N).
	即, 总的复杂度是O(N).
*/

