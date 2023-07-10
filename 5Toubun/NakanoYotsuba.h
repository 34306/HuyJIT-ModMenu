#import "NakanoMiku.h"
#include <mach/mach.h>
#include <mach-o/dyld.h>
#include <CoreFoundation/CoreFoundation.h>
#include <string>


bool hasASLR() {
    const struct mach_header *mach;
    //Checkout this one, I use Xina/Dopamine to test my mod menu and I set it to 1
    //If you're on rootful, just change it to 0
    //If you're building for jailed (or called non-jailbreak), both 0 and 1 can work
    mach = _dyld_get_image_header(1);
    if (mach->flags & MH_PIE) {
        return true;
    } else {
        return false;
    }
}

long long get_slide() {
    //Same thing with this one. I'm kinna lazy to coding it automatically detect, so...
    return _dyld_get_image_vmaddr_slide(1);
}

long long calculateAddress(long long offset) {
    if (hasASLR()) {
        long long slide = get_slide();
        return (slide + offset);
    } else {
        return offset;
    }
}

bool getType(unsigned int data) {
    int a = data & 0xffff8000;
    int b = a + 0x00008000;
    int c = b & 0xffff7fff;
    return c;
}
//Well, using vm quite easier patch than KittyMemory right?
//Use vm for NULL, like the game only have Binary as main
bool vm(long long offset, unsigned int data) {
    kern_return_t err;
    mach_port_t port = mach_task_self();
    long long address = calculateAddress(offset);

    err = vm_protect(port, (mach_vm_address_t)address, sizeof(data), false, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
    if (err != KERN_SUCCESS) {
        return false;
    }

    if (getType(data)) {
        data = CFSwapInt32(data);
        err = vm_write(port, (mach_vm_address_t)address, (vm_offset_t)&data, sizeof(data));
    } else {
        data = (unsigned short)data;
        data = CFSwapInt16(data);
        err = vm_write(port, (mach_vm_address_t)address, (vm_offset_t)&data, sizeof(data));
    }

    if (err != KERN_SUCCESS) {
        return false;
    }

    err = vm_protect(port, (mach_vm_address_t)address, sizeof(data), false, VM_PROT_READ | VM_PROT_EXECUTE);
    if (err != KERN_SUCCESS) {
        return false;
    }

    return true;
}

//Patching for Unity section, I just copy and paste it from KittyMemory becasue I'm lazy :D
struct MemoryFileInfo {
    uint32_t index;
    const struct mach_header *header;
    const char *name;
    long long address;
};

MemoryFileInfo getBaseInfo() {
    MemoryFileInfo info;
    info.index = 0;
    info.header = _dyld_get_image_header(0);
    info.name = _dyld_get_image_name(0);
    info.address = _dyld_get_image_vmaddr_slide(0);
    return info;
}

MemoryFileInfo getMemoryFileInfo(const std::string& fileName) {
    MemoryFileInfo info;
    const uint32_t imageCount = _dyld_image_count();
    for (uint32_t i = 0; i < imageCount; i++) {
        const char *name = _dyld_get_image_name(i);
        if (!name)
            continue;
        std::string fullpath(name);
        if (fullpath.length() < fileName.length() || fullpath.compare(fullpath.length() - fileName.length(), fileName.length(), fileName) != 0)
            continue;
        info.index = i;
        info.header = _dyld_get_image_header(i);
        info.name = _dyld_get_image_name(i);
        info.address = _dyld_get_image_vmaddr_slide(i);
        break;
    }
    return info;
}

uintptr_t getAbsoluteAddress(const char *fileName, uintptr_t address) {
    MemoryFileInfo info;
    if (fileName)
        info = getMemoryFileInfo(fileName);
    else
        info = getBaseInfo();
    if (info.address == 0)
        return 0;
    return info.address + address;
}

//Well, at here I use vm_unity for the game that contains "UnityFramework.framework/UnityFramework" file, you can change it if needed, for example: LoL WildRift, FEProj is the correct binary for you.
bool vm_unity(long long offset, unsigned int data) {
    //Change binary name here if it not UnityFramework
    const char *fileName = "UnityFramework";
    uintptr_t address = getAbsoluteAddress(fileName, offset);
    if (address == 0)
        return false;

    kern_return_t err;
    mach_port_t port = mach_task_self();

    err = vm_protect(port, (mach_vm_address_t)address, sizeof(data), false, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
    if (err != KERN_SUCCESS) {
        return false;
    }

    if (getType(data)) {
        data = CFSwapInt32(data);
        err = vm_write(port, (mach_vm_address_t)address, (vm_offset_t)&data, sizeof(data));
    } else {
        data = (unsigned short)data;
        data = CFSwapInt16(data);
        err = vm_write(port, (mach_vm_address_t)address, (vm_offset_t)&data, sizeof(data));
    }

    if (err != KERN_SUCCESS) {
        return false;
    }

    err = vm_protect(port, (mach_vm_address_t)address, sizeof(data), false, VM_PROT_READ | VM_PROT_EXECUTE);
    if (err != KERN_SUCCESS) {
        return false;
    }

    return true;
}
