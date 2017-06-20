#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <linux/netlink.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "USBFinder.h"

#define UEVENT_BUFFER_SIZE      2048


#ifdef _DEBUG_ON_
#define TRACE       printf
#else
#define TRACE(...)
#endif


static int init_hotplug_sock(void)
{
    int retval;
    struct sockaddr_nl snl;
    const int buffersize = 16 * 1024 * 1024;
	struct timeval timeout= {1,0};

    memset(&snl, 0x00, sizeof(struct sockaddr_nl));

    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
    snl.nl_groups = 1;

    int hotplug_sock = socket(
                PF_NETLINK,
                SOCK_DGRAM,
                NETLINK_KOBJECT_UEVENT);

    if (hotplug_sock == -1)
    {
        TRACE("error getting socket: %s", strerror(errno));
        return -1;
    }

    /* set receive buffersize */
    setsockopt(
                hotplug_sock,
                SOL_SOCKET,
                SO_RCVBUFFORCE,
                &buffersize,
                sizeof(buffersize));

	/* set receive timeout */
    setsockopt(
                hotplug_sock,
                SOL_SOCKET,
                SO_RCVTIMEO,
                (char*)&timeout,
                sizeof(struct timeval));

    retval = bind(
                hotplug_sock,
                (struct sockaddr *) &snl,
                sizeof(struct sockaddr_nl));

    if (retval < 0)
    {
        TRACE("bind failed: %s", strerror(errno));
        close(hotplug_sock);
        hotplug_sock = -1;
        return -1;
    }

    return hotplug_sock;
}

static bool GetDeviceInfo(
    const char *        buf,
    USB_DEVICE_INFO *   info,
    char *              portName)
{
    const char *prefix = "add@";
    const char *temp1 = "/tty/ttyACM";
    const char *sysPrefix ="/sys";
    const char *PID ="idProduct";
    const char *VID ="idVendor";

    char mybuf[UEVENT_BUFFER_SIZE]={0};

    char *firstPosition;
    char *lastPosition;

    memcpy(mybuf,buf,strlen(buf));

    firstPosition = strstr(mybuf,prefix);
    lastPosition = strstr(mybuf, temp1);

    TRACE("first position is :%s\nlast position is:%s\n",
          firstPosition,lastPosition);

    int prefixLen = strlen(prefix);
    int length = lastPosition - firstPosition - prefixLen;
    char path_without_sys_prefix [UEVENT_BUFFER_SIZE]={0};
    char path_without_sys_prefix_up_dir[UEVENT_BUFFER_SIZE]={0};
    char path_with_sys_prefix[UEVENT_BUFFER_SIZE]={0};
    char path_of_pid[UEVENT_BUFFER_SIZE]={0};
    char path_of_vid[UEVENT_BUFFER_SIZE]={0};

    if(firstPosition && lastPosition)
    {
        TRACE("firstPosition + prefixLen: %s\n",firstPosition+prefixLen);

        memcpy(path_without_sys_prefix,firstPosition+prefixLen,length);

        char* lastToken=rindex(path_without_sys_prefix,'/');
        int len = strlen(lastToken);
        TRACE("len is %d\n lastToken is: %s\n",len,lastToken);

        memcpy(path_without_sys_prefix_up_dir,path_without_sys_prefix,length-len);
        TRACE("after memcpy...%s\n",path_without_sys_prefix);
        sprintf(path_with_sys_prefix,"%s%s",sysPrefix,path_without_sys_prefix_up_dir);
        TRACE("path is: %s\n",path_with_sys_prefix);

        sprintf(path_of_vid,"%s/%s",path_with_sys_prefix,VID);
        sprintf(path_of_pid,"%s/%s",path_with_sys_prefix,PID);
        TRACE("vid is %s\n",path_of_vid);
        TRACE("pid is %s\n",path_of_pid);

        //read file
        char tmpbuf[5] = {0};

        int fd = open(path_of_vid,O_RDONLY);
        read(fd,tmpbuf,sizeof(tmpbuf)-1);
        close(fd);

        TRACE("vid is %s\n",tmpbuf);

        info->vid = strtol(tmpbuf, NULL, 16);
        TRACE("info->vid = %04x\n", info->vid);

        fd = open(path_of_pid,O_RDONLY);
        read(fd,tmpbuf,sizeof(tmpbuf)-1);
        close(fd);

        TRACE("pid is %s\n", tmpbuf);

        info->pid = strtol(tmpbuf, NULL, 16);
        TRACE("info->pid = %04x\n", info->pid);

        char portNameTmp[UEVENT_BUFFER_SIZE]={0};
        memcpy(portNameTmp,rindex(lastPosition,'/'),strlen(lastPosition));
        sprintf(portName,"/dev%s",portNameTmp);
        TRACE("portName is: %s\n",portName);

        return true;
    }

    return false;
}

static bool WaitForDeviceReady(const char *path)
{
#if 1
    int fd = -1;

    // UEVENT is earlier than create tty node
    // so sleep some time...
    TRACE("<%s>: waiting...\n", path);

    for (int i=0; i<100; ++i)
    {
        if ((fd=open(path, O_RDWR|O_NONBLOCK|O_NOCTTY, 0)) > 0)
        {
            TRACE("<%s>: ready~\n", path);
            close(fd);
            usleep(100000);
            return true;
        }
        else
        {
            TRACE("[%d]open failed: %d, %s\n",
                i, errno, strerror(errno));
        }
        usleep(100000);
    }

    TRACE("<%s>: not ready!\n", path);

    return false;
#else
    usleep(500000);
    return true;
#endif
}

static bool VerifyDeviceInfo(
    const USB_DEVICE_INFO info[],
    const USB_DEVICE_INFO *inst)
{
    for (size_t i=0; info[i].vid != 0; ++i)
    {
        if (info[i].vid == inst->vid &&
            info[i].pid == inst->pid )
        {
            return true;
        }
    }
    return false;
}

bool FindUSBPort(const USB_DEVICE_INFO info[], char *portName)
{
    int hotplug_sock = init_hotplug_sock();

    char buf[UEVENT_BUFFER_SIZE*2] = {0};

    ssize_t bytes = recv(hotplug_sock, &buf, sizeof(buf), 0);

    close(hotplug_sock);

    TRACE("recv returns: %ld\n", bytes);

    if (bytes > 0)
    {
        TRACE("notify: %s\n", buf);

        USB_DEVICE_INFO newdev = { 0 };

#ifdef _DEBUG_ON_
        TRACE("=============================\n");
        for (int i=0; info[i].vid != 0; ++i)
        {
            TRACE("target device %d: %04x/%04x\n",
                  i, info[i].vid, info[i].pid);
        }
        TRACE("=============================\n");
#endif

        if (GetDeviceInfo(buf, &newdev, portName) &&
            VerifyDeviceInfo(info, &newdev))
        {
            TRACE("got device: %s\n", portName);
            return WaitForDeviceReady(portName);
        }

        TRACE("device not wanted: %04x/%04x\n",
              newdev.vid, newdev.pid);
    }
    return false;
}

#ifdef _MAKE_RUNNABLE_
int main()
{
    USB_DEVICE_INFO info[] =
    {
        {0x0e8d, 0x0003},
        {0x0e8d, 0x2000},
        {0x0e8d, 0x2001},
        {0x0000, 0x0000}
    };

    char portName[128]={0};

    TRACE("Waiting for usb port...\n");

    while(!FindUSBPort(info, portName))
    {
        ;
	}

    TRACE("\nport Name is %s\n",portName);

    struct termios newtio, oldtio;
    //open port
    TRACE("openning USB port\n");
    int fd = open(portName, O_RDWR | O_NONBLOCK | O_NOCTTY, 0);
    if(fd < 0)
    {
        fd = NULL;
        TRACE("USB port open failed: %d, %s\n",errno,strerror(errno));
        return -1;
    }
    TRACE("USB port open succeeded: %s\n",portName);

    TRACE("setting USB port attr...\n");
    //set port
    tcgetattr(fd, &oldtio);
    bzero(&newtio, sizeof(struct termios));
    newtio.c_cflag = B115200 | CS8 | CREAD | CRTSCTS;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    //time out
    newtio.c_cc[VMIN] = 0;
    newtio.c_cc[VTIME] = 1;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    TRACE("closing USB port\n");
    close(fd);
}
#endif
