#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
//
# include "ATcpServer.hpp"
// # include "ResponseGeneration.cpp"

int main()
{
	ATcpServer	server;
	server.pollEventLoop();
	// AutoIndex ai("/Users/samjaabo/Desktop/WEBSERVE", "/");
	// std::cout << ai.getOutput() << std::endl;
	return (0);
}

// int main( void )
// {
// 	t_config		config;
// 	CGI::build(config, 0);
// 	while(1)
// 	{
// 		usleep(1000000);
// 		CGI::checkTimeoutAndExitedProcesses(1000);
// 	}
// 	return 0;
// }

// #include <iostream>
// #include <ctime>
// #include <poll.h>

// int main() {
//     clock_t start, end;

//     start = clock();

//     // Example poll usage
//     struct pollfd fds[1];
//     fds[0].fd = 0 /* your file descriptor */;
//     fds[0].events = POLLIN;
//     // int result = poll(fds, 1, 3000);
// 	usleep(3000000);

//     end = clock();

//     uint64_t elapsedSeconds = static_cast<uint64_t>(end - start) / CLOCKS_PER_SEC;

//     std::cout << "poll elapsed time: " << elapsedSeconds << " seconds." << std::endl;

//     return 0;
// }