#include "XVulkan.h"
#include "BVulkan.h"
XBufferObject::XBufferObject() {
	mBuffer = 0;
	mMemory = 0;
}
XBufferObject::~XBufferObject() {
	if (mBuffer!=0){
		vkDestroyBuffer(GetVulkanDevice(), mBuffer, nullptr);
	}
	if (mMemory!=0){
		vkFreeMemory(GetVulkanDevice(), mMemory, nullptr);
	}
}
void xglBufferData(XVulkanHandle buffer, int size, void *data) {
	XBufferObject*vbo = (XBufferObject*)buffer;
	xGenVertexBuffer(size, vbo->mBuffer, vbo->mMemory);
	xBufferSubVertexData(vbo->mBuffer, data, size);
}
VkResult xGenBuffer(VkBuffer&buffer, VkDeviceMemory&buffermemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
	VkBufferCreateInfo bufferinfo = {};
	bufferinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferinfo.size = size;
	bufferinfo.usage = usage;
	VkResult ret = vkCreateBuffer(GetVulkanDevice(), &bufferinfo, nullptr, &buffer);
	if (ret!=VK_SUCCESS){
		printf("failed to create buffer\n");
		return ret;
	}
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(GetVulkanDevice(), buffer, &requirements);
	VkMemoryAllocateInfo memoryallocinfo = {};
	memoryallocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryallocinfo.allocationSize = requirements.size;
	memoryallocinfo.memoryTypeIndex = xGetMemoryType(requirements.memoryTypeBits, properties);
	ret = vkAllocateMemory(GetVulkanDevice(), &memoryallocinfo, nullptr, &buffermemory);
	if (ret!=VK_SUCCESS){
		printf("failed to alloc memory\n");
		return ret;
	}
	vkBindBufferMemory(GetVulkanDevice(), buffer, buffermemory, 0);
	return VK_SUCCESS;
}
void xBufferSubData(VkBuffer buffer, VkBufferUsageFlags usage, const void * data, VkDeviceSize size) {
	VkBuffer tempbuffer;
	VkDeviceMemory tempmemory;
	xGenBuffer(tempbuffer, tempmemory, size, usage,VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	void*host_memory;
	vkMapMemory(GetVulkanDevice(), tempmemory, 0, size, 0, &host_memory);
	memcpy(host_memory, data, (size_t)size);
	vkUnmapMemory(GetVulkanDevice(), tempmemory);

	VkCommandBuffer commandbuffer;
	xBeginOneTimeCommandBuffer(&commandbuffer);
	VkBufferCopy copy = {0,0,size};
	vkCmdCopyBuffer(commandbuffer, tempbuffer, buffer, 1, &copy);
	xEndOneTimeCommandBuffer(commandbuffer);

	vkDestroyBuffer(GetVulkanDevice(), tempbuffer, nullptr);
	vkFreeMemory(GetVulkanDevice(), tempmemory, nullptr);
}
uint32_t xGetMemoryType(uint32_t type_filters, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(GetVulkanPhysicalDevice(), &memory_properties);
	for (uint32_t i=0;i<memory_properties.memoryTypeCount;++i){
		uint32_t flag = 1 << i;
		if ((flag&type_filters)&&(memory_properties.memoryTypes[i].propertyFlags&properties)==properties){
			return i;
		}
	}
	return 0;
}
void xBeginOneTimeCommandBuffer(VkCommandBuffer*commandbuffer) {
	xGenCommandBuffer(commandbuffer, 1);
	VkCommandBufferBeginInfo cbbi = {};
	cbbi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cbbi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(*commandbuffer, &cbbi);
}
void xEndOneTimeCommandBuffer(VkCommandBuffer commandbuffer) {
	vkEndCommandBuffer(commandbuffer);
	xWaitForCommandFinish(commandbuffer);
	vkFreeCommandBuffers(GetVulkanDevice(), GetCommandPool(), 1, &commandbuffer);
}
void xGenCommandBuffer(VkCommandBuffer*commandbuffer, int count, VkCommandBufferLevel level) {
	VkCommandBufferAllocateInfo cbai = {};
	cbai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbai.level = level;
	cbai.commandPool = GetCommandPool();
	cbai.commandBufferCount = count;
	vkAllocateCommandBuffers(GetVulkanDevice(), &cbai, commandbuffer);
}
void xWaitForCommandFinish(VkCommandBuffer commandbuffer) {
	VkSubmitInfo submitinfo = {};
	submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitinfo.commandBufferCount = 1;
	submitinfo.pCommandBuffers = &commandbuffer;
	VkFenceCreateInfo fci = {};
	fci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	VkFence fence;
	vkCreateFence(GetVulkanDevice(), &fci, nullptr, &fence);
	vkQueueSubmit(GetGraphicQueue(), 1, &submitinfo, fence);
	vkWaitForFences(GetVulkanDevice(), 1, &fence, VK_TRUE,1000000000);
	vkDestroyFence(GetVulkanDevice(), fence, nullptr);
}