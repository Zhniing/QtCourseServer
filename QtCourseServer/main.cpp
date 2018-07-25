//#pragam warning(disable: 4996)
#include <iostream>
#include <vector>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace std;

int main() {
	io_service ios;
	ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), 11230));
	cout << acceptor.local_endpoint().address() << endl;
	while (true) {
		ip::tcp::socket sock(ios);
		acceptor.accept(sock);

		cout << "Client: ";
		cout << sock.remote_endpoint().address() << endl;

		sock.write_some(buffer("hello asio!"));
	}
	return 0;
}