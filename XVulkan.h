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
struct XMatrix4x4f {
	float mData[16];
};
struct XVector4f {
	float mData[4];
};
enum XUniformBufferType {
	kXUniformBufferTypeMatrix,
	kXUniformBufferTypeVector,
	kXUniformBufferTypeCount
};
struct XUniformBuffer {
	VkBuffer mBuffer;
	VkDeviceMemory mMemory;
	XUniformBufferType mType;
	std::vector<XMatrix4x4f> mMatrices;
	std::vector<XVector4f> mVector4s;
	XUniformBuffer();
	~XUniformBuffer();
};
class GraphicPipeline;
struct XProgram {
	VkPipelineShaderStageCreateInfo mShaderStage[2];
	int mShaderStagetCount;
	VkShaderModule mVertexShader, mFragmentShader;
	VkDescriptorSetLayout mDescriptorSetLayout;
	std::vector<VkDescriptorSetLayoutBinding> mDescriptorSetLayoutBindings;
	std::vector<VkDescriptorPoolSize> mDescriptorPoolSize;
	std::vector<VkWriteDescriptorSet> mWriteDescriptorSet;
	VkDescriptorSet mDescriptorSet;
	VkDescriptorPool mDescriptorPool;
	XUniformBuffer mVertexShaderMatrixUniformBuffer;
	XUniformBuffer mFragmentShaderMatrixUniformBuffer;
	XUniformBuffer mVertexShaderVectorUniformBuffer;
	XUniformBuffer mFragmentShaderVectorUniformBuffer;
	GraphicPipeline mFixedPipeline;
	XProgram();
	~XProgram();
};
void xglBufferData(XVulkanHandle vbo, int size, void *data);
VkResult xGenBuffer(VkBuffer&buffer, VkDeviceMemory&buffermemory, VkDeviceSize size, VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties);
void xBufferSubData(VkBuffer buffer, VkBufferUsageFlags usage, const void * data, VkDeviceSize size);
#define xGenVertexBuffer(size,buffer,buffermemory) \
	xGenBuffer(buffer,buffermemory,size,VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
#define xBufferSubVertexData(buffer,data,size) \
	xBufferSubData(buffer,VK_BUFFER_USAGE_TRANSFER_SRC_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,data,size);
uint32_t xGetMemoryType(uint32_t type_filters, VkMemoryPropertyFlags properties);
void xBeginOneTimeCommandBuffer(VkCommandBuffer*commandbuffer);
void xEndOneTimeCommandBuffer(VkCommandBuffer commandbuffer);
void xGenCommandBuffer(VkCommandBuffer*commandbuffer, int count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
void xWaitForCommandFinish(VkCommandBuffer commandbuffer);
