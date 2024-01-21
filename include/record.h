#pragma once

#include <memory>
#include <string>

template <typename T>
using ptr=std::shared_ptr<T>;

struct TableRecord
{
    TableRecord(int rec_id, std::string rec_name) : id(rec_id), name(rec_name) {}

    int id;
    std::string name;
};

struct JoinedRecord
{
    JoinedRecord(ptr<TableRecord> left, ptr<TableRecord> right) : m_left(left), m_right(right) {}
    ptr<TableRecord> m_left;
    ptr<TableRecord> m_right;
};
