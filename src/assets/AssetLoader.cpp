//
// Created by tomas on 27.8.22.
//

#include "AssetLoader.h"
#include "ObjReader.h"
#include <physfs.h>
#include <QCoreApplication>
#include <iostream>
#include <filesystem>
#include <memory>

using namespace Galax::Assets;
AssetLoader::AssetLoader(std::string assetFile) {
    this->assetFile = assetFile;
    PHYSFS_init(QCoreApplication::arguments().at(0).toStdString().c_str());

}

bool AssetLoader::load() {
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


std::vector<std::shared_ptr<Mesh>> AssetLoader::getMesh(const std::string& directory, int levels, std::string name){
    if(name.empty()) {
        name = directory;
    }
    exists(path+directory);
    std::vector<std::shared_ptr<Mesh>> result;

    for(int i = 0; i < levels; i++) {
        auto file = directory + "/lod" + std::to_string(i) + ".obj";
        auto mesh = getMesh(file, name + " (lod" + std::to_string(i)+")");
        result.push_back(mesh);
    }
    return result;

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
