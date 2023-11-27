#include "request_queue.h"
using namespace std;

    RequestQueue::RequestQueue(const SearchServer& search_server)
        :search_server_(search_server)
    {
    }
    
    vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
        vector<Document> searched_documents = search_server_.FindTopDocuments(raw_query, status);

        AddQueryResult(raw_query, searched_documents);

        return searched_documents;
    }

    vector<Document> RequestQueue::AddFindRequest(const string& raw_query) {
        return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
    }

    int RequestQueue::GetNoResultRequests() const {
        return no_result_requests_;
    }

    void RequestQueue::AddQueryResult(const string& query, const vector<Document>& searched_documents) {
        if (searched_documents.empty()) {
            ++no_result_requests_;
            requests_.push_back({ query, true });
        }
        else {
            requests_.push_back({ query, });
        }

        ++query_count_;
        if (query_count_ > min_in_day_) {
            if (requests_.front().is_empty) {
                --no_result_requests_;
            }
            requests_.pop_front();
        }
    }