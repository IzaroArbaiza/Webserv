#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::beast;

class WebSocketServer {
public:
    WebSocketServer(io_context& io_ctx, short port)
        : acceptor_(io_ctx, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

    void start_accept() {
        socket_ = std::make_shared<tcp::socket>(io_ctx);
        acceptor_.async_accept(*socket_,
            [this](boost::system::error_code ec) {
                if (!ec) {
                    std::cout << "Accepted connection\n";
                    start_handle();
                } else {
                    std::cout << "Accept failed: " << ec.message() << "\n";
                }
            });
    }

    void start_handle() {
        auto websocket = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket_));
        websocket->async_accept(
            [this, websocket](boost::system::error_code ec) {
                if (!ec) {
                    std::cout << "WebSocket accepted\n";
                    handle_message(websocket);
                } else {
                    std::cout << "WebSocket accept failed: " << ec.message() << "\n";
                }
            });
    }

    void handle_message(std::shared_ptr<websocket::stream<tcp::socket>> websocket) {
        websocket->async_read_some(
            [this, websocket](boost::asio::yield_context yield, boost::beast::error_code ec, boost::beast::buffer buffer) {
                if (!ec) {
                    std::cout << "Received message: " << boost::beast::buffers_to_string(buffer) << "\n";
                    // Procesar el mensaje y enviar respuesta
                    std::string response = "Hello, client!";
                    websocket->async_write_some(
                        boost::asio::buffer(response),
                        [this, websocket](boost::system::error_code ec) {
                            if (!ec) {
                                std::cout << "Sent response\n";
                            } else {
                                std::cout << "Send failed: " << ec.message() << "\n";
                            }
                        });
                } else {
                    std::cout << "Read failed: " << ec.message() << "\n";
                }
            });
    }

private:
    tcp::acceptor acceptor_;
    std::shared_ptr<tcp::socket> socket_;
};

int main() {
    io_context io_ctx;
    WebSocketServer server(io_ctx, 8080);
    io_ctx.run();
    return 0;
}