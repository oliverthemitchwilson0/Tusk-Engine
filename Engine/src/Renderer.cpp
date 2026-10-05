#include <Tusk/Renderer.h>

#include <SDL3/SDL.h>

//https://gpuforbeginners.com/

namespace Tusk
{
	struct Renderer::Impl
	{
		struct DeviceDeleter
		{
			void operator()(SDL_GPUDevice* device) { SDL_DestroyGPUDevice(device); }
		};
		
		std::unique_ptr<SDL_GPUDevice, DeviceDeleter> device;
		SDL_GPUCommandBuffer* commandBuffer = nullptr;
		SDL_GPUTexture* swapchainTexture;

		~Impl() { Shutdown(); }

		void Shutdown()
		{
			device.reset();
		}
	};

	Renderer::Renderer() : impl(std::make_unique<Impl>()) {}

	Renderer::~Renderer()
	{
	}

	bool Renderer::Initilise(SDL_Window* window, bool debugActive)
	{
		SDL_GPUShaderFormat formatFlags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;
		impl->device.reset(SDL_CreateGPUDevice(formatFlags, debugActive, nullptr)); //Try create a GPU device
		if(!impl->device)
		{
			std::fprintf(stderr, "SDL_CreateGPUDevice: %s\n", SDL_GetError());
			impl->Shutdown();
			return false;
		}

		if(!SDL_ClaimWindowForGPUDevice(impl->device.get(), window)) //Bind the Window to our GPU Device.
		{
			std::fprintf(stderr, "SDL_ClaimWindowForGPUDevice: %s\n", SDL_GetError());
			impl->Shutdown();
			return false;
		}

		impl->commandBuffer = SDL_AcquireGPUCommandBuffer(impl->device.get()); //Try aquire the Command Buffer.
		if(!impl->commandBuffer)
		{
			std::fprintf(stderr, "Couldn't Aqquire Command Buffer: %s\n", SDL_GetError());
			impl->Shutdown();
			return false;
		}

		if (!SDL_WaitAndAcquireGPUSwapchainTexture(impl->commandBuffer, window, &impl->swapchainTexture, nullptr, nullptr)) //Try aquire the Swapchain Texture.
		{
			std::fprintf(stderr, "Couldn't acquire swapchain texture: %s\n", SDL_GetError());
			impl->Shutdown();
			return false;
		}

		//TEMP

		/*SDL_GPUColorTargetInfo colorTargetInfo;
		colorTargetInfo.texture = impl->swapchainTexture;
		colorTargetInfo.clear_color = { 0.4f, 0.6f, 0.9f, 1.0f };
		colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(impl->commandBuffer, &colorTargetInfo, 0, nullptr);
		SDL_EndGPURenderPass(renderPass);

		SDL_SubmitGPUCommandBuffer(impl->commandBuffer);*/

		return true;
	}
}