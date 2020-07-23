#include "pch.h"
#include "Instruction.h"

using namespace std;

InstructionData::InstructionData() {
    format = "";
    name = "";
    transformer = nullptr;
}
InstructionData::InstructionData(const char* format_, const char* name_) {
    format = format_;
    name = name_;
    transformer = nullptr;
}
InstructionData::InstructionData(const char* format_, const char* name_, ExpressionMorph* transformer_) 
    : InstructionData(format_, name_) 
{
    transformer = transformer_;
}
void InstructionData::Dispose() {
    if (transformer) delete transformer;
    transformer = nullptr;
}

InstructionDataManager* InstructionDataManager::base_ = nullptr;
InstructionDataManager::InstructionDataManager() {
    if (base_ != nullptr) return;
    base_ = this;

    {
        map<uint16_t, InstructionData> map_v4p = {
            { 0, InstructionData("", "") },
            { 1, InstructionData("", "Destroy") },
            { 2, InstructionData("", "") },
            { 3, InstructionData("S", "SetSprite") },
            { 4, InstructionData("oS", "", new RegexMorph("goto $0 @ $1")) },
            { 5, InstructionData("SoS", "", new RegexMorph("if ($0--) goto $1 @ $2")) },
            { 6, InstructionData("SS", "", new RegexMorph("$0 = $1")) },
            { 7, InstructionData("ff", "", new RegexMorph("$0 = $1")) },
            { 8, InstructionData("SS", "", new RegexMorph("$0 += $1")) },
            { 9, InstructionData("ff", "", new RegexMorph("$0 += $1")) },
            { 10, InstructionData("SS", "", new RegexMorph("$0 -= $1")) },
            { 11, InstructionData("ff", "", new RegexMorph("$0 -= $1")) },
            { 12, InstructionData("SS", "") },
            { 13, InstructionData("ff", "") },
            { 18, InstructionData("SSS", "") },
            { 19, InstructionData("fff", "") },
            { 20, InstructionData("SSS", "") },
            { 21, InstructionData("fff", "") },
            { 22, InstructionData("SSS", "", new RegexMorph("$0 = $1 * $2")) },
            { 23, InstructionData("fff", "", new RegexMorph("$0 = $1 * $2")) },
            { 24, InstructionData("SSS", "", new RegexMorph("$0 = $1 / $2")) },
            { 25, InstructionData("fff", "", new RegexMorph("$0 = $1 / $2")) },
            { 26, InstructionData("SSS", "") },
            { 27, InstructionData("fff", "") },
            { 30, InstructionData("SSSS", "") },
            { 40, InstructionData("SS", "") },
            { 42, InstructionData("ff", "Sin") },
            { 43, InstructionData("ff", "Cos") },
            { 48, InstructionData("fff", "SetPosition") },
            { 49, InstructionData("fff", "SetAngle") },
            { 50, InstructionData("ff", "SetScale") },
            { 51, InstructionData("S", "SetAlpha") },
            { 52, InstructionData("SSS", "SetColor") },
            { 53, InstructionData("fff", "SetRotationRate") },
            { 56, InstructionData("SSfff", "ChangePosition") },
            { 57, InstructionData("SSSSS", "ChangeColor") },
            { 58, InstructionData("SSS", "ChangeAlpha") },
            { 59, InstructionData("SSfff", "ChangeAngle") },
            { 60, InstructionData("SSff", "ChangeScale") },
            { 61, InstructionData("", "FlipTextureU") },
            { 62, InstructionData("", "FlipTextureV") },
            { 63, InstructionData("", "") },
            { 64, InstructionData("S", "SetEntryPoint") },
            { 65, InstructionData("S", "SetTextureOriginType") },
            { 66, InstructionData("S", "SetBlendMode") },
            { 67, InstructionData("S", "SetRotationMode") },
            { 68, InstructionData("S", "SetLayer") },
            { 69, InstructionData("", "") },
            { 70, InstructionData("f", "") },
            { 71, InstructionData("f", "") },
            { 73, InstructionData("S", "") },
            { 74, InstructionData("S", "") },
            { 75, InstructionData("S", "") },
            { 76, InstructionData("SSS", "SetPrimitiveColor") },
            { 77, InstructionData("S", "SetPrimitiveAlpha") },
            { 78, InstructionData("SSSSS", "ChangePrimitiveColor") },
            { 79, InstructionData("SSS", "ChangePrimitiveAlpha") },
            { 80, InstructionData("S", "") },
            { 81, InstructionData("", "") },
            { 82, InstructionData("S", "") },
            { 83, InstructionData("", "") },
            { 84, InstructionData("S", "InitPolarCoordinates") },
            { 85, InstructionData("S", "") },
            { 86, InstructionData("S", "") },
            { 87, InstructionData("S", "") },
            { 88, InstructionData("S", "RunScript") },
            { 89, InstructionData("S", "") },
            { 90, InstructionData("S", "") },
            { 91, InstructionData("S", "") },
            { 92, InstructionData("S", "") },
            { 93, InstructionData("SSf", "") },
            { 94, InstructionData("SSf", "") },
            { 95, InstructionData("S", "") },
            { 96, InstructionData("Sff", "") },
            { 100, InstructionData("Sfffffffff", "ChangePositionBezier") },
            { 101, InstructionData("S", "") },
            { 102, InstructionData("SS", "SetSpriteRandom") },
            { 103, InstructionData("ff", "GenerateRectangle") },
            { 104, InstructionData("fS", "GenerateStar") },
            { 105, InstructionData("fS", "") },
            { 106, InstructionData("fS", "") },
            { 107, InstructionData("SSff", "") },
            { 108, InstructionData("ff", "") },
            { 110, InstructionData("ff", "") },
            { 111, InstructionData("S", "") },
            { 112, InstructionData("S", "") },
            { 113, InstructionData("SSf", "") },
            { 114, InstructionData("S", "") }
        };
        InstructionDataList* listv4p = new InstructionDataList;
        listv4p->Initialize(map_v4p);
        mapList_.insert(std::make_pair(4U, listv4p));
    }
    {
        map<uint16_t, InstructionData> map_v8 = {
            { 0, InstructionData("", "") },
            { 1, InstructionData("", "Destroy") },
            { 2, InstructionData("", "") },
            { 3, InstructionData("", "Leave") },
            { 4, InstructionData("", "") },
            { 5, InstructionData("S", "SetEntryPoint") },
            { 6, InstructionData("S", "") },
            { 7, InstructionData("", "") },
            //Arithmetic-assign
            { 100, InstructionData("SS", "", new RegexMorph("$0 = $1")) },
            { 101, InstructionData("ff", "", new RegexMorph("$0 = $1")) },
            { 102, InstructionData("SS", "", new RegexMorph("$0 += $1")) },
            { 103, InstructionData("ff", "", new RegexMorph("$0 += $1")) },
            { 104, InstructionData("SS", "", new RegexMorph("$0 -= $1")) },
            { 105, InstructionData("ff", "", new RegexMorph("$0 -= $1")) },
            { 106, InstructionData("SS", "", new RegexMorph("$0 *= $1")) },
            { 107, InstructionData("ff", "", new RegexMorph("$0 *= $1")) },
            { 108, InstructionData("SS", "", new RegexMorph("$0 /= $1")) },
            { 109, InstructionData("ff", "", new RegexMorph("$0 /= $1")) },
            { 110, InstructionData("SS", "", new RegexMorph("$0 %= $1")) },
            { 111, InstructionData("ff", "", new RegexMorph("$0 %= $1")) },
            //Arithmetic
            { 112, InstructionData("SSS", "", new RegexMorph("$0 = $1 + $2")) },
            { 113, InstructionData("fff", "", new RegexMorph("$0 = $1 + $2")) },
            { 114, InstructionData("SSS", "", new RegexMorph("$0 = $1 - $2")) },
            { 115, InstructionData("fff", "", new RegexMorph("$0 = $1 - $2")) },
            { 116, InstructionData("SSS", "", new RegexMorph("$0 = $1 * $2")) },
            { 117, InstructionData("fff", "", new RegexMorph("$0 = $1 * $2")) },
            { 118, InstructionData("SSS", "", new RegexMorph("$0 = $1 / $2")) },
            { 119, InstructionData("fff", "", new RegexMorph("$0 = $1 / $2")) },
            { 120, InstructionData("SSS", "", new RegexMorph("$0 = $1 % $2")) },
            { 121, InstructionData("fff", "", new RegexMorph("$0 = $1 % $2")) },
            { 122, InstructionData("SS", "") },
            { 124, InstructionData("ff", "") },
            { 125, InstructionData("ff", "") },
            { 130, InstructionData("ffff", "") },
            { 131, InstructionData("ffff", "") },
            //Jumps
            { 200, InstructionData("oS", "", new RegexMorph("goto $0 @ $1")) },
            { 201, InstructionData("SoS", "", new RegexMorph("if ($0--) goto $1 @ $2")) },
            { 202, InstructionData("SSoS", "", new RegexMorph("if ($0 == $1) goto $2 @ $3")) },
            { 203, InstructionData("ffoS", "", new RegexMorph("if ($0 == $1) goto $2 @ $3")) },
            { 204, InstructionData("SSoS", "", new RegexMorph("if ($0 != $1) goto $2 @ $3")) },
            { 205, InstructionData("ffoS", "", new RegexMorph("if ($0 != $1) goto $2 @ $3")) },
            { 206, InstructionData("SSoS", "", new RegexMorph("if ($0 < $1) goto $2 @ $3")) },
            { 207, InstructionData("ffoS", "", new RegexMorph("if ($0 < $1) goto $2 @ $3")) },
            { 208, InstructionData("SSoS", "", new RegexMorph("if ($0 <= $1) goto $2 @ $3")) },
            { 209, InstructionData("ffoS", "", new RegexMorph("if ($0 <= $1) goto $2 @ $3")) },
            { 210, InstructionData("SSoS", "", new RegexMorph("if ($0 > $1) goto $2 @ $3")) },
            { 211, InstructionData("ffoS", "", new RegexMorph("if ($0 > $1) goto $2 @ $3")) },
            { 212, InstructionData("SSoS", "", new RegexMorph("if ($0 >= $1) goto $2 @ $3")) },
            { 213, InstructionData("ffoS", "", new RegexMorph("if ($0 >= $1) goto $2 @ $3")) },
            //ANM
            { 300, InstructionData("S", "SetSprite") },
            { 301, InstructionData("SS", "SetSpriteRandom") },
            { 302, InstructionData("S", "SetRotationMode") },
            { 303, InstructionData("S", "SetBlendMode") },
            { 304, InstructionData("S", "SetLayer") },
            { 305, InstructionData("S", "") },
            { 306, InstructionData("S", "") },
            { 307, InstructionData("S", "") },
            { 308, InstructionData("", "FlipTextureU") },
            { 309, InstructionData("", "FlipTextureV") },
            { 311, InstructionData("S", "") },
            { 312, InstructionData("SS", "") },
            { 313, InstructionData("S", "") },
            { 314, InstructionData("S", "") },
            { 315, InstructionData("S", "") },
            { 316, InstructionData("", "") },
            { 317, InstructionData("", "") },
            { 400, InstructionData("fff", "SetPosition") },
            { 401, InstructionData("fff", "SetAngle") },
            { 402, InstructionData("ff", "SetScale") },
            { 403, InstructionData("S", "SetAlpha") },
            { 404, InstructionData("SSS", "SetColor") },
            { 405, InstructionData("S", "SetPrimitiveAlpha") },
            { 406, InstructionData("SSS", "SetPrimitiveColor") },
            { 407, InstructionData("SSfff", "ChangePosition") },
            { 408, InstructionData("SSSSS", "ChangeColor") },
            { 409, InstructionData("SSS", "ChangeAlpha") },
            { 410, InstructionData("SSfff", "ChangeAngle") },
            { 411, InstructionData("SSf", "ChangeAngleZ") },
            { 412, InstructionData("SSff", "ChangeScale") },
            { 413, InstructionData("SSSSS", "ChangePrimitiveColor") },
            { 414, InstructionData("SSS", "ChangePrimitiveAlpha") },
            { 415, InstructionData("fff", "SetRotationRate") },
            { 416, InstructionData("ff", "") },
            { 417, InstructionData("SS", "ChangeAlpha_s") },
            { 420, InstructionData("Sfffffffff", "ChangePositionBezier") },
            { 421, InstructionData("S", "SetTextureOriginType") },
            { 422, InstructionData("", "") },
            { 423, InstructionData("S", "") },
            { 424, InstructionData("S", "EnableAutoRotate") },
            { 425, InstructionData("f", "SetTextureScrollU") },
            { 426, InstructionData("f", "SetTextureScrollV") },
            { 428, InstructionData("SSf", "") },
            { 429, InstructionData("Sf", "") },
            { 430, InstructionData("SSff", "ChangeScaleInverse") },
            { 431, InstructionData("S", "") },
            { 432, InstructionData("S", "") },
            { 433, InstructionData("SSff", "") },
            { 434, InstructionData("ff", "") },
            { 436, InstructionData("ff", "") },
            { 437, InstructionData("S", "") },
            { 438, InstructionData("S", "") },
            { 439, InstructionData("S", "") },
            //Loading
            { 500, InstructionData("S", "RunScript") },
            { 501, InstructionData("S", "RunScript_s") },
            { 502, InstructionData("S", "") },
            { 503, InstructionData("S", "") },
            { 504, InstructionData("S", "") },
            { 505, InstructionData("Sff", "") },
            { 507, InstructionData("S", "") },
            { 508, InstructionData("S", "") },
            { 509, InstructionData("", "") },
            //Texture generation
            { 600, InstructionData("S", "InitPolarCoordinates") },
            { 602, InstructionData("S", "") },
            { 603, InstructionData("ff", "GenerateRectangle") },
            { 604, InstructionData("fS", "GenerateStar") },
            { 605, InstructionData("fS", "GenerateStar_s") },
            { 606, InstructionData("ff", "GenerateRectangle_1") },
            { 607, InstructionData("ff", "GenerateRectangle_2") },
            { 608, InstructionData("ff", "GenerateRectangle_3") },
            { 609, InstructionData("S", "") },
            { 610, InstructionData("S", "") },
            { 611, InstructionData("ffS", "") },
            { 612, InstructionData("ff", "") }
        };
        InstructionDataList* listv8 = new InstructionDataList;
        listv8->Initialize(map_v8);
        mapList_.insert(std::make_pair(8U, listv8));
    }
}
InstructionDataManager::~InstructionDataManager() {
    for (auto itr = mapList_.begin(); itr != mapList_.end(); ++itr) {
        delete itr->second;
        itr->second = nullptr;
    }
}
InstructionDataList* InstructionDataManager::GetList(uint32_t version) {
    auto itr = mapList_.end();
    switch (version) {
    case 4:
    case 7:
        itr = mapList_.find(4U);
        break;
    case 8:
        itr = mapList_.find(8U);
        break;
    }
    if (itr == mapList_.end()) return nullptr;
    return itr->second;
}

InstructionDataList::InstructionDataList() {

}
InstructionDataList::~InstructionDataList() {
    for (auto itr = mapFormatParameter_.begin(); itr != mapFormatParameter_.end(); ++itr)
        itr->second.Dispose();
}
void InstructionDataList::Initialize(const map<uint16_t, InstructionData>& instrMap) {
    mapFormatParameter_ = instrMap;
}
InstructionData* InstructionDataList::GetData(uint16_t id) {
    auto itrFind = mapFormatParameter_.find(id);
    if (itrFind == mapFormatParameter_.end()) return nullptr;
    return &itrFind->second;
}

ExpressionMorph::ExpressionMorph() {

}
RegexMorph::RegexMorph(const string& pattern) {
    pattern_ = pattern;
}
string RegexMorph::Morph(InstructionData* data, vector<string>* listParam) {
    string res;
    for (auto itr = pattern_.begin(); itr != pattern_.end();) {
        char current = *itr;
        if (current == '$' && std::next(itr) != pattern_.end()) {
            auto itrNext = std::next(itr);
            if (!std::isdigit(*itrNext)) goto no_pattern;

            char* nextPtr = &*itrNext;
            char* endPtr = nullptr;
            uint32_t index = strtoul(nextPtr, &endPtr, 10);
            if (index < listParam->size())
                res += (*listParam)[index];
            else goto no_pattern;

            itr += endPtr - nextPtr + 1;
            continue;
        }
no_pattern:
        res += current;
        ++itr;
    }
    return res;
}