#include "daemon/daemon.hpp"

#include <string>
#include <signal.h>

msm::daemon* daemon_ptr = nullptr; 
void signal_received(int)
{
	if (daemon_ptr)
	{
		daemon_ptr->notify_off_state();
	}

	exit(0);
}

int main(int argc, char *argv[])
{
	std::string path;

	if (argc > 1)
	{
		path = argv[1];
	}
	else 
	{
		path = "/etc/msm/config.yml";
	}

	struct sigaction sigaction_;
	memset(&sigaction_, 0, sizeof(sigaction_));
	sigaction_.sa_handler = signal_received;
	sigfillset(&sigaction_.sa_mask);
	sigaction(SIGINT, &sigaction_, nullptr);
	sigaction(SIGTERM, &sigaction_, nullptr);

	msm::daemon daemon(path);
	daemon_ptr = &daemon; 
	daemon.run(); 
}