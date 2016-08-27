#include "file.h"

_file::_file() {

}

_file::~_file() {

}

bool _file::load(const std::string& path, filetype type, fileaccess access) {
	return false;
}

bool _file::free() {
	return false;
}

File::File() {

}

File::~File() {

}

bool File::loadFile(const std::string& fID, const std::string& path, filetype type, fileaccess access) {
	return false;
}

bool File::freeFile(const std::string& fID) {
	return false;
}

bool File::read(const std::string& fID, void* buf, u32 size, u32 numobjs) {
	return false;
}

bool File::write(const std::string& fID, void* buf, u32 size, u32 numobjs) {
	return false;
}

u64 File::getOffset(const std::string& fID) {
	return 0;
}

u64 File::setOffset(const std::string& fID, u64 offset, fileseek start) {
	return 0;
}

bool File::loadLib(const std::string& lID, const std::string& path) {
	return false;
}

bool File::loadLibFolder(const std::string& path) {
	return false;
}

bool File::freeLib(const std::string& lID) {
	return false;
}

void* loadFunc(const std::string& lID, const std::string& func) {
	return NULL;
}

std::vector<std::string> File::getNamesInFolder(const std::string& path) {
	return std::vector<std::string>();
}

bool File::copyFile(const std::string& src, const std::string& dest) {
	return false;
}
