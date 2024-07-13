/*
 *
 *      IL2CPP RESOLVER FOR THEOS
 *           by Batchh
 *          Version 0.3
 *
 */

#include <dlfcn.h>

// Credit KittyMemory
class MemoryInfo
{
public:
    uint32_t index;
    const mach_header *header;
    const char *name;
    intptr_t address;
};

// Credit KittyMemory
MemoryInfo getBaseAddress(const std::string &fileName)
{
    MemoryInfo _info;

    const uint32_t imageCount = _dyld_image_count();

    for (uint32_t i = 0; i < imageCount; i++)
    {
        const char *name = _dyld_get_image_name(i);
        if (!name)
            continue;

        std::string fullpath(name);

        if (fullpath.length() < fileName.length() || fullpath.compare(fullpath.length() - fileName.length(), fileName.length(), fileName) != 0)
            continue;

        _info.index = i;
        _info.header = _dyld_get_image_header(i);
        _info.name = _dyld_get_image_name(i);
        _info.address = _dyld_get_image_vmaddr_slide(i);

        break;
    }
    return _info;
}

namespace IL2CPP
{
    const void *(*il2cpp_assembly_get_image)(const void *assembly);
    void *(*il2cpp_domain_get)();
    void **(*il2cpp_domain_get_assemblies)(const void *domain, size_t *size);
    const char *(*il2cpp_image_get_name)(void *image);
    void *(*il2cpp_class_from_name)(const void *image, const char *namespaze, const char *name);
    void *(*il2cpp_class_get_field_from_name)(void *klass, const char *name);
    void *(*il2cpp_class_get_method_from_name)(void *klass, const char *name, int argsCount);
    size_t (*il2cpp_field_get_offset)(void *field);
    void (*il2cpp_field_static_get_value)(void *field, void *value);
    void (*il2cpp_field_static_set_value)(void *field, void *value);

    void *(*il2cpp_string_new)(const char *str);
    void *(*il2cpp_string_new_utf16)(const wchar_t *str, int32_t length);
    uint16_t *(*il2cpp_string_chars)(void *str);
}

void Il2CppAttach()
{
    NSString *appPath = [[NSBundle mainBundle] bundlePath];
    NSString *unityFrameworkPath = [appPath stringByAppendingPathComponent:@"Frameworks/UnityFramework.framework/UnityFramework"]; // THIS NEED TO BE CHANGED IF Il2CPP ISN'T INIT IN "UnityFramework"
    void *handle = dlopen([unityFrameworkPath UTF8String], RTLD_LAZY);
    while (!handle)
    {
        //[menu showPopup:@"Error" description:@"Failed to load UnityFramework"];
        NSLog(@"Error: Failed to load UnityFramework");
        handle = dlopen([unityFrameworkPath UTF8String], RTLD_LAZY);
        sleep(1);
    }

    IL2CPP::il2cpp_assembly_get_image = reinterpret_cast<const void *(*)(const void *)>(dlsym(handle, "il2cpp_assembly_get_image"));
    IL2CPP::il2cpp_domain_get = reinterpret_cast<void *(*)()>(dlsym(handle, "il2cpp_domain_get"));
    IL2CPP::il2cpp_domain_get_assemblies = reinterpret_cast<void **(*)(const void *, size_t *)>(dlsym(handle, "il2cpp_domain_get_assemblies"));
    IL2CPP::il2cpp_image_get_name = reinterpret_cast<const char *(*)(void *)>(dlsym(handle, "il2cpp_image_get_name"));
    IL2CPP::il2cpp_class_from_name = reinterpret_cast<void *(*)(const void *, const char *, const char *)>(dlsym(handle, "il2cpp_class_from_name"));
    IL2CPP::il2cpp_class_get_method_from_name = reinterpret_cast<void *(*)(void *, const char *, int)>(dlsym(handle, "il2cpp_class_get_method_from_name"));
    IL2CPP::il2cpp_class_get_field_from_name = reinterpret_cast<void *(*)(void *, const char *)>(dlsym(handle, "il2cpp_class_get_field_from_name"));
    IL2CPP::il2cpp_field_get_offset = reinterpret_cast<size_t (*)(void *)>(dlsym(handle, "il2cpp_field_get_offset"));
    IL2CPP::il2cpp_field_static_get_value = reinterpret_cast<void (*)(void *, void *)>(dlsym(handle, "il2cpp_field_static_get_value"));
    IL2CPP::il2cpp_field_static_set_value = reinterpret_cast<void (*)(void *, void *)>(dlsym(handle, "il2cpp_field_static_set_value"));

    // Additional IL2CPP function assignments
    IL2CPP::il2cpp_string_new = reinterpret_cast<void *(*)(const char *)>(dlsym(handle, "il2cpp_string_new"));
    IL2CPP::il2cpp_string_new_utf16 = reinterpret_cast<void *(*)(const wchar_t *, int32_t)>(dlsym(handle, "il2cpp_string_new_utf16"));
    IL2CPP::il2cpp_string_chars = reinterpret_cast<uint16_t *(*)(void *)>(dlsym(handle, "il2cpp_string_chars"));

    dlclose(handle);
}

void *Il2CppGetImageByName(const char *image)
{
    size_t size;
    void **assemblies = IL2CPP::il2cpp_domain_get_assemblies(IL2CPP::il2cpp_domain_get(), &size);
    for (int i = 0; i < size; ++i)
    {
        void *img = (void *)IL2CPP::il2cpp_assembly_get_image(assemblies[i]);
        const char *img_name = IL2CPP::il2cpp_image_get_name(img);
        if (strcmp(img_name, image) == 0)
        {
            return img;
        }
    }
    return 0;
}

class Il2CppString
{
private:
    void *str;

public:
    // Constructors
    Il2CppString(const char *utf8Str)
    {
        str = IL2CPP::il2cpp_string_new(utf8Str);
    }

    Il2CppString(const wchar_t *utf16Str, int32_t length)
    {
        str = IL2CPP::il2cpp_string_new_utf16(utf16Str, length);
    }

    // Destructor
    ~Il2CppString()
    {
        // Release IL2CPP string if needed
    }

    // Get characters from the IL2CPP string
    uint16_t *getChars()
    {
        return IL2CPP::il2cpp_string_chars(str);
    }

    // Convert IL2CPP string to UTF-8
    std::string toUtf8String()
    {
        uint16_t *chars = getChars();
        if (!chars)
        {
            return "";
        }

        std::string utf8Str;
        for (int i = 0; chars[i] != '\0'; ++i)
        {
            utf8Str += static_cast<char>(chars[i]);
        }
        return utf8Str;
    }

    // Convert IL2CPP string to UTF-16
    std::wstring toUtf16String()
    {
        uint16_t *chars = getChars();
        if (!chars)
        {
            return L"";
        }

        std::wstring utf16Str;
        for (int i = 0; chars[i] != '\0'; ++i)
        {
            utf16Str += static_cast<wchar_t>(chars[i]);
        }
        return utf16Str;
    }

    // Get internal IL2CPP string pointer (if needed)
    void *getInternalString()
    {
        return str;
    }
};

class Il2CppField
{
private:
    void *image;
    void *klass;
    void *field;

public:
    // Constructor initializes the image from assembly name
    Il2CppField(const char *assemblyName)
    {
        image = Il2CppGetImageByName(assemblyName);
        if (!image)
        {
            //[menu showPopup:@"Error" description:@"Cannot find specified image."];
            NSLog(@"Error: Cannot find specified image.");
        }
    }

    // Get class by namespace and class name
    Il2CppField &getClass(const char *namespaze, const char *className)
    {
        klass = IL2CPP::il2cpp_class_from_name(image, namespaze, className);
        if (!klass)
        {
            //[menu showPopup:@"Error" description:[NSString stringWithFormat:@"Cannot find class %s in namespace %s.", className, namespaze]];
            NSLog(@"Error: Cannot find class %s in namespace %s.", className, namespaze);
        }
        return *this;
    }

    // Get field by field name
    Il2CppField &getField(const char *fieldName)
    {
        field = IL2CPP::il2cpp_class_get_field_from_name(klass, fieldName);
        if (!field)
        {
            //[menu showPopup:@"Error" description:[NSString stringWithFormat:@"Cannot find field %s in class.", fieldName]];
            NSLog(@"Error: Cannot find field %s in class.", fieldName);
        }
        return *this;
    }

    // Get field offset
    size_t getOffset() const
    {
        return IL2CPP::il2cpp_field_get_offset(field);
    }

    // Get field value
    template <typename T>
    T getValue()
    {
        T value;
        IL2CPP::il2cpp_field_static_get_value(field, &value);
        return value;
    }

    // Set field value
    template <typename T>
    void setValue(T value)
    {
        IL2CPP::il2cpp_field_static_set_value(field, &value);
    }

    // Show field value using menu popup
    template <typename T>
    void showValue(const char *fieldName)
    {
        T value = getValue<T>();
        // NSString* message = [NSString stringWithFormat:@"%s Value = %d", fieldName, value];
        // [menu showPopup:@"Field Value" description:message];
        NSLog(@"%s Value = %d", fieldName, value);
    }
};

class Il2CppMethod
{
private:
    void *image;
    void *klass;
    void *method;

public:
    // Constructor initializes the image from assembly name
    Il2CppMethod(const char *assemblyName)
    {
        image = Il2CppGetImageByName(assemblyName);
        if (!image)
        {
            //[menu showPopup:@"Error" description:@"Cannot find specified image."];
            NSLog(@"Error: Cannot find specified image.");
        }
    }

    // Get class by namespace and class name
    Il2CppMethod &getClass(const char *namespaze, const char *className)
    {
        klass = IL2CPP::il2cpp_class_from_name(image, namespaze, className);
        if (!klass)
        {
            //[menu showPopup:@"Error" description:[NSString stringWithFormat:@"Cannot find class %s in namespace %s.", className, namespaze]];
            NSLog(@"Error: Cannot find class %s in namespace %s.", className, namespaze);
        }
        return *this;
    }

    // Get method by method name and number of arguments
    uint64_t getMethod(const char *methodName, int argsCount)
    {
        void **methodPointer = (void **)IL2CPP::il2cpp_class_get_method_from_name(klass, methodName, argsCount);
        if (!methodPointer || !*methodPointer)
        {
            //[menu showPopup:@"Error" description:[NSString stringWithFormat:@"Cannot find method %s with %d arguments.", methodName, argsCount]];
            NSLog(@"Error: Cannot find method %s with %d arguments.", methodName, argsCount);
            return 0;
        }
        method = *methodPointer;

        MemoryInfo info = getBaseAddress("UnityFramework");
        uint64_t rvaOffset = reinterpret_cast<uint64_t>(method) - info.address;

        return rvaOffset;
    }

    // Invoke the method with given arguments
    template <typename Ret, typename... Args>
    Ret invoke(Args... args)
    {
        using MethodType = Ret (*)(Args...);
        MethodType methodFunc = reinterpret_cast<MethodType>(method);
        return methodFunc(args...);
    }
};