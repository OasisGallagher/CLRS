#include <iostream>
#include <vector>
#pragma warning(disable:4996)

// radix sort, and how to use it.
class DateSorter {
public:
	struct Date;
	typedef unsigned (*GetCmpValue)(const Date& date);
	DateSorter() : cont_(NULL), cursize_(0) { }
	~DateSorter() { delete[] cont_; }

	// insert a date.
	void PushDate(const Date& date) {
		if(cont_ == NULL) cont_ = new Date[128];
		if(cursize_ >= 128) throw "container is full";
		cont_[cursize_++] = date;
	}
	// sort the dates with radix sort.
	void SortDate() {
		// sort the day part, month part and year part one by one.
		CountSort(31, cont_, cursize_, GetDay);
		CountSort(12, cont_, cursize_, GetMonth);
		CountSort(100, cont_, cursize_, GetYear);
	}

	void Trace() {
		for(unsigned i = 0; i < cursize_; ++i)
			std::cout << cont_[i] << std::endl;
	}

	struct Date {
		Date(unsigned Y = YEAR_BEGIN, unsigned M = 1, unsigned D = 1) 
			: year(Y), month(M), day(D) {
		}
		friend std::ostream& operator << (std::ostream& os, const Date& date);
		unsigned year;
		unsigned month;
		unsigned day;
	};
private:
	// sort part of Date returned by cmp.
	void CountSort(unsigned upper, Date* buffer, unsigned size, GetCmpValue cmp) {
		unsigned* counter = new unsigned[++upper];
		std::fill(counter, counter + upper, 0);
		for(unsigned i = 0; i < size; ++i)
			++counter[cmp(buffer[i])];

		for(unsigned i = 1; i < upper; ++i)
			counter[i] += counter[i - 1];

		Date* result = new Date[size];
		// do NOT use for(unsigned i = size; i >= 0; --i) ...
		// since unsigned is always greater than or equal to 0.
		unsigned mutable_size = size;
		do {
			--mutable_size;
			result[counter[cmp(buffer[mutable_size])] - 1] = buffer[mutable_size];
			counter[mutable_size] -= unsigned(counter[mutable_size] > 0);
		}while(mutable_size != 0);

		std::copy(result, result + size, buffer);

		delete[] result;
		delete[] counter;
	}

	// returns the key to be compared.
	static unsigned GetYear(const Date& date) {
		return date.year - YEAR_BEGIN;
	}
	static unsigned GetMonth(const Date& date) {
		return date.month;
	}
	static unsigned GetDay(const Date& date) {
		return date.day;
	}
	typedef Date* DateContainer;
	DateContainer cont_;
	unsigned cursize_;
	static const int YEAR_BEGIN = 1970;
};

std::ostream& operator << (std::ostream& os, const DateSorter::Date& date) {
	return os << "year: " << date.year << ", month: " << date.month << ", day: " << date.day;
}

int main() {
	DateSorter st;
	st.PushDate(DateSorter::Date(2013, 10, 13));
	st.PushDate(DateSorter::Date(1989, 7, 23));
	st.PushDate(DateSorter::Date(1995, 2, 27));
	st.PushDate(DateSorter::Date(2000, 7, 31));
	st.PushDate(DateSorter::Date(2012, 12, 20));
	st.PushDate(DateSorter::Date(1989, 11, 11));
	st.SortDate();
	st.Trace();
}
