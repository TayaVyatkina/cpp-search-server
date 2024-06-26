#pragma once
#include "search_server.h"
#include "document.h"
#include <deque>

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        std::string query;
        bool is_empty = false;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
    int no_result_requests_ = 0;
    int query_count_ = 0;

    void AddQueryResult(const std::string& query, const std::vector<Document>& searched_documents);
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    std::vector<Document> searched_documents = search_server_.FindTopDocuments(raw_query, document_predicate);

    AddQueryResult(raw_query, searched_documents);

    return searched_documents;
}