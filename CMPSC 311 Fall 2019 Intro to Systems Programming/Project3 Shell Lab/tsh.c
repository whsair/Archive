/* 
 * tsh - A tiny shell program with job control
 * 
 * Hongshuo Wang hkw5146
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
  pid_t pid;              /* job PID */
  int jid;                /* job ID [1, 2, ...] */
  int state;              /* UNDEF, BG, FG, or ST */
  char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
  char c;
  char cmdline[MAXLINE];
  int emit_prompt = 1; /* emit prompt (default) */

  /* Redirect stderr to stdout (so that driver will get all output
   * on the pipe connected to stdout) */
  dup2(1, 2);

  /* Parse the command line */
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
      case 'h':             /* print help message */
        usage();
        break;
      case 'v':             /* emit additional diagnostic info */
        verbose = 1;
        break;
      case 'p':             /* don't print a prompt */
        emit_prompt = 0;  /* handy for automatic testing */
        break;
      default:
        usage();
    }
  }

  /* Install the signal handlers */

  /* These are the ones you will need to implement */
  Signal(SIGINT,  sigint_handler);   /* ctrl-c */
  Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
  Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

  /* This one provides a clean way to kill the shell */
  Signal(SIGQUIT, sigquit_handler); 

  /* Initialize the job list */
  initjobs(jobs);

  /* Execute the shell's read/eval loop */
  while (1) {

    /* Read command line */
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
      app_error("fgets error");
    if (feof(stdin)) { /* End of file (ctrl-d) */
      fflush(stdout);
      exit(0);
    }

    /* Evaluate the command line */
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  } 

  exit(0); /* control never reaches here */
}

/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
 */
void eval(char *cmdline) 
{ 

// Argument list execve()  
  char *argv[MAXARGS];

// Holds modified command line/
  char buf[MAXLINE];

// determine whether the job run in background or fowardground (bg = 1 ; fg = 0) 
  int bg; 

// process id
  pid_t pid;
// create the copy or the buffle of command line the used typed in
  strcpy(buf, cmdline);

// create some mask in order to avoid the trace between addjob and deletejob
  sigset_t mask_one;

// defined function parseline which parses the space-separated command-line
// arguments and builds the argv vector that will eventually be passed to execve
// returns 1 if user request a background job 
// reutrn 0 => program run in the forground
  bg = parseline(buf, argv);
  
// ignore empty line
  if (argv[0] == NULL) {
    return;
  }

// helper function builtin_command
// whichs checks whether the first command-line argument is a built-in shell
// command. If so, it interprets the command immediately and return 1; Otherwise
// return 0


  if (!builtin_cmd(argv)) {


// When builtin_command returns 0, then the shell creates a child process and
// executes the requested program inside the child

// According the professor told in the class, we should set up our mask before
// we call the fork function. Otherwise, the child function will possibly terminated
// before it is add to the add to job list. This will cause a race between deletejob
// and addjob since the childhandler will delete the child job from the list whatever
// it terminated immediately.

// create a mask to block Child-signal and check whether it got a error
    if (sigemptyset(&mask_one) != 0) {
      unix_error("Sigempty Failed");
    }
    if (sigaddset(&mask_one, SIGCHLD) != 0) {
      unix_error("Sigaddset Failed");
    }
    
    if (sigprocmask(SIG_BLOCK, &mask_one, NULL)) {
        unix_error("Sigprocmask Failed");
    }

    if ((pid = fork()) == 0) {
      if (sigprocmask(SIG_UNBLOCK, &mask_one, NULL) != 0) {
        unix_error("Sigprocmask Failed in Child");
      }

// Create a new process group whose process group ID is its current PID
// set each child with unique group ID
// check whether the setpgid is set correctly
      if (setpgid(0,0) < 0) {
        unix_error("Setpgid failed (in eval)\n");
      }

// Check whether the command is predefined in our shell if not exit the process.
      if (execve(argv[0], argv, environ) < 0) {
        printf("%s: Command not found\n", argv[0]);
        exit(0);
      }
      
      if (!bg) {
        int status;
   
// Detect the wait function whether a error occurs;
        if (waitpid(pid, &status, 0) < 0) {
          unix_error("waitfg: waitpid error");   
        } 
      }
    
      
    } else {

// In the parent process we addjob first before unblock the ChildMask.
// in order to receive the SIGNCHILD 
      if (bg) {

// addjob on the job list and set its state as BG (background)
        addjob(jobs, pid, BG, cmdline);

// unblock the child mask
        if (sigprocmask(SIG_UNBLOCK, &mask_one, NULL)) {
          unix_error("Sigprocmask unblock Failed (BG)");
        }

// if the command have to run in background, we need also print out message to show its
// job id, process id and its cmdline.
        printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);

// Otherwise, this command needs to run in foreground, and we have to wait the process terminated
// before we start execute the next command
      } else {

// Add foreground job on the list
        addjob(jobs, pid, FG, cmdline);

// The child process can be terminated at this time. No race between addjob and deleted the job now.
        if (sigprocmask(SIG_UNBLOCK, &mask_one, NULL)) {
          unix_error("Sigprocmask unblock Failed (FG)");
        }

// wait current job
        waitfg(pid);
      }

  
    } 
  }
  return; 
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
  static char array[MAXLINE]; /* holds local copy of command line */
  char *buf = array;          /* ptr that traverses command line */
  char *delim;                /* points to first space delimiter */
  int argc;                   /* number of args */
  int bg;                     /* background job? */

  strcpy(buf, cmdline);
  buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
  while (*buf && (*buf == ' ')) /* ignore leading spaces */
    buf++;

  /* Build the argv list */
  argc = 0;
  if (*buf == '\'') {
    buf++;
    delim = strchr(buf, '\'');
  }
  else {
    delim = strchr(buf, ' ');
  }

  while (argc < MAXARGS-1 && delim) {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while (*buf && (*buf == ' ')) /* ignore spaces */
      buf++;

    if (*buf == '\'') {
      buf++;
      delim = strchr(buf, '\'');
    }
    else {
      delim = strchr(buf, ' ');
    }
  }
  if (delim) {
    fprintf(stderr, "Too many arguments.\n");
    argc = 0; //treat it as an empty line.
  }
  argv[argc] = NULL;

  if (argc == 0)  /* ignore blank line */
    return 1;

  /* should the job run in the background? */
  if ((bg = (*argv[argc-1] == '&')) != 0) {
    argv[--argc] = NULL;
  }
  return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) 
{ 
// quit command (quit shell)
  if (!strcmp(argv[0], "quit")) {
    exit(0);
  }

// jobs command (list all background jobs)
  if (!strcmp(argv[0], "jobs")) {

      listjobs(jobs);
      return 1;
  }
// background command  (bg <job>) 
// restarts <job > by sending it a SIGCONT signal, and then runs it in 
// the background. The <job> argrument can be either a PID or JID

  if (!strcmp(argv[0], "bg")) {
    do_bgfg(argv);
    return 1;
  }

// fg <job> command
// restarts <job> by sending if a SIGCONT signal, and then runs it in
// the foreground. The <job> argument can be either a PID or JID.
  else if (!strcmp(argv[0], "fg")) {
    do_bgfg(argv);
    return 1;
  }

// Ignore singleton &
  if (!strcmp(argv[0], "&")) {
    return 1;
  }
  return 0;   
}
/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{ 

// those are temporary holder to hold job and pid and jobid
  struct job_t *job = NULL;
  int jobid = 0;
  int pid = 0;


// Check whether match the syntax of fg and bg
// for example, fg %(number) or bg %(number)
// number denote a postive integer number
// Otherwise should return error (jobID format)
//
// fg (number) or bg (number)
// (pID format)

// check whether our command has null on second element
// if it is print out the error message
  if (argv[1] == NULL) {
    
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return; 
  } 

// check the jid format
  if (argv[1][0] == '%') {

// set job id to jid if we are doing the background 
    jobid = atoi(&argv[1][1]);

// if the job id is not in our job list return no such job
// else set the job to corresponding id
    if ((job = getjobjid(jobs, jobid)) == NULL ) {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  }

// check given pid syntax 
  else if (isdigit(argv[1][0])) {

// get the pid from the second element from out commend
    pid = atoi(argv[1]);

// Instead of check jobid we need to check the pid whether
// is a actual process
// pid2jid is a function map the pid to job id 
// it will return pid with corresponding job if pid is found
// in job list; otherwise it will return 0
    job = getjobjid(jobs, jobid); 
    if (pid2jid(pid) == 0) {
      printf("(%d): No such process\n", pid);
      return; 
    }
  } else {

// We have already go through all executed fg and bg format;
// Thus, the rest format must be invalid 
// return anyway 
      printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    return;
  }

// To this process, we have make sure that our command in valid format
// Next thing is to execute them in out program
// Send the restart siginal first to corresponding pid's job
// and check whether out SIGCONT is sent correctly
  
  if (kill(-(job->pid), SIGCONT)) {
    unix_error("Kill failed in dobgfg");
    return;
  }
    

// check whether it is background or foreground command
// and set up corresponding job's state
// We need to set up the job state by corresponding input syntax
  if (!strcmp(argv[0], "bg")) {
    job->state = BG;

// if it is a background job we need to print out its job id
// and process id and input commandline
    printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
  }
  else if (!strcmp(argv[0], "fg")) {
    job->state = FG;
// forground fg has to wait the currunt process 
    waitfg(job->pid);
  }
  

  return;

}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{

// fpid is helper function that return the PID of current foreground job
// if return 0 if the job doesn't exist.
// The while to help us track of the current job whether is in our joblist
// if it is in the joblist then let the shell sleep 2s
// else break the loop
  while (pid == fgpid(jobs)) {
    if (sleep(1) == -1) {
      unix_error("Sleep Failed in waitfg");
    } 
  } 
  return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
// status create a place holder for waitppid function to hold the information
  int status;
  pid_t pid;

// By the hind
// we use waitpid function to handle terminated or stopped child
// -1 => we deal with all children;
//
// WHOHANG is the option that return immediately(return 0) if none of the
// child processes in the wait set has terminated yet.it will help us to 
// do some useful work while waiting for a child to terminate
//
// WUNTRACED is the option that suspend exexcution of the calling process
// until a process in the wait set becomes either terminated or stopped.
// return the PID of the terminated or stopped child that cause the return.
// This option is useful when you want to check for both terminated and stopped
// children
//
// Since waitpid will return each child pid that cause waitpid return
// This while loop helps us to find all the zombine children then we can
// deal with them one by one by check the status

  while ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0) {
    
// Deal with the zombine children that received a terminated signal
    if (WIFSIGNALED(status)) {

// print out the information
      printf("Job [%d] (%d) terminated by signal %d\n", pid2jid(pid), pid, WTERMSIG(status));

// since it is a zombine child we need to removed it from the list
      deletejob(jobs, pid);
    }
// Deal with the children that received a STCSTOP or SIGTSTP signal
    else if (WIFSTOPPED(status)) {

// This is a trick. 
// In order to aviod set our current job's state to ST twice.
// I call the sigstp-handler when we received a stop signal 
// by passing the unique signal (pid + 32)
// Normally, the process is when we receive a stop signal, then we
// need to print out the message, set the job's state to be ST
// and sent the signal
// But when we received the signal by child, the child have already
// stopped. It's useless to sent the signal to it again.
// We just need to set this state to be ST.
// The unique sent signal is pid + 32 since our signal range is [0. 31]
// pid + 32 can make sure it cannot be treat as the normal stop signal
// whose signal value is 2. 
// call sigtstp_handler to get the job and set up job's state to Stop (ST)
// since the child is stopped (no matter it is running foreground or background)
// This is a efficient way to aviod set up job's state twice
      sigtstp_handler(pid + 32);

    }

// Deal with the child normally terminated (WIFEXiSTED(status))
    else if (WIFEXITED(status)) {
      deletejob(jobs, pid);
    }
  }
// get from the book if pid is not zero and there is no more child in shell
// return error

  if (errno != ECHILD) {
    if (pid != 0) {
      unix_error("waitpid error");
    }
  }

  return;
}



/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */

// deal with SIGINT when user type ctrl-c or call by the child-hander
void sigint_handler(int sig) 
{

// use fgpid function to get the current job's pid

  pid_t pid = fgpid(jobs);

// we only focusing on the groundjob
  if (pid != 0) {
    
// send SIGINT signal to interrupt the current job
    if (kill(-pid, SIGINT) != 0) {
      unix_error("SIGINT Handler Faild (Kill)");
    }
  }

  return;

}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */

// just like what said before this time we need to handle the ctrl-z
// we need to stop current job after we type ctrl-z or handle the stopped
// child.
void sigtstp_handler(int sig) 
{

// help function fgpid() return the current foreground job if it exists
// in the joblist else return 0
  pid_t pid = fgpid(jobs);
  struct job_t *job;
  job = getjobpid(jobs, pid);
  if (pid != 0) {

// send SIGSTOP or SIGTSTP to
// stop current job if user typed ctrl-z

// if the current job's state is ST 
// That means it haved been handled
// just return doesn't need to do anything
    if ((job->state) == ST) {
      return;
    }
// check if it is the normal case when we received the stop signal
// first print out the message, then set current job's state to be ST
// Last sent the signal to make the current stop
    if ((sig == SIGSTOP) || (sig == SIGTSTP)) {
      printf("Job [%d] (%d) stopped by signal %d\n", pid2jid(pid), pid, sig);
      job->state = ST;
// we need to send SIGTSTP signal to current job if user typed crtl-z
      if (kill(-pid, SIGTSTP) != 0) {
        unix_error("SIGTSTP Handler Failed (Kill)");
      }

    }
// Trick part:
// Sent by our child-handler. just print out the message and set up the state
// No need to send SIGTSTP 
    else if ((pid + 32) == sig) {
      printf("Job [%d] (%d) stopped by signal %d\n", pid2jid(pid), pid, SIGTSTP);
      job->state = ST;
    }
    
  }

  return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
  job->pid = 0;
  job->jid = 0;
  job->state = UNDEF;
  job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++)
    clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
  int i, max=0;

  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid > max)
      max = jobs[i].jid;
  return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
  int i;

  if (pid < 1)
    return 0;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == 0) {
      jobs[i].pid = pid;
      jobs[i].state = state;
      jobs[i].jid = nextjid++;
      if (nextjid > MAXJOBS)
        nextjid = 1;
      strcpy(jobs[i].cmdline, cmdline);
      if(verbose){
        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
      }
      return 1;
    }
  }
  printf("Tried to create too many jobs\n");
  return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
  int i;

  if (pid < 1)
    return 0;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == pid) {
      clearjob(&jobs[i]);
      nextjid = maxjid(jobs)+1;
      return 1;
    }
  }
  return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].state == FG)
      return jobs[i].pid;
  return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
  int i;

  if (pid < 1)
    return NULL;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].pid == pid)
      return &jobs[i];
  return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
  int i;

  if (jid < 1)
    return NULL;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid == jid)
      return &jobs[i];
  return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
  int i;

  if (pid < 1)
    return 0;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].pid == pid) {
      return jobs[i].jid;
    }
  return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
  int i;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid != 0) {
      printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
      switch (jobs[i].state) {
        case BG: 
          printf("Running ");
          break;
        case FG: 
          printf("Foreground ");
          break;
        case ST: 
          printf("Stopped ");
          break;
        default:
          printf("listjobs: Internal error: job[%d].state=%d ", 
              i, jobs[i].state);
      }
      printf("%s", jobs[i].cmdline);
    }
  }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
  printf("Usage: shell [-hvp]\n");
  printf("   -h   print this message\n");
  printf("   -v   print additional diagnostic information\n");
  printf("   -p   do not emit a command prompt\n");
  exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
  fprintf(stdout, "%s: %s\n", msg, strerror(errno));
  exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
  fprintf(stdout, "%s\n", msg);
  exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
  struct sigaction action, old_action;

  action.sa_handler = handler;  
  sigemptyset(&action.sa_mask); /* block sigs of type being handled */
  action.sa_flags = SA_RESTART; /* restart syscalls if possible */

  if (sigaction(signum, &action, &old_action) < 0)
    unix_error("Signal error");
  return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
  printf("Terminating after receipt of SIGQUIT signal\n");
  exit(1);
}



