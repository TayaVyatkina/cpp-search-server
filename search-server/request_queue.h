#pragma once
#include "search_server.h"
#include <deque>

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    vector<Document> AddFindRequest(const string& raw_query, DocumentPredicate document_predicate) {
        vector<Document> searched_documents = search_server_.FindTopDocuments(raw_query, document_predicate);

        AddQueryResult(raw_query, searched_documents);

        return searched_documents;
    }

    vector<Document> AddFindRequest(const string& raw_query, DocumentStatus status);

    vector<Document> AddFindRequest(const string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        string query;
        bool is_empty = false;
    };
    deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
    int no_result_requests_ = 0;
    int query_count_ = 0;

    void AddQueryResult(const string& query, const vector<Document>& searched_documents);
};