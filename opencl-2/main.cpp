/*
 * Author : Alexandre OBELLIANNE
 * Contact : morphet (little internet a) laposte.net
 *
 * 20/05/2012
 *
 * (c)Copyright - 2012 Alexandre OBELLIANNE
 *
 * Published under GPL : http://www.gnu.org/licenses/gpl.txt
 *
 *
 * This file is part of TutoSOAP.
 *
 * TutoSOAP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TutoSOAP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TutoSOAP.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <CL/cl.h>

using namespace std;

/** Source code of the OpencCL kernel, the program we will execute on our
*   OpenCL device.
*/
const char *kernelBytes = "\n" \
                          "__kernel void square(                    \n" \
                          "   __global int* input,                  \n" \
                          "   __global int* output,                 \n" \
                          "   const unsigned int count)             \n" \
                          "{                                        \n" \
                          "   int i = get_global_id(0);             \n" \
                          "   if(i < count)                         \n" \
                          "       output[i] = input[i] * input[i];  \n" \
                          "}                                        \n" \
                          "\n";

/** A callback function which will be used to report errors during OpenCL context
*   creation AND/OR execution. This not necessary for the example, but it is
*   important to know it exists. Please refer to the Khronos Group documentation
*   for further information.
*/
void CL_CALLBACK onOpenCLError(const char *errinfo,  const void *private_info,
                               size_t cb, void *user_data)
{
    printf("Error while creating context or working in this context : %s", errinfo);
}

/** Our main function.
*/
int main(void)
{
    cl_int result;      //Will be used for every OpenCL function call

    /***************************************************************************************/
    /*****                    Getting available platforms                              *****/
    /***************************************************************************************/
    //Declarations
    cl_uint             numEntries = 1;     //Max number of platform ids we want to get
    cl_platform_id*     platforms;          //List of platforms IDs
    cl_uint             numPlatforms;       //The actual number of returned platform IDs

    //Allocations
    platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*numEntries);

    //We use the clGetPlatformIDs function
    result = clGetPlatformIDs(numEntries, platforms, &numPlatforms);
    if(result != CL_SUCCESS)
    {
        cout << "Error while getting available platforms" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                     Getting available GPU devices                            *****/
    /****************************************************************************************/
    //Declarations
    cl_uint             maxDevices = 1;     //Max number of devices we want to get
    cl_device_id*       deviceIDs;          //List of device IDs
    cl_uint             numDevices;         //The actual number of returned device IDs returned

    //Allocations
    deviceIDs = (cl_device_id*)malloc(maxDevices*sizeof(cl_device_id));

    //We use the clGetDeviceIDs function
    result = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, maxDevices,
                            deviceIDs, &numDevices);
    if(result != CL_SUCCESS)
    {
        cout << "Error while getting available devices" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                     Creation of the OpenCL context                           *****/
    /****************************************************************************************/
    //Declarations
    cl_context_properties*  properties = 0;   //We don't use any property for now
    cl_uint                 usedDevices = 1;  //The number of devices we want to use


    //We use the clCreateContext function
    cl_context context = clCreateContext(properties, usedDevices, deviceIDs,
                                         &onOpenCLError, NULL, &result);
    if(result != CL_SUCCESS)
    {
        cout << "Error while creating the OpenCL context" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                     Creation of the command queue                            *****/
    /****************************************************************************************/
    //Declarations
    cl_command_queue_properties commandQueueProperties = 0;     //No properties used for now

    //Creation of the command queue using the context created above and the ID of the device
    //we want to use. Since the beginning, we are looking for using only one device, so this
    //is simply the first element of the deviceIDs array.
    cl_command_queue commands =
        clCreateCommandQueue(context, deviceIDs[0], commandQueueProperties, &result);
    if(result != CL_SUCCESS)
    {
        cout << "Error while creating the command queue" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                  Creation of the program from sources                        *****/
    /****************************************************************************************/
    //Declarations
    cl_uint     stringsCountForSource = 1;  //The number of char arrays we pass for sources.
    //In our case, 1 array which is "kernerlBytes"
    size_t*     stringLengths = NULL;       //Lengths for each string composing the kernel. Ours
    //is null terminated, so we simply set it to NULL

    //Execution
    cl_program program =
        clCreateProgramWithSource(context, 1, (const char**)&kernelBytes, stringLengths, &result);
    if(result != CL_SUCCESS)
    {
        cout << "Error while creating the program" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                       Compilation of the program                             *****/
    /****************************************************************************************/
    //Declarations
    char*               compileOptions = NULL;  //Compile options. See documentation.
    const cl_device_id* deviceList = NULL;      //We want this compilation to be for all devices
    cl_uint             devicesCount = 0;       //No particular devices given

    //Execution giving NULL as a callback function (and to the user data assigned to this
    //function) as we want to wait for the program to be compiled.
    //This function is used to handle the finishing of the compilation asynchronously. See
    //documentation for further detail.
    result = clBuildProgram(program, devicesCount, deviceList, compileOptions, NULL, NULL);
    if(result != CL_SUCCESS)
    {
        cout << "Error while compiling the program" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                          Creation of the kernel                              *****/
    /****************************************************************************************/
    //A kernel is represented as a function so that sources compiled in a program can contain
    //multiple kernels. We have to tell which one to use.
    cl_kernel kernel = clCreateKernel(program, "square", &result);
    if(result != CL_SUCCESS)
    {
        cout << "Error while creating the kernel" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                      Initialization of input data                            *****/
    /****************************************************************************************/
    //Initialization
    size_t numberOfValues = 50;
    size_t sizeOfBuffers = numberOfValues*sizeof(int);
    int* inputIntegers = (int*)malloc(sizeOfBuffers);  //Our input data array
    for(size_t i=0 ; i<numberOfValues ; i++)                           //We put some numbers in it
    {
        inputIntegers[i] = i;       //I know, I'm very lazy.....
    }

    //Creation and allocation of the input data for the kernel
    cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY,
                                        sizeOfBuffers, NULL, &result);
    if(result != CL_SUCCESS)
    {
        cout << "Error while initializing input data" << endl;
        exit(1);
    }

    //Declarations for the copy of data
    cl_bool     blockingWrite = CL_TRUE;    //We want to wait until the copy is finished
    size_t      offset = 0;                 //We start at 0 in our host buffer for the copy
    cl_event    dataInputCopyEvent;         //An identifier for this copy operation
    cl_event*   eventsToWait = NULL;        //List of events we want to wait the end before
    //performing this copy operation
    cl_uint     numEvents = 0;              //Number of events

    //Copying the data the device input buffer we've just created
    result = clEnqueueWriteBuffer(commands, inputBuffer, blockingWrite, offset, sizeOfBuffers,
                                  inputIntegers, numEvents, eventsToWait, &dataInputCopyEvent);
    if(result != CL_SUCCESS)
    {
        cout << "Error while writing input data" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                      Initialization of output data                           *****/
    /****************************************************************************************/
    //Here, we are just allocating a buffer onto the OpenCL device. After the execution, we
    //will use it to copy data on the host memory.
    cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                         sizeOfBuffers, NULL, &result);
    if(result != CL_SUCCESS)
    {
        cout << "Error while initializing output data" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                         Setting kernel arguments                             *****/
    /****************************************************************************************/
    //We will tell OpenCL what are the arguments for the kernel using their index the
    //declaration of the kernel (see kernel sources) : input at index 0 and output at index 1
    result = 0;
    result |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputBuffer);
    result |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &outputBuffer);
    result |= clSetKernelArg(kernel, 2, sizeof(size_t), &numberOfValues);
    if(result != CL_SUCCESS)
    {
        cout << "Error while setting kernel arguments" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                   Finally, execution of the kernel !!!!                      *****/
    /****************************************************************************************/
    //Declarations
    cl_uint   workDim = 1;                      //We can use dimensions to organize data. Here
    //we only got 1 dimension in our array, so we
    //use 1.
    size_t*   globalWorkOffset = NULL;          //Offsets used for each dimension to give
    //identfication values to work-items
    size_t    globalWorkSize =  numberOfValues; //Number of values for each dimension we use
    size_t    localWorkSize;                    //Size of a work-group in each dimension
    cl_event  kernelExecEvent;                  //The event for the execution of the kernel
    eventsToWait = NULL;              //List of events we want to wait the end before
    //performing this copy operation
    numEvents = 0;                    //Number of events

    //If have to set a correct work group size. The total number of work-items we want to run
    //(in our case, numberOfValues work-items) should be divisable by the work-group size.
    //I will not make to muche test here, just the case where numberOfValues is less than
    //the maximum work-group size.
    result = clGetKernelWorkGroupInfo(kernel, deviceIDs[0], CL_KERNEL_WORK_GROUP_SIZE,
                                      sizeof(localWorkSize), &localWorkSize, NULL);
    if(localWorkSize > numberOfValues) localWorkSize = numberOfValues;
    if(result != CL_SUCCESS)
    {
        cout << "Error while getting maximum work group size" << endl;
        exit(1);
    }

    //Execution
    result = clEnqueueNDRangeKernel(commands, kernel, workDim, globalWorkOffset, &globalWorkSize,
                                    &localWorkSize, numEvents, eventsToWait, &kernelExecEvent);
    if(result != CL_SUCCESS)
    {
        cout << "Error while executing the kernel" << endl;
        exit(1);
    }

    /****************************************************************************************/
    /*****                          Getting back the results                            *****/
    /****************************************************************************************/
    //Declarations
    cl_bool     blockingRead = CL_TRUE;
    offset = 0;
    int*        resultArray;
    cl_event    readResultsEvent;           //The event for the execution of the kernel
    eventsToWait = NULL;        //List of events we want to wait the end before
    //performing this copy operation
    numEvents = 0;              //Number of events

    //Allocations
    resultArray = (int*)malloc(numberOfValues*sizeof(int));

    //Waiting for all commands to end. Note we coul have use the kernelExecEvent as an event
    //to wait the end. But the clFinish function is simplier in this case.
    clFinish(commands);

    //Execution
    clEnqueueReadBuffer(commands, outputBuffer, blockingRead, offset, sizeOfBuffers,
                        resultArray, numEvents, eventsToWait, &readResultsEvent);

    /****************************************************************************************/
    /*****                              Output of result                                *****/
    /****************************************************************************************/
    for(size_t i=0 ; i<numberOfValues ; i++)
    {
        cout << resultArray[i] << "\t";
        if((i+1) % 10 == 0 && i != 0)
        {
            cout << endl;
        }
    }


    /****************************************************************************************/
    /*****                          Free data. Good habits ^^                           *****/
    /****************************************************************************************/
    free(platforms);
    free(deviceIDs);
    free(inputIntegers);
    free(resultArray);

    return 0;
}
