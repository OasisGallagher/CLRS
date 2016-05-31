#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <boost/pending/disjoint_sets.hpp>
#include <boost/pending/property.hpp>

class Element
{
public:
    explicit
    Element(int n) : mSomeInt(n) { }

    int someInt() const { return mSomeInt; }

    // The following class members are specific for the disjoint_sets
    // implementation
    size_t dsID;
    size_t dsRank;
    size_t dsParent;

private:
    int mSomeInt;
};

inline bool
operator == (Element const& lhs, Element const& rhs)
{
    return lhs.someInt() == rhs.someInt();
}

inline bool
operator != (Element const& lhs, Element const& rhs)
{
    return !(lhs == rhs);
}

class Parent
{
public:
    Parent(std::vector<Element>& e) : mElements(e) { }
    std::vector<Element>& mElements;
};

class Rank
{
public:
    Rank(std::vector<Element>& e) : mElements(e) { }
    std::vector<Element>& mElements;
};

template <>
struct boost::property_traits<Rank*>
{
    typedef size_t value_type;
};

inline Element const&
get(Parent* pa, Element const& k)
{
    return pa->mElements.at(k.dsParent);
}

// 
inline void	// 使得k的parent指向val.
put(Parent* pa, Element k, Element const& val)
{
    pa->mElements.at(k.dsID).dsParent = val.dsID;
}

inline size_t const&	// 获取k的rank.
get(Rank*, Element const& k)
{
    return k.dsRank;
}

inline void	// 设置k的rank为val.
put(Rank* pa, Element k, size_t const& val)
{
    pa->mElements.at(k.dsID).dsRank = val;
}

int main()
{
    std::vector<Element> elements;
    elements.reserve(30);
    for (size_t i = 0; i < elements.capacity(); ++i)
    {
        elements.push_back(Element(rand() % 90));
    }

    for (size_t i = 0; i < elements.size(); ++i)
    {
        elements[i].dsID = i;
    }

    Rank rank(elements);
    Parent parent(elements);

    boost::disjoint_sets<Rank*, Parent*> sets(&rank, &parent);

    for (size_t i = 0; i < elements.size(); ++i)
    {
        sets.make_set(elements.at(i));
    }

    for (size_t i = 0; i < elements.size(); ++i)
    {
        // Union between this element and one randomly chosen from the rest
        size_t j = rand() % elements.size();
        sets.union_set(elements[i], elements[j]);
    }

    std::cout << "Found " << sets.count_sets(elements.begin(), elements.end()) << " sets:" << std::endl;

    sets.compress_sets(elements.begin(), elements.end());

    std::cout << std::endl << "After path compression:" << std::endl;
}
