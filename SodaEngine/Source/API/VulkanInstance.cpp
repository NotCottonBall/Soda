#include <SodaEnginePCH.h>

#include <SDL3/SDL_vulkan.h>
#include <algorithm>
#include <cstring>
#include <volk.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "API/VulkanInstance.h"
#include "Logging/EngineLogger.h"
#include "Logging/SDEAssert.h"
#include "SodaEngine/Version.h"

namespace SodaEngine
{
// Declarations
VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *callbackData, void *userData);

// Mechs
constexpr std::string VulkanMessageTypeToString(
    VkDebugUtilsMessageTypeFlagsEXT messageType)
{
  switch(messageType)
  {
  case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
    return "[General]";
  case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
    return "[Performance]";
  case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
    return "[Validation]";
  default:
    return "[]";
  }

  return "[]";
}

// Class Functions
VulkanInstance::VulkanInstance()
{
  SDE_ASSERT_CRITICAL(Initialize(), "Failed To Create Vulkan Instance!");
}

VulkanInstance::~VulkanInstance()
{
  if(m_Instance)
    vkDestroyInstance(m_Instance, nullptr);
  volkFinalize();
}

bool VulkanInstance::Initialize()
{
  if(volkInitialize() != VK_SUCCESS)
  {
    SDE_LOG_CRITICAL("Failed To Initialize Volk");
    return false;
  }

  VkApplicationInfo appInfo{
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = "Soda Engine App",
      .applicationVersion = VK_MAKE_API_VERSION(0, SODA_ENGINE_VERSION_MAJOR,
                                                SODA_ENGINE_VERSION_MINOR,
                                                SODA_ENGINE_VERSION_PATCH),
      .pEngineName = "Soda Engine",
      .engineVersion = VK_MAKE_API_VERSION(0, SODA_ENGINE_VERSION_MAJOR,
                                           SODA_ENGINE_VERSION_MINOR,
                                           SODA_ENGINE_VERSION_PATCH),
      .apiVersion = VK_API_VERSION_1_4,
  };

  // Gettings Required Extensions
  uint32_t sdlExtensionCount = 0;
  const char *const *sdlExtensions =
      SDL_Vulkan_GetInstanceExtensions(&sdlExtensionCount);
  std::vector<const char *> requestedExtensions{
  // Extensions that we need will be added here
#ifdef SD_DEBUG
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
  };
  for(int i = 0; i < sdlExtensionCount; i++)
    requestedExtensions.push_back(sdlExtensions[i]);

  // Checking Support For Extensions
  SDE_ASSERT_CRITICAL(CheckExtensionSupport(requestedExtensions),
                      "Failed To Get Requested Extensions");
  SDE_LOG_TRACE("Requested Extensions Are Supported And Are Added");

  // Vulkan Layers That We Would Need
  std::vector<const char *> requestedLayers{
#ifdef SD_DEBUG
      "VK_LAYER_KHRONOS_validation"
#endif
  };

  // Checking Support For Layers
  SDE_ASSERT_CRITICAL(CheckLayerSupport(requestedLayers),
                      "Failed To Get Requested Layers");
  SDE_LOG_TRACE("Requested Layers Are Supported And Are Added");

  VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
      .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
      .pfnUserCallback = vkDebugMessengerCallback,
  };

  VkInstanceCreateInfo instanceCreateInfo{
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pNext = &debugMessengerCreateInfo,
      .pApplicationInfo = &appInfo,
      .enabledLayerCount = static_cast<uint32_t>(requestedLayers.size()),
      .ppEnabledLayerNames = requestedLayers.data(),
      .enabledExtensionCount =
          static_cast<uint32_t>(requestedExtensions.size()),
      .ppEnabledExtensionNames = requestedExtensions.data(),
  };

  if(vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) != VK_SUCCESS)
    return false;

  volkInitialize();

  SDE_LOG_INFO("Successfully Created Vulkan Instance");
  return true;
}

// Mechanics
bool VulkanInstance::CheckExtensionSupport(
    std::vector<const char *> &extensionsRequested)
{
  uint32_t extensionsCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
  std::vector<VkExtensionProperties> supportedExtensions(extensionsCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount,
                                         supportedExtensions.data());
  if(supportedExtensions.empty())
  {
    SDE_LOG_CRITICAL(
        "vkEnumerateInstanceExtensionProperties Returned 0 Available "
        "Extensions.");
    return false;
  }

  SDE_LOG_TRACE("Available Extensions: ");
  for(const VkExtensionProperties &ext : supportedExtensions)
    SDE_LOG_TRACE("\t{}", ext.extensionName);

  SDE_LOG_TRACE("Requested Extensions: ");
  for(const char *name : extensionsRequested)
    SDE_LOG_TRACE("\t{}", name);

  SDE_LOG_TRACE(
      "Checking Requested Extensions With The Available Extensions...");

  std::vector<const char *> missingExt;
  for(const char *req : extensionsRequested)
  {
    if(!(std::any_of(supportedExtensions.begin(), supportedExtensions.end(),
                     [&](const VkExtensionProperties &ext) {
                       return std::strcmp(req, ext.extensionName) == 0;
                     })))
    {
      missingExt.push_back(req);
    }
  }

  if(!missingExt.empty())
  {
    SDE_LOG_CRITICAL("Below Requested Layers Are Not Supported Or Installed: ");
    for(const char *c : missingExt)
      SDE_LOG_CRITICAL("\t{}", c);
    return false;
  }

  return true;
}

bool VulkanInstance::CheckLayerSupport(
    std::vector<const char *> &layersRequested)
{
  uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  std::vector<VkLayerProperties> supportedLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data());
  if(supportedLayers.empty())
  {
    SDE_LOG_CRITICAL(
        "vkEnumerateInstanceLayerProperties Returned 0 Available Layers.");
    return false;
  }

  SDE_LOG_TRACE("Available Layers: ");
  for(const VkLayerProperties &layer : supportedLayers)
    SDE_LOG_TRACE("\t{}", layer.layerName);

  SDE_LOG_TRACE("Requested Layers: ");
  for(const char *lName : layersRequested)
    SDE_LOG_TRACE("\t{}", lName);

  SDE_LOG_TRACE("Checking Requested Layers With The Available Layers...");

  std::vector<const char *> missingLayers;
  for(const char *req : layersRequested)
  {
    if(!(std::any_of(supportedLayers.begin(), supportedLayers.end(),
                     [&](const VkLayerProperties &layer) {
                       return std::strcmp(req, layer.layerName) == 0;
                     })))
    {
      missingLayers.push_back(req);
    }
  }

  if(!missingLayers.empty())
  {
    SDE_LOG_CRITICAL("Below Requested Layers Are Not Supported Or Installed: ");
    for(const char *c : missingLayers)
      SDE_LOG_CRITICAL("\t{}", c);
    return false;
  }

  return true;
}

// Callbacks
VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *callbackData, void *userData)
{
  std::string messagePrefix =
      "[Vulkan] " + VulkanMessageTypeToString(messageType);

  switch(messageSeverity)
  {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    SDE_LOG_TRACE("{} {}", messagePrefix, callbackData->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    SDE_LOG_INFO("{} {}", messagePrefix, callbackData->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    SDE_LOG_WARNING("{} {}", messagePrefix, callbackData->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    SDE_LOG_ERROR("{} {}", messagePrefix, callbackData->pMessage);
    break;
  default:
    SDE_LOG_DEBUG("{} {}", messagePrefix, callbackData->pMessage);
    break;
  }
  return VK_SUCCESS;
}
} // namespace SodaEngine
