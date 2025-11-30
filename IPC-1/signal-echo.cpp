#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <ucontext.h>

void handler(int sig, siginfo_t* info, void* ucontext) {
    ucontext_t* ctx = (ucontext_t*)ucontext;

    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;

    passwd* pw = getpwuid(sender_uid);
    const char* username = pw ? pw->pw_name : "unknown";

    unsigned long EIP = ctx->uc_mcontext.gregs[REG_RIP];
    unsigned long EAX = ctx->uc_mcontext.gregs[REG_RAX];
    unsigned long EBX = ctx->uc_mcontext.gregs[REG_RBX];

    std::cout << "Received SIGUSR1 from PID " << sender_pid
              << ", UID " << sender_uid
              << " (" << username << ")\n";

    std::cout << "Registers: EIP=" << std::hex << EIP
              << " EAX=" << EAX
              << " EBX=" << EBX << std::dec << "\n";
}

int main() {
    std::cout << "Process PID = " << getpid() << "\n";

    struct sigaction sa{};
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigaction(SIGUSR1, &sa, nullptr);

    while (true) {
        sleep(10);
    }
}

