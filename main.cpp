#include <windows.h>
#include <iostream>
#include <fstream>

void errlog(char* err) {
	std::cerr << err << std::endl;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		errlog("usage: brainfucked [filename]");
		return 0;
	}

	std::ifstream code(argv[1]);
	std::ofstream c_code;
	char path[MAX_PATH];
	if (code) {
		GetCurrentDirectory(MAX_PATH, path);
		strcat_s(path, "\\c_file.cpp");
		c_code.open(path);
	}
	if (!code) {
		errlog("error: file not recognized");
		return 0;
	}

	if (c_code.is_open()) {
		c_code << "#include <stdio.h>\n";
		c_code << "int main() {\n";
		c_code << "char array[30000] = { 0 };\n";
		c_code << "char *ptr = array;\n";

		while (code.good()) {
			char command = code.get();
			switch (command) {
			case '>':
				c_code << "++ptr;\n";
				break;
			case '<':
				c_code << "--ptr;\n";
				break;
			case '+':
				c_code << "++*ptr;\n";
				break;
			case '-':
				c_code << "--*ptr;\n";
				break;
			case '.':
				c_code << "putchar(*ptr);\n";
				break;
			case ',':
				c_code << "*ptr = getchar();\n";
				break;
			case '[':
				c_code << "while (*ptr) {\n";
				break;
			case ']':
				c_code << "}\n";
				break;
			default:
				break;
			}
		}
		c_code << "}\n";
	}
	else {
		errlog("error: could not open intermediate file");
		return 0;
	}
	
	c_code.close();

	std::cout << path << " is to be compiled. (MSVC is needed)" << std::endl;
	std::cout << "Compiling with MSVC..." << std::endl;

	char compile_command[MAX_PATH] = "cl /EHsc ";
	strcat_s(compile_command, path);
	std::system("vcvars32");
	std::system(compile_command);
}