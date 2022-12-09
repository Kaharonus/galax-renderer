//
// Created by tomas on 27.8.22.
//

#include <assets/AssetLoader.h>
#include <assets/ObjReader.h>
#include <assets/PngReader.h>
#include <Extensions.h>


#include <physfs.h>
#include <QCoreApplication>
#include <iostream>
#include <filesystem>
#include <memory>
#include <thread>
#include <future>

using namespace Galax::Assets;
AssetLoader::AssetLoader() {
    PHYSFS_init(QCoreApplication::arguments().at(0).toStdString().c_str());
}

bool AssetLoader::load(const std::string& assetFile) {
    this->assetFile = assetFile;
    //check if file exists using stdlib
    std::filesystem::path assetPath(assetFile);
    auto absolutePath = std::filesystem::absolute(assetPath).string();
    // Check if file exists
    if (!std::filesystem::exists(assetFile)) {
        std::cout << "Asset file " << absolutePath << " does not exist" << std::endl;
        return false;
    }
    loaded = PHYSFS_mount(assetFile.c_str(),"/", 1);
    return loaded;
}

std::shared_ptr<Shader> AssetLoader::getShader(std::string asset, Shader::Type type, std::string name) {
    if(name.empty()) {
        name = asset;
    }
    auto source = readTextFile(std::move(asset));
    return std::make_shared<Shader>(name, source, type);
}

std::string AssetLoader::readTextFile(std::string asset) {
    asset = path + asset;
    exists(asset);
    PHYSFS_File* file = PHYSFS_openRead(asset.c_str());
    if(file == nullptr) {
        throw std::runtime_error("Could not open file " + asset);
    }
    PHYSFS_sint64 size = PHYSFS_fileLength(file);
    char buffer[size];
    PHYSFS_readBytes(file, buffer, size);
    std::string source(buffer, size);
    PHYSFS_close(file);
    return source;
}


std::shared_ptr<Mesh> AssetLoader::getMesh(const std::string& asset, std::string name){

    auto fullPath = path + asset;
    exists(fullPath);
    PHYSFS_File* file = PHYSFS_openRead(fullPath.c_str());
    if(file == nullptr) {
        throw std::runtime_error("Could not open file " + asset);
    }
    ObjReader reader(file);
    reader.load();
    PHYSFS_close(file);

    if(name.empty()){
        if(reader.name.empty()){
            name = asset;
        } else {
            name = reader.name;
        }
    }
    auto mesh = std::make_shared<Mesh>(name);
    for(int i = 0; i < reader.vertexIndices.size(); i++) {
        auto vertex = reader.vertices[reader.vertexIndices[i]];
        auto normal = reader.normals[reader.normalIndices[i]];
        auto texCoord = reader.texCoords[reader.texCoordIndices[i]];
        mesh->addVertex(vertex, normal, texCoord);
    }

    for(int i = 0; i < reader.vertexIndices.size(); i += 3) {
        mesh->addTriangleIndex(i, i + 1, i + 2);
    }

    return mesh;
}

void AssetLoader::exists(const std::string &file) {
    auto exists = PHYSFS_exists(file.c_str());
    assert(exists);
}

std::shared_ptr<Texture> AssetLoader::getCubemap(const std::string& asset, std::string name) {
    auto fullPath = path + asset;
    if(name.empty()){
        name = asset;
    }
    auto skybox = std::make_shared<Texture>(name, Texture::TYPE_CUBE);
    skybox->setFiltering(Texture::Filtering::LINEAR);

    std::vector<std::string> names = {"PositiveX.png", "NegativeX.png", "PositiveY.png", "NegativeY.png", "PositiveZ.png", "NegativeZ.png"};

    auto threads = std::vector<std::future<std::vector<unsigned char>>>();
    for(auto [i, fileName] : enumerate(names)) {
        //spawn a thread for each file
        auto future = std::async([&](){
            auto sidePath = fullPath + "/" + fileName;
            exists(sidePath);
            PHYSFS_File* file = PHYSFS_openRead(sidePath.c_str());
            PHYSFS_sint64 size = PHYSFS_fileLength(file);
            char* buffer = new char[size];
            PHYSFS_readBytes(file, buffer, size);
            PHYSFS_close(file);
            PngReader reader;
            auto texture = reader.read(buffer, size);
            delete[] buffer;
            return texture;
        });
        threads.push_back(std::move(future));
    }
    //wait for all threads to finish
    for(auto [i, thread] : enumerate(threads)) {
       skybox->setData(thread.get(), i);
    }

    return skybox;
}

std::vector<std::string> AssetLoader::getFiles(const std::string &dir) {
	auto fullPath = path + dir;
	exists(fullPath);
	auto files = std::vector<std::string>();
	auto dirHandle = PHYSFS_enumerateFiles(fullPath.c_str());
	for(auto file = dirHandle; *file != nullptr; file++) {
		files.push_back(*file);
	}
	PHYSFS_freeList(dirHandle);
	return files;
}

