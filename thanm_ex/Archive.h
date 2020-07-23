#pragma once
#include "pch.h"

#pragma pack(push, 1)

struct EntryHeader {
	uint32_t version;
	uint16_t countSprite;
	uint16_t countScript;
	uint16_t zero1;
	uint16_t w, h;
	uint16_t format;
	uint32_t nameOffset;
	uint16_t xOff, yOff;
	uint32_t d3dMemoryPriority;
	uint32_t thtxOffset;
	uint16_t bThtxData;
	uint16_t resizeable;
	uint32_t nextEntryPos;
	uint64_t zero2[3];
};
struct ThtxHeader {
	uint32_t magic;
	uint16_t zero1;
	uint16_t format;
	uint16_t w, h;
	uint32_t dataSize;
};

struct Sprite {
	uint32_t id;
	float x, y, w, h;
};

struct ScriptOffset {
	uint32_t id;
	uint32_t offset;
};
struct Value {
	union {
		int S;
		float f;
	};
};
struct Instruction {
	struct {
		uint16_t id;
		uint16_t size;
		int16_t time;
		uint16_t paramType;
	} head;
	std::vector<Value> params;
};

class InstructionDataList;
class Script {
public:
	Script();

	void AddLabels(InstructionDataList* listData);
public:
	uint32_t id;
	uint32_t baseOffset;
	std::vector<Instruction> codes;

	std::set<uint32_t> setCodeLabel;
};

#pragma pack(pop)

class Entry {
public:
	Entry();
	~Entry();

	EntryHeader* GetHeader() { return &header_; }
	ThtxHeader* GetThtxHeader() { return &thtx_; }

	void SetID(uint32_t id) { id_ = id; }
	void SetName(const std::string& name) { name_ = name; }

	std::map<uint32_t, Sprite>* GetSpriteMap() { return &mapSprite_; }
	void AddScript(Script& script) { listScript_.push_back(script); }

	void Write(std::ofstream& out);
private:
	EntryHeader header_;
	ThtxHeader thtx_;

	uint32_t id_;
	std::string name_;

	std::map<uint32_t, Sprite> mapSprite_;
	std::vector<Script> listScript_;
};

class Archive {
public:
	Archive();
	~Archive();

	void Load(const std::wstring& path);

	void Write(const std::wstring& path);
private:
	std::vector<Entry> entries;
	std::streamsize sizeFile;
};