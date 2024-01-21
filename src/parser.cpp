#include <command.h>
#include <parser.h>

#include <string>

using std::string, std::vector;

void Parser::ParseInput(const string& input_text)
{
    using namespace std;
    std::string InputText="";
    if (input_text.empty()) return;
    {
        int i=0;
        while(std::isspace( input_text[i] )) i++;
        InputText=input_text.substr(i);
    }
    if (!m_unfinished_command.empty())
        m_unfinished_command += InputText;
    const string& parse_str = m_unfinished_command.empty() ? InputText : m_unfinished_command;
    size_t start = 0;
    size_t terminator_pos;
    while(start < parse_str.size())
    {
        terminator_pos = parse_str.find_first_of(m_terminator, start);
        if (terminator_pos != string::npos)
        {
            m_ready_commands.push(ParseCommand(parse_str, start, terminator_pos));
            start = terminator_pos + 1;
        }
        else break;
    }
    if(start < parse_str.size())
        m_unfinished_command = parse_str.substr(start);
}

Command Parser::ParseCommand(const string& full_text,
                             size_t start,
                             size_t text_end)
{
    auto command_end = full_text.find_first_of(m_separator, start);
    command_end = command_end != string::npos ? command_end : text_end;
    vector<string> args;
    size_t arg_start;
    size_t arg_end;
    for(arg_start = command_end + 1;
        arg_start < text_end;
        arg_start = arg_end + 1)
    {   
        arg_end = full_text.find_first_of(m_separator, arg_start);    
        if ((arg_end != string::npos) && (arg_end < text_end))    
        {
            args.push_back(full_text.substr(arg_start, arg_end - arg_start));
        }
        else
        {
            args.push_back(full_text.substr(arg_start, text_end - arg_start));
            break;
        }
    }
    return Command(full_text.substr(start, command_end - start), std::move(args));
}

bool Parser::HasReadyCommands() const
{
    return !m_ready_commands.empty();
}

Command Parser::GetReadyCommand()
{
    auto command = m_ready_commands.front();
    m_ready_commands.pop();
    return command;
}