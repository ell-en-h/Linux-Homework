#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
using namespace std;

static inline void ltrim(string &s) {
    size_t p = s.find_first_not_of(" \t");
    s = (p == string::npos) ? "" : s.substr(p);
}
static inline void rtrim(string &s) {
    size_t p = s.find_last_not_of(" \t");
    s = (p == string::npos) ? "" : s.substr(0, p+1);
}
static inline void trim(string &s) { ltrim(s); rtrim(s); }

void split_commands_ops(const string &input, vector<string> &commands, vector<string> &ops) {
    commands.clear();
    ops.clear();
    size_t i = 0, n = input.size();
    string cur;
    while (i < n) {
        while (i < n && isspace((unsigned char)input[i])) ++i;
        if (i >= n) break;
        cur.clear();
        while (i < n) {
            if (input[i] == '&' && i+1 < n && input[i+1] == '&') break;
            if (input[i] == '|' && i+1 < n && input[i+1] == '|') break;
            if (input[i] == ';') break;
            cur.push_back(input[i]);
            ++i;
        }
        trim(cur);
        if (!cur.empty()) commands.push_back(cur);
        while (i < n && isspace((unsigned char)input[i])) ++i;
        if (i >= n) break;
        if (input[i] == '&' && i+1 < n && input[i+1] == '&') { ops.push_back("&&"); i += 2; }
        else if (input[i] == '|' && i+1 < n && input[i+1] == '|') { ops.push_back("||"); i += 2; }
        else if (input[i] == ';') { ops.push_back(";"); ++i; }
    }
}

bool parse_command(const string &cmd, vector<string> &args, string &outFile, bool &append, bool &silent) {
    args.clear();
    outFile.clear();
    append = false;
    silent = false;
    istringstream ss(cmd);
    string tok;
    while (ss >> tok) {
        if (tok == ">" || tok == ">>") {
            append = (tok == ">>");
            if (!(ss >> outFile)) {
                cerr << "Redirection without filename\n";
                return false;
            }
        }
        else if (tok == "silent") silent = true;
        else args.push_back(tok);
    }
    return true;
}

int run_single_command(const string &cmd, const string &shell_cwd) {
    vector<string> args;
    string outFile;
    bool append = false;
    bool silent = false;
    if (!parse_command(cmd, args, outFile, append, silent)) return 1;
    if (args.empty()) return 0;
    vector<char*> argv;
    vector<string> storage = args;
    for (size_t i = 0; i < storage.size(); ++i) argv.push_back(&storage[i][0]);
    argv.push_back(nullptr);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        const char *oldpath = getenv("PATH");
        string newpath;
        if (oldpath && strlen(oldpath) > 0) newpath = shell_cwd + string(":") + string(oldpath);
        else newpath = shell_cwd;
        setenv("PATH", newpath.c_str(), 1);
        if (silent) {
            string logname = to_string((long)getpid()) + ".log";
            int fd = open(logname.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) _exit(1);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        } else if (!outFile.empty()) {
            int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
            int fd = open(outFile.c_str(), flags, 0644);
            if (fd == -1) _exit(1);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(argv[0], argv.data());
        _exit(127);
    } else {
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) return 1;
        if (WIFEXITED(status)) return WEXITSTATUS(status);
        return 1;
    }
}

int main() {
    char cwd_buf[4096];
    if (!getcwd(cwd_buf, sizeof(cwd_buf))) return 1;

string shell_cwd = string(cwd_buf);
    while (true) {
        cout << "interactive-shell$ ";
        cout.flush();
        string line;
        if (!getline(cin, line)) { cout << "\n"; break; }
        trim(line);
        if (line.empty()) continue;
        if (line == "exit") break;
        vector<string> commands;
        vector<string> ops;
        split_commands_ops(line, commands, ops);
        int lastStatus = 0;
        for (size_t i = 0; i < commands.size(); ++i) {
            string cmd = commands[i];
            trim(cmd);
            if (cmd.empty()) continue;
            if (i == 0) lastStatus = run_single_command(cmd, shell_cwd);
            else {
                string op = ops[i-1];
                if (op == ";") lastStatus = run_single_command(cmd, shell_cwd);
                else if (op == "&&") { if (lastStatus == 0) lastStatus = run_single_command(cmd, shell_cwd); }
                else if (op == "||") { if (lastStatus != 0) lastStatus = run_single_command(cmd, shell_cwd); }
                else lastStatus = run_single_command(cmd, shell_cwd);
            }
        }
    }
    return 0;
}
