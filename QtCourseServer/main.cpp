#pragma warning(disable: 4996)
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/fstream.hpp>
#define LOGIN 1
#define SIGNUP 2

using namespace boost::asio;
using namespace std;

void sendStockBrowser(int start) {
	io_service ios;
	ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), 11231));
	//ip::tcp::socket sock(ios);
	//acceptor.accept(sock);
	//cout << "Client: ";
	//cout << sock.remote_endpoint().address() << endl;

	boost::filesystem::path p("..\\data2000.csv");
	boost::filesystem::ifstream ifs(p);//���ļ��ж�ȡ����

	vector<string> tupleVec;//������д��vector�з���ʹ��
	string tuple;
	getline(ifs, tuple);
	for (int i = 0; i < start; i++) {
		getline(ifs, tuple); // �ƶ��ļ�ָ�뵽��start��
	}
	for (int i = 0; i < 100; i++) { // ��start��ʼ����100������
		getline(ifs, tuple);

		ip::tcp::iostream tcp_stream;
		acceptor.accept(*tcp_stream.rdbuf());
		tcp_stream << tuple;

		//sock.write_some(buffer(tuple));
		if (i % 50 == 0) {
			cout << "send: " << i << endl;
		}
	}
	//while () {
	//	//boost::algorithm::split(tupleVec, tuple, boost::algorithm::is_any_of(","));
	//	
	//	//for (vector<string>::iterator it = tupleVec.begin(); it != tupleVec.end(); it++) {
	//	//	sock.write_some(buffer(tuple));
	//	//}
	//}
}

void sendStockHistory() {
	io_service ios;
	ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), 11231));

	boost::filesystem::path p("..\\data3000.csv");
	boost::filesystem::ifstream ifs(p);//���ļ��ж�ȡ����

	vector<string> tupleVec;
	string tuple;
	getline(ifs, tuple);
	for (int i = 0; i < 200; i++) {
		getline(ifs, tuple);

		ip::tcp::iostream tcp_stream;
		acceptor.accept(*tcp_stream.rdbuf());
		tcp_stream << tuple;

		//sock.write_some(buffer(tuple));
		if (i % 100 == 0) {
			cout << "send: " << i << endl;
		}
	}
}

int main() {
	io_service ios;
	ip::tcp::endpoint ep(ip::tcp::v4(), 11230);
	ip::tcp::acceptor acceptor(ios, ep);
	cout << "server start!" << endl;

	while (true) {  //whileѭ�����ַ������˱��ּ���
		ip::tcp::socket sock(ios);
		acceptor.accept(sock);
		cout << "Client: " << sock.remote_endpoint().address() << endl;
		vector<char> buf(100);
		sock.read_some(buffer(buf));
		vector<string> cmdVec;
		boost::algorithm::split(cmdVec, buf, boost::algorithm::is_any_of("|"));//ʹ�÷ָ���õ�ָ���ע����Ϣ

		if (buf.size() <= 0) {
			cout << "cmdΪ��" << endl;
			continue;
		}
		string cmd = cmdVec[0];
		//cout << cmd << endl;h
		if (cmd == "s") {
			boost::filesystem::path p("..\\account");
			boost::filesystem::ifstream ifs(p);//��ע����Ϣд��account��
			boost::filesystem::ofstream ofs(p, ios::app);
			
			string acc;
			bool existAcc = false;
			while (getline(ifs, acc)) {
				vector<string> accVec;
				boost::algorithm::split(accVec, acc, boost::algorithm::is_any_of(" "));
				if (accVec[0] == cmdVec[1]) { // �û����Ѵ���
					sock.write_some(buffer("reject"));
					cout << "Signup failed!" << endl;
					existAcc = true;
					break;
				}
			}
			if (!existAcc) {
				sock.write_some(buffer("accept"));//ע����Ϣ������
				ofs << cmdVec[1] << " " << cmdVec[2] << endl;
				cout << "Signup success!" << endl;
			}
		}
		else if (cmd == "l") {
			boost::filesystem::path p("..\\account");
			boost::filesystem::ifstream ifs(p);

			string acc;
			bool loginAccept = false;
			while (getline(ifs, acc)) {
				vector<string> accVec;
				boost::algorithm::split(accVec, acc, boost::algorithm::is_any_of(" "));
				if (accVec[0] == cmdVec[1] && accVec[1] == cmdVec[2]) {
					sock.write_some(buffer("accept"));
					cout << "Login success!" << endl;
					loginAccept = true;
					break;
				}
			}
			if (!loginAccept) {
				sock.write_some(buffer("reject"));
				cout << "Login failed!" << endl;
			}
		}
		else if (cmd == "b") {
			int start = stoi(cmdVec[1]);
			cout << "Sending StockBrowse..." << endl;
			sendStockBrowser(start);//��½�ɹ���ӷ���˻�ȡ����
			cout << "StockBrowse send over." << endl;
		}
		else if (cmd == "h") {
			cout << "Sending StockHistory..." << endl;
			sendStockHistory();//��½�ɹ���ӷ���˻�ȡ����
			cout << "StockHistory send complete." << endl;
		}
	}
	//sendData();
	//getchar();
	return 0;
}