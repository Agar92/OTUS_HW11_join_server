#include <parser.h>
#include <database.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket, std::shared_ptr<Database> database) :
    m_socket(std::move(socket)), m_database(database) {}

  void start(){read();}

private:
  void read()
  {
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_read_buffer, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            std::ostream os(&m_write_streambuf);

            mParser->ParseInput(std::string{m_read_buffer, length});
            while (mParser->HasReadyCommands())
            {
                try
                { 
                  m_database->Interpret(mParser->GetReadyCommand(), os);
                }
                catch(const std::exception& ex)
                {
                  os << "ERR " << ex.what() << std::endl;
                }
             }                            
          }
          write();
        });
  }

  void write()
  {
    auto self(shared_from_this());
    m_socket.async_send(m_write_streambuf.data(),
    [this, self](boost::system::error_code ec, std::size_t length)
    {
      m_write_streambuf.consume(length);
      if (!ec){read();}
    });
  }

  tcp::socket m_socket;
  std::shared_ptr<Database> m_database;

  inline static const int  max_length = 1024;
  char m_read_buffer[max_length];
  boost::asio::streambuf m_write_streambuf;

  std::unique_ptr<Parser> mParser = std::make_unique<Parser>(' ', '\n');
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port) :
    m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
  {
    accept();
  }

private:
  void accept()
  {
    m_acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket), m_database)->start();
          }

          accept();
        });
  }

  tcp::acceptor m_acceptor;
  std::shared_ptr<Database> m_database = std::make_shared<Database>();
};

int main(int argc, char* argv[])
{
  try
  {
    short port;
    if (argc != 2)
    {
      port = 10000;
    }
    else
    {
      port = std::atoi(argv[1]);
    }
    boost::asio::io_context io_context;
    server server(io_context, port);
    io_context.run();
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception: " << ex.what() << "\n";
  }
  return 0;
}