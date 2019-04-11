#include <errno.h>
#include <sysdep-cancel.h>
#include <sys/syscall.h>
#include <kernel-features.h>
#include <sysdep.h>
#include <shim_unistd.h>


#ifdef __NR_send_request
size_t send_request(int fd, const void * buf, size_t count)
{
	return INLINE_SYSCALL (send_request, 3, fd, buf, count);
}

#endif

#ifdef __NR_extend_request
size_t extend_request(int fd, const void * buf, size_t count)
{
	return INLINE_SYSCALL (extend_request, 3, fd, buf, count);
}

#endif
#ifdef __NR_send_response
size_t send_response(int fd, const void * buf, size_t count)
{
	return INLINE_SYSCALL (send_response, 3, fd, buf, count);
}

#endif
#ifdef __NR_send_user_data
size_t send_user_data(int fd, const void * buf, size_t count)
{
	return INLINE_SYSCALL (send_user_data, 3, fd, buf, count);
}

#endif
#ifdef __NR_read_nonuser_data
size_t read_nonuser_data(int fd, void * buf, size_t count)
{
	return INLINE_SYSCALL (read_nonuser_data, 3, fd, buf, count);
}

#endif

#ifdef __NR_gather_response
size_t gather_response(int fd, void * buf, size_t count)
{
	return INLINE_SYSCALL (gather_response, 3, fd, buf, count);
}

#endif

#ifdef __NR_read_user_data
size_t read_user_data(int fd, void * buf, size_t count)
{
	return INLINE_SYSCALL (read_user_data, 3, fd, buf, count);
}

#endif

