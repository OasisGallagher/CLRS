/**
a) 	��������, ��ν�ĸ�������, ���е����ָ�����N, ��ʾ����, ��������, ���е�����, ���е�
	λ����������N. ��������[12, 345], N = 5.����[67, 897]һ��, N = 5.
	���Ȱ������ֵĳ��Ƚ������е����ֵַ���ͬbucket��, ������Ϊi������, �ŵ�(i - 1)��bucket��.
	Ȼ���ÿ��bucket����radix sort.
	ʱ�����:
		����, ͨ��log10(x)�Ϳ���֪��x�ĳ���, ��˿����ڳ���ʱ���ڽ������е����ֵַ�bucket��.
		Ȼ��, ��bucket����radixsort, ���ĸ��Ӷ�Ϊ
			��bucket�е����ֵĳ��� * (���ֵĽ���(������10) + ���ֵĸ���).
		չ��:
			��bucket�е����ֵĳ��� * ���ֵĽ���(������10) + ��bucket�е����ֵĳ��� * ���ֵĸ���.
		��:
			��bucket�е����ֵĳ��� * ���ֵĽ���(������10) + ���bucket�����е������������ֵĸ���(��123��3��, 45��2��).
		�������е�bucket, �����Գ���, ���õ�
			CONSTANT + SUM(��bucket�е����ֵĳ��� * (���ֵĽ���(������10) + ���ֵĸ���))
		��: CONSTANT + N = O(N)
*/


/**
b)	���ַ��������������, ��Щ�ַ��������������ܵ���ĸ����N(��������ܵ���ĸ�ĸ���, ��������ͬ��2����ĸ��������ĸ).
	��һ�ַ������ǰѶ̵��ַ�����������0, ʹ�����볤���ַ���������ͬ, Ȼ��Ӻ���ǰ����countsort.
	����ÿ���ַ���, Ȼ��β����0, ʹ�ø��ӶȲ�������O(N).
	�����Ҫ��ǰ������countsort. ��Ȼ, ֱ�ӵ����ǲ����Ե�.����:
	b a
	a c
	a b
	countsort��һλ:
	a c
	a b
	b a
	countsort�ڶ�λ:
	b a
	a b
	a c
	������Ȼ�Ǵ����.
	�������һλ֮��, ��Ҫ����, ����ac��ab�ĵڶ�λ, �Լ�ba�ĵڶ�λ(����countsort).
	Ҳ���ǽ��������һλ���ַ���������, ��һλ��ͬ�ļ����ַ������յڶ�λ��������.
*/

// �ַ���������, ����ַ����ĳ���.
size_t MaxLengthOfStringArray(const String* input, int from, int to);
const size_t maxlength = MaxLengthOfStringArray(input, 0, CountOfArrayElements);

// �����ַ����ĵ�p��λ��, ��������input[from, to).
void CountSort(String& input, int from, int to, int p);

size_t GetFirstStringWithCharAtPos(const String* input, int from, int to, int ch, int p);
size_t GetLastStringWithCharAtPos(const String* input, int from, int to, int ch, int p);

typedef std::string String;
// inputΪ��Ҫ������ַ�������.
// fromΪ������������[from, to)�����ڵ��ַ���.
// p��ʾ�����ַ����ĵ�p��λ�ý���countsort.���p�����ַ����ĳ���, ��ô�̵��ַ����ŵ�����.
void SortStrArray(String* input, int from, int to, int p) {
	if(p >= maxlength) return;
	CountSort(input, from, to, p);
	
	// ����Ĺ��̵�Ŀ��, �����ҵ��ϴ�CountSort�������pλ�õ��ַ���ͬ���������ʼ����.
	// ���λ��, ��CountSort�м����Ը���, �����ٽ��м���.
	// ��Ϊcountsortͳ����, pλ��Ϊa���ַ����ĸ���(ca), Ϊb�ĸ���(cb) ...
	// ����, ������first, last����0, ca - 1, ��һ�ֵ�first, last����ca, cb - 1 ...
	// �������ʱ���Ǽ�����CountSort��O(26 + (from - to))���.
	// ����ļ��㷽��ֻ��Ϊ��˵��.
	for(int c = 'a'; c <= 'z'; ++c) {
		// ȡ�õ�һ��pλ����c���ַ���������.
		size_t first = GetFirstStringWithCharAtPos(input, from, to, c, p);
		size_t last = GetLastStringWithCharAtPos(input, from, to, c, p);
		if(first < last) {
			SortStrArray(input, first, last, p + 1);
		}
	}
}
/**
	���Ӷȷ���: ��ȡ����ַ������ȵĹ���, ��������ÿ���ַ����е�ÿ���ַ�, ��O(N).
	SortStrArrayҲ�Ǳ����ַ����е�ÿ���ַ�, û���ظ�����, ����Ҳ��O(N).
	��, �ܵĸ��Ӷ���O(N).
*/

