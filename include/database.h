#pragma once

#include <command.h>
#include <table.h>

#include <functional>
#include <ostream>
#include <map>
#include <mutex>

using std::ostream;

class Database
{
public:
    Database();
    void Interpret(Command command, ostream& out);
    
private:
    void InterpretInsert(const vector<string>& args, ostream& out);
    void InterpretTruncate(const vector<string>& args, ostream& out);
    void InterpretIntersection(const vector<string>& args, ostream& out);
    void InterpretSymmetricDifference(const vector<string>& args, ostream& out);

    bool FindTable(const string& table_name, Table*& table);
    void WriteSuccess(ostream& out);
    void WriteJoinedRecord(const JoinedRecord& join, ostream& out);

    Table A, B;
    std::map<string, Table*> m_tables
    {
        {string("A"), &A},
        {string("B"), &B}
    }; 

    std::map<string, void (Database::*)(const vector<string>&, ostream&)> m_interpreters;

};
