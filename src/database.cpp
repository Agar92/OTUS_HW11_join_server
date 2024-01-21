#include <database.h>

Database::Database()
{
    m_interpreters.emplace("INSERT", &Database::InterpretInsert);
    m_interpreters.emplace("TRUNCATE", &Database::InterpretTruncate);
    m_interpreters.emplace("INTERSECTION", &Database::InterpretIntersection);
    m_interpreters.emplace("SYMMETRIC_DIFFERENCE", &Database::InterpretSymmetricDifference);
}

void Database::Interpret(Command command, ostream& out)
{
    auto it_interpreter = m_interpreters.find(command.m_name);
    if (it_interpreter == m_interpreters.end())
        throw std::invalid_argument("unknown command " + command.m_name);
    auto interpreter = it_interpreter->second;
    std::invoke(interpreter, this, command.m_arguments, out);
}

void Database::InterpretInsert(const vector<string>& args, ostream& out)
{
    if (args.size() != 3)
        throw std::invalid_argument("wrong args: 'table id name' are needed");
    Table * table;
    if (!Database::FindTable(args[0], table))
        throw std::invalid_argument("no table " + args[0]);
    int id;
    try
    {
        id = std::stoi(args[1]);
    }
    catch(const std::exception& e)
    {
        throw std::invalid_argument("wrong id format: " + args[1]);
    }
    if (!table->Insert(id, args[2]))
        throw std::invalid_argument("duplicate " + args[1]);
    WriteSuccess(out);
}

void Database::InterpretTruncate(const vector<string>& args, ostream& out)
{
    if (args.size() != 1) throw std::invalid_argument("wrong args: table_name is needed");
    Table * table;
    if (!Database::FindTable(args[0], table)) throw std::invalid_argument("no table " + args[0]);
    table->Truncate();
    WriteSuccess(out);
}

void Database::InterpretIntersection(const vector<string>& args, ostream& out)
{
    if (!args.empty()) throw std::invalid_argument("wrong args: no one is needed");
    auto intersection = Intersect(A, B);
    if (intersection.m_records.empty()) out << "EMPTY" << std::endl;
    for(const auto& join: intersection.m_records) WriteJoinedRecord(join, out);
    WriteSuccess(out);
}

void Database::InterpretSymmetricDifference(const vector<string>& args, ostream& out)
{
    if (!args.empty()) throw std::invalid_argument("wrong args: no one is needed");
    auto diff = SymmetricDifference(A, B);
    if (diff.m_records.empty()) out << "EMPTY" << std::endl;
    for(const auto& d: diff.m_records) WriteJoinedRecord(d, out);
    WriteSuccess(out);
}

bool Database::FindTable(const string& table_name, Table*& table)
{
    auto it_table = m_tables.find(table_name);
    if (it_table == m_tables.end())
        return false;
    table = it_table->second;
    return true;
}

void Database::WriteSuccess(ostream& out)
{
    out << "OK" << std::endl;
}

void Database::WriteJoinedRecord(const JoinedRecord& join, ostream& out)
{
    out << join.m_left->id << "," << join.m_left->name << "," << join.m_right->name << std::endl;
}
