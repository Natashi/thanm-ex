#include "pch.h"
#include "Util.h"

void Utility::StreamPrint(std::ostream& out, const char* str, ...) {
	va_list vl;
	__crt_va_start(vl, str);

	char ch[1024];
	_vsprintf_s_l(ch, 1024, str, nullptr, vl);
	out << ch;

	__crt_va_end(vl);
}
std::string Utility::PrintFloat(float f) {
	char buf[64];
	size_t size = _scprintf("%63.6f", f);
	sprintf_s(buf, 64, "%.6f", f);

	std::string res = buf;
	if (size > 0) {
		auto itr = res.rbegin();
		for (; itr != res.rend(); ++itr) {
			char c = *itr;
			if (c == '.' || c != '0') break;
		}

		res = std::string(res.begin(), itr.base());
		if (res.back() == '.') res.push_back('0');
	}
	res.push_back('f');

	return res;
}