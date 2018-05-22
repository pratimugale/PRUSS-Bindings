%module pruss
%{
extern void modprobe();
extern void modunprobe();  
extern void pru_enable(int);
extern void pru_disable(int);
extern void exec_program(char*, int);
extern void send_msg(char*, char*);
extern void get_msg(char*, char*);
%}

extern void modprobe();
extern void modunprobe();
extern void pru_enable(int);
extern void pru_disable(int);
extern void exec_program(char*, int);
extern void send_msg(char*, char*);
extern void get_msg(char*, char*);


