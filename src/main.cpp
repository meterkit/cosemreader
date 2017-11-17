
#include <iostream>
#include "JsonReader.h"
#include "CosemClient.h"
#include "Configuration.h"
#include "csm_array.h"

CosemClient client;

extern "C" int csm_sys_get_lls_password(uint8_t sap, uint8_t *array, uint8_t max_size)
{
    (void)sap;
    std::string lls = client.GetLls();

    uint32_t size = (lls.size() > max_size) ? max_size : lls.size();

    lls.copy((char*)array, size);

    return TRUE;
}

int main(int argc, char **argv)
{
    bool ok = true;


    setbuf(stdout, NULL); // disable printf buffering

    puts("** DLMS/Cosem Client started\r\n");

    if (argc >= 3)
    {
        std::string commFile(argv[1]); // First file is the communication parameters
        std::string objectsFile(argv[2]); // Second is the objects to retrieve

        if (argc >= 4)
        {
            client.SetStartDate(std::string(argv[3])); // startDate for the profiles
        }

        if (argc >= 5)
        {
            client.SetStartDate(std::string(argv[4])); // endDate for the profiles
        }

        // Before application, test connectivity
        ok = client.Initialize(commFile, objectsFile);

        while(ok)
        {
            sleep(1);
            ok = client.PerformTask();
        }
    }
    else
    {
        printf("Usage example: cosem_client /path/comm.json /another/objectlist.json 2017-08-01.00:00:00 2017-10-23.14:55:02\r\n");
        puts("\r\nDate-time format: %Y-%m-%d.%H:%M:%S");
    }

    printf("** Exit task loop, waiting for reading thread...\r\n");
    client.WaitForStop();

    return 0;

}

