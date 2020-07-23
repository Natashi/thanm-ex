#pragma once
#include "pch.h"
#include "Util.h"

class ExpressionMorph;
class InstructionData {
public:
	InstructionData();
	InstructionData(const char* format_, const char* name_);
	InstructionData(const char* format_, const char* name_, ExpressionMorph* transformer_);

	void Dispose();

	std::string format;
	std::string name;
	ExpressionMorph* transformer;
};

class InstructionDataList {
public:
	InstructionDataList();
	~InstructionDataList();

	void Initialize(const std::map<uint16_t, InstructionData>& instrMap);

	InstructionData* GetData(uint16_t id);
protected:
	std::map<uint16_t, InstructionData> mapFormatParameter_;
};

class InstructionDataManager {
	static InstructionDataManager* base_;
public:
	InstructionDataManager();
	~InstructionDataManager();

	static InstructionDataManager* GetBase() { return base_; }

	InstructionDataList* GetList(uint32_t version);
protected:
	std::map<uint32_t, InstructionDataList*> mapList_;
};

class ExpressionMorph {
public:
	ExpressionMorph();

	virtual std::string Morph(InstructionData* data, std::vector<std::string>* listParam) = 0;
};
class RegexMorph : public ExpressionMorph {
public:
	RegexMorph(const std::string& pattern);

	virtual std::string Morph(InstructionData* data, std::vector<std::string>* listParam);
protected:
	std::string pattern_;
};