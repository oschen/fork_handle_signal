#include<stdio.h>
#include<assert.h>
#include<iostream>
#include<sys/time.h>
#include<sys/wait.h>


using namespace std;

void process_signal(int signo)
{
  fprintf(stderr,"\nsignal %d catched\n",signo);
  cout<<"Please wait for clear operation"<<endl;
  exit(0);

}

int set_signal_handler()
{
  assert(signal(SIGPIPE,SIG_IGN) != SIG_ERR);
  assert(signal(SIGINT,process_signal) != SIG_ERR);
  assert(signal(SIGTERM,process_signal) != SIG_ERR);
  assert(signal(SIGQUIT,process_signal) != SIG_ERR);
  assert(signal(SIGTRAP,process_signal) != SIG_ERR);
  assert(signal(SIGTSTP,process_signal) != SIG_ERR);
  return 0;
}

int check_signal_core()
{
  pid_t child_pid;
  char core_file[30]={0};
  char core_file_time[100]={0};
  int child_status=0;
  struct timeval tms;
  char timestr[30]={0};
  timerclear(&tms);
  cout<<"start the process"<<endl;

  child_pid=fork();
  if(child_pid>0)
  {
    cout<<"This parent process of child process "<<child_pid<<endl;
    wait(&child_status);
    if(WIFEXITED(child_status))
    {
      cout<<"child process exited ,exit code ="<<WEXITSTATUS(child_status)<<endl;
      if(WEXITSTATUS(child_status)!=0)
        cout<<"child process exec error"<<endl;
    }
    else if(WIFSIGNALED(child_status))
    {
      cout<<"child process killed by signal("<<WTERMSIG(child_status)<<")"<<endl;
      if(WTERMSIG(child_status)==9)
      {
        exit(0);
      }
    }
    else if(WCOREDUMP(child_status))
    {
      cout<<"child core file generated"<<endl;
    }
    else if(WIFSTOPPED(child_status))
    {
      cout<<"child process stopped(signal "<<WSTOPSIG(child_status)<<")"<<endl;
    }
    else if(WIFCONTINUED(child_status))
       cout<<"child process continued"<<endl;
    else
       cout<<"caught unexpected child_status ( "<<child_status<<")"<<endl;

     sprintf(core_file,"/cores/core.%d",child_pid);
     if(access(core_file,F_OK)==0)
     {
       gettimeofday(&tms,NULL);
       strftime(timestr,30,"%Y%m%d%H%M%S",localtime(&tms.tv_sec));
       sprintf(core_file_time,"%s.%s",core_file,timestr);
       cout<<core_file<<"  "<<core_file_time<<endl;
       assert(rename(core_file,core_file_time)==0);
     }
     cout<<"parent process exit"<<endl;
     exit(0);
}
  while(true)
  {
    sleep(1);
    cout<<"child process is running..."<<endl;
    char *p=NULL;
    strcpy(p,"abc");
  }
  return 0;
}

int main(int argc,char* argv[])
{
  set_signal_handler();
  check_signal_core();
}
