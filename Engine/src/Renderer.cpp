#include <Tusk/Renderer.h>

#include <SDL3/SDL.h>
#include <string>
#include <filesystem>
#include <array>

#include <iostream>

//https://gpuforbeginners.com/

namespace Tusk
{
	struct Renderer::Impl
	{
		struct DeviceDeleter
		{
			void operator()(SDL_GPUDevice* device) { SDL_DestroyGPUDevice(device); }
		};

		//https://gpuforbeginners.com/chapter03/
		SDL_GPUShader* LoadShader(const std::string& shaderFilename)
		{
			if(!this->device.get())
			{
				return nullptr;
			}

			//Find the stage of the shader.
			SDL_GPUShaderStage stage;
			if (shaderFilename.ends_with(".vert"))
			{
				stage = SDL_GPU_SHADERSTAGE_VERTEX;
			}
			else if (shaderFilename.ends_with(".frag"))
			{
				stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
			}
			else
			{
				std::fprintf(stderr, "Couldn't deduce shader stage from file name: %s\n", shaderFilename.c_str());
				return nullptr;
			}

			std::filesystem::path fullPath = std::filesystem::path(SDL_GetBasePath()) / "Shaders";
			//std::cout << fullPath << std::endl;
			SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
			const char* entrypoint;

			SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(this->device.get()); //Find what platform we are using.
			//Define the format based on platform
			if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
			{
				fullPath /= shaderFilename + ".spv";
				format = SDL_GPU_SHADERFORMAT_SPIRV;
				entrypoint = "main";
			}
			else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
			{
				fullPath /= shaderFilename + ".msl";
				format = SDL_GPU_SHADERFORMAT_MSL;
				entrypoint = "main0";
			}
			else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
			{
				fullPath /= shaderFilename + ".dxil";
				format = SDL_GPU_SHADERFORMAT_DXIL;
				entrypoint = "main";
			}
			else
			{
				std::fprintf(stderr, "Couldn't find a supported shader format for backend %s\n", SDL_GetGPUDeviceDriver(this->device.get()));
				return nullptr;
			}

			//Load Shaderfile from Disk.
			size_t fileSize;
			void* code = SDL_LoadFile(fullPath.string().c_str(), &fileSize);
			if (code == nullptr)
			{
				std::fprintf(stderr, "Couldn't load shader file from disk %s\n", SDL_GetError());
				return nullptr;
			}

			//Bundle collected shader info
			SDL_GPUShaderCreateInfo shaderInfo = SDL_GPUShaderCreateInfo{
				.code_size = fileSize,
				.code = static_cast<Uint8*>(code),
				.entrypoint = entrypoint,
				.format = format,
				.stage = stage,
			};

			//Create SDL GPU Shader.
			SDL_GPUShader* shader = SDL_CreateGPUShader(this->device.get(), &shaderInfo);
			SDL_free(code);
			if (shader == nullptr)
			{
				//SDL_Log("Couldn't create shader from file %s: %s", fullPath.c_str(), SDL_GetError());
				return nullptr;
			}
	
			return shader;
		}
		
		std::unique_ptr<SDL_GPUDevice, DeviceDeleter> device;
		SDL_Window* window = nullptr;
		SDL_GPUCommandBuffer* commandBuffer = nullptr;
		SDL_GPUTexture* swapchainTexture;
		SDL_GPUGraphicsPipeline* pipeline = nullptr;

		~Impl() { Shutdown(); }

		void Shutdown()
		{
			SDL_ReleaseWindowFromGPUDevice(device.get(), window);
			device.reset();
		}
	};

	Renderer::Renderer() : impl(std::make_unique<Impl>()) {}

	Renderer::~Renderer()
	{
	}

	bool Renderer::Initilise(SDL_Window* window, bool debugActive)
	{
		impl->window = window; //This may be temp until I find a better solution.

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

		//Test
		SDL_GPUShader* shader = impl->LoadShader("Default.frag");

		return true;
	}

	void Renderer::Clear(float red, float green, float blue)
	{
		impl->commandBuffer = SDL_AcquireGPUCommandBuffer(impl->device.get()); //Try aquire the Command Buffer. A new command buffer must be retrieved each frame.
		if (!impl->commandBuffer)
		{
			std::fprintf(stderr, "Couldn't Aqquire Command Buffer: %s\n", SDL_GetError());
			//impl->Shutdown();
		}

		if (!SDL_WaitAndAcquireGPUSwapchainTexture(impl->commandBuffer, impl->window, &impl->swapchainTexture, nullptr, nullptr)) //Try aquire the Swapchain Texture. A new swapchain texture must be retrieved each frame.
		{
			std::fprintf(stderr, "Couldn't acquire swapchain texture: %s\n", SDL_GetError());
			//impl->Shutdown();
		}

		SDL_GPUColorTargetInfo colorTargetInfo = {
			.texture = impl->swapchainTexture,
			.clear_color = { red, green, blue, 1.0f },
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_STORE,
		};

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(impl->commandBuffer, &colorTargetInfo, 1, nullptr); //Begin Render Pass.
		SDL_EndGPURenderPass(renderPass); //End Render Pass.

		SDL_SubmitGPUCommandBuffer(impl->commandBuffer); //Submit to GPU.
		impl->commandBuffer = nullptr;
		impl->swapchainTexture = nullptr;
	}

	//https://gpuforbeginners.com/chapter03/
	bool Renderer::CreatePipeline()
	{
		//Load Shaders
		SDL_GPUShader* vertexShader = impl->LoadShader("Default.vert");
		if (vertexShader == nullptr)
		{
			SDL_Log("Couldn't create vertex shader!");
			return false;
		}

		SDL_GPUShader* fragmentShader = impl->LoadShader("Default.frag");
		if (fragmentShader == nullptr)
		{
			SDL_Log("Couldn't create fragment shader!");
			return false;
		}

		//Description of the vertex buffer layout.
		std::array vertexBufferDescriptions{
			SDL_GPUVertexBufferDescription{
				.slot = 0,
				.pitch = sizeof(Vertex), //Stride of one vertex
				.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
				.instance_step_rate = 0,
			},
		};

		//Detail of each value inside the vertex buffer.
		std::array vertexAttributes{
			SDL_GPUVertexAttribute{
				.location = 0, //First data point
				.buffer_slot = 0,
				.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT,
				.offset = 0 * sizeof(float),
			},
			SDL_GPUVertexAttribute{
				.location = 1, 
				.buffer_slot = 0,
				.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT,
				.offset = 1 * sizeof(float), //Describes the offset to get to this value
			},
			SDL_GPUVertexAttribute{
				.location = 2,
				.buffer_slot = 0,
				.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT,
				.offset = 2 * sizeof(float),
			},
		};

		std::array colorTargetDescriptions{
			SDL_GPUColorTargetDescription{
				.format = SDL_GetGPUSwapchainTextureFormat(impl->device.get(), impl->window)
			}
		};

		SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = SDL_GPUGraphicsPipelineCreateInfo{
			.vertex_shader = vertexShader,
			.fragment_shader = fragmentShader,
			.vertex_input_state = SDL_GPUVertexInputState{
				.vertex_buffer_descriptions = vertexBufferDescriptions.data(),
				.num_vertex_buffers = vertexBufferDescriptions.size(),
				.vertex_attributes = vertexAttributes.data(),
				.num_vertex_attributes = vertexAttributes.size(),
			},
			.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, //Drawing with triangle list
			.rasterizer_state = SDL_GPURasterizerState{
				.fill_mode = SDL_GPU_FILLMODE_FILL,
			},
			.target_info = SDL_GPUGraphicsPipelineTargetInfo{ //Draw to our swapchain.
				.color_target_descriptions = colorTargetDescriptions.data(),
				.num_color_targets = colorTargetDescriptions.size(),
			},
		};

		impl->pipeline = SDL_CreateGPUGraphicsPipeline(impl->device.get(), &pipelineCreateInfo);
		if (impl->pipeline == nullptr)
		{
			SDL_Log("Couldn't create graphics pipeline! %s", SDL_GetError());
			return false;
		}

		SDL_ReleaseGPUShader(impl->device.get(), vertexShader);
		SDL_ReleaseGPUShader(impl->device.get(), fragmentShader);

		return true;
	}
}