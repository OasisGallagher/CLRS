using System;
using System.Collections.Generic;

class KmpMatcher
{
	int[] pi;
	string pattern;
	public KmpMatcher(string p)
	{
		if (string.IsNullOrEmpty(p))
			throw new ArgumentNullException();
		pattern = p;
		pi = prefix(p);
	}

	int[] prefix(string p)
	{
		int charMatched = 0;
		int[] result = new int[p.Length];
		result[0] = 0;

		for (int i = 1; i < p.Length; ++i)
		{
			for (; charMatched != 0 && p[charMatched] != p[i]; )
				charMatched = result[charMatched - 1];

			if (p[charMatched] == p[i])
				++charMatched;

			result[i] = charMatched;
		}
		
		// result[i]表示p中, [0, i]这i+1个字符的最大前缀长度.
		// 因此, 在获取时, 如果匹配了n个字符时的最大前缀长度,
		// 需要使用result[n - 1].
		return result;
	}

	public List<int> Match(string text)
	{
		int charMatched = 0;
		List<int> result = new List<int>();

		for (int i = 0; i < text.Length; ++i)
		{
			for (; charMatched != 0 && pattern[charMatched] != text[i]; )
				charMatched = pi[charMatched - 1];

			if (pattern[charMatched] == text[i])
				++charMatched;

			if (charMatched == pattern.Length)
			{
				result.Add(i + 1 - charMatched);
				charMatched = pi[charMatched - 1];
			}
		}

		return result;
	}
}

class Application
{
	static void Main()
	{
		string text = "ababcababdefgababd";
		string pattern = "ababd";
		KmpMatcher matcher = new KmpMatcher(pattern);
		List<int> matches = matcher.Match(text);
		foreach (int m in matches)
		{
			Console.WriteLine(text.Substring(m, pattern.Length));
		}
	}
}
