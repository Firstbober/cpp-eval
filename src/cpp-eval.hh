#pragma once
#include <cstdio>
#include <dlfcn.h>
#include <fstream>
#include <string>

extern void* __CPPEVAL_INTERNALS_HANDLE__ = NULL;

#define EVAL_FUNCTION extern "C"

#define __sys__eval_init_handle(code, __internal_code__, __internal_trp__) \
	__CPPEVAL_INTERNALS_HANDLE__ = std::__CPPEVAL_INTERNALS__sys__eval(code, __internal_code__, __internal_trp__);

#define __sys__eval_init_fncdef(name, funcname, ...) \
	void (*name)(__VA_ARGS__); \
	name = reinterpret_cast<void (*)(__VA_ARGS__)>(dlsym(__CPPEVAL_INTERNALS_HANDLE__, funcname));

#define __sys__eval_run(name, ...) \
	name(__VA_ARGS__); \
	dlclose(__CPPEVAL_INTERNALS_HANDLE__); \
	exit(0)

namespace std {

inline void eval(const char *code) {
	printf("If you see this then probably preprocessor failed or tried running eval in eval.\n");
}

inline auto __CPPEVAL_INTERNALS__sys__eval(std::string code, std::string __internal_code__ = "", std::string __internal_trp__ = "")
	-> void* {
	auto replace = [](std::string &str, const std::string &from, const std::string &to) -> void {
		if (from.empty()) return;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	};

	std::ofstream cppevalh;
	cppevalh.open("/tmp/cpp-eval.hh");
	// cppevalh << "@@@_PREGEN_CPPEVALH_@@@";
	cppevalh << "#pragma once\n#include <cstdio>\n#include <dlfcn.h>\n#include <fstream>\n#include <string>\n\nextern void* __CPPEVAL_INTERNALS_HANDLE__ = NULL;\n\n#define EVAL_FUNCTION extern \"C\"\n\n#define __sys__eval_init_handle(code, __internal_code__, __internal_trp__) \\\n	__CPPEVAL_INTERNALS_HANDLE__ = std::__CPPEVAL_INTERNALS__sys__eval(code, __internal_code__, __internal_trp__);\n\n#define __sys__eval_init_fncdef(name, funcname, ...) \\\n	void (*name)(__VA_ARGS__); \\\n	name = reinterpret_cast<void (*)(__VA_ARGS__)>(dlsym(__CPPEVAL_INTERNALS_HANDLE__, funcname));\n\n#define __sys__eval_run(name, ...) \\\n	name(__VA_ARGS__); \\\n	dlclose(__CPPEVAL_INTERNALS_HANDLE__); \\\n	exit(0)\n\nnamespace std {\n\ninline void eval(const char *code) {\n	printf(\"If you see this then probably preprocessor failed or tried running eval in eval.\\n\");\n}\n\ninline auto __CPPEVAL_INTERNALS__sys__eval(std::string code, std::string __internal_code__ = \"\", std::string __internal_trp__ = \"\")\n	-> void* {\n	auto replace = [](std::string &str, const std::string &from, const std::string &to) -> void {\n		if (from.empty()) return;\n		size_t start_pos = 0;\n		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {\n			str.replace(start_pos, from.length(), to);\n			start_pos += to.length();\n		}\n	};\n\n	std::ofstream cppevalh;\n	cppevalh.open(\"/tmp/cpp-eval.hh\");\n	cppevalh << \"@@@_PREGEN_CPPEVALH_@@@\";\n	cppevalh.close();\n\n	std::ofstream new_code;\n	new_code.open(\"/tmp/evalout.cpp\");\n\n	replace(__internal_code__, __internal_trp__, code + \";\");\n\n	new_code << __internal_code__;\n	new_code.close();\n\n	system(\"g++ -ldl -fPIC -shared /tmp/evalout.cpp -o /tmp/eval.so\");\n	void *handle = dlopen(\"/tmp/eval.so\", RTLD_NOW);\n	return handle;\n}\n\n\n}\n\n";
	cppevalh.close();

	std::ofstream new_code;
	new_code.open("/tmp/evalout.cpp");

	replace(__internal_code__, __internal_trp__, code + ";");

	new_code << __internal_code__;
	new_code.close();

	system("g++ -w -ldl -fPIC -shared /tmp/evalout.cpp -o /tmp/eval.so");
	void *handle = dlopen("/tmp/eval.so", RTLD_NOW);
	return handle;
}


}

