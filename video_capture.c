#include "video_capture.h"

#include <uuids.h>
#include <stdint.h>
#include <stdio.h>

#include "device_info.h"



HRESULT get_system_devices(
    single_linked_list_t** pp_device_list,
    const GUID* category
)
{
    HRESULT hr = S_OK;
    ICreateDevEnum* p_system_device_enumerator = NULL;
    IEnumMoniker* p_enumerator_category = NULL;
    IMoniker* p_moniker = NULL;
    ULONG c_fetched = 0;
    single_linked_list_t* p_list = NULL;

    if (pp_device_list == NULL || category == NULL)
    {
        return E_POINTER;
    }

    hr = CoCreateInstance(
        &CLSID_SystemDeviceEnum, 
        NULL, 
        CLSCTX_INPROC_SERVER,
        &IID_ICreateDevEnum,
        (LPVOID*)&p_system_device_enumerator
    );

    if (FAILED(hr))
    {
        return hr;
    }

    hr = p_system_device_enumerator->lpVtbl->CreateClassEnumerator(
        p_system_device_enumerator, 
        category, 
        &p_enumerator_category, 
        0
    );

    if (hr != S_OK)
    {
        p_system_device_enumerator->lpVtbl->Release(p_system_device_enumerator);
        return hr;
    }

    hr = initialize_list(&p_list, 0);
    if (p_list == NULL)
    {
        hr = E_POINTER;
        goto cleanup;
    }

    while (p_enumerator_category->lpVtbl->Next(
        p_enumerator_category, 1, &p_moniker, &c_fetched
    ) == S_OK)
    {
        IPropertyBag* p_property_bag = NULL;
        VARIANT variant_name = { 0 };
        wchar_t* pwc_name = NULL;
        wchar_t* pwc_device_path = NULL;
        device_info_t* device = NULL;

        hr = p_moniker->lpVtbl->BindToStorage(
            p_moniker, 
            0, 
            0, 
            &IID_IPropertyBag,
            (LPVOID*)&p_property_bag
        );

        if (FAILED(hr))
        {
            p_moniker->lpVtbl->Release(p_moniker);
            continue;
        }

        VariantInit(&variant_name);
        hr = p_property_bag->lpVtbl->Read(
            p_property_bag, 
            L"FriendlyName", 
            &variant_name, 
            NULL
        );

        if (!SUCCEEDED(hr))
        {
            VariantClear(&variant_name);
            continue;
        }

        pwc_name = _wcsdup((wchar_t*)variant_name.bstrVal);
        if (pwc_name == NULL)
        {
            VariantClear(&variant_name);
            p_property_bag->lpVtbl->Release(p_property_bag);
            hr = S_OK;
            continue;
        }
        VariantClear(&variant_name);

        VariantInit(&variant_name);
        hr = p_property_bag->lpVtbl->Read(
            p_property_bag, L"DevicePath", &variant_name, NULL
        );

        if (!SUCCEEDED(hr)) 
        {
            free(pwc_name);
            VariantClear(&variant_name);
            hr = S_OK;
            continue;
        }

        pwc_device_path = _wcsdup((wchar_t*)variant_name.bstrVal);
        if (pwc_device_path == NULL) 
        {
            free(pwc_name);
            free(device);
            VariantClear(&variant_name);
            p_property_bag->lpVtbl->Release(p_property_bag);
            continue;
        }
        VariantClear(&variant_name);

        device = (device_info_t*)malloc(sizeof(device_info_t));
        if (device == NULL)
        {
            free(pwc_device_path);
            free(pwc_name);
            free(device);
            VariantClear(&variant_name);
            p_property_bag->lpVtbl->Release(p_property_bag);
            continue;
        }

        device->p_moniker = p_moniker;
        device->pwc_name = pwc_name;
        device->pwc_device_path = pwc_device_path;

        add_item_to_end(p_list, device);

        p_property_bag->lpVtbl->Release(p_property_bag);
    }

cleanup:
    if (p_enumerator_category)
    {
        p_enumerator_category->lpVtbl->Release(p_enumerator_category);
    }

    if (p_system_device_enumerator)
    {
        p_system_device_enumerator->lpVtbl->Release(p_system_device_enumerator);
    }

    if (FAILED(hr))
    {
        delete_list(&p_list);
        return hr;
    }

    *pp_device_list = p_list;
    return hr;
}

HRESULT init_capture_graph_builder(
    IGraphBuilder** pp_graph,
    ICaptureGraphBuilder2** pp_build
)
{
    HRESULT hr = S_OK;
    IGraphBuilder* p_graph = NULL;
    ICaptureGraphBuilder2* p_build = NULL;

    if (pp_graph == NULL || pp_build == NULL)
    {
        return E_POINTER;
    }

    hr = CoCreateInstance(
        &CLSID_CaptureGraphBuilder2, 
        NULL, 
        CLSCTX_INPROC_SERVER,
        &IID_ICaptureGraphBuilder2, 
        (LPVOID*)&p_build
    );

    if (FAILED(hr))
    {
        return hr;
    }

    hr = CoCreateInstance(&CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER,
        &IID_IGraphBuilder, (LPVOID*)&p_graph);

    if (FAILED(hr))
    {
        p_build->lpVtbl->Release(p_build);
        return hr;
    }

    p_build->lpVtbl->SetFiltergraph(p_build, p_graph);
    *pp_build = p_build;
    *pp_graph = p_graph;
    return S_OK;
}

HRESULT select_video_device(
    const single_linked_list_t* devices,
    IBaseFilter** pp_filter,
    wchar_t** ppwc_device_name
)
{
    HRESULT hr = S_OK;
    node_t* current = NULL;
    uint32_t selected_device_number = 0;
    device_info_t* device_info = NULL;

    if (devices == NULL || pp_filter == NULL || ppwc_device_name == NULL)
    {
        return E_POINTER;
    }

    if (is_list_empty(devices)) 
    {
        return S_OK;
    }

    wprintf(L"Select device:\n");
    current = devices->head;
    for (int i = 1; current; current = current->next, i++)
    {
        device_info_t* current_device_info = NULL;
        current_device_info = ((device_info_t*)current->data);
        wprintf(L"%d. %s\n", i, current_device_info->pwc_name);
    }
    wprintf(L"Input device number:\n");
    wscanf_s(L"%d", &selected_device_number);

    device_info = get_list_element_at(devices, selected_device_number - 1);
    if (device_info == NULL)
    {
        wprintf(L"Can't find device with number %d\n", selected_device_number);
        return E_POINTER;
    }

    hr = device_info->p_moniker->lpVtbl->BindToObject(
        device_info->p_moniker,
        NULL, 
        NULL, 
        &IID_IBaseFilter, 
        (void**)pp_filter
    );

    if (FAILED(hr))
    {
        wprintf(L"Can't create moniker filter\n");
        return hr;
    }

    *ppwc_device_name = _wcsdup(device_info->pwc_name);
    if (*ppwc_device_name == NULL)
    {
        (*pp_filter)->lpVtbl->Release(*pp_filter);
        return E_POINTER;
    }
    return hr;
}

HRESULT run_render_stream(
    IGraphBuilder* p_graph,
    ICaptureGraphBuilder2* p_builder,
    IBaseFilter* p_filter,
    const wchar_t* pwc_device_name,
    IMediaControl** pp_control
)
{
    HRESULT hr = S_OK;
    IMediaControl* p_control = NULL;

    if (p_graph == NULL || p_builder == NULL || p_filter == NULL ||
        pwc_device_name == NULL || pp_control == NULL)
    {
        return E_POINTER;
    }

    hr = p_graph->lpVtbl->AddFilter(p_graph, p_filter, pwc_device_name);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = p_builder->lpVtbl->RenderStream(
        p_builder, 
        &PIN_CATEGORY_PREVIEW,
        &MEDIATYPE_Video, 
        (IUnknown*)p_filter,
        NULL, 
        NULL
    );

    if (FAILED(hr))
    {
        return hr;
    }

    hr = p_graph->lpVtbl->QueryInterface(
        p_graph, 
        &IID_IMediaControl,
        (void**)&p_control
    );

    if (FAILED(hr))
    {
        return hr;
    }

    *pp_control = p_control;
    return S_OK;
}
