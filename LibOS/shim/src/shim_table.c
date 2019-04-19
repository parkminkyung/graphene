/* -*- mode:c; c-file-style:"k&r"; c-basic-offset: 4; tab-width:4; indent-tabs-mode:nil; mode:auto-fill; fill-column:78; -*- */
/* vim: set ts=4 sw=4 et tw=78 fo=cqt wm=0: */

/* Copyright (C) 2014 Stony Brook University
   This file is part of Graphene Library OS.

   Graphene Library OS is free software: you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   Graphene Library OS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/*
 * shim_table.c
 *
 * This file contains the system call table used by application libraries.
 */

#include <shim_table.h>
#include <shim_internal.h>

void debug_unsupp (int num){
    debug ("Unsupported system call %d\n", num);
}

shim_fp shim_table [LIBOS_SYSCALL_BOUND] = {
    (shim_fp) __shim_read,
    (shim_fp) __shim_write,
    (shim_fp) __shim_open,
    (shim_fp) __shim_close,
    (shim_fp) __shim_stat,  // three: file information
    (shim_fp) __shim_fstat, 
    (shim_fp) __shim_lstat,
    (shim_fp) __shim_poll, // waits for one of a set of file descriptors to become ready to perform I/O
    (shim_fp) __shim_lseek, // repositions the file offset of the open file
    (shim_fp) __shim_mmap, // creates a new mapping in the virtual address space of the calling process
    (shim_fp) __shim_mprotect, // changes the access protections for the calling process's memory pages containing any part of the address range
    (shim_fp) __shim_munmap, // mmap 해제
    (shim_fp) __shim_brk, // change the location of the program break 
    (shim_fp) __shim_rt_sigaction, // signal handling 
    (shim_fp) __shim_rt_sigprocmask,  
    (shim_fp) __shim_rt_sigreturn,
    (shim_fp) __shim_ioctl, // only FIONREAD supported
    (shim_fp) __shim_pread64, // file
    (shim_fp) __shim_pwrite64, // file
    (shim_fp) __shim_readv, // file
    (shim_fp) __shim_writev, // file
    (shim_fp) __shim_access, // file
    (shim_fp) __shim_pipe, // pipe creation 
    (shim_fp) __shim_select, // file
    (shim_fp) __shim_sched_yield, // scheduler yielding
    (shim_fp) __shim_mremap, // XXX from here 
    (shim_fp) __shim_msync, 
    (shim_fp) __shim_mincore, 
    (shim_fp) __shim_madvise,
    (shim_fp) __shim_shmget,
    (shim_fp) __shim_shmat,
    (shim_fp) __shim_shmctl, // XXX to here
    (shim_fp) __shim_dup, // duplicate file descriptor
    (shim_fp) __shim_dup2, 
    (shim_fp) __shim_pause,
    (shim_fp) __shim_nanosleep,
    (shim_fp) __shim_getitimer,
    (shim_fp) __shim_alarm,
    (shim_fp) __shim_setitimer,
    (shim_fp) __shim_getpid,
    (shim_fp) __shim_sendfile,
    (shim_fp) __shim_socket,
    (shim_fp) __shim_connect,
    (shim_fp) __shim_accept,
    (shim_fp) __shim_sendto,
    (shim_fp) __shim_recvfrom,
    (shim_fp) __shim_sendmsg,
    (shim_fp) __shim_recvmsg,
    (shim_fp) __shim_shutdown,
    (shim_fp) __shim_bind,
    (shim_fp) __shim_listen,
    (shim_fp) __shim_getsockname,
    (shim_fp) __shim_getpeername,
    (shim_fp) __shim_socketpair,
    (shim_fp) __shim_setsockopt,
    (shim_fp) __shim_getsockopt,
    (shim_fp) __shim_clone,
    (shim_fp) __shim_fork,
    (shim_fp) __shim_vfork,
    (shim_fp) __shim_execve,
    (shim_fp) __shim_exit,
    (shim_fp) __shim_wait4,
    (shim_fp) __shim_kill,
    (shim_fp) __shim_uname,
    (shim_fp) __shim_semget,
    (shim_fp) __shim_semop,
    (shim_fp) __shim_semctl,
    (shim_fp) __shim_shmdt, // XXX 
    (shim_fp) __shim_msgget,
    (shim_fp) __shim_msgsnd,
    (shim_fp) __shim_msgrcv,
    (shim_fp) __shim_msgctl,
    (shim_fp) __shim_fcntl,
    (shim_fp) __shim_flock, // XXX
    (shim_fp) __shim_fsync,
    (shim_fp) __shim_fdatasync,
    (shim_fp) __shim_truncate,
    (shim_fp) __shim_ftruncate,
    (shim_fp) __shim_getdents,
    (shim_fp) __shim_getcwd,
    (shim_fp) __shim_chdir,
    (shim_fp) __shim_fchdir,
    (shim_fp) __shim_rename,
    (shim_fp) __shim_mkdir,
    (shim_fp) __shim_rmdir,
    (shim_fp) __shim_creat,
    (shim_fp) __shim_link, // XXX
    (shim_fp) __shim_unlink, 
    (shim_fp) __shim_symlink, // XXX
    (shim_fp) __shim_readlink,
    (shim_fp) __shim_chmod,
    (shim_fp) __shim_fchmod,
    (shim_fp) __shim_chown,
    (shim_fp) __shim_fchown,
    (shim_fp) __shim_lchown, // XXX
    (shim_fp) __shim_umask,
    (shim_fp) __shim_gettimeofday,
    (shim_fp) __shim_getrlimit,
    (shim_fp) __shim_getrusage,
    (shim_fp) __shim_sysinfo, // XXX
    (shim_fp) __shim_times, // XXX
    (shim_fp) __shim_ptrace, // XXX
    (shim_fp) __shim_getuid,
    (shim_fp) __shim_syslog, // XXX
    (shim_fp) __shim_getgid,
    (shim_fp) __shim_setuid,
    (shim_fp) __shim_setgid,
    (shim_fp) __shim_geteuid,
    (shim_fp) __shim_getegid,
    (shim_fp) __shim_setpgid,
    (shim_fp) __shim_getppid,
    (shim_fp) __shim_getpgrp,
    (shim_fp) __shim_setsid,
    (shim_fp) __shim_setreuid, // XXX from here
    (shim_fp) __shim_setregid,
    (shim_fp) __shim_getgroups,
    (shim_fp) __shim_setgroups,
    (shim_fp) __shim_setresuid,
    (shim_fp) __shim_getresuid,
    (shim_fp) __shim_setresgid, 
    (shim_fp) __shim_getresgid, // XXX to here
    (shim_fp) __shim_getpgid,
    (shim_fp) __shim_setfsuid, // XXX
    (shim_fp) __shim_setfsgid, // XXX
    (shim_fp) __shim_getsid,
    (shim_fp) __shim_capget, // XXX 
    (shim_fp) __shim_capset, // XXX 
    (shim_fp) __shim_rt_sigpending,  
    (shim_fp) __shim_rt_sigtimedwait, // XXX
    (shim_fp) __shim_rt_sigqueueinfo, // XXX
    (shim_fp) __shim_rt_sigsuspend,
    (shim_fp) __shim_sigaltstack,
    (shim_fp) __shim_utime, // XXX from 
    (shim_fp) __shim_mknod,
    (shim_fp) __shim_uselib,
    (shim_fp) __shim_personality,
    (shim_fp) __shim_ustat,
    (shim_fp) __shim_statfs,
    (shim_fp) __shim_fstatfs,
    (shim_fp) __shim_sysfs,
    (shim_fp) __shim_getpriority,
    (shim_fp) __shim_setpriority,
    (shim_fp) __shim_sched_setparam,
    (shim_fp) __shim_sched_getparam,
    (shim_fp) __shim_sched_setscheduler,
    (shim_fp) __shim_sched_getscheduler,
    (shim_fp) __shim_sched_get_priority_max,
    (shim_fp) __shim_sched_get_priority_min,
    (shim_fp) __shim_sched_rr_get_interval,
    (shim_fp) __shim_mlock,
    (shim_fp) __shim_munlock,
    (shim_fp) __shim_mlockall,
    (shim_fp) __shim_munlockall,
    (shim_fp) __shim_vhangup,
    (shim_fp) __shim_modify_ldt,
    (shim_fp) __shim_pivot_root,
    (shim_fp) __shim__sysctl,
    (shim_fp) __shim_prctl, // XXX to
    (shim_fp) __shim_arch_prctl,
    (shim_fp) __shim_adjtimex, // XXX
    (shim_fp) __shim_setrlimit,
    (shim_fp) __shim_chroot,
    (shim_fp) __shim_sync, // XXX from
    (shim_fp) __shim_acct,
    (shim_fp) __shim_settimeofday, 
    (shim_fp) __shim_mount,
    (shim_fp) __shim_umount2,
    (shim_fp) __shim_swapon,
    (shim_fp) __shim_swapoff,
    (shim_fp) __shim_reboot,
    (shim_fp) __shim_sethostname,
    (shim_fp) __shim_setdomainname,
    (shim_fp) __shim_iopl,
    (shim_fp) __shim_ioperm,
    (shim_fp) __shim_create_module,
    (shim_fp) __shim_init_module,
    (shim_fp) __shim_delete_module,
    (shim_fp) 0, // shim_get_kernel_syms,
    (shim_fp) __shim_query_module,
    (shim_fp) __shim_quotactl,
    (shim_fp) 0, // shim_nfsservctl,
    (shim_fp) 0, // shim_getpmsg,
    (shim_fp) 0, // shim_putpmsg,
    (shim_fp) 0, // shim_afs_syscall,
    (shim_fp) 0, // shim_tuxcall,
    (shim_fp) 0, // shim_security, // to 
    (shim_fp) __shim_gettid,
    (shim_fp) __shim_readahead, // from
    (shim_fp) __shim_setxattr,
    (shim_fp) __shim_lsetxattr,
    (shim_fp) __shim_fsetxattr,
    (shim_fp) __shim_getxattr,
    (shim_fp) __shim_lgetxattr,
    (shim_fp) __shim_fgetxattr,
    (shim_fp) __shim_listxattr,
    (shim_fp) __shim_llistxattr,
    (shim_fp) __shim_flistxattr,
    (shim_fp) __shim_removexattr,
    (shim_fp) __shim_lremovexattr,
    (shim_fp) __shim_fremovexattr,
    (shim_fp) __shim_tkill,
    (shim_fp) __shim_time,
    (shim_fp) __shim_futex,
    (shim_fp) __shim_sched_setaffinity,
    (shim_fp) __shim_sched_getaffinity,
    (shim_fp) __shim_set_thread_area,
    (shim_fp) __shim_io_setup,
    (shim_fp) __shim_io_destroy,
    (shim_fp) __shim_io_getevents,
    (shim_fp) __shim_io_submit,
    (shim_fp) __shim_io_cancel,
    (shim_fp) __shim_get_thread_area,
    (shim_fp) __shim_lookup_dcookie,
    (shim_fp) __shim_epoll_create,
    (shim_fp) 0, // shim_epoll_ctl_old,
    (shim_fp) 0, // shim_epoll_wait_old,
    (shim_fp) __shim_remap_file_pages,
    (shim_fp) __shim_getdents64,
    (shim_fp) __shim_set_tid_address,
    (shim_fp) __shim_restart_syscall,
    (shim_fp) __shim_semtimedop,
    (shim_fp) __shim_fadvise64,
    (shim_fp) __shim_timer_create,
    (shim_fp) __shim_timer_settime,
    (shim_fp) __shim_timer_gettime,
    (shim_fp) __shim_timer_getoverrun,
    (shim_fp) __shim_timer_delete,
    (shim_fp) __shim_clock_settime,
    (shim_fp) __shim_clock_gettime,
    (shim_fp) __shim_clock_getres,
    (shim_fp) __shim_clock_nanosleep,
    (shim_fp) __shim_exit_group,
    (shim_fp) __shim_epoll_wait,
    (shim_fp) __shim_epoll_ctl,
    (shim_fp) __shim_tgkill,
    (shim_fp) __shim_utimes,
    (shim_fp) 0, // shim_vserver,
    (shim_fp) __shim_mbind,
    (shim_fp) __shim_set_mempolicy,
    (shim_fp) __shim_get_mempolicy,
    (shim_fp) __shim_mq_open,
    (shim_fp) __shim_mq_unlink,
    (shim_fp) __shim_mq_timedsend,
    (shim_fp) __shim_mq_timedreceive,
    (shim_fp) __shim_mq_notify,
    (shim_fp) __shim_mq_getsetattr,
    (shim_fp) 0, // shim_kexec_load,
    (shim_fp) __shim_waitid,
    (shim_fp) 0, // shim_add_key,
    (shim_fp) 0, // shim_request_key,
    (shim_fp) 0, // shim_keyctl,
    (shim_fp) __shim_ioprio_set,
    (shim_fp) __shim_ioprio_get,
    (shim_fp) __shim_inotify_init,
    (shim_fp) __shim_inotify_add_watch,
    (shim_fp) __shim_inotify_rm_watch,
    (shim_fp) __shim_migrate_pages,
    (shim_fp) __shim_openat,
    (shim_fp) __shim_mkdirat,
    (shim_fp) __shim_mknodat,
    (shim_fp) __shim_fchownat,
    (shim_fp) __shim_futimesat,
    (shim_fp) __shim_newfstatat,
    (shim_fp) __shim_unlinkat,
    (shim_fp) __shim_renameat,
    (shim_fp) __shim_linkat,
    (shim_fp) __shim_symlinkat,
    (shim_fp) __shim_readlinkat,
    (shim_fp) __shim_fchmodat,
    (shim_fp) __shim_faccessat,
    (shim_fp) __shim_pselect6,
    (shim_fp) __shim_ppoll,
    (shim_fp) __shim_unshare,
    (shim_fp) __shim_set_robust_list,
    (shim_fp) __shim_get_robust_list,
    (shim_fp) __shim_splice,
    (shim_fp) __shim_tee,
    (shim_fp) __shim_sync_file_range,
    (shim_fp) __shim_vmsplice,
    (shim_fp) __shim_move_pages,
    (shim_fp) __shim_utimensat,
    (shim_fp) __shim_epoll_pwait,
    (shim_fp) __shim_signalfd,
    (shim_fp) __shim_timerfd_create,
    (shim_fp) __shim_eventfd,
    (shim_fp) __shim_fallocate,
    (shim_fp) __shim_timerfd_settime,
    (shim_fp) __shim_timerfd_gettime,
    (shim_fp) __shim_accept4,
    (shim_fp) __shim_signalfd4,
    (shim_fp) __shim_eventfd2,
    (shim_fp) __shim_epoll_create1,
    (shim_fp) __shim_dup3,
    (shim_fp) __shim_pipe2,
    (shim_fp) __shim_inotify_init1,
    (shim_fp) __shim_preadv,
    (shim_fp) __shim_pwritev,
    (shim_fp) __shim_rt_tgsigqueueinfo,
    (shim_fp) __shim_perf_event_open,
    (shim_fp) __shim_recvmmsg,
    (shim_fp) __shim_fanotify_init,
    (shim_fp) __shim_fanotify_mark,
    (shim_fp) __shim_prlimit64,
    (shim_fp) __shim_name_to_handle_at,
    (shim_fp) __shim_open_by_handle_at,
    (shim_fp) __shim_clock_adjtime,
    (shim_fp) __shim_syncfs,
    (shim_fp) __shim_sendmmsg,
    (shim_fp) __shim_setns,
    (shim_fp) __shim_getcpu,

    [LIBOS_SYSCALL_BASE] = (shim_fp) NULL,

    (shim_fp) __shim_sandbox_create,    /* 303 */
    (shim_fp) __shim_sandbox_attach,    /* 304 */
    (shim_fp) __shim_sandbox_current,   /* 305 */
    (shim_fp) __shim_msgpersist,        /* 306 */
    (shim_fp) __shim_benchmark_rpc,     /* 307 */
    (shim_fp) __shim_send_rpc,          /* 308 */
    (shim_fp) __shim_recv_rpc,          /* 309 */
    (shim_fp) __shim_checkpoint,        /* 310 */
    
// mkpark
    (shim_fp) __shim_send_request,
// mkpark
    (shim_fp) __shim_extend_request,
    (shim_fp) __shim_send_response,
    (shim_fp) __shim_send_user_data,
    (shim_fp) __shim_read_nonuser_data,
    (shim_fp) __shim_gather_response,
    (shim_fp) __shim_read_user_data,
};
