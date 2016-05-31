#include <string>
#include <iostream>

int native_match(const std::string& _text, const std::string& _pattern)
{
	if(_text.length() < _pattern.length()) return 0;
	int match_count = 0;
	size_t dis = _text.length() - _pattern.length();

	for(size_t i = 0; i <= dis; ++i)
	{
		size_t j = 0;
		for(; j < _pattern.length() && _pattern[j] == _text[i + j]; ++j)
			;

		if(j == _pattern.length())
		{
			std::cout << "match at shift " << i << std::endl;
			++match_count;
		}
	}
	return match_count;
}

int main()
{
	std::cout << native_match("3141231231231239921", "12312") << " matching(s) in total.\n";
}
