
#include <shim_internal.h>
#include <shim_utils.h>
#include <shim_table.h>
#include <shim_thread.h>
#include <shim_handle.h>
#include <shim_fs.h>
#include <shim_profile.h>

#include <pal.h>
#include <pal_error.h>

#include <errno.h>
#include <dirent.h>

#include <linux/stat.h>
#include <linux/fcntl.h>


size_t shim_do_send_request(int fd, const void * buf, size_t count)
{
	debug("Hello_world\n");

	return 0;
}


size_t shim_do_extend_request(int fd, const void * buf, size_t count)
{
	debug("%s\n", __FUNCTION__);
	if (!buf || test_user_memory((void *) buf, count, false))
		return -EFAULT;

	struct shim_handle * hdl = get_fd_handle(fd, NULL, NULL);
	if (!hdl)
		return -EBADF;

	int ret = 0;
	if (!(hdl->acc_mode & MAY_WRITE))
		return -EACCES;

	struct shim_mount * fs = hdl->fs;
	assert (fs && fs->fs_ops);

	if (!fs->fs_ops->extend_request)
		return -EBADF;

	if (hdl->type == TYPE_DIR)
		return -EISDIR;

	ret = fs->fs_ops->extend_request(hdl, buf, count);

	put_handle(hdl);

	return ret;
}


size_t shim_do_send_response(int fd, const void * buf, size_t count)
{
debug("%s\n", __FUNCTION__);
	if (!buf || test_user_memory((void *) buf, count, false))
		return -EFAULT;

	struct shim_handle * hdl = get_fd_handle(fd, NULL, NULL);
	if (!hdl)
		return -EBADF;

	int ret = 0;
	if (!(hdl->acc_mode & MAY_WRITE))
		return -EACCES;

	struct shim_mount * fs = hdl->fs;
	assert (fs && fs->fs_ops);

	if (!fs->fs_ops->send_response)
		return -EBADF;

	if (hdl->type == TYPE_DIR)
		return -EISDIR;

	ret = fs->fs_ops->send_response(hdl, buf, count);

	put_handle(hdl);

	return ret;

}


size_t shim_do_send_user_data(int fd, const void * buf, size_t count)
{
debug("%s\n", __FUNCTION__);
	if (!buf || test_user_memory((void *) buf, count, false))
		return -EFAULT;

	struct shim_handle * hdl = get_fd_handle(fd, NULL, NULL);
	if (!hdl)
		return -EBADF;

	int ret = 0;
	if (!(hdl->acc_mode & MAY_WRITE))
		return -EACCES;

	struct shim_mount * fs = hdl->fs;
	assert (fs && fs->fs_ops);

	if (!fs->fs_ops->send_user_data)
		return -EBADF;

	if (hdl->type == TYPE_DIR)
		return -EISDIR;

	ret = fs->fs_ops->send_user_data(hdl, buf, count);

	put_handle(hdl);

	return ret;
}


size_t shim_do_read_nonuser_data(int fd, void * buf, size_t count)
{
	debug("%s\n", __FUNCTION__);
	if (!buf || test_user_memory(buf, count, true))
		return -EFAULT;

	struct shim_handle * hdl = get_fd_handle(fd, NULL, NULL);
	if (!hdl)
		return -EBADF;

	int ret = 0;
	if (!(hdl->acc_mode & MAY_READ))
		return -EACCES;

	struct shim_mount * fs = hdl->fs;
	assert (fs && fs->fs_ops);

	if (!fs->fs_ops->read_nonuser_data)
		return -EBADF;

	if (hdl->type == TYPE_DIR)
		return -EISDIR;

	ret = fs->fs_ops->read_nonuser_data(hdl, buf, count);

	put_handle(hdl);
	return ret;
}


size_t shim_do_gather_response(int fd, void * buf, size_t count)
{
	debug("%s\n", __FUNCTION__);
	if (!buf || test_user_memory(buf, count, true))
		return -EFAULT;

	struct shim_handle * hdl = get_fd_handle(fd, NULL, NULL);
	if (!hdl)
		return -EBADF;

	int ret = 0;
	if (!(hdl->acc_mode & MAY_READ))
		return -EACCES;

	struct shim_mount * fs = hdl->fs;
	assert (fs && fs->fs_ops);

	if (!fs->fs_ops->gather_response)
		return -EBADF;

	if (hdl->type == TYPE_DIR)
		return -EISDIR;

	ret = fs->fs_ops->gather_response(hdl, buf, count);

	put_handle(hdl);
	return ret;
}


size_t shim_do_read_user_data(int fd, void * buf, size_t count)
{
	debug("%s\n", __FUNCTION__);
	if (!buf || test_user_memory(buf, count, true))
		return -EFAULT;

	struct shim_handle * hdl = get_fd_handle(fd, NULL, NULL);
	if (!hdl)
		return -EBADF;

	int ret = 0;
	if (!(hdl->acc_mode & MAY_READ))
		return -EACCES;

	struct shim_mount * fs = hdl->fs;
	assert (fs && fs->fs_ops);

	if (!fs->fs_ops->read_user_data)
		return -EBADF;

	if (hdl->type == TYPE_DIR)
		return -EISDIR;

	ret = fs->fs_ops->read_user_data(hdl, buf, count);

	put_handle(hdl);
	return ret;
}


