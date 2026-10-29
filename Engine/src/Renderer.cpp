#include <Tusk/Renderer.h>
#include <Tusk/Graphics/CameraData.h>
#include <Tusk/AssetTypes/MeshAsset.h>
#include <Tusk/AssetTypes/TextureAsset.h>

#include <SDL3/SDL.h>
#include <string>
#include <filesystem>
#include <array>

#include <Tusk/Logging.h>

//https://gpuforbeginners.com/
//https://github.com/TheSpydog/SDL_gpu_examples/blob/main/Examples/TexturedAnimatedQuad.c
//https://glusoft.com/sdl3-tutorials/display-texture-sdl3_gpu/

namespace Tusk
{
	struct GPUBufferDeleter
	{
		SDL_GPUDevice* device = nullptr;
		void operator()(SDL_GPUBuffer* buffer) { SDL_ReleaseGPUBuffer(device, buffer); }
	};

	struct GPUTransferBufferDeleter
	{
		SDL_GPUDevice* device = nullptr;
		void operator()(SDL_GPUTransferBuffer* buffer) { SDL_ReleaseGPUTransferBuffer(device, buffer); }
	};

	struct GPUShaderDeleter
	{
		SDL_GPUDevice* device = nullptr;
		void operator()(SDL_GPUShader* shader) { SDL_ReleaseGPUShader(device, shader); }
	};

	struct GPUTextureDeleter
	{
		SDL_GPUDevice* device = nullptr;
		void operator()(SDL_GPUTexture* texture) { SDL_ReleaseGPUTexture(device, texture); }
	};

	//Structure containing vertex and index buffer of one mesh.
	struct GPUMeshBuffers
	{
		//SDL_GPUBuffer* vertexBuffer = nullptr;
		std::unique_ptr<SDL_GPUBuffer, GPUBufferDeleter> vertexBuffer;
		std::unique_ptr<SDL_GPUBuffer, GPUBufferDeleter> indexBuffer;
		size_t indexCount = 0;
	};

	//Cpu struct definition of the vertex shader's Camera Buffer
	struct CameraBufferType
	{
		Math::mat4 view;
		Math::mat4 projection;
	};

	//Cpu struct definition of the vertex shader's Transform Buffer
	struct TransformBufferType
	{
		Math::mat4 world;
	};

	struct Renderer::Impl
	{
		struct GPUDeviceDeleter
		{
			void operator()(SDL_GPUDevice* device) { SDL_DestroyGPUDevice(device); }
		};

		struct GPUGraphicsPipelineDeleter
		{
			SDL_GPUDevice* device = nullptr;
			void operator()(SDL_GPUGraphicsPipeline* pipeline) { SDL_ReleaseGPUGraphicsPipeline(device, pipeline); }
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
				Logging::Log("Couldn't deduce shader stage from file name: %s", shaderFilename.c_str());
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
				Logging::Log("Couldn't find a supported shader format for backend %s", SDL_GetGPUDeviceDriver(this->device.get()));
				return nullptr;
			}

			//Load Shaderfile from Disk.
			size_t fileSize;
			void* code = SDL_LoadFile(fullPath.string().c_str(), &fileSize);
			if (code == nullptr)
			{
				Logging::Log("Couldn't load shader file from disk %s", SDL_GetError());
				return nullptr;
			}

			Uint32 uniforBufferCount = 0;
			if(stage == SDL_GPU_SHADERSTAGE_VERTEX)
			{
				uniforBufferCount = 2;
			}

			Uint32 samplerCount = 0;
			if (stage == SDL_GPU_SHADERSTAGE_FRAGMENT)
			{
				samplerCount = 1;
			}



			//Bundle collected shader info
			SDL_GPUShaderCreateInfo shaderInfo = SDL_GPUShaderCreateInfo{
				.code_size = fileSize,
				.code = static_cast<Uint8*>(code),
				.entrypoint = entrypoint,
				.format = format,
				.stage = stage,
				.num_samplers = samplerCount,
				.num_storage_textures = 0,

				.num_storage_buffers = 0,
				.num_uniform_buffers = uniforBufferCount,
			};

			//Create SDL GPU Shader.
			SDL_GPUShader* shader = SDL_CreateGPUShader(this->device.get(), &shaderInfo);
			SDL_free(code);
			if (shader == nullptr)
			{
				Logging::Log("Couldn't create shader from file %s: %s", fullPath.c_str(), SDL_GetError());
				return nullptr;
			}
	
			return shader;
		}
		
		SDL_Window* window = nullptr;
		std::unique_ptr<SDL_GPUDevice, GPUDeviceDeleter> device;
		std::unique_ptr<SDL_GPUGraphicsPipeline, GPUGraphicsPipelineDeleter> pipeline{ nullptr, GPUGraphicsPipelineDeleter{ .device = this->device.get(), } };
		SDL_GPUTexture* swapchainTexture = nullptr;
		SDL_GPUCommandBuffer* commandBuffer = nullptr;
		SDL_GPURenderPass* renderPass = nullptr;

		SDL_GPUTextureFormat depthStencilFormat;
		SDL_GPUSampler* sampler = nullptr;

		CameraData cameraData;

		std::vector<GPUMeshBuffers> meshBuffersBin;
		std::unordered_map<std::string, size_t> meshBuffersLookup;

		std::unique_ptr<SDL_GPUTexture, GPUTextureDeleter> depthTexture{ nullptr, GPUTextureDeleter{ .device = this->device.get() } };
		std::vector<std::unique_ptr<SDL_GPUTexture, GPUTextureDeleter>> textureBin;
		std::unordered_map<std::string, size_t> textureLookup;

		~Impl() { Shutdown(); }

		void Shutdown()
		{
			meshBuffersBin.clear();
			textureBin.clear();
			depthTexture.reset();
			pipeline.reset();
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
			Logging::Log("Failed to create GPU Device: %s", SDL_GetError());
			impl->Shutdown();
			return false;
		}

		if(!SDL_ClaimWindowForGPUDevice(impl->device.get(), window)) //Bind the Window to our GPU Device.
		{
			Logging::Log("Failed to claim window for GPU Device: %s", SDL_GetError());
			impl->Shutdown();
			return false;
		}

		if(!CreatePipeline())
		{
			impl->Shutdown();
			return false;
		}

		CreateDepthTexture();

		CreateTextureSampler();

		return true;
	}

	void Renderer::Clear(float red, float green, float blue)
	{
		impl->commandBuffer = SDL_AcquireGPUCommandBuffer(impl->device.get()); //Try aquire the Command Buffer. A new command buffer must be retrieved each frame.
		if (!impl->commandBuffer)
		{
			Logging::Log("Failed to Aqquire Command Buffer during Clear() %s", SDL_GetError());
			//impl->Shutdown();
		}

		if (!SDL_WaitAndAcquireGPUSwapchainTexture(impl->commandBuffer, impl->window, &impl->swapchainTexture, nullptr, nullptr)) //Try aquire the Swapchain Texture. A new swapchain texture must be retrieved each frame.
		{
			Logging::Log("Failed to Aqquire Swapchain Texture during Clear() %s", SDL_GetError());
			//impl->Shutdown();
		}

		SDL_GPUColorTargetInfo colorTargetInfo = {
			.texture = impl->swapchainTexture,
			.clear_color = { red, green, blue, 1.0f },
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_STORE,
		};

		SDL_GPUDepthStencilTargetInfo depthTargetInfo{
			.texture = impl->depthTexture.get(),
			.clear_depth = 1.f,
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_DONT_CARE,
		};

		auto& cameraData = impl->cameraData;
		//TEMP - Probably shouldn't have direct access to the window within the renderer.
		int winW, winH;
		SDL_GetWindowSize(impl->window, &winW, &winH);

		CameraBufferType camBuff{
			.view = Math::lookAt(cameraData.position, cameraData.position + cameraData.forward, Math::vec3(0.f, 1.f, 0.f)),
			.projection = Math::perspective(Math::radians(cameraData.fovDegrees), (static_cast<float>(winW) / static_cast<float>(winH)), cameraData.nearPlane, cameraData.farPlane),
		};

		PushCameraBuffer(camBuff);

		impl->renderPass = SDL_BeginGPURenderPass(impl->commandBuffer, &colorTargetInfo, 1, &depthTargetInfo); //Begin Render Pass.

		SDL_BindGPUGraphicsPipeline(impl->renderPass, impl->pipeline.get());
	}

	void Renderer::Draw(MeshID mesh, TextureID texture, Math::mat4 transformMat)
	{

		if (mesh.id == -1 || mesh.id >= impl->meshBuffersBin.size())
			return;

		auto& meshBuffers = impl->meshBuffersBin.at(mesh.id); //Get required GPU mesh buffers.

		std::array vertexBuffers{
			SDL_GPUBufferBinding{
				.buffer = meshBuffers.vertexBuffer.get(),
				.offset = 0,
			},
		};

		std::array indexBuffers{
			SDL_GPUBufferBinding
			{
				.buffer = meshBuffers.indexBuffer.get(),
				.offset = 0,
			},
		};

		SDL_GPUTextureSamplerBinding textureBinding{ 
			.texture = impl->textureBin[0].get(), 
			.sampler = impl->sampler
		};

		//if(texture.id != -1)
			SDL_BindGPUFragmentSamplers(impl->renderPass, 0, &textureBinding, 1);


		TransformBufferType matBuff
		{
			.world = transformMat,
		};

		SDL_BindGPUVertexBuffers(impl->renderPass, 0, vertexBuffers.data(), vertexBuffers.size());
		SDL_BindGPUIndexBuffer(impl->renderPass, indexBuffers.data(), SDL_GPU_INDEXELEMENTSIZE_16BIT);

		PushTransformBuffer(matBuff);

		//SDL_DrawGPUPrimitives(impl->renderPass, vertexCount, 1, 0, 0);
		SDL_DrawGPUIndexedPrimitives(impl->renderPass, meshBuffers.indexCount, 1, 0, 0, 0);
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
			Logging::Log("Couldn't create vertex buffer: %s", SDL_GetError());
			SDL_ReleaseGPUBuffer(device, newVertexBuffer);
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
			Logging::Log("Couldn't create index buffer: %s", SDL_GetError());
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
			Logging::Log("Couldn't create transfer buffer: %s", SDL_GetError());
			SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
			return nullptr;
		}

		return transferBuffer;
	}

	MeshID Renderer::CreateGPUMeshBuffers(const MeshAsset& asset)
	{
		//Check if vertices & indices are populated.
		if (asset.vertices.empty() || asset.indices.empty())
			return MeshID();

		//Create Vertex Buffer.
		uint32_t verticesSize = asset.vertices.size() * sizeof(Vertex);
		std::unique_ptr<SDL_GPUBuffer, GPUBufferDeleter> vertexBuffer{ CreateVertexBuffer(impl->device.get(), verticesSize), GPUBufferDeleter{.device = impl->device.get(),} };
		if (!vertexBuffer)
			return MeshID();

		//Create Index Buffer.
		uint32_t indicesSize = asset.indices.size() * sizeof(uint16_t);
		std::unique_ptr<SDL_GPUBuffer, GPUBufferDeleter> indexBuffer{ CreateIndexBuffer(impl->device.get(), indicesSize), GPUBufferDeleter{.device = impl->device.get(),} };
		if (!indexBuffer)
			return MeshID();

		//Create Transfer Buffers (2).
		std::unique_ptr<SDL_GPUTransferBuffer, GPUTransferBufferDeleter> verTransferBuffer{ nullptr, GPUTransferBufferDeleter{.device = impl->device.get(),} };
		std::unique_ptr<SDL_GPUTransferBuffer, GPUTransferBufferDeleter> indTransferBuffer{ nullptr, GPUTransferBufferDeleter{.device = impl->device.get(),} };
		verTransferBuffer.reset(CreateTransferBuffer(impl->device.get(), verticesSize));
		indTransferBuffer.reset(CreateTransferBuffer(impl->device.get(), indicesSize));
		if(!verTransferBuffer || !indTransferBuffer)
			return MeshID();
		
		//Map both transfer buffers.
		//Transfer buffer mapping - allowing the cpu to treat the transfer buffer as a sequence of vertices or indices (or any other selected type).
		Vertex* verTransferData = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(impl->device.get(), verTransferBuffer.get(), false));
		uint16_t* indTransferData = static_cast<uint16_t*>(SDL_MapGPUTransferBuffer(impl->device.get(), indTransferBuffer.get(), false));
		if (!verTransferData || !indTransferData)
		{
			Logging::Log("Failed to map transfer buffer(s): %s", SDL_GetError());
			return MeshID();
		}

		//Copy CPU data to transfer buffers.
		SDL_memcpy(verTransferData, asset.vertices.data(), verticesSize); //Copy CPU data to the transfer buffer.
		SDL_memcpy(indTransferData, asset.indices.data(), indicesSize); //Copy CPU data to the transfer buffer.

		//Unmap transfer buffers.
		SDL_UnmapGPUTransferBuffer(impl->device.get(), verTransferBuffer.get());
		SDL_UnmapGPUTransferBuffer(impl->device.get(), indTransferBuffer.get());

		//Aquire command buffer
		SDL_GPUCommandBuffer* uploadCommandBuffer = SDL_AcquireGPUCommandBuffer(impl->device.get());
		if(!uploadCommandBuffer)
		{
			Logging::Log("Failed to Aqquire Command Buffer during CreateMeshBuffers() %s", SDL_GetError());
			return MeshID();
		}

		//Start of copy pass.
		SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCommandBuffer);

		//Location of Vertex Data.
		SDL_GPUTransferBufferLocation bufferLocation = SDL_GPUTransferBufferLocation{
			.transfer_buffer = verTransferBuffer.get(),
			.offset = 0,
		};

		//Destination of Vertex Data.
		SDL_GPUBufferRegion bufferRegion = SDL_GPUBufferRegion{
			.buffer = vertexBuffer.get(),
			.offset = 0,
			.size = verticesSize,
		}; 

		//Record copy opperation into Copy Pass.
		SDL_UploadToGPUBuffer(copyPass, &bufferLocation, &bufferRegion, false);
		
		//Remap to location of index buffer and destination of index buffer.
		bufferLocation.transfer_buffer = indTransferBuffer.get();
		bufferRegion.buffer = indexBuffer.get();
		bufferRegion.size = indicesSize;

		SDL_UploadToGPUBuffer(copyPass, &bufferLocation, &bufferRegion, false);

		//End of copy pass.
		SDL_EndGPUCopyPass(copyPass);

		//Submit command buffer to GPU.
		if (!SDL_SubmitGPUCommandBuffer(uploadCommandBuffer))
		{
			Logging::Log("Failed to submit GPU command buffer during CreateMeshBuffers(): %s", SDL_GetError());
			return MeshID();
		}
		
		//Push buffers into mesh buffers bin.
		impl->meshBuffersLookup[asset.name] = impl->meshBuffersBin.size();
		impl->meshBuffersBin.push_back(
			GPUMeshBuffers{
				.vertexBuffer{vertexBuffer.release(), GPUBufferDeleter{.device = impl->device.get(),}} ,
				.indexBuffer{indexBuffer.release(), GPUBufferDeleter{.device = impl->device.get()}},
				.indexCount = asset.indices.size(),
			}
		);

		//Return index of data as ID.
		return MeshID{ .id = impl->meshBuffersBin.size() - 1, };
	}

	void Renderer::CreateGPUTexture(const TextureAsset& asset)
	{
		auto* convertedSurface = SDL_ConvertSurface(asset.surface, SDL_PIXELFORMAT_ABGR8888);
		if(!convertedSurface)
		{
			Logging::Log("Failed to Convert Surface.");
			return;
		}

		auto pixelFormat = asset.surface->format;
		auto gpuTextureFormat = SDL_GetGPUTextureFormatFromPixelFormat(convertedSurface->format);

		if(gpuTextureFormat == SDL_GPU_TEXTUREFORMAT_INVALID)
		{
			Logging::Log("Failed to create GPU texture. Pixel format invalid: %s", SDL_GetPixelFormatName(pixelFormat));
			return;
		}

		SDL_GPUTextureCreateInfo textureCreateInfo{
			.type = SDL_GPU_TEXTURETYPE_2D,
			.format = gpuTextureFormat,
			.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
			.width = static_cast<Uint32>(convertedSurface->w),
			.height = static_cast<Uint32>(convertedSurface->h),
			.layer_count_or_depth = 1,
			.num_levels = 1,
		};

		impl->textureLookup[asset.name] = impl->textureBin.size();
		impl->textureBin.push_back({ SDL_CreateGPUTexture(impl->device.get(), &textureCreateInfo), GPUTextureDeleter{ .device = impl->device.get() } });

		//Create Transfer Buffer for texture.
		std::unique_ptr<SDL_GPUTransferBuffer, GPUTransferBufferDeleter> texTransferBuffer{ CreateTransferBuffer(impl->device.get(), asset.surface->w * asset.surface->h * SDL_BYTESPERPIXEL(convertedSurface->format)), GPUTransferBufferDeleter{.device = impl->device.get() } };

		//Map transfer buffer.
		Uint8* textureTransferData = static_cast<Uint8*>(SDL_MapGPUTransferBuffer(impl->device.get(), texTransferBuffer.get(), false));

		SDL_memcpy(textureTransferData, convertedSurface->pixels, (convertedSurface->w * convertedSurface->h * SDL_BYTESPERPIXEL(convertedSurface->format)));
		
		SDL_UnmapGPUTransferBuffer(impl->device.get(), texTransferBuffer.get());

		SDL_DestroySurface(convertedSurface);

		SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(impl->device.get());
		SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

		SDL_GPUTextureTransferInfo location {
			.transfer_buffer = texTransferBuffer.get(),
			.offset = 0
		};
		SDL_GPUTextureRegion region {
			.texture = impl->textureBin[impl->textureBin.size() - 1].get(),
			.w = static_cast<Uint32>(asset.surface->w),
			.h = static_cast<Uint32>(asset.surface->h),
			.d = 1
		};

		SDL_UploadToGPUTexture(
			copyPass,
			&location,
			&region,
			false
		);

		SDL_EndGPUCopyPass(copyPass);
		SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
	}

	MeshID Renderer::GetGPUMesh(const std::string& name)
	{
		auto found = impl->meshBuffersLookup.find(name);
		if(found == impl->meshBuffersLookup.end())
		{
			Logging::Log("Failed to find GPU mesh: %s", name.c_str());
			return MeshID();
		}

		return MeshID{
			.id = impl->meshBuffersLookup[name],
		};
	}

	void Renderer::SetCameraData(const CameraData& data)
	{
		impl->cameraData = data;
	}

	void Renderer::ResizeDepthTexture()
	{
		CreateDepthTexture(); //Function call resets unique_ptr so this is a fine solution for now.
	}

	bool Renderer::CreateDepthTexture()
	{
		int w, h;
		SDL_GetWindowSizeInPixels(impl->window, &w, &h);

		SDL_GPUTextureCreateInfo depthTextureCreateInfo = {
			.type = SDL_GPU_TEXTURETYPE_2D,
			.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
			.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
			.width = static_cast<Uint32>(w),
			.height = static_cast<Uint32>(h),
			.layer_count_or_depth = 1,
			.num_levels = 1,
		};

		impl->depthTexture.reset(SDL_CreateGPUTexture(impl->device.get(), &depthTextureCreateInfo));

		return true;
	}

	bool Renderer::CreatePipeline()
	{
		//Load Shaders to CPU
		std::unique_ptr<SDL_GPUShader, GPUShaderDeleter> vertexShader{ impl->LoadShader("Default.vert"), GPUShaderDeleter{.device = impl->device.get(), } };
		if (vertexShader == nullptr)
		{
			Logging::Log("Couldn't create vertex shader during CreatePipeline().");
			return false;
		}

		std::unique_ptr<SDL_GPUShader, GPUShaderDeleter> fragmentShader{ impl->LoadShader("Default.frag"), GPUShaderDeleter{.device = impl->device.get(), } };
		if (fragmentShader == nullptr)
		{
			Logging::Log("Couldn't create fragment shader during CreatePipeline().");
			return false;
		}

		//Define information for the Input Assembler

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
				.offset = 0,
			},
			SDL_GPUVertexAttribute{
				.location = 1,
				.buffer_slot = 0,
				.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
				.offset = sizeof(Math::vec3),
			},
			SDL_GPUVertexAttribute{
				.location = 2, 
				.buffer_slot = 0,
				.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
				.offset = sizeof(Math::vec3) + sizeof(Math::vec2),
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
			.vertex_shader = vertexShader.get(),
			.fragment_shader = fragmentShader.get(),
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
			.depth_stencil_state = SDL_GPUDepthStencilState {
				.compare_op = SDL_GPU_COMPAREOP_LESS,
				.enable_depth_test = true,
				.enable_depth_write = true,
			},
			.target_info = SDL_GPUGraphicsPipelineTargetInfo{ //Draw to our swapchain.
				.color_target_descriptions = colorTargetDescriptions.data(),
				.num_color_targets = colorTargetDescriptions.size(),
				.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT,
				.has_depth_stencil_target = true,
			},
		};

		//Submit and create pipeline.
		impl->pipeline.reset(SDL_CreateGPUGraphicsPipeline(impl->device.get(), &pipelineCreateInfo));
		if (impl->pipeline == nullptr)
		{
			Logging::Log("Failed to create graphics pipeline. %s", SDL_GetError());
			return false;
		}

		return true;
	}

	void Renderer::CreateTextureSampler()
	{
		SDL_GPUSamplerCreateInfo sampler_info{
			.min_filter = SDL_GPU_FILTER_NEAREST,
			.mag_filter = SDL_GPU_FILTER_NEAREST,
			.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
			.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
			.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
			.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		};

		impl->sampler = SDL_CreateGPUSampler(impl->device.get(), &sampler_info);
	}

	void Renderer::PushCameraBuffer(const CameraBufferType& buff)
	{
		if (impl->commandBuffer)
			SDL_PushGPUVertexUniformData(impl->commandBuffer, 0, &buff, sizeof(CameraBufferType));
		else
			Logging::Log("Failed to push camera buffer to vertex shader, missing command buffer");
	}

	void Renderer::PushTransformBuffer(const TransformBufferType& buff)
	{
		if (impl->commandBuffer)
			SDL_PushGPUVertexUniformData(impl->commandBuffer, 1, &buff, sizeof(TransformBufferType));
		else
			Logging::Log("Failed to push transform buffer to vertex shader, missing command buffer");
	}
}