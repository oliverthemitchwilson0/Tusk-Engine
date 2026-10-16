#include <Tusk/Renderer.h>

#include <SDL3/SDL.h>
#include <string>
#include <filesystem>
#include <array>

//https://gpuforbeginners.com/

namespace Tusk
{
	struct MeshBufferData
	{
		SDL_GPUBuffer* vertexBuffer = nullptr;
		SDL_GPUBuffer* indexBuffer = nullptr;
		Uint16 indexCount = 0;
	};

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
				.num_uniform_buffers = 1,
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
		SDL_GPUGraphicsPipeline* pipeline = nullptr;
		SDL_GPUTexture* swapchainTexture = nullptr;
		SDL_GPUCommandBuffer* commandBuffer = nullptr;
		SDL_GPURenderPass* renderPass = nullptr;

		std::vector<MeshBufferData> meshBuffers;

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
		if(!CreatePipeline())
		{
			impl->Shutdown();
			return false;
		}

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

		impl->renderPass = SDL_BeginGPURenderPass(impl->commandBuffer, &colorTargetInfo, 1, nullptr); //Begin Render Pass.

		SDL_BindGPUGraphicsPipeline(impl->renderPass, impl->pipeline);
	}

	void Renderer::Draw(SDL_GPUBuffer* vertexBuffer, SDL_GPUBuffer* indexBuffer, int indexCount, MatrixBuffer matBuff)
	{

		std::array vertexBuffers{
			SDL_GPUBufferBinding{
				.buffer = vertexBuffer,
				.offset = 0,
			},
		};

		std::array indexBuffers{
			SDL_GPUBufferBinding
			{
				.buffer = indexBuffer,
				.offset = 0,
			},
		};

		SDL_BindGPUVertexBuffers(impl->renderPass, 0, vertexBuffers.data(), vertexBuffers.size());
		SDL_BindGPUIndexBuffer(impl->renderPass, indexBuffers.data(), SDL_GPU_INDEXELEMENTSIZE_16BIT);

		SDL_PushGPUVertexUniformData(impl->commandBuffer, 0, &matBuff, sizeof(MatrixBuffer));

		//SDL_DrawGPUPrimitives(impl->renderPass, vertexCount, 1, 0, 0);
		SDL_DrawGPUIndexedPrimitives(impl->renderPass, indexCount, 1, 0, 0, 0);
	}

	void Renderer::Present()
	{
		SDL_EndGPURenderPass(impl->renderPass); //End Render Pass.

		SDL_SubmitGPUCommandBuffer(impl->commandBuffer); //Submit to GPU.
		impl->commandBuffer = nullptr;
		impl->swapchainTexture = nullptr;
		impl->renderPass = nullptr;
	}

	SDL_GPUBuffer* CreateVertexBuffer(SDL_GPUDevice* device, uint32_t verticesSize)
	{
		//Define the usage of the new buffer
		SDL_GPUBufferCreateInfo vertexBufferCreateInfo = SDL_GPUBufferCreateInfo{
			.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
			.size = verticesSize,
		};

		//Create buffer with defined ussage.
		SDL_GPUBuffer* newVertexBuffer = SDL_CreateGPUBuffer(device, &vertexBufferCreateInfo);
		if (!newVertexBuffer)
		{
			SDL_Log("Couldn't create vertex buffer: %s", SDL_GetError());
			return nullptr;
		}

		return newVertexBuffer;
	}

	SDL_GPUBuffer* CreateIndexBuffer(SDL_GPUDevice* device, uint32_t indicesSize)
	{
		//Define the usage of the new buffer
		SDL_GPUBufferCreateInfo indexBufferCreateInfo{
			.usage = SDL_GPU_BUFFERUSAGE_INDEX,
			.size = indicesSize,
		};

		//Create buffer with defined ussage.
		SDL_GPUBuffer* newIndexBuffer = SDL_CreateGPUBuffer(device, &indexBufferCreateInfo);
		if (!newIndexBuffer)
		{
			SDL_Log("Couldn't create index buffer: %s", SDL_GetError());
			SDL_ReleaseGPUBuffer(device, newIndexBuffer);
			return nullptr;
		}

		return newIndexBuffer;
	}

	SDL_GPUTransferBuffer* CreateTransferBuffer(SDL_GPUDevice* device, uint32_t bufferSize)
	{
		//Transfer buffer - Temporary buffer used for uploading data.
		SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo = SDL_GPUTransferBufferCreateInfo{
			.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
			.size = bufferSize,
		};

		SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferBufferCreateInfo);
		if (transferBuffer == nullptr)
		{
			SDL_Log("Couldn't create transfer buffer: %s", SDL_GetError());
			return nullptr;
		}

		return transferBuffer;
	}

	void Renderer::CreateMeshBuffers(std::span<Vertex> vertices, std::span<uint16_t> indices)
	{
		if (vertices.empty() || indices.empty())
			return;

		//Create Vertex Buffer.
		uint32_t verticesSize = vertices.size() * static_cast<uint32_t>(sizeof(Vertex));
		auto* vertexBuffer = CreateVertexBuffer(impl->device.get(), verticesSize);
		if (!vertexBuffer)
			return;

		//Create Index Buffer.
		uint32_t indicesSize = indices.size() * static_cast<uint32_t>(sizeof(uint16_t));
		auto* indexBuffer = CreateIndexBuffer(impl->device.get(), indicesSize);
		if (!indexBuffer)
		{
			SDL_ReleaseGPUBuffer(impl->device.get(), vertexBuffer);
			return;
		}

		//Create Transfer Buffers (2).
		auto* verTransferBuffer = CreateTransferBuffer(impl->device.get(), verticesSize);
		auto* indTransferBuffer = CreateTransferBuffer(impl->device.get(), indicesSize);
		if(!verTransferBuffer)
		{
			SDL_ReleaseGPUBuffer(impl->device.get(), vertexBuffer);
			SDL_ReleaseGPUBuffer(impl->device.get(), indexBuffer);
			return;
		}
		
		//Map both transfer buffers.
		//Transfer buffer mapping - allowing the cpu to treat the transfer buffer as a sequence of vertices or indices.
		Vertex* verTransferData = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(impl->device.get(), verTransferBuffer, false));
		uint16_t* indTransferData = static_cast<uint16_t*>(SDL_MapGPUTransferBuffer(impl->device.get(), indTransferBuffer, false));
		if (!verTransferData || !indTransferData)
		{
			SDL_Log("Couldn't map transfer buffer: %s", SDL_GetError());
			SDL_ReleaseGPUBuffer(impl->device.get(), vertexBuffer);
			SDL_ReleaseGPUBuffer(impl->device.get(), indexBuffer);
			SDL_ReleaseGPUTransferBuffer(impl->device.get(), verTransferBuffer);
			SDL_ReleaseGPUTransferBuffer(impl->device.get(), indTransferBuffer);
			return;
		}

		//Copy CPU data to transfer buffers.
		SDL_memcpy(verTransferData, vertices.data(), verticesSize); //Copy CPU data to the transfer buffer.
		SDL_memcpy(indTransferData, indices.data(), indicesSize); //Copy CPU data to the transfer buffer.

		SDL_UnmapGPUTransferBuffer(impl->device.get(), verTransferBuffer); //Unmap transfer buffer.
		SDL_UnmapGPUTransferBuffer(impl->device.get(), indTransferBuffer); //Unmap transfer buffer.

		//Aquire command buffer
		SDL_GPUCommandBuffer* uploadCommandBuffer = SDL_AcquireGPUCommandBuffer(impl->device.get());
		if(!uploadCommandBuffer)
		{
			/*SDL_Log("Couldn't acquire GPU command buffer: %s", SDL_GetError());
			SDL_ReleaseGPUTransferBuffer(impl->device.get(), transferBuffer);
			SDL_ReleaseGPUBuffer(impl->device.get(), vertexBuffer);
			return nullptr;*/
		}

		SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCommandBuffer);

		//Location of Vertex Data.
		SDL_GPUTransferBufferLocation bufferLocation = SDL_GPUTransferBufferLocation{
			.transfer_buffer = verTransferBuffer,
			.offset = 0,
		};

		//Destination of Vertex Data.
		SDL_GPUBufferRegion bufferRegion = SDL_GPUBufferRegion{
			.buffer = vertexBuffer,
			.offset = 0,
			.size = verticesSize,
		}; 
		
		//Bind transfer buffers to copy pass.
		//End copy pass.

		//Record copy opperation into Copy Pass.
		SDL_UploadToGPUBuffer(copyPass, &bufferLocation, &bufferRegion, false);
		
		//Remap to location of index buffer and destination of index buffer.
		bufferLocation.transfer_buffer = indTransferBuffer;
		bufferRegion.buffer = indexBuffer;
		bufferRegion.size = indicesSize;

		SDL_EndGPUCopyPass(copyPass);

		if (!SDL_SubmitGPUCommandBuffer(uploadCommandBuffer))
		{
			SDL_Log("Couldn't submit GPU command buffer: %s", SDL_GetError());
			return;
		}
		
		SDL_ReleaseGPUTransferBuffer(impl->device.get(), verTransferBuffer);
		SDL_ReleaseGPUTransferBuffer(impl->device.get(), indTransferBuffer);
		
		//return vertexBuffer;

		//Submit to GPU.
	}

	bool Renderer::CreatePipeline()
	{
		//Load Shaders to CPU
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

		//Define information for the Input Assembler/

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
				.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
				.offset = 0 * sizeof(Math::vec3),
			},
			SDL_GPUVertexAttribute{
				.location = 1, 
				.buffer_slot = 0,
				.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
				.offset = 1 * sizeof(Math::vec3),
			},
		};

		//Define the swapcahin texture format.
		std::array colorTargetDescriptions{
			SDL_GPUColorTargetDescription{
				.format = SDL_GetGPUSwapchainTextureFormat(impl->device.get(), impl->window)
			}
		};

		//Create full pipeline info.
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
				.cull_mode = SDL_GPU_CULLMODE_BACK,
				.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
			},
			.target_info = SDL_GPUGraphicsPipelineTargetInfo{ //Draw to our swapchain.
				.color_target_descriptions = colorTargetDescriptions.data(),
				.num_color_targets = colorTargetDescriptions.size(),
			},
		};

		//Submit and create pipeline.
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