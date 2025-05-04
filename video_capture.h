#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include "silgle_linked_list.h"

#include <strmif.h>
#include <control.h>



/**
 * @brief Retrieves a list of system devices for a specified device category.
 * @param pp_device_list Pointer to a pointer that will receive the list of devices.
 * @param category Pointer to a GUID representing the device category (e.g., CLSID_VideoInputDeviceCategory).
 * @return S_OK on success, or an error code on failure (e.g., E_POINTER for invalid parameters).
 * @note
 * - The caller is responsible for freeing the returned device list using delete_list().
 * - Uses COM to enumerate devices and retrieve their properties (FriendlyName and DevicePath).
 * - Devices are stored in a single-linked list of device_info_t structures.
 */
HRESULT get_system_devices(
    single_linked_list_t** pp_device_list, const GUID* category
);

/**
 * @brief Initializes a capture graph builder and a filter graph for video capture.
 * @param pp_graph Pointer to a pointer that will receive the initialized IGraphBuilder.
 * @param pp_build Pointer to a pointer that will receive the initialized ICaptureGraphBuilder2.
 * @return S_OK on success, or an error code on failure (e.g., E_POINTER for invalid parameters).
 * @note
 * - Creates instances of CLSID_CaptureGraphBuilder2 and CLSID_FilterGraph.
 * - Associates the filter graph with the capture graph builder.
 * - The caller is responsible for releasing the returned interfaces.
 */
HRESULT init_capture_graph_builder(
    IGraphBuilder** pp_graph, ICaptureGraphBuilder2** pp_build
);

/**
 * @brief Allows the user to select a video device from a list and retrieves its filter and name.
 * @param devices Pointer to a single-linked list containing device_info_t structures.
 * @param pp_filter Pointer to a pointer that will receive the IBaseFilter for the selected device.
 * @param ppwc_device_name Pointer to a pointer that will receive a copy of the selected device's friendly name.
 * @return S_OK on success, or an error code on failure (e.g., E_POINTER for invalid parameters or invalid selection).
 * @note
 * - Displays a list of devices and prompts the user to input a device number.
 * - Binds the selected device's moniker to an IBaseFilter.
 * - The caller is responsible for releasing the filter and freeing the device name string.
 */
HRESULT select_video_device(
    const single_linked_list_t* devices, 
    IBaseFilter** pp_filter, 
    wchar_t** ppwc_device_name
);

/**
 * @brief Sets up and runs a video preview stream for a selected device.
 * @param p_graph Pointer to the IGraphBuilder for the filter graph.
 * @param p_builder Pointer to the ICaptureGraphBuilder2 for building the capture graph.
 * @param p_filter Pointer to the IBaseFilter representing the video capture device.
 * @param pwc_device_name Pointer to the friendly name of the device (used as the filter name).
 * @param pp_control Pointer to a pointer that will receive the IMediaControl interface to control the stream.
 * @return S_OK on success, or an error code on failure (e.g., E_POINTER for invalid parameters).
 * @note
 * - Adds the device filter to the graph and renders a preview stream.
 * - Retrieves the IMediaControl interface to allow starting/stopping the stream.
 * - The caller is responsible for releasing the IMediaControl interface.
 */
HRESULT run_render_stream(
    IGraphBuilder* p_graph, 
    ICaptureGraphBuilder2* p_builder, 
    IBaseFilter* p_filter, 
    const wchar_t* pwc_device_name, 
    IMediaControl** pp_control
);

#endif // !VIDEO_CAPTURE_H
