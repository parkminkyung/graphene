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
 * fs.c
 *
 * This file contains codes for implementation of 'socket' filesystem.
 */

#include <shim_internal.h>
#include <shim_ipc.h>
#include <shim_fs.h>
#include <shim_profile.h>

#include <pal.h>
#include <pal_error.h>

#include <errno.h>

#include <linux/stat.h>
#include <linux/fcntl.h>

#include <asm/mman.h>
#include <asm/unistd.h>
#include <asm/prctl.h>
#include <asm/fcntl.h>

#include "../../wolfssl/ssl.h"

static int socket_close (struct shim_handle * hdl)
{
    return 0;
}

static int socket_read_handshake (struct shim_handle * hdl, void * buf,
                        size_t count)
{
    int bytes = 0;
    struct shim_sock_handle * sock = &hdl->info.sock;

    if (!count)
        return 0;

    // mkpark
//    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_ACCEPTED &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = ENOTCONN;
  //      unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = EDESTADDRREQ;
    //    unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

 //   unlock(hdl->lock);

    bytes = DkStreamRead(hdl->pal_handle, 0, count, buf, NULL, 0); // wolfSSL_read_sock(hdl, buf, count);

    if (!bytes)
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_ENDOFSTREAM:
                return 0;
            default: {
                int err = PAL_ERRNO;
                lock(hdl->lock);
                sock->error = err;
                unlock(hdl->lock);
                return -err;
            }
        }

    return bytes;
}

static int socket_write_handshake (struct shim_handle * hdl, const void * buf,
                         size_t count)
{
    debug("socket_write_handshake\n");
    struct shim_sock_handle * sock = &hdl->info.sock;

    // mkpark
//    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_ACCEPTED &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = ENOTCONN;
  //      unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = EDESTADDRREQ;
    //    unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

 //   unlock(hdl->lock);

    if (!count)
        return 0;

    int bytes = DkStreamWrite(hdl->pal_handle, 0, count, (void *) buf, NULL);

    if (!bytes) {
        int err;
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_CONNFAILED:
                err = EPIPE;
                break;
            default:
                err = PAL_ERRNO;
                break;
        }
        lock(hdl->lock);
        sock->error = err;
        unlock(hdl->lock);
        return -err;
    }

    return bytes;
}


static int socket_read (struct shim_handle * hdl, void * buf,
                        size_t count)
{
    int bytes = 0;
    struct shim_sock_handle * sock = &hdl->info.sock;

    if (!count)
        return 0;

    enum process_state proc_state = cur_process.state; 
    if (proc_state == CONFINED){
    	debug("%s:%d: confined.. should not be called \n", __FUNCTION__, __LINE__);
;//        return -ECANCELED;
    }


    // mkpark
    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_ACCEPTED &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = ENOTCONN;
        unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = EDESTADDRREQ;
        unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

    unlock(hdl->lock);

    bytes = DkStreamRead(hdl->pal_handle, 0, count, buf, NULL, 0);
//    bytes = wolfSSL_read_sock(hdl, buf, count);

    if (!bytes)
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_ENDOFSTREAM:
                return 0;
            default: {
                int err = PAL_ERRNO;
                lock(hdl->lock);
                sock->error = err;
                unlock(hdl->lock);
                return -err;
            }
        }

    return bytes;
}

static int socket_write (struct shim_handle * hdl, const void * buf,
                         size_t count)
{
    debug("socket_write entered!\n");
    struct shim_sock_handle * sock = &hdl->info.sock;

    enum process_state proc_state = cur_process.state; 
    if (proc_state == CONFINED){
    	debug("%s:%d: confined.. should not be called \n", __FUNCTION__, __LINE__);
;//        return -ECANCELED;
    }


    // mkpark
    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_ACCEPTED &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = ENOTCONN;
        unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = EDESTADDRREQ;
        unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

    unlock(hdl->lock);

    if (!count)
        return 0;

    int bytes = DkStreamWrite(hdl->pal_handle, 0, count, (void *) buf, NULL);
    //int bytes = wolfSSL_write_sock(hdl, (void *)buf, count); //DkStreamWrite(hdl->pal_handle, 0, count, (void *) buf, NULL);

    if (!bytes) {
        int err;
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_CONNFAILED:
                err = EPIPE;
                break;
            default:
                err = PAL_ERRNO;
                break;
        }
        lock(hdl->lock);
        sock->error = err;
        unlock(hdl->lock);
        return -err;
    }

    return bytes;
}

static int socket_hstat (struct shim_handle * hdl, struct stat * stat)
{
    if (!stat)
        return 0;

    enum process_state proc_state = cur_process.state; 
    if (proc_state == CONFINED){
    	debug("%s:%d: confined.. should not be called \n", __FUNCTION__, __LINE__);
;//        return -ECANCELED;
    }


    PAL_STREAM_ATTR attr;

    if (!DkStreamAttributesQuerybyHandle(hdl->pal_handle, &attr))
        return -PAL_ERRNO;

    memset(stat, 0, sizeof(struct stat));

    stat->st_ino    = 0;
    stat->st_size   = (off_t) attr.pending_size;
    stat->st_mode   = S_IFSOCK;

    return 0;
}

static int socket_checkout (struct shim_handle * hdl)
{
    hdl->fs = NULL;
    return 0;
}

static int socket_poll (struct shim_handle * hdl, int poll_type)
{
    struct shim_sock_handle * sock = &hdl->info.sock;
    int ret = 0;

    enum process_state proc_state = cur_process.state; 
    if (proc_state == CONFINED){
    	debug("%s:%d: confined.. should not be called \n", __FUNCTION__, __LINE__);
;//        return -ECANCELED;
    }


    lock(hdl->lock);

    if (poll_type & FS_POLL_RD) {
        if (sock->sock_type == SOCK_STREAM) {
            if (sock->sock_state == SOCK_CREATED ||
                sock->sock_state == SOCK_BOUND ||
                sock->sock_state == SOCK_SHUTDOWN) {
                ret = -ENOTCONN;
                goto out;
            }
        }

        if (sock->sock_type == SOCK_DGRAM &&
            sock->sock_state == SOCK_SHUTDOWN) {
            ret = -ENOTCONN;
            goto out;
        }
    }

    if (poll_type & FS_POLL_WR) {
        if (sock->sock_type == SOCK_STREAM) {
            if (sock->sock_state == SOCK_CREATED ||
                sock->sock_state == SOCK_BOUND ||
                sock->sock_state == SOCK_LISTENED ||
                sock->sock_state == SOCK_SHUTDOWN) {
                ret = -ENOTCONN;
                goto out;
            }
        }

        if (sock->sock_type == SOCK_DGRAM &&
            sock->sock_state == SOCK_SHUTDOWN) {
            ret = -ENOTCONN;
            goto out;
        }

    }

    if (!hdl->pal_handle) {
        ret = -EBADF;
        goto out;
    }

    PAL_STREAM_ATTR attr;
    if (!DkStreamAttributesQuerybyHandle(hdl->pal_handle, &attr)) {
        ret = -PAL_ERRNO;
        goto out;
    }

    if (poll_type == FS_POLL_SZ) {
        ret = attr.pending_size;
        goto out;
    }

    ret = 0;
    if (attr.disconnected) 
        ret |= FS_POLL_ER;
    if ((poll_type & FS_POLL_RD) && attr.readable)
        ret |= FS_POLL_RD;
    if ((poll_type & FS_POLL_WR) && attr.writeable)
        ret |= FS_POLL_WR;

out:
    if (ret < 0) {
        debug("socket_poll failed (%d)\n", ret);
        sock->error = -ret;
    }

    unlock(hdl->lock);
    return ret;
}

static int socket_setflags (struct shim_handle * hdl, int flags)
{
    if (!hdl->pal_handle)
        return 0;

    enum process_state proc_state = cur_process.state; 
    if (proc_state == CONFINED){
    	debug("%s:%d: confined.. should not be called \n", __FUNCTION__, __LINE__);
;//        return -ECANCELED;
    }


    PAL_STREAM_ATTR attr;

    if (!DkStreamAttributesQuerybyHandle(hdl->pal_handle, &attr))
        return -PAL_ERRNO;

    if (attr.nonblocking) {
        if (flags & O_NONBLOCK)
            return 0;

        attr.nonblocking = PAL_FALSE;
    } else {
        if (!(flags & O_NONBLOCK))
            return 0;

        attr.nonblocking = PAL_TRUE;
    }

    if (!DkStreamAttributesSetbyHandle(hdl->pal_handle, &attr))
       return -PAL_ERRNO;

    return 0;
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// --------XXX new message types--------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------


static int socket_read_nonuser_data (struct shim_handle * hdl, void * buf,
                        size_t count)
{
    debug("entered %s \n", __FUNCTION__);
    int bytes = 0;
    struct shim_sock_handle * sock = &hdl->info.sock;

    enum process_state proc_state = cur_process.state; 
    if (proc_state == CONFINED){
    	debug("%s:%d: confined.. should not be called \n", __FUNCTION__, __LINE__);
;//        return -ECANCELED;
    }


    if (!count)
        return 0;

    // mkpark
    lock(hdl->lock);
    debug ("locked! %s sock_state %d %d\n", __FUNCTION__, sock->sock_state,
             SOCK_LISTENED);


    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_ACCEPTED &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = ENOTCONN;
        unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM){
        unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

    unlock(hdl->lock);
    debug ("restart! %s \n", __FUNCTION__);

    // read user data
    bytes = wolfSSL_read_sock(hdl, sock->userdata, USERDATA_SZ); //  DkStreamRead(hdl->pal_handle, 0, USERDATA_SZ, sock->userdata, NULL, 0);
    if (!bytes)
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_ENDOFSTREAM:
                return 0;
            default: {
                int err = PAL_ERRNO;
                lock(hdl->lock);
                sock->error = err;
                unlock(hdl->lock);
                return -err;
            }
        }
    else if (bytes != USERDATA_SZ){
        int err = PAL_ERRNO;
        lock(hdl->lock);
        sock->error = err;
        unlock(hdl->lock);
        return -err;
    }

    debug ("read1! %s \n", __FUNCTION__);
    // read non-user data
    bytes = wolfSSL_read_sock(hdl, buf, count); // DkStreamRead(hdl->pal_handle, 0, count, buf, NULL, 0);

    if (!bytes)
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_ENDOFSTREAM:
                return 0;
            default: {
                int err = PAL_ERRNO;
                lock(hdl->lock);
                sock->error = err;
                unlock(hdl->lock);
                return -err;
            }
        }
    debug ("read2! %s \n", __FUNCTION__);

    sock->sock_state = SOCK_NONUSER_RECVED;
    return bytes;
}


static int socket_gather_response (struct shim_handle * hdl, void * buf,
                        size_t count)
{
    int bytes = 0;
    struct shim_sock_handle * sock = &hdl->info.sock;

    if (!count)
        return 0;

    // mkpark
    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_REQUEST_SENT) {
        sock->error = ENOTCONN;
        unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM){
        unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

    unlock(hdl->lock);

    bytes = wolfSSL_read_sock(hdl, buf, count); // DkStreamRead(hdl->pal_handle, 0, count, buf, NULL, 0);

    if (!bytes)
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_ENDOFSTREAM:
                return 0;
            default: {
                int err = PAL_ERRNO;
                lock(hdl->lock);
                sock->error = err;
                unlock(hdl->lock);
                return -err;
            }
        }
    
    sock->sock_state = SOCK_RESPONSE_RECVED;
    return bytes;
}


// XXX same to the socket_read
static int socket_read_user_data (struct shim_handle * hdl, void * buf,
                        size_t count)
{
    int bytes = 0;
    struct shim_sock_handle * sock = &hdl->info.sock;

    if (!count)
        return 0;

    // mkpark
    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_ACCEPTED &&
        sock->sock_state != SOCK_CONNECTED &&
        sock->sock_state != SOCK_BOUNDCONNECTED) {
        sock->error = ENOTCONN;
        unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM){
        unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

    unlock(hdl->lock);

    bytes = wolfSSL_read_sock(hdl, buf, count); // DkStreamRead(hdl->pal_handle, 0, count, buf, NULL, 0);

    if (!bytes)
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_ENDOFSTREAM:
                return 0;
            default: {
                int err = PAL_ERRNO;
                lock(hdl->lock);
                sock->error = err;
                unlock(hdl->lock);
                return -err;
            }
        }
    
    sock->sock_state = SOCK_USERDATA;
    return bytes;
}

static int socket_extend_request (struct shim_handle * hdl, const void * buf,
                         size_t count)
{
    struct shim_sock_handle * sock = &hdl->info.sock;

    // mkpark
    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_NONUSER_RECVED) {
        sock->error = ENOTCONN;
        unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM){
        unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

    unlock(hdl->lock);

    if (!count)
        return 0;

    int bytes = wolfSSL_write_sock(hdl, sock->userdata, USERDATA_SZ); //DkStreamWrite(hdl->pal_handle, 0, count, (void *) buf, NULL);

    if (!bytes) {
        int err;
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_CONNFAILED:
                err = EPIPE;
                break;
            default:
                err = PAL_ERRNO;
                break;
        }
        lock(hdl->lock);
        sock->error = err;
        unlock(hdl->lock);
        return -err;
    }


    bytes = wolfSSL_write_sock(hdl, buf, count); //DkStreamWrite(hdl->pal_handle, 0, count, (void *) buf, NULL);

    if (!bytes) {
        int err;
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_CONNFAILED:
                err = EPIPE;
                break;
            default:
                err = PAL_ERRNO;
                break;
        }
        lock(hdl->lock);
        sock->error = err;
        unlock(hdl->lock);
        return -err;
    }

    sock->sock_state = SOCK_REQUEST_SENT;
    return bytes;
}


static int socket_send_response (struct shim_handle * hdl, const void * buf,
                         size_t count)
{
    struct shim_sock_handle * sock = &hdl->info.sock;

    // mkpark
    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_RESPONSE_RECVED) {
        sock->error = ENOTCONN;
        unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM){
        unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

    unlock(hdl->lock);

    if (!count)
        return 0;

    int bytes = wolfSSL_write_sock(hdl, buf, count); // DkStreamWrite(hdl->pal_handle, 0, count, (void *) buf, NULL);

    if (!bytes) {
        int err;
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_CONNFAILED:
                err = EPIPE;
                break;
            default:
                err = PAL_ERRNO;
                break;
        }
        lock(hdl->lock);
        sock->error = err;
        unlock(hdl->lock);
        return -err;
    }
    
    sock->sock_state = SOCK_RESPONSE_SENT;
    return bytes;
}


static int socket_send_user_data (struct shim_handle * hdl, const void * buf,
                         size_t count)
{
    struct shim_sock_handle * sock = &hdl->info.sock;

    // mkpark
    lock(hdl->lock);

    if (sock->sock_type == SOCK_STREAM &&
        sock->sock_state != SOCK_USERDATA) {
        sock->error = ENOTCONN;
        unlock(hdl->lock);
        return -ENOTCONN;
    }

    if (sock->sock_type == SOCK_DGRAM){
        unlock(hdl->lock);
        return -EDESTADDRREQ;
    }

    unlock(hdl->lock);

    if (!count)
        return 0;

    int bytes = wolfSSL_write_sock(hdl, buf, count); // DkStreamWrite(hdl->pal_handle, 0, count, (void *) buf, NULL);

    if (!bytes) {
        int err;
        switch(PAL_NATIVE_ERRNO) {
            case PAL_ERROR_CONNFAILED:
                err = EPIPE;
                break;
            default:
                err = PAL_ERRNO;
                break;
        }
        lock(hdl->lock);
        sock->error = err;
        unlock(hdl->lock);
        return -err;
    }

    sock->sock_state = SOCK_PROCESSED;
    return bytes;
}


struct shim_fs_ops socket_fs_ops = {
        .close              = &socket_close,
        .read               = &socket_read,
        .read_handshake     = &socket_read_handshake,
        .read_nonuser_data  = &socket_read_nonuser_data,
        .gather_response    = &socket_gather_response,
        .read_user_data     = &socket_read_user_data,
        .write              = &socket_write,
        .write_handshake    = &socket_write_handshake,
        .extend_request     = &socket_extend_request,
        .send_response      = &socket_send_response,
        .send_user_data     = &socket_send_user_data,
        .hstat              = &socket_hstat,
        .checkout           = &socket_checkout,
        .poll               = &socket_poll,
        .setflags           = &socket_setflags,
    };

struct shim_mount socket_builtin_fs = { .type = "socket",
                                        .fs_ops = &socket_fs_ops, };
