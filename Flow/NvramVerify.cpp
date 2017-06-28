
#include <stdio.h>
#include "../Logger/Log.h"

typedef struct _NvRamHeader
{
    unsigned int map_size;
    unsigned int dat_size;
    unsigned int ________;
    unsigned int checksum;

    //unsigned char map_file[4/*map_size*/];

} NvRamHeader;


static bool CalcCheckSum(
        FILE *stream,
        unsigned int offset,
        unsigned int length,
        unsigned int &checksum)
{
    if (fseek(stream, offset, SEEK_SET) != 0)
    {
        LOGD("Failed to find the beginning of nvram data\n");
        return false;
    }

    bool flag = true;
    unsigned int temp = 0;
    const unsigned int total = length;

    checksum = 0;

    while (length >= sizeof(unsigned int))
    {
        if (fread(&temp, 1, sizeof(temp), stream) < sizeof(temp))
        {
            LOGD("Failed to read %d bytes\n", sizeof(temp));
            return false;
        }

        if (flag)
        {
            checksum ^= temp;
        }
        else
        {
            checksum += temp;
        }

        flag = !flag;

        length -= sizeof(unsigned int);
    }

    if (length > 0)
    {
        if (fread(&temp, 1, length, stream) < length)
        {
            LOGD("Failed to read %d bytes\n", length);
            return false;
        }
        checksum += temp;
    }

    checksum ^= total;

    LOGD("Calculate check sum done.\n");

    return true;
}

bool DoNvRamVerify(
        const char *bin_file,
        unsigned int block_size)
{
    FILE *stream = fopen(bin_file, "rb");

    if (NULL == stream)
    {
        LOGD("Failed to open bin file (%s)\n", bin_file);
        return false;
    }

    NvRamHeader header;
    memset(&header, 0, sizeof(header));
    unsigned int checksum = 0;

    fseek(stream, 0, SEEK_SET);

    if (fread(&header, 1, sizeof(header), stream) < sizeof(header))
    {
        LOGD("Failed to read nvram header\n");
        goto TheExit;
    }

    LOGD("Read nvram header done.\n");

    if (!CalcCheckSum(stream, block_size, header.dat_size, checksum))
    {
        goto TheExit;
    }

    LOGD("stored check sum = 0x%08x\n", header.checksum);
    LOGD("calculated check sum = 0x%08x\n", checksum);

TheExit:

    fclose(stream);

    return (checksum == header.checksum);
}
