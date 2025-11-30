#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <ucontext.h>
#include <cstdio>

void handler(int sig, siginfo_t* info, void* ucontext) {
    ucontext_t* ctx = (ucontext_t*)ucontext;

    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;

    passwd* pw = getpwuid(sender_uid);
    const char* username = pw ? pw->pw_name : "unknown";

    unsigned long EIP = ctx->uc_mcontext.gregs[REG_RIP];
    unsigned long EAX = ctx->uc_mcontext.gregs[REG_RAX];
    unsigned long EBX = ctx->uc_mcontext.gregs[REG_RBX];

    char buf[256];
    int n = snprintf(buf, sizeof(buf),
        "Received SIGUSR1 from PID %d, UID %d (%s)\nRegisters: EIP=0x%lx EAX=0x%lx EBX=0x%lx\n",
        sender_pid, sender_uid, username, EIP, EAX, EBX);

    write(STDOUT_FILENO, buf, n);
}

int main() {
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "Process PID = %d\n", getpid());
    write(STDOUT_FILENO, buf, n);

    struct sigaction sa{};
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    
    if(sigaction(SIGUSR1, &sa, nullptr) == -1) {
        const char* err = "sigaction failed\n";
        write(STDOUT_FILENO, err, 17);
        return 1;
    }
    while (true) {
        sleep(10);
    }
}

