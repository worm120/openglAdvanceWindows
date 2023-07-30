#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
typedef void* XVulkanHandle;
struct XBufferObject {
	VkBuffer mBuffer;
	VkDeviceMemory mMemory;
	XBufferObject();
	~XBufferObject();
};
void xglBufferData(XVulkanHandle vbo, int size, void *data);
VkResult xGenBuffer(VkBuffer&buffer, VkDeviceMemory&buffermemory, VkDeviceSize size, VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties);
#define xGenVertexBuffer(size,buffer,buffermemory) \
	xGenBuffer(buffer,buffermemory,size,VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)