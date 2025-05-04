#include <dshow.h>

#include "video_capture.h"
#include "device_info.h"
#include "utils.h"



int main(void)
{
    HRESULT hr = S_OK;
    IGraphBuilder* p_graph = NULL;
    ICaptureGraphBuilder2* p_builder = NULL;
    single_linked_list_t* devices = NULL;
    IBaseFilter* p_filter = NULL;
    wchar_t* pwc_device_name = NULL;
    IMediaControl* p_control = NULL;

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        goto cleanup;
    }

    hr = get_system_devices(&devices, &CLSID_VideoInputDeviceCategory);
    if (FAILED(hr))
    {
        goto cleanup;
    }

    if (is_list_empty(devices))
    {
        wprintf(L"Can't find any devices. Press \"Enter\" to exit...\n");
        wait_for_console_enter();
        goto cleanup;
    }

    hr = select_video_device(devices, &p_filter, &pwc_device_name);
    if (FAILED(hr))
    {
        goto cleanup;
    }

    if (FAILED(hr))
    {
        goto cleanup;
    }

    hr = init_capture_graph_builder(&p_graph, &p_builder);
    if (FAILED(hr))
    {
        goto cleanup;
    }

    hr = run_render_stream(p_graph, p_builder, p_filter, pwc_device_name, &p_control);
    if (!SUCCEEDED(hr))
    {
        goto cleanup;
    }

    hr = p_control->lpVtbl->Run(p_control);
    if (!SUCCEEDED(hr))
    {
        goto cleanup;
    }
    wprintf(L"Render stream running\n");

    wprintf(L"Playing is started. Press \"Enter\" to stop...\n");
    wait_for_console_enter();

    p_control->lpVtbl->Stop(p_control);
    wprintf(L"Render stream stopped\n");

cleanup:
    if (devices)
    {
        for (node_t* cur = devices->head; cur; cur = cur->next)
        {
            device_info_t* device_info = ((device_info_t*)cur->data);
            device_info->p_moniker->lpVtbl->Release(device_info->p_moniker);
            free(device_info->pwc_name);
            free(device_info->pwc_device_path);
            free(device_info);
        }
        delete_list(&devices);
    }

    if (p_control)
    {
        p_control->lpVtbl->Stop(p_control);
        p_control->lpVtbl->Release(p_control);
    }

    if (pwc_device_name)
    {
        free(pwc_device_name);
    }

    if (p_filter)
    {
        p_filter->lpVtbl->Release(p_filter);
    }

    if (p_builder)
    {
        p_builder->lpVtbl->Release(p_builder);
    }

    if (p_graph)
    {
        p_graph->lpVtbl->Release(p_graph);
    }

    CoUninitialize();

    print_h_result(hr);

    return hr;
}
