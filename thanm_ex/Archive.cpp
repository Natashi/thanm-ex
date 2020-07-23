#include "pch.h"
#include "Instruction.h"
#include "Archive.h"
#include "Util.h"

using namespace std;

Script::Script() {
	id = 0;
	baseOffset = 0;
}
void Script::AddLabels(InstructionDataList* listData) {
	if (listData == nullptr) return;
	for (auto iCode = codes.cbegin(); iCode != codes.cend(); ++iCode) {
		InstructionData* data = listData->GetData(iCode->head.id);
		if (data) {
			size_t iFind = data->format.find('o');
			if (iFind != string::npos)
				setCodeLabel.insert(iCode->params[iFind].S);
		}
	}
}

Entry::Entry() {
	memset(&header_, 0x00, sizeof(EntryHeader));
}
Entry::~Entry() {
}

void Entry::Write(std::ofstream& out) {
	InstructionDataList* listFormatData = InstructionDataManager::GetBase()->GetList(header_.version);

	Utility::StreamPrint(out, "Entry %u <version=%u> {\n", id_, header_.version);
	Utility::StreamPrint(out, "\tHeader {\n");
	Utility::StreamPrint(out, 
		"\t\tName = \"%s\"\n"
		"\t\tFormat = %u\n"
		"\t\tWidth = %u\n"
		"\t\tHeight = %u\n"
		"\t\tOffsetX = %u\n"
		"\t\tOffsetY = %u\n"
		"\t\tMemoryPriority = %u\n"
		"\t\tResize = %u\n",
		name_.c_str(), header_.format, 
		header_.w, header_.h, header_.xOff,  header_.yOff,
		header_.d3dMemoryPriority, header_.resizeable
	);
	Utility::StreamPrint(out, "\t}\n");
	if (header_.bThtxData) {
		Utility::StreamPrint(out, "\tTHTX {\n");

		std::string strFormat = "UNKNOWN_";
		switch (thtx_.format) {
		case 1:
			strFormat = "BGRA_8888";
			break;
		case 3:
			strFormat = "RGB_565";
			break;
		case 5:
			strFormat = "ARGB_4444";
			break;
		case 7:
			strFormat = "GRAY_8";
			break;
		default:
			strFormat = "UNKNOWN_";
			strFormat += std::to_string(thtx_.format);
			break;
		}

		Utility::StreamPrint(out, "\t\tSize = %u\n", thtx_.dataSize);
		Utility::StreamPrint(out, "\t\tFormat = %s\n", strFormat.c_str());
		Utility::StreamPrint(out, "\t\tWidth = %u\n", thtx_.w);
		Utility::StreamPrint(out, "\t\tHeight = %u\n", thtx_.h);

		Utility::StreamPrint(out, "\t}\n");
	}

	if (mapSprite_.size() > 0U) {
		Utility::StreamPrint(out, "\n");
		Utility::StreamPrint(out, "\tSprite {\n");
		for (auto itr = mapSprite_.cbegin(); itr != mapSprite_.cend();) {
			const Sprite* s = &(itr->second);
			Utility::StreamPrint(out, "\t\t[%u] = (%.0f, %.0f, %.0f, %.0f)",
				itr->first, s->x, s->y, s->x + s->w, s->y + s->h);
			if (++itr != mapSprite_.cend())
				Utility::StreamPrint(out, ",");
			Utility::StreamPrint(out, "\n");
		}
		Utility::StreamPrint(out, "\t}\n");
	}

	Utility::StreamPrint(out, "}\n");

	if (listScript_.size() > 0U) {
		Utility::StreamPrint(out, "\n");
		for (auto itrScript = listScript_.begin(); itrScript != listScript_.end();) {
			Utility::StreamPrint(out, "Script %d {\n", itrScript->id);

			uint32_t countByte = 0;
			set<uint32_t>::iterator itrLabel = itrScript->setCodeLabel.begin();
			set<uint32_t>::iterator itrLabelEnd = itrScript->setCodeLabel.end();
			/*
			{
				Utility::StreamPrint(out, "\tLabels {\n");
				for (auto& itr : itrScript->setCodeLabel) {
					Utility::StreamPrint(out, "\t\t%d,\n", itr);
				}
				Utility::StreamPrint(out, "\t}\n");
			}
			*/

			int16_t time = INT16_MAX;
			for (auto iCode = itrScript->codes.cbegin(); iCode != itrScript->codes.cend(); ++iCode) {
				if (time != iCode->head.time)
					Utility::StreamPrint(out, "%d:\n", iCode->head.time);
				time = iCode->head.time;

				if (itrLabel != itrLabelEnd && *itrLabel == countByte) {
					Utility::StreamPrint(out, "label_%d:\n", countByte);
					++itrLabel;
				}

				InstructionData* formatData = nullptr;
				if (listFormatData)
					formatData = listFormatData->GetData(iCode->head.id);

				vector<string> listStrParam;
				{
					size_t countFormat = formatData ? formatData->format.size() : 0U;
					size_t iParam = 0;
					for (auto itrParam = iCode->params.cbegin(); itrParam != iCode->params.cend(); ++itrParam, ++iParam) {
						char type = 'S';
						if (formatData != nullptr) {
							if (iParam < countFormat)
								type = formatData->format[iParam];
							else
								printf("Warning: Incorrect format table for ins_%d", iCode->head.id);
						}

						string strParam;

						bool bRegister = (iCode->head.paramType >> iParam) & 0b1;
						if (bRegister) strParam.push_back('[');
						switch (type) {
						case 'f':
							strParam += Utility::PrintFloat(itrParam->f);
							break;
						case 'o':
						{
							char tmp[256];
							sprintf_s(tmp, "label_%d", itrParam->S);
							strParam += string(tmp);
							break;
						}
						case 'S':
						default:
							strParam += to_string(itrParam->S);
							break;
						}
						if (bRegister) strParam.push_back(']');

						listStrParam.push_back(strParam);
					}
				}

				if (formatData != nullptr && formatData->transformer != nullptr) {
					string expression = formatData->transformer->Morph(formatData, &listStrParam);
					Utility::StreamPrint(out, "\t%s;\n", expression.c_str());
				}
				else {
					if (formatData != nullptr && formatData->name.size() > 0)
						Utility::StreamPrint(out, "\t%s(", formatData->name.c_str());
					else
						Utility::StreamPrint(out, "\tins_%d(", iCode->head.id);
					for (auto itrParam = listStrParam.cbegin(); itrParam != listStrParam.cend();) {
						Utility::StreamPrint(out, "%s", itrParam->c_str());
						if (++itrParam != listStrParam.cend())
							Utility::StreamPrint(out, ", ");
					}
					Utility::StreamPrint(out, ");\n");
				}

				countByte += iCode->head.size;
			}

			Utility::StreamPrint(out, "}\n");
			if (++itrScript != listScript_.cend())
				Utility::StreamPrint(out, "\n");
		}
	}
}

Archive::Archive() {
	sizeFile = 0;
}
Archive::~Archive() {

}
void Archive::Load(const wstring& path) {
	ifstream file;
	file.open(path, ios::binary);

	if (!file.is_open()) throw string("Cannot open file for reading.");

	file.seekg(0, ios::end);
	sizeFile = file.tellg();
	file.seekg(0, ios::beg);

	try {
		size_t iEntry = 0;
		while (true) {
			Entry entry;
			EntryHeader* header = entry.GetHeader();

			entry.SetID(iEntry);

			size_t readPos = file.tellg();

			{
				file.read((char*)header, sizeof(EntryHeader));

				switch (header->version) {
				case 4:
				case 7:
				case 8:
					break;
				default:
					printf("Warning: Unsupported entry version (%d), skipping.\n", header->version);
					goto next_entry;
				}

				std::vector<uint32_t> listOffsetSprite;
				std::vector<ScriptOffset> listOffsetScript;

				listOffsetSprite.resize(header->countSprite);
				file.read((char*)listOffsetSprite.data(), (size_t)header->countSprite * sizeof(uint32_t));

				listOffsetScript.resize(header->countScript);
				file.read((char*)listOffsetScript.data(), (size_t)header->countScript * sizeof(ScriptOffset));

				file.clear();
				{
					char tmp[0x40];
					file.seekg(readPos + header->nameOffset);
					file.read(tmp, 0x40);
					entry.SetName(std::string(tmp));
				}
				file.clear();
				{
					std::map<uint32_t, Sprite>* mapSprite = entry.GetSpriteMap();
					for (auto itr = listOffsetSprite.cbegin(); itr != listOffsetSprite.cend(); ++itr) {
						Sprite s;
						file.seekg(readPos + (*itr));
						file.read((char*)&s, sizeof(Sprite));
						mapSprite->insert(std::make_pair(s.id, s));
					}
				}
				file.clear();
				{
					for (auto itr = listOffsetScript.cbegin(); itr != listOffsetScript.cend(); ++itr) {
						Script script;
						script.id = itr->id;
						script.baseOffset = itr->offset;

						size_t readLimit = 0;
						if (itr != std::prev(listOffsetScript.cend()))
							readLimit = std::next(itr)->offset - itr->offset;
						else if (header->thtxOffset != 0)
							readLimit = header->thtxOffset - itr->offset;
						else if (header->nextEntryPos != 0)
							readLimit = header->nextEntryPos - itr->offset;
						else
							readLimit = sizeFile - itr->offset;

						file.clear();
						file.seekg(readPos + itr->offset);

						size_t read = 0;
						while (read < readLimit) {
							Instruction instr;
							file.read((char*)&(instr.head), sizeof(instr.head));
							read += sizeof(instr.head);

							if (instr.head.id == 0xFFFF) break;

							size_t countParam = (instr.head.size - sizeof(instr.head)) / sizeof(Value);
							for (size_t i = 0; i < countParam; ++i) {
								Value v;
								file.read((char*)&v, sizeof(Value));
								instr.params.push_back(v);
							}
							read += sizeof(Value) * countParam;

							script.codes.push_back(instr);
						}

						{
							InstructionDataList* listFormatData = InstructionDataManager::GetBase()->GetList(header->version);
							if (listFormatData) script.AddLabels(listFormatData);
						}
						entry.AddScript(script);
					}
				}
				file.clear();
				{
					ThtxHeader* thtx = entry.GetThtxHeader();
					file.seekg(readPos + header->thtxOffset);
					file.read((char*)thtx, sizeof(ThtxHeader));
				}
			}

			entries.push_back(entry);
next_entry:
			if (header->nextEntryPos == 0) break;
			file.seekg(readPos + header->nextEntryPos, ios::beg);
			++iEntry;
		}
	}
	catch (string& str) {
		file.close();
		throw str;
	}

	file.close();
}
void Archive::Write(const std::wstring& path) {
	ofstream out;
	out.open(path, ios::trunc);

	try {
		if (!out.is_open()) throw string("Cannot open file for writing.");
		for (auto itr = entries.begin(); itr != entries.end();) {
			itr->Write(out);
			if (++itr != entries.end())
				Utility::StreamPrint(out, "\n");
		}
	}
	catch (string& str) {
		out.close();
		throw str;
	}

	out.close();
}