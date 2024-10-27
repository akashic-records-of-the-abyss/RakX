#include <RakX/server.hpp>
#include <print>

using namespace RakX;

int main() {
	
	Server srv{};
	const auto err = srv.init_server(
		ServerInitConfig{ IPv4::from_string("127.0.0.1").value(), 19132}
	);
	if (err.is_err()) {
		std::println("{}", err.as_str());
		return -1;
	}
	srv.start();
	using namespace std::chrono_literals;
	while (true) { std::this_thread::sleep_for(1s); }
}