#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DeviceDesc
{
    cl_device_id    deviceId;
    cl_device_type  deviceType;
    char*           deviceTypeString;
    char*           deviceName;
} DeviceDesc;



int main(void)
{
    printf("Hello world!\n");
    size_t              i;
    size_t              maxDevices = 5;
    cl_device_id*       deviceIDs = (cl_device_id*)malloc(maxDevices*sizeof(cl_device_id));
    cl_platform_id*     platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id));
    cl_int              err;
    cl_uint             num_entries = 1;
    cl_uint             available;
    cl_uint             numDevices;
    DeviceDesc*         devices;

    // Get available platforms (we will use the first result, I think most of the time we have
    // one platform on personal computers)-lOpenCL
    cl_int result = clGetPlatformIDs(num_entries, platforms, &available);

    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, maxDevices, deviceIDs, &numDevices);

    devices = (DeviceDesc*)malloc(numDevices*sizeof(DeviceDesc));

    for(i=0 ; i<numDevices ; i++)
    {
        devices[i].deviceId = deviceIDs[i];
        size_t actualSize;

        //Getting the device type (processor, graphics card, accelerator)
        result = clGetDeviceInfo(
                     deviceIDs[i],
                     CL_DEVICE_TYPE,
                     sizeof(cl_device_type),
                     &devices[i].deviceType,
                     &actualSize);

        //Getting the human readable device type
        switch(devices[i].deviceType)
        {
        case CL_DEVICE_TYPE_CPU:
            devices[i].deviceTypeString = "Processor";
            break;
        case CL_DEVICE_TYPE_GPU:
            devices[i].deviceTypeString = "Graphics card";
            break;
        case CL_DEVICE_TYPE_ACCELERATOR:
            devices[i].deviceTypeString = "Accelerator";
            break;
        default:
            devices[i].deviceTypeString = "NONE";
            break;
        }

        //Getting the device name
        size_t deviceNameLength = 4096;
        char* tempDeviceName = (char*)malloc(4096);
        result |= clGetDeviceInfo(
                      deviceIDs[i],
                      CL_DEVICE_NAME,
                      deviceNameLength,
                      tempDeviceName,
                      &actualSize);
        if(result == CL_SUCCESS)
        {
            devices[i].deviceName = (char*)malloc(actualSize);
            memcpy(devices[i].deviceName, tempDeviceName, actualSize);
            free(tempDeviceName);
        }

        //If an error occurred
        if(result != CL_SUCCESS)
        {
            printf("Error while getting device info\n");
            return -1;
        }
    }

    //And finally we print the information we wanted to have
    for(i=0 ; i<numDevices ; i++)
    {
        printf("Device %s is of type %s\n", devices[i].deviceName, devices[i].deviceTypeString);
    }

    //Free memory
    free(deviceIDs);
    free(platforms);
    return 0;
}

