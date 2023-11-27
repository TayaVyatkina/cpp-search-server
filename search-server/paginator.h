#pragma once
#include <sstream>

template <typename It>
class IteratorRange {
public:
    IteratorRange(It page_begin, It page_end, int page_size)
        : page_begin(page_begin)
        , page_end(page_end)
        , page_size(page_size) {
    }
    It begin() const {
        return page_begin;
    }
    It end() const {
        return page_end;
    }
    size_t size() const {
        return page_size;
    }
private:
    It page_begin, page_end;
    size_t page_size;
};

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator page_begin, Iterator page_end, size_t page_count) {
        if (page_count < 1) {
            throw invalid_argument("page count is invalid");
        }
        size_t page_size = distance(page_begin, page_end) / page_count;
        size_t add_to_page = distance(page_begin, page_end) % page_count;

        for (auto i = 0; i < page_count; ++i) {
            if (add_to_page > 0) {
                pages_.push_back(IteratorRange(page_begin, (page_begin + page_size + 1), page_size + 1));
                --add_to_page;
                page_begin += page_size + 1;
            }
            else {
                pages_.push_back(IteratorRange(page_begin, page_begin + page_size, page_size));
                page_begin += page_size;
            }
        }
    }
    auto begin() const {
        return pages_.begin();
    }

    auto end() const {
        return pages_.end();
    }

private:
    vector<IteratorRange<Iterator>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}



template <typename Iterator>
std::ostream& operator<<(std::ostream& os, const IteratorRange<Iterator>& range) {
    for (Iterator it = range.begin(); it != range.end(); ++it)
    {
        os << *it;
    }

    return os;
}
