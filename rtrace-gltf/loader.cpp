	#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>
#include <fstream>

#include "loader.h"

using namespace Microsoft;


///COPY FROM https://github.com/microsoft/glTF-SDK/blob/master/GLTFSDK.Samples/Deserialize/Source/main.cpp
// The glTF SDK is decoupled from all file I/O by the IStreamReader (and IStreamWriter)
    // interface(s) and the C++ stream-based I/O library. This allows the glTF SDK to be used in
    // sandboxed environments, such as WebAssembly modules and UWP apps, where any file I/O code
    // must be platform or use-case specific.
class StreamReader : public glTF::IStreamReader
{
public:
    StreamReader(filesystem::path pathBase) : m_pathBase(std::move(pathBase))
    {
        //assert(m_pathBase.has_root_path());                                                   ///Commented because hits for some reason
    }

    // Resolves the relative URIs of any external resources declared in the glTF manifest
    std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override
    {
        // In order to construct a valid stream:
        // 1. The filename argument will be encoded as UTF-8 so use filesystem::u8path to
        //    correctly construct a path instance.
        // 2. Generate an absolute path by concatenating m_pathBase with the specified filename
        //    path. The filesystem::operator/ uses the platform's preferred directory separator
        //    if appropriate.
        // 3. Always open the file stream in binary mode. The glTF SDK will handle any text
        //    encoding issues for us.
        auto streamPath = m_pathBase / filename;                                                ///NO UTF8 convertion because function not available on new <filesystem> lib
        auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

        // Check if the stream has no errors and is ready for I/O operations
        if (!stream || !(*stream))
        {
            throw std::runtime_error("Unable to create a valid input stream for uri: " + filename);
        }

        return stream;
    }

private:
    std::filesystem::path m_pathBase;
};

/// <summary>
/// Extracted from PrintInfo()
/// </summary>
/// <returns>The gltf designed by the given path</returns>
glTF::Document DeserializeDocument(filesystem::path path, glTF::Document& document, std::unique_ptr<glTF::GLTFResourceReader>& resourceReader) {
    // Pass the absolute path, without the filename, to the stream reader
    auto streamReader = std::make_unique<StreamReader>(path.parent_path());

    filesystem::path pathFile = path.filename();
    filesystem::path pathFileExt = pathFile.extension();

    std::string manifest;

    auto MakePathExt = [](const std::string& ext)
    {
        return "." + ext;
    };

    // If the file has a '.gltf' extension then create a GLTFResourceReader
    if (pathFileExt == MakePathExt(glTF::GLTF_EXTENSION))
    {
        auto gltfStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
        auto gltfResourceReader = std::make_unique<glTF::GLTFResourceReader>(std::move(streamReader));

        std::stringstream manifestStream;

        // Read the contents of the glTF file into a string using a std::stringstream
        manifestStream << gltfStream->rdbuf();
        manifest = manifestStream.str();

        resourceReader = std::move(gltfResourceReader);
    }

    // If the file has a '.glb' extension then create a GLBResourceReader. This class derives
        // from GLTFResourceReader and adds support for reading manifests from a GLB container's
        // JSON chunk and resource data from the binary chunk.
    if (pathFileExt == MakePathExt(glTF::GLB_EXTENSION))
    {
        auto glbStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
        auto glbResourceReader = std::make_unique<glTF::GLBResourceReader>(std::move(streamReader), std::move(glbStream));

        manifest = glbResourceReader->GetJson(); // Get the manifest from the JSON chunk

        resourceReader = std::move(glbResourceReader);
    }

    if (!resourceReader)
    {
        throw std::runtime_error("Command line argument path filename extension must be .gltf or .glb");
    }

    try
    {
        document = glTF::Deserialize(manifest);
    }
    catch (const glTF::GLTFException& ex)
    {
        std::stringstream ss;

        ss << "Microsoft::glTF::Deserialize failed: ";
        ss << ex.what();

        throw std::runtime_error(ss.str());
    }

    return document;
}
///END COPY

Scene* loadScene(filesystem::path path)
{
	std::unique_ptr<glTF::GLTFResourceReader> resourceReader;
	glTF::Document document;
	DeserializeDocument(path, document, resourceReader);

	Scene* scene = new Scene;
	scene->root = new Node;

	map<string, Mesh*> meshes;
	for (const glTF::Mesh& gltfMesh : document.meshes.Elements())
	{
		std::cout << "Loading mesh id=" << gltfMesh.id << " : ";

		Mesh* mesh = new Mesh;
		for (const auto& gltfPrimitive : gltfMesh.primitives)
		{
			std::cout << ". ";

			if (gltfPrimitive.mode != glTF::MESH_TRIANGLES) {
				std::cout << "(WARN : Primitive mode is not MESH_TRIANGLES !)";
			}

			std::string accessorId;
			Primitive* primitive = new Primitive;

			if (gltfPrimitive.TryGetAttributeAccessorId(glTF::ACCESSOR_POSITION, accessorId))
			{
				const glTF::Accessor& posAccessor = document.accessors.Get(accessorId);
				const auto posData = resourceReader->ReadBinaryData<float>(document, posAccessor);
				
				for (int i = 0; i < posData.size(); i += 3) {
					vec<float, 3> position = { posData[i], posData[(size_t)i + 1], posData[(size_t)i + 2] };
					primitive->positions.push_back(position);
				}

				const glTF::Accessor& indAccessor = document.accessors.Get(gltfPrimitive.indicesAccessorId);
				const auto indData = resourceReader->ReadBinaryData<uint16_t>(document, indAccessor);

				for (int i = 0; i < indData.size(); i += 3) {
					vec<int, 3> triangle = { indData[i], indData[(size_t)i + 1], indData[(size_t)i + 2] };
					primitive->triangles.push_back(triangle);
				}

				mesh->primitives.push_back(primitive);
			}
			else {
				std::cout << "(WARN : ACCESSOR_POSITION Attribute Accessor not found !)";
			}
		}
		std::cout << "OK" << endl;
		meshes[gltfMesh.id] = mesh;
	}

    map<string, Camera*> cameras;
    for (const glTF::Camera& gltfCam : document.cameras.Elements())
    {
        std::cout << "Loading camera id=" << gltfCam.id << " : ";

        Camera* camera = new Camera;

        camera->fov = gltfCam.GetPerspective().yfov;
        camera->ratio = gltfCam.GetPerspective().aspectRatio;

        std::cout << "OK" << endl;
        cameras[gltfCam.id] = camera;
    }

	map<string, Node*> nodes;
	for (const glTF::Node& gltfNode : document.nodes.Elements())
	{
		std::cout << "Loading node name=" << gltfNode.name << " : ";

		if (gltfNode.GetTransformationType() == glTF::TransformationType::TRANSFORMATION_MATRIX) {
			std::cout << "(WARN : TRANSFORMATION_MATRIX transform type is not supported !)";
		}
		Node* node = new Node;	
		node->name = gltfNode.name;
		node->translation = { gltfNode.translation.x, gltfNode.translation.y, gltfNode.translation.z };
		node->rotation = { gltfNode.rotation.x, gltfNode.rotation.y, gltfNode.rotation.z, gltfNode.rotation.w };
		node->scale = { gltfNode.scale.x, gltfNode.scale.y, gltfNode.scale.z };
        node->mesh = meshes[gltfNode.meshId];
        node->camera = cameras[gltfNode.cameraId];

		std::cout << "OK" << endl;
		nodes[gltfNode.id] = node;
	}

	for (const glTF::Node& gltfNode : document.nodes.Elements())
	{
		Node* node = nodes[gltfNode.id];
		for (const string child : gltfNode.children) {
			Node* child = nodes[gltfNode.id];
			node->childs.push_back(child);
		}
	}

	for (int i = 0; i < document.scenes.Size(); i++) {
		glTF::Scene gltfScene = document.scenes[i];
		for (const string nodeId : gltfScene.nodes) {
			scene->root->childs.push_back(nodes[nodeId]);
		}
	}

	return scene;
}
