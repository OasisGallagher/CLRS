#include <string>
#include <iostream>
#define ValueOf(_Char)	(_Char - '0')

int HornerRole(const std::string& _Str, const int _Radix = 10)
{
	int result = 0;
	for(size_t i = 0; i < _Str.length(); ++i)
		result = _Radix * result + ValueOf(_Str[i]);
	return result;
}

int RabinKarpStringMatching(const std::string& _Text, const std::string& _Pattern)
{
	int count = 0;
	const int Sieve = 13, Radix = 10;
	const size_t text_length = _Text.length(), pat_length = _Pattern.length();
	if(pat_length > text_length) return false;

	int height = 1;
	for(size_t i = 0; i < pat_length - 1; ++i)
		height *= 10;
	
	int p_value = 0, t_value = 0;
	/*for(size_t i = 0; i < pat_length; ++i)
	{	// Horner role.
		p_value = (p_value * Radix + ValueOf(_Pattern[i])) % Sieve;
		t_value = (t_value * Radix + ValueOf(_Text[i])) % Sieve;
	}*/
	p_value = HornerRole(_Pattern) % Sieve;
	t_value = HornerRole(_Text.substr(0, pat_length)) % Sieve;

	const size_t ShiftMax = text_length - pat_length;
	
	for(size_t i = 0; i <= ShiftMax; ++i)
	{
		const int HighBit = ValueOf(_Text[i]);
		if(p_value == t_value)
		{
			if(_Pattern == _Text.substr(i, pat_length))
			{
				++count;
				std::cout << "string match at shift " << i << std::endl;
			}
		}
		
		if(i < ShiftMax)
		{
			t_value = ((t_value - (height % Sieve) * HighBit) * Radix + ValueOf(_Text[i + pat_length])) % Sieve;
			// if t_value < 0, then...
			(t_value < 0) && (t_value += (1 - t_value / Sieve) * Sieve);
		}
	}

	return count;
}

int main()
{
	//std::cout << "input text: ";
	std::string text("2359023141526739921"), pattern("31415");
	//std::cin >> text;
	//std::cout << "input pattern: ";
	//std::cin >> pattern;

	std::cout << RabinKarpStringMatching(text, pattern) << " matching(s) in total.\n";
}
