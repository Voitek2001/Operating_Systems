

const int SIGNAL_ID = SIGUSR1;

void action_handler(int sig_no, siginfo_t* info, void* context);
void create_sigaction(int sig_no, int flags, void(* action)(int, siginfo_t*, void*));
