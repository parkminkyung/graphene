
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


size_t shim_do_send_request(void)
{
	debug("Hello_world\n");
	return 0;
}
