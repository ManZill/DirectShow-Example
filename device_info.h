#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <objidl.h>



/**
 * @brief Structure to store information about a device.
 *
 * This structure holds information about a device, including its moniker,
 * name, and device path, typically used for managing device connections
 * in a Windows environment.
 *
 * @param p_moniker Pointer to the IMoniker interface for the device.
 * @param pwc_name Pointer to a wide-character string containing the device name.
 * @param pwc_device_path Pointer to a wide-character string containing the device path.
 */
typedef struct device_info_s {
    IMoniker* p_moniker;
    wchar_t* pwc_name;
    wchar_t* pwc_device_path;
} device_info_t;

#endif // !DEVICE_INFO_H
