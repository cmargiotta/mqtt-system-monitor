#include <chrono>
#include <string>
#include <thread>

#include <signal.h>

#include "daemon/daemon.hpp"

using namespace std::chrono_literals;

msm::daemon *daemon_ptr = nullptr;
void         signal_received(int)
{
    if (daemon_ptr)
    {
        daemon_ptr->notify_state(false);
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

    while (true)
    {
        try
        {
            // Trying to start the daemon
            msm::daemon daemon(path);
            daemon_ptr = &daemon;
            daemon.run();
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << '\n';
            // An error occurred, retry after a second
            std::this_thread::sleep_for(1000ms);
        }
    }
}