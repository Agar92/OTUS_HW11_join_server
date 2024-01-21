#pragma once

#include <record.h>

#include <memory>
#include <shared_mutex>
#include <set>
#include <string>
#include <vector>

using std::string, std::vector, std::set, std::shared_mutex;

struct RecordsCompare
{
    bool operator()(const ptr<TableRecord>& left, const ptr<TableRecord>& right) const
    {
        return left->id < right->id;
    }
};


struct View
{
    vector<JoinedRecord> m_records;    
};

class Table
{
public:
    bool Insert(int id, string name); 
    void Truncate(); 
    friend View Intersect(const Table& left, const Table& right);
    friend View SymmetricDifference(const Table& left, const Table& right);
private:
    set<ptr<TableRecord>, RecordsCompare> m_records;
    mutable shared_mutex m_commands_mutex;
};
